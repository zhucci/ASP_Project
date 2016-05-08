#include "AssemblyGraphBuilder.h"
#include "boost/graph/graphviz.hpp"
#include <iostream>
#include <cstdio>
#include <process.h>
#include <Geom_SurfaceOfLinearExtrusion.hxx>

using namespace asp;
using namespace boost;

int AssemblyGraphBuilder::Init(asp::Assembly *convertedAsm){
	//write_graphviz
	auto pShapeMap = convertedAsm->GetUnitMap();
	if (!pShapeMap->size())
		return -1;

	if(asmGraph)
		asmGraph = new assemblyGraph(pShapeMap->size());

	if(partGraphSet)
		partGraphSet = new partGraphMap;

	auto AsmGraph_EdgeMap =
		get(edge_ContactDesc, *asmGraph);
	
	auto AsmGraph_VMap = get(vertex_BodyFormDesc,*asmGraph);
	std::map<PartUri, assemblyGraph::vertex_descriptor> partUri_VertexDescMap;

	for (auto &p : *pShapeMap){
		//pushToPartSetGraph(p.first, dynamic_cast<Part*> (p.second));
		//addEdgeToAssemblyGraph(p.first, dynamic_cast<Part*> (p.second));
		Part * part = dynamic_cast<Part*>(p.second);
		auto vertex_desc = getBodyDescriptor(part);
		auto desc = add_vertex(*asmGraph);
		put(AsmGraph_VMap,desc,vertex_desc);
		partUri_VertexDescMap.emplace(part->uri,vertex_desc);
	}
	ContactDesc desc;
	for (auto p1= pShapeMap->begin();p1!=pShapeMap->end();p1++){
		Part * part = dynamic_cast<Part*>(p1->second);
		auto V1 = partUri_VertexDescMap.find(part->GetUri());
		auto p2 = p1;
		for (++p2;p2!=pShapeMap->end();++p2){
			Part * coPart = dynamic_cast<Part*>(p2->second);
			if(getContactDesc(part, coPart, desc)){
				auto V2 = partUri_VertexDescMap.find(coPart->GetUri());
				auto edge_Desc = add_edge(V1->second,V2->second,desc, *asmGraph);
				//put(AsmGraph_EdgeMap, edge_Desc, desc);
			}
		}
	}
	assemblyGraph asm2(*asmGraph);
	typedef property_map<assemblyGraph,edge_ContactDesc_t>::type asmGraph_EdgePropMap;
	typedef property_map<assemblyGraph,vertex_BodyForm_t>::type asmGraph_VertexPropMap;
	typedef property_map_equivalent<asmGraph_EdgePropMap, asmGraph_EdgePropMap> asmGraphEdgeComp;
	typedef property_map_equivalent<asmGraph_VertexPropMap, asmGraph_VertexPropMap> asmGraphVertexComp;
	asmGraphEdgeComp AG_edge_comp = make_property_map_equivalent(get(edge_ContactDesc,*asmGraph),get(edge_ContactDesc,asm2));
	asmGraphVertexComp AG_vertex_comp = make_property_map_equivalent(get(vertex_BodyFormDesc, *asmGraph), get(vertex_BodyFormDesc,asm2));
	user_callback<assemblyGraph, assemblyGraph> result(asm2,*asmGraph);

	vf2_subgraph_iso(asm2, *asmGraph, result, vertex_order_by_mult(asm2),
		edges_equivalent(AG_edge_comp).vertices_equivalent(AG_vertex_comp));

	
	std::ofstream graphStream("Graph.dot");
	if (graphStream.is_open()){
	//	write_graphviz(graphStream,graph1);
	//	execl("dot.exe -Tpng Graph.dot > output.png",NULL);
	//	execl("output.png",NULL);
	}
		
	return 0;
}

_int AssemblyGraphBuilder::pushToPartSetGraph(PartUri pUri, asp::Part* part){
	//if pUri is already in map
	if (!partGraphSet)
		return -1;
	if (partGraphSet->find(pUri) != partGraphSet->end())
		return -2;
	
	return 0;
}
_int AssemblyGraphBuilder::addEdgeToAssemblyGraph(PartUri pUri, Part* part){
	if (!asmGraph)
		return -1;
	
	return 0;
}

_bool AssemblyGraphBuilder::GetTraitOfFace(asp::SurfaceAttribute &s1, std::pair<gp_Pnt, gp_Vec> &traits){

	gp_Vec traitVecF1;
	gp_Pnt traitPntF1;
	
	_bool traitVecF1Status{ false }, traitPntF1Status{ false };
	auto f1Type = s1.surf.GetType();
	
	if (f1Type == GeomAbs_Plane){

		traitVecF1 = s1.surf.Plane().Axis().Direction();
		if (s1.myShape.Orientation() == TopAbs_REVERSED)
			traitVecF1.Reverse();
		_real ui, us, vi, vs;
		BRepTools::UVBounds(s1.myShape, ui, us, vi, vs);
		s1.surf.D0((us + ui) / 2., (vs + vi) / 2., traitPntF1);
		traitVecF1Status = true;
		traitPntF1Status = true;
	}
	else if (f1Type == GeomAbs_Cylinder){
		traitVecF1 = s1.surf.Cylinder().Axis().Direction();
		gp_Pnt loc = s1.surf.Cylinder().Axis().Location();
		_real ui, us, vi, vs;
		BRepTools::UVBounds(s1.myShape, ui, us, vi, vs);
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
	else if (f1Type == GeomAbs_Cone){
		traitVecF1 = s1.surf.Cone().Axis().Direction();
		gp_Pnt loc = s1.surf.Cone().Axis().Location();
		_real ui, us, vi, vs;
		BRepTools::UVBounds(s1.myShape, ui, us, vi, vs);
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
	else if (f1Type == GeomAbs_Torus){
		traitVecF1 = s1.surf.Torus().Axis().Direction();
		gp_Pnt loc = s1.surf.Torus().Axis().Location();
		_real ui, us, vi, vs;
		BRepTools::UVBounds(s1.myShape, ui, us, vi, vs);
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
	else if (f1Type == GeomAbs_SurfaceOfRevolution){
		traitVecF1 = s1.surf.AxeOfRevolution().Direction();
		gp_Pnt loc = s1.surf.AxeOfRevolution().Location();
		_real ui, us, vi, vs;
		BRepTools::UVBounds(s1.myShape, ui, us, vi, vs);
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
	else if (f1Type == GeomAbs_SurfaceOfExtrusion){
		auto surf = *((Handle(Geom_SurfaceOfLinearExtrusion)*)&s1.surf);
		if (!surf.IsNull()){
			gp_Pnt loc;
			surf->BasisCurve()->D1(surf->BasisCurve()->FirstParameter(), loc,traitVecF1);
			traitVecF1Status = true;
		}
	}
	
	GProp_GProps propsTester;

	if (!traitPntF1Status){
		
		BRepGProp::LinearProperties(s1.myShape, propsTester);
		traitPntF1 = propsTester.CentreOfMass();
		traitPntF1Status=true;
	}
	if (!traitVecF1Status){
		Extrema_ExtPS extrema;
		_real ui, us, vi, vs;
		BRepTools::UVBounds(s1.myShape, ui, us, vi, vs);
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
			if (s1.myShape.Orientation() == TopAbs_REVERSED)
				tanU.Reverse();
			tanU.Normalize();
			traitVecF1=tanU;
		}	
		traitVecF1Status=true;
	}
	if (!traitPntF1Status || !traitVecF1Status)
		return false;
	else{
		traits.first = traitPntF1;
		traits.second = traitVecF1;
		
	}
	return true;
}

AssemblyGraphBuilder::FaceSpartial AssemblyGraphBuilder::getFaceSpartialDescriptor( asp::SurfaceAttribute &s1, asp::SurfaceAttribute &s2){
	
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

AssemblyGraphBuilder::BodyForm AssemblyGraphBuilder::getBodyDescriptor(asp::Part* part){
	
	BodyForm bForm;

	bForm.MatVolRatio=(part->MaterialVolume() / part->BoundVolume());
	bForm.partUri = part->GetUri();
	return bForm;
}
AssemblyGraphBuilder::FaceType AssemblyGraphBuilder::getFaceType(const asp::SurfaceAttribute &surface1){
	FaceType fType;
	if (surface1.Func == asp::SurfaceFunction::_Base)
		fType.FaceFunctionType=(1);
	else
		fType.FaceFunctionType = (0);
	return fType;
}
_int AssemblyGraphBuilder::getContactDesc(asp::Part* part1, asp::Part* part2, AssemblyGraphBuilder::ContactDesc &desc){
	ContactDesc cType;
	_int cntTypeValue{0};
	_int ObstPartUri = part2->GetUri();
	for (asp::Contact &cont : part1->colOfCont){
		if (cont.obstURI == ObstPartUri){
			cntTypeValue += std::pow(2,10*(int)cont.Type);
		}
	}
	desc.contactType = cntTypeValue;

	return cntTypeValue;
	
}
std::string AssemblyGraphBuilder::GetPartGraphImageFileName(_int PartUri){
	std::string FileName("djfdj");

	return FileName;
}