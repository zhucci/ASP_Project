#include "AssemblyGraphBuilder.h"
#include "boost/graph/graphviz.hpp"
#include <iostream>
#include <cstdio>
#include <process.h>
#include <Geom_SurfaceOfLinearExtrusion.hxx>
#include <exception>
using namespace asp;
using namespace boost;


int AssemblyGraphBuilder::Init(asp::Assembly *convertedAsm){
	//write_graphviz
	auto pShapeMap = convertedAsm->GetUnitMap();
	if (!pShapeMap->size())
		return -1;

	if(!asmGraph)
		asmGraph = new assemblyGraph();

	if(!partGraphSet)
		partGraphSet = new partGraphMap;
	std::vector<Part*> parts;
	for (auto &p : *pShapeMap){
		auto part = dynamic_cast<Part*>(p.second);
		parts.push_back(part);

		//Create part face graph and emplace theme in partGraphSet
		auto res = GetPartGraphsSet(part, false);
		partGraphSet->emplace(part->GetUri(),res);

	}

	asmGraph->clear();

	(*asmGraph) = GetAssemblyGraph(&parts);

		
	return 0;
}

assemblyGraph AssemblyGraphBuilder::GetAssemblyGraph(std::vector<Part*> *parts){
	assemblyGraph aGraph;
	std::map<PartUri, graph_traits<assemblyGraph>::vertex_descriptor> partUri_VertexDescMap;

	for (auto &part : *parts){


		auto vertex_desc = getBodyDescriptor(part);
		auto desc = add_vertex(*asmGraph);
		vertex_desc.Vertex_Index = (_int) desc;
		(*asmGraph)[desc] = vertex_desc;

		partUri_VertexDescMap.emplace(part->uri, desc);
	}

	//auto map2 = get(graph_traits<assemblyGraph>::vertex_descriptor(), *asmGraph);

	ContactDesc desc;
	for (auto p1 = parts->begin(); p1 != parts->end(); p1++){
		Part * part = *p1;
		auto V1 = partUri_VertexDescMap.find(part->GetUri());
		auto p2 = p1;
		for (++p2; p2 != parts->end(); ++p2){
			Part * coPart = *p2;
			if (getContactDesc(part, coPart, desc)){
				auto V2 = partUri_VertexDescMap.find(coPart->GetUri());
				auto edge_Desc = add_edge(V1->second, V2->second, desc, *asmGraph);
				//put(AsmGraph_EdgeMap, edge_Desc, desc);
			}
		}
	}
	return 0;
}
int AssemblyGraphBuilder::GripFacesIsomorphism(std::vector<Part*> *partsWithGripFaces, std::vector<PartIsomorphism> isomorphism, bool ){

	assemblyGraph patternAsm(GetAssemblyGraph(partsWithGripFaces));
	partGraphMap gripedPartMap;
	if (currentGraphSetForMatch)
		 delete currentGraphSetForMatch;
	currentGraphSetForMatch = new partGraphMap;
	for (auto &p : *partsWithGripFaces){
		currentGraphSetForMatch->emplace(p->GetUri(),GetPartGraphsSet(p, true));//true - use just Base and Grip faces for small graph constraction
	}

	std::ofstream graphStream("Graph.dot");
	if (graphStream.is_open()){
		write_graphviz(graphStream, *asmGraph);
		//	execl("dot.exe -Tpng Graph.dot > output.png",NULL);
		//	execl("output.png",NULL);
	}

	Iso_AssemblyCallback<assemblyGraph, assemblyGraph> result(this, patternAsm, *asmGraph,true);


	vf2_subgraph_iso(patternAsm, *asmGraph, result, get(&BodyForm::Vertex_Index, patternAsm), get(&BodyForm::Vertex_Index, *asmGraph), vertex_order_by_mult(patternAsm), ContactDescCompare(patternAsm, *asmGraph), BodyFormCompare(patternAsm,*asmGraph));
	return 0;
}
void AssemblyGraphBuilder::ShowAssemblyIsomorphism(std::map<int, int> *){

}

bool AssemblyGraphBuilder::IsomorphismOfPartCompare(std::map<int, int> *currentIsoResult, bool SaveResultFlag){
	if(!currentGraphSetForMatch)
		return false;
	for (auto &pPair : *currentIsoResult){
		std::pair<partGraph, partGraph> sGSet = currentGraphSetForMatch->find(pPair.first)->second;
		std::pair<partGraph, partGraph> lGSet = partGraphSet->find(pPair.second)->second;
		//If contact faces don't match continue assembly isomorphism calc
		
		if (SaveResultFlag){
			currentIsoPart.clear();
		if (!IsomorphismOfPartCompare(sGSet, lGSet, &currentIsoPart))
			return false;
		}
		else if (!IsomorphismOfPartCompare(sGSet, lGSet, NULL))
			return false;
		if (SaveResultFlag){
			currentIsoPart.back().isoPart.first = pPair.first;
			currentIsoPart.back().isoPart.second = pPair.second;
		}
	}
	//All parts has isomorphsim
	return true;
}
bool AssemblyGraphBuilder::IsomorphismOfPartCompare(std::pair<partGraph, partGraph> smallGSet, std::pair<partGraph, partGraph> largeGSet,std::vector<PartIsomorphism> *result){
	try{
	typedef 	Iso_FaceCallback<partGraph,partGraph> faceCallBack;

	Iso_FaceFalseCallback sayFalseCallBack;

	auto edge_comp = FaceSpartialCompare(smallGSet.first, largeGSet.first);
	auto vert_comp = FaceTypeCompare(smallGSet.first, largeGSet.first);

	if(!vf2_subgraph_iso(smallGSet.first, largeGSet.first, sayFalseCallBack, get(&FaceType::Vertex_Index, smallGSet.first), get(&FaceType::Vertex_Index, largeGSet.first), vertex_order_by_mult(smallGSet.first), edge_comp, vert_comp))
		return false;
	
	faceCallBack callBack(smallGSet.second, largeGSet.second,false,result);
	

	edge_comp = FaceSpartialCompare(smallGSet.second, largeGSet.second);
	vert_comp = FaceTypeCompare(smallGSet.second, largeGSet.second);

	if (vf2_subgraph_iso(smallGSet.second, largeGSet.second, callBack, get(&FaceType::Vertex_Index, smallGSet.second), get(&FaceType::Vertex_Index, largeGSet.second), vertex_order_by_mult(smallGSet.second), edge_comp, vert_comp)){
		IsDone = true;
		return true;
	}
	}
	catch (std::exception){
		return false;
	}
	return false;
}
bool AssemblyGraphBuilder::IsomorphismOfPartCompare(partGraph smallGSet, partGraph largeGSet){

	return true;
}
std::pair<partGraph, partGraph> AssemblyGraphBuilder::GetPartGraphsSet( asp::Part* part, bool justBaseAndGripFaces){
	//PartUri pUri = part->GetUri();
	partGraph smallG;
	partGraph fullG;
	_real MaxDistance = 0;
//	_int VertCounter = 0;
	auto end = part->colOfSurf.end();
	std::map<FaceUri, int> faceToSmallGraphMap;
	std::map<FaceUri, int> faceToFullGraphMap;

	for (auto iter = part->colOfSurf.begin(); iter != end; ++iter){
		auto Vert1Prop = getFaceType(*iter);

		
		if (justBaseAndGripFaces && (iter->Func == _Base || iter->Func == _Grip)){
			Vert1Prop.Vertex_Index = (_int) add_vertex(fullG);
				fullG[Vert1Prop.Vertex_Index] = Vert1Prop;
				faceToFullGraphMap.emplace(Vert1Prop.faceUri, Vert1Prop.Vertex_Index);
		}
		else if (!justBaseAndGripFaces){
			Vert1Prop.Vertex_Index = (_int) add_vertex(fullG);
			fullG[Vert1Prop.Vertex_Index] = Vert1Prop;
			faceToFullGraphMap.emplace(Vert1Prop.faceUri, Vert1Prop.Vertex_Index);
		}

		
		if (iter->Func == _Base){
			Vert1Prop.Vertex_Index = (_int)add_vertex(smallG);
			smallG[Vert1Prop.Vertex_Index] = Vert1Prop;
			faceToSmallGraphMap.emplace(Vert1Prop.faceUri, Vert1Prop.Vertex_Index);
		}
	}

	for (auto iter = part->colOfSurf.begin(); iter != end; ++iter){
		auto coIter = iter;
		auto v1 = faceToFullGraphMap.find(iter->uri);
//		int res = v1->second;
		if (v1 != faceToFullGraphMap.end())
		for (coIter++; coIter != end; ++coIter){
			auto v2 = faceToFullGraphMap.find(coIter->uri);
			if (v2 == faceToFullGraphMap.end())
				continue;

			auto EdgeProp = getFaceSpartialDescriptor(*iter, *coIter);
			MaxDistance = MaxDistance>EdgeProp.DistDesc ? MaxDistance : EdgeProp.DistDesc;
			auto newE = add_edge(v1->second,v2->second, EdgeProp, fullG);

			if (coIter->Func == _Base && iter->Func == _Base){
				v1 = faceToSmallGraphMap.find(iter->uri);
				if (v1 == faceToSmallGraphMap.end())
					continue;
				v2 = faceToSmallGraphMap.find(coIter->uri);
				if (v2 == faceToSmallGraphMap.end())
					continue;
				auto newE = add_edge(v1->second, v2->second, EdgeProp, smallG);
			}
		}
	}

			BGL_FORALL_EDGES(e, fullG, partGraph){
				smallG[e].DistDesc /= MaxDistance;
			}
			BGL_FORALL_EDGES(e, smallG, partGraph){
				fullG[e].DistDesc /= MaxDistance;
			}
			return std::pair<partGraph,partGraph>(smallG,fullG);
}


_bool AssemblyGraphBuilder::GetTraitOfFace(asp::SurfaceAttribute &s1, std::pair<gp_Pnt, gp_Vec> &traits){

	gp_Vec traitVecF1;
	gp_Pnt traitPntF1;
	
	_bool traitVecF1Status{ false }, traitPntF1Status{ false };
	auto f1Type = s1.surf.GetType();
	
	if (f1Type == GeomAbs_Plane){

		traitVecF1 = s1.surf.Plane().Axis().Direction();
		if (s1.surf.Face().Orientation() == TopAbs_REVERSED)
			traitVecF1.Reverse();
		_real ui, us, vi, vs;
		BRepTools::UVBounds(s1.surf.Face(), ui, us, vi, vs);
		s1.surf.D0((us + ui) / 2., (vs + vi) / 2., traitPntF1);
		traitVecF1Status = true;
		traitPntF1Status = true;
	}
	else if (f1Type == GeomAbs_Cylinder && (s1.surf.IsUClosed() || s1.surf.IsVClosed())){
		

		traitVecF1 = s1.surf.Cylinder().Axis().Direction();
		gp_Pnt loc = s1.surf.Cylinder().Axis().Location();
		_real ui, us, vi, vs;
		BRepTools::UVBounds(s1.surf.Face(), ui, us, vi, vs);
		gp_Pnt p;
		s1.surf.D0((us + ui) / 2., (vs + vi) / 2., p);
		gp_Vec p_Loc(loc, p);
		_real dist = p_Loc.Magnitude();
		p_Loc.Normalize();
		
		if (p_Loc.Dot(traitVecF1)<0)
			traitVecF1.Reverse();
		traitPntF1 = gp_Pnt(loc.XYZ() + traitVecF1.XYZ() * dist*p_Loc.Dot(traitVecF1));
		traitVecF1Status = true;
		traitPntF1Status = true;
	}
	else if (f1Type == GeomAbs_Cone && (s1.surf.IsUClosed() || s1.surf.IsVClosed())){
		traitVecF1 = s1.surf.Cone().Axis().Direction();
		gp_Pnt loc = s1.surf.Cone().Axis().Location();
		_real ui, us, vi, vs;
		BRepTools::UVBounds(s1.surf.Face(), ui, us, vi, vs);
		gp_Pnt p;
		s1.surf.D0((us + ui) / 2., (vs + vi) / 2., p);
		gp_Vec p_Loc(loc, p);
		_real dist = p_Loc.Magnitude();
		p_Loc.Normalize();

		if (p_Loc.Dot(traitVecF1)<0)
			traitVecF1.Reverse();
		traitPntF1 = gp_Pnt(loc.XYZ() + traitVecF1.XYZ() * dist*p_Loc.Dot(traitVecF1));
		traitVecF1Status = true;
		traitPntF1Status = true;
	}
	else if (f1Type == GeomAbs_Torus && (s1.surf.IsUClosed() || s1.surf.IsVClosed())){
		traitVecF1 = s1.surf.Torus().Axis().Direction();
		gp_Pnt loc = s1.surf.Torus().Axis().Location();
		_real ui, us, vi, vs;
		BRepTools::UVBounds(s1.surf.Face(), ui, us, vi, vs);
		gp_Pnt p;
		s1.surf.D0((us + ui) / 2., (vs + vi) / 2., p);
		gp_Vec p_Loc(loc, p);
		_real dist = p_Loc.Magnitude();
		p_Loc.Normalize();

		if (p_Loc.Dot(traitVecF1)<0)
			traitVecF1.Reverse();
		traitPntF1 = gp_Pnt(loc.XYZ() + traitVecF1.XYZ() * dist*p_Loc.Dot(traitVecF1));
		traitVecF1Status = true;
		traitPntF1Status = true;
	}
	else if (f1Type == GeomAbs_SurfaceOfRevolution && (s1.surf.IsUClosed() || s1.surf.IsVClosed())){
		traitVecF1 = s1.surf.AxeOfRevolution().Direction();
		gp_Pnt loc = s1.surf.AxeOfRevolution().Location();
		_real ui, us, vi, vs;
		BRepTools::UVBounds(s1.surf.Face(), ui, us, vi, vs);
		gp_Pnt p;
		s1.surf.D0((us + ui) / 2., (vs + vi) / 2., p);
		gp_Vec p_Loc(loc, p);
		_real dist = p_Loc.Magnitude();
		p_Loc.Normalize();

		if (p_Loc.Dot(traitVecF1)<0)
			traitVecF1.Reverse();
		traitPntF1 = gp_Pnt(loc.XYZ() + traitVecF1.XYZ() * dist*p_Loc.Dot(traitVecF1));
		traitVecF1Status = true;
		traitPntF1Status = true;
	}
	/*
	else if (f1Type == GeomAbs_SurfaceOfExtrusion){
		auto surf = *((Handle(Geom_SurfaceOfLinearExtrusion)*)&s1.surf);
		if (!surf.IsNull()){
			gp_Pnt loc;
			surf->BasisCurve()->D1(surf->BasisCurve()->FirstParameter(), loc,traitVecF1);
			traitVecF1Status = true;
		}
	}
	*/
	//GProp_GProps propsTester;

	if (!traitPntF1Status){
		
		//BRepGProp::LinearProperties(s1.surf.Face(), propsTester);
		//traitPntF1 = propsTester.CentreOfMass();
		_real ui, us, vi, vs;
		gp_Vec tanU, tanV;
		BRepTools::UVBounds(s1.surf.Face(), ui, us, vi, vs);
		
		s1.surf.D1((ui + us) / 2., (vi + vs) / 2., traitPntF1,tanU,tanV );

		tanU.Cross(tanV);

		if (s1.surf.Face().Orientation() == TopAbs_REVERSED)
			tanU.Reverse();
		tanU.Normalize();

		traitVecF1 = tanU;
		
		traitPntF1Status=true;
		traitVecF1Status=true;

	}
	if (!traitVecF1Status){
		Extrema_ExtPS extrema;
		_real ui, us, vi, vs;
		BRepTools::UVBounds(s1.surf.Face(), ui, us, vi, vs);
		extrema.Initialize(s1.surf,ui,us,vi,vs,Precision::Confusion(),
			Precision::Confusion());
		extrema.Perform(traitPntF1);
		if (extrema.IsDone()  && extrema.NbExt() ){
			
			gp_Vec tanU, tanV;
			_real U,V;
			extrema.Point(1).Parameter(U,V);
			gp_Pnt pnt;
			s1.surf.D1(U,V,pnt,tanU,tanV);
			tanU.Cross(tanV);
			if (s1.surf.Face().Orientation() == TopAbs_REVERSED)
				tanU.Reverse();
			tanU.Normalize();
			traitVecF1=tanU;
			traitVecF1Status = true;
		}	
		
	}
	if (!traitPntF1Status || !traitVecF1Status)
		return false;
	else{
		traits.first = traitPntF1;
		traits.second = traitVecF1;
		
	}
	return true;
}

FaceSpartial AssemblyGraphBuilder::getFaceSpartialDescriptor( asp::SurfaceAttribute &s1, asp::SurfaceAttribute &s2){
	
	FaceSpartial fSpartialDesc;
	//CenterPoint of surface
	//Point onSurface
	//Normale

	gp_Vec traitVecF1, traitVecF2;
	gp_Pnt traitPntF1, traitPntF2;
	std::pair<gp_Pnt, gp_Vec> traitF1, traitF2;
	if (!GetTraitOfFace(s1, traitF1) || !GetTraitOfFace(s2, traitF2))
		Standard_Failure::Raise("Traits computation error");

	fSpartialDesc.DistDesc = traitF1.first.Distance(traitF2.first);

	gp_Vec p1p2(traitF1.first,traitF2.first);
	_bool p1p2IsGoodVector = p1p2.Magnitude() >RealSmall();
	_real cos_F1_F2 = traitF1.second.Dot(traitF2.second);

	if (p1p2IsGoodVector)
		p1p2.Normalize();

	if (p1p2IsGoodVector && (p1p2.Dot(traitF1.second)>AsmVecTol() || p1p2.Reversed().Dot(traitF2.second)>AsmVecTol()))
		fSpartialDesc.AngDesc = 1 + cos_F1_F2;
	else
		fSpartialDesc.AngDesc = 3 - cos_F1_F2;

	return fSpartialDesc;
}

BodyForm AssemblyGraphBuilder::getBodyDescriptor(asp::Part* part){
	
	BodyForm bForm;

	bForm.MatVolRatio=(part->MaterialVolume() / part->BoundVolume());
	bForm.partUri = part->GetUri();
	return bForm;
}
FaceType AssemblyGraphBuilder::getFaceType(const asp::SurfaceAttribute &surface1){
	FaceType fType;

	fType.faceUri = surface1.uri;

	if (surface1.Func == asp::SurfaceFunction::_Base)
		fType.FaceFunctionType=(1);
	else
		fType.FaceFunctionType = (0);

	fType.FaceFormType=(int)surface1.surf.GetType();

	return fType;
}
_int AssemblyGraphBuilder::getContactDesc(asp::Part* part1, asp::Part* part2, ContactDesc &desc){
//	ContactDesc cType;
	_int cntTypeValue{0};
	_int ObstPartUri = part2->GetUri();
	for (asp::Contact &cont : part1->colOfCont){
		if (cont.obstURI == ObstPartUri){
			cntTypeValue += (int) std::pow(2,10*(int)cont.Type);
		}
	}
	desc.contactType = cntTypeValue;

	return cntTypeValue;
	
}
std::string AssemblyGraphBuilder::GetPartGraphImageFileName(_int /*PartUri*/){

	std::string FileName;

	return FileName;
}