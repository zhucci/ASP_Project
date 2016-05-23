#include "ContactSpot.h"
#include "BRepExtrema_ExtPF.hxx"
#include "TopoDS_Vertex.hxx"
#include "BRepBuilderAPI_MakeVertex.hxx"
#include "LProp3d_SLProps.hxx"
#include "BRepLProp_SLProps.hxx"
#include "Geom2dAdaptor.hxx"
#include "BRepTools.hxx"
#include "Extrema_ExtPS.hxx"
#include "BRepClass3d_SolidClassifier.hxx"
#include "Part.h"
#include "BRepClass3d_SolidExplorer.hxx"
#include "BRepExtrema_DistShapeShape.hxx"
#include "BRepClass3d_SClassifier.hxx"
#include "BRepClass_FaceClassifier.hxx"
using namespace asp;


ContactSpot::ContactSpot(SurfaceAttribute &surface1,
	SurfaceAttribute &surface2, Part* Part1, Part* Part2):
	f1{surface1.surf},f2{surface2.surf},prt1{Part1},prt2{Part2}{

	isDone = false;
	O_f1 = surface1.myShape.Orientation();
	O_f2 = surface2.myShape.Orientation();
	
	Perform();
}
void ContactSpot::Perform(){
//Try simple some lighter algorithm 
	//if(OverLayAreaEl())
	//	isDone = true;

	if(OverLayAreaGen())
		isDone=true;
}


_int ContactSpot::CheckPntPairs(std::list<ContactSpot::facePntCorrespond> &mainPnts, _bool ShapeSwap,_int GoodPntRequirement){
	_int GoodPntAmount=0;
	_int InToPntAmount=0;
	_int PntClassifyFailure=0;
	_int MaxClassifyAttempt=2;

	auto *ProbablyCrossedPntContact =
		new std::list<facePntCorrespond>::iterator[mainPnts.size()];

	for (auto p = mainPnts.begin(); p != mainPnts.end();)
	{
		if (p->correctPair){
			GoodPntAmount++; 
			p++;
			continue;
		}
	
		gp_Vec p1p2(p->pnt3d.first, p->pnt3d.second);

		_real magnitude = p1p2.Magnitude();

		if (magnitude > Max(SSGapSize, SSCrossSize)){
			p++;
			continue;
		}
		_real n1_n2 = p->normal.first.Dot(p->normal.second);
		if (n1_n2 - ContDirParPrecision>-1){
			p++;
			continue;
		}
		if (magnitude < 1e-2){
			p->correctPair=true;
			p->contType=_Contact;
			p++;
			continue;
		}
		//if |p1p2| > RealSmall we can make normalization
		p1p2.Normalize();

		_real p1p2_n1 = p1p2.Dot(p->normal.first);

		p1p2.Reverse();
		_real p2p1_n2 = p->normal.second.Dot(p1p2);

		if (magnitude<SSGapSize && p1p2_n1 + ContDirParPrecision>1 && p2p1_n2 + ContDirParPrecision>1){
				p->correctPair = true;
				p->contType=_InGap;
				GoodPntAmount++;	
			}
		else if(/*magnitude<SSCrossSize() && */ //Constant true
			p1p2_n1 - ContDirParPrecision<-1 && p2p1_n2 - ContDirParPrecision<-1){
			ProbablyCrossedPntContact[InToPntAmount]=p;
			InToPntAmount++;	
		}
		p++;
	}

if (GoodPntAmount>GoodPntRequirement)
	return GoodPntAmount;

//check is p1 in the part2 
	BRepClass3d_SolidClassifier checker;

	if (ShapeSwap)
		checker.Load(prt1->getShape());
	else
		checker.Load(prt2->getShape());
	_int delta = InToPntAmount / MaxClassifyAttempt;
	delta = delta ? delta : 1;

	for (_int i = 0; i<InToPntAmount; i += delta){
		auto p = ProbablyCrossedPntContact[i];
		gp_Pnt pnt(p->pnt3d.first.XYZ() + p->normal.first.XYZ() * 10 * Precision::Confusion());

		checker.Perform(pnt, Precision::Confusion());
		TopAbs_State state = checker.State();
		if (state == TopAbs_OUT ){
			PntClassifyFailure++;
		}	
	}
	if (!PntClassifyFailure)
		for (_int i = 0; i<InToPntAmount; i ++){
			ProbablyCrossedPntContact[i]->contType=_Intersect;
			ProbablyCrossedPntContact[i]->correctPair = Standard_True;
			GoodPntAmount++;
	}

	return GoodPntAmount;

}
_bool ContactSpot::PntOnSurfGenerate(BRepAdaptor_Surface *surf, TopAbs_Orientation surfOrient, std::list<facePntCorrespond> *corPnts){
	try{
		_real dltU, dltV;
		BRepClass_FaceClassifier classifier;

		_real Urange, Vrange;
		_real uSup, vSup, uInf, vInf;
		BRepTools::UVBounds(surf->Face(),uInf,uSup, vInf,vSup);
		Urange = uSup - uInf;
		Vrange = vSup - vInf;

		dltU = Urange / AmtPntForCS;
		dltV = Vrange / AmtPntForCS;

		if (surf->IsUPeriodic()){

			if(dltU < MinContactPntAngDistance)
				dltU = MinContactPntAngDistance;
		}
		else if (dltU < MinContactPntDistance){
			dltU = MinContactPntDistance;
		}
		if (surf->IsVPeriodic()){

			if (dltV < MinContactPntAngDistance)
				dltV = MinContactPntAngDistance;
		}
		else if (dltV < MinContactPntDistance){
			dltV = MinContactPntDistance;
		}

		gp_Pnt2d pnt(uInf,vInf);
		ContactSpot::facePntCorrespond corPnt;

		while(pnt.X()<uSup){

			pnt.SetY(vInf);

			while(pnt.Y()<vSup){

				classifier.Perform(surf->Face(), pnt, Precision::Confusion());

				// ### Can be TopAbs_IN situation ?
				if (classifier.State() != TopAbs_OUT ){
					gp_Vec tanU, tanV;
					corPnt.pnt2d.first = pnt;
					surf->D1(pnt.X(), pnt.Y(), corPnt.pnt3d.first, tanU, tanV);

					tanU.Cross(tanV);

					if (surfOrient != TopAbs_FORWARD)
						tanU.Reverse();
					tanU.Normalize();
					corPnt.normal.first = tanU;

					corPnts->push_back(corPnt);

				}
				pnt.SetY(pnt.Y()+dltV);
			}
			pnt.SetX(pnt.X() + dltU);
		}
	}
	catch (Standard_Failure)
	{
		return false;
	}

	return true;
}
_bool ContactSpot::PntOnFaceEdgesGenerate(BRepAdaptor_Surface *surf, TopAbs_Orientation surfOrient, std::list<facePntCorrespond> *corPnts){
	TopExp_Explorer edgeExp;
	for (edgeExp.Init(surf->Face(), TopAbs_EDGE); edgeExp.More(); edgeExp.Next()){

		_real ui, us;
		Handle_Geom2d_Curve faceCurve = BRep_Tool::CurveOnSurface(TopoDS::Edge(edgeExp.Current()), surf->Face(), ui, us);

		_real Urange = us - ui;

		_real dlt = Urange / AmtPntForCS;
		
		if (faceCurve->IsPeriodic()){

			if (dlt < MinContactPntAngDistance)
				dlt = MinContactPntAngDistance;
		}
		else if (dlt < MinContactPntDistance){
			dlt = MinContactPntDistance;
		}
		_real u=ui;
		while(u<us){
			ContactSpot::facePntCorrespond corPnt;

			faceCurve->D0(u, corPnt.pnt2d.first);
			u += dlt;
			gp_Vec tanU, tanV;

			surf->D1(corPnt.pnt2d.first.X(), corPnt.pnt2d.first.Y(), corPnt.pnt3d.first, tanU, tanV);
			tanU.Cross(tanV);

			if (surfOrient != TopAbs_FORWARD)
				tanU.Reverse();
			tanU.Normalize();
			corPnt.normal.first = tanU;
			
			corPnts->push_back(corPnt);
		}
	}
	return true;
}
_bool ContactSpot::OverLayAreaGen(){

	/*We have some points right now*/
	try{

		//f1 - surface 1
		//f2 - surface 2
		//which surface has the longest edge perimeter 

		_real f1_perimeter =0;
		_real f2_perimeter =0;
		_int NeedPntAmount =4;
		GProp_GProps propsTester;
		BRepGProp::LinearProperties(f1.Face(),propsTester);
		f1_perimeter=propsTester.Mass();
		
		BRepGProp::LinearProperties(f2.Face(), propsTester);
		
		f2_perimeter = propsTester.Mass();
		BRepAdaptor_Surface * smlSurf;
		BRepAdaptor_Surface * bgSurf;
		TopAbs_Orientation smlFaceOrientation, bgFaceOrientation;
		Standard_Boolean AreFacesSwaped=false;
		//k - Point generation on each edge from smaller wire

		if (f1_perimeter < f2_perimeter){
			smlSurf = &f1;
			bgSurf = &f2;
			smlFaceOrientation = O_f1;
			bgFaceOrientation = O_f2;
		}
		else {
			smlSurf = &f2;
			bgSurf = &f1;
			smlFaceOrientation = O_f2;
			bgFaceOrientation = O_f1;
			AreFacesSwaped = true;
		}
			
		std::list<ContactSpot::facePntCorrespond> mainPnts;

		PntOnSurfGenerate(smlSurf, smlFaceOrientation, &mainPnts);
		if (mainPnts.size()<3*NeedPntAmount)
			PntOnFaceEdgesGenerate(smlSurf, smlFaceOrientation, &mainPnts);

		//Extremum points on bigger face
		BRepExtrema_ExtPF extBuilder;
		extBuilder.Initialize(bgSurf->Face(), Extrema_ExtFlag_MIN, Extrema_ExtAlgo_Grad);
		gp_Pnt pnt3d;
		_real U, V;
		gp_Vec tanU, tanV;
		//Handle_Geom_Surface bgCorSurf = BRep_Tool::Surface(bgSurf->Face());
		for (auto p = mainPnts.begin(); p != mainPnts.end();){
			extBuilder.Perform(BRepBuilderAPI_MakeVertex(p->pnt3d.first),bgSurf->Face());
			if (extBuilder.IsDone() && extBuilder.NbExt()){
				
				extBuilder.Parameter(1, U, V);

				gp_Pnt2d pnt2d(U,V);
				
				bgSurf->D1(U, V, p->pnt3d.second, tanU, tanV);

				//pnt3d.Transform(bgSurf->Face().Location().Transformation());
				p->pnt2d.second = pnt2d;
				//p->pnt3d.second=pnt3d;

				tanU.Cross(tanV);
				if (bgFaceOrientation != TopAbs_FORWARD)
					tanU.Reverse();
				tanU.Normalize();
				p->normal.second = tanU;

				p++;
			}
			else
				p = mainPnts.erase(p);
		}

		
		
		CheckPntPairs(mainPnts, AreFacesSwaped, NeedPntAmount);

		//Extremum points back to initial face
		
		extBuilder.Initialize(smlSurf->Face(), Extrema_ExtFlag_MIN, Extrema_ExtAlgo_Grad);
		for (auto p = mainPnts.begin(); p != mainPnts.end();){

			if (p->correctPair){
				p++;
				continue;
			}
			extBuilder.Perform(BRepBuilderAPI_MakeVertex(p->pnt3d.second), smlSurf->Face());
			if (extBuilder.IsDone() && extBuilder.NbExt()){
				_real U, V;
				extBuilder.Parameter(1, U, V);
				gp_Pnt2d pnt2d(U, V);
				
				gp_Vec tanU, tanV;
				smlSurf->D1(U, V, p->pnt3d.first, tanU, tanV);
				tanU.Cross(tanV);
				if (smlFaceOrientation != TopAbs_FORWARD)
					tanU.Reverse();
				tanU.Normalize();

				p->normal.first = tanU;
				
				p->pnt2d.first = pnt2d;

				p++;
			}
			else
				p = mainPnts.erase(p);
		}

		
		_int GoodPntAmount = CheckPntPairs(mainPnts, AreFacesSwaped, NeedPntAmount);
	
	if (GoodPntAmount>NeedPntAmount){
		//pair on edge situation check

		//Square calculation
		_real u_i1, u_s1, v_i1, v_s1;
		_real u_i2,u_s2, v_i2, v_s2;
		Bnd_Box2d SSBndBox;
		Bnd_Box2d BSBndBox;	
		_real Square1=0,Square2=0;
		
		for (auto p = mainPnts.begin(); p != mainPnts.end();p++){
			if (p->correctPair){
				SSBndBox.Add(p->pnt2d.first);
				BSBndBox.Add(p->pnt2d.second);
			}
		}
		SSBndBox.Get(u_i1,v_i1,u_s1,v_s1);
		BSBndBox.Get(u_i2,v_i2,u_s2,v_s2);
		
		TopoDS_Face cntFace1;
		TopoDS_Face cntFace2;
		try{
			Handle_Geom_Surface ss = BRep_Tool::Surface(smlSurf->Face());
			BRepBuilderAPI_MakeFace MF(ss, u_i1, u_s1, v_i1, v_s1, Precision::Confusion());
		//	for (TopExp_Explorer exp(smlSurf->Face(), TopAbs_WIRE); exp.More(); exp.Next()){
		//		MF.Add(TopoDS::Wire(exp.Current()));
		//	}

		   cntFace1 = MF.Face();
			GProp_GProps propsTester;
			BRepGProp::SurfaceProperties(cntFace1, propsTester);
			Square1 = propsTester.Mass();
		}
		catch (Standard_Failure){
			std::cerr <<__FILE__<<" "<<__LINE__<<" Contact face constraction failure"<<std::endl;
		}
		try{
			Handle_Geom_Surface bs = BRep_Tool::Surface(bgSurf->Face());
			_real _u_i1, _u_s1, _v_i1, _v_s1;
			bs->Bounds(_u_i1, _u_s1, _v_i1, _v_s1);

			BRepBuilderAPI_MakeFace MF(bs, u_i2, u_s2, v_i2, v_s2, Precision::Confusion());
			
		//	for (TopExp_Explorer exp(bgSurf->Face(), TopAbs_WIRE); exp.More(); exp.Next()){
		//		MF.Add(TopoDS::Wire(exp.Current()));
		//	}

			cntFace2 = MF.Face();
			GProp_GProps propsTester;
			BRepGProp::SurfaceProperties(cntFace2, propsTester);
			Square2 = propsTester.Mass();
		}
		catch (Standard_Failure){
			std::cerr << __FILE__ << " " << __LINE__ << " Contact face constraction failure" << std::endl;
		}
		
		if (Square1 > MinContactSquare && Square2 > MinContactSquare){
				for (auto p = mainPnts.begin(); p != mainPnts.end();p++){
					if (p->correctPair){
							gp_Ax1 sS(p->pnt3d.first,p->normal.first);
							gp_Ax1 gS(p->pnt3d.second, p->normal.second);
							gp_Ax1 *f1Axis = AreFacesSwaped ? &gS : &sS;
							gp_Ax1 *f2Axis = AreFacesSwaped ? &sS : &gS;
							if (!SameVectorInSetAlready(*f1Axis, f1VecSpot)){
							f1VecSpot.push_back(*f1Axis);
							f2VecSpot.push_back(*f2Axis);
							}
						
				}
			}
			//	_int CrossPnt{ 0 }, GapPnt{ 0 }, ContPnt{ 0 };
				_int typesCount[3]={0,0,0};

				for (auto & p : mainPnts){
					if (p.correctPair)
						switch (p.contType){
						case ContactType::_Contact :
							typesCount[0]++;
							break;
						case ContactType::_InGap:
							typesCount[1]++;
							break;
						case ContactType::_Intersect:
							typesCount[2]++;
							break;
						}
				}
				_int max;
				if (typesCount[0]>typesCount[1]){
					max = typesCount[0];
					contactType=_Contact;
				}
				else{
					max = typesCount[1];
					contactType = _InGap;
				}
				if (typesCount[2] > max){
					contactType = _Intersect;
				}
			return true;
	}
		
	}

	}
	catch(Standard_Failure)
	{
		std::cerr<<__FILE__<<": "<<__LINE__<<":"<<Standard_Failure::Caught()->GetMessageString()<<endl;
	}

	return false;
}

_bool ContactSpot::SameVectorInSetAlready(gp_Ax1 &axis, std::vector<gp_Ax1> &collection){
	for (auto &ax : collection){
		if (ax.Direction().Dot(axis.Direction())+AsmVecTol()>1)
			return true;
	}
}


_bool ContactSpot:: OverLayAreaEl( ){

	
//	_real SSPntPair[4];

	Extrema_POnSurf p1, p2;

//Try simple some lighter algorithm 

			if(f1.GetType()==f2.GetType()){ 

				if(f1.GetType()==GeomAbs_Plane ){

					Extrema_ExtElSS extEl(f1.Plane(),f2.Plane());
					/*
					if(!extEl.IsDone()) ||
					   !extEl.IsParallel() ||
					    extEl.SquareDistance()-LinTol()>SquareGap ||
						!f1.Plane().Axis().Direction().IsOpposite(f2.Plane().Axis().Direction(),AngTol()))
						return true;
						*/
		//===========================
		//		DEBUG section		=
		//===========================
					if (!extEl.IsDone())
						return false;
					if (!extEl.IsParallel()){
						
						return false;
					}
					_real dist = extEl.SquareDistance();
					if (dist - LinTol() > SSGapSize)
						return true;
					gp_Ax1 f1Dir, f2Dir;
					if (O_f1 == TopAbs_FORWARD)
						f1Dir = f1.Plane().Axis();
					else
						f1Dir = f1.Plane().Axis().Reversed();
					if (O_f2 == TopAbs_FORWARD)
						f2Dir = f2.Plane().Axis();
					else
						f2Dir = f2.Plane().Axis().Reversed();
					
					if(!f1Dir.Direction().IsOpposite(f2Dir.Direction(), AngTol()))
						return false;

					//Blocking direction

					
				
					if (dist<SSContactGapSize)
						contactType=ContactType::_Contact;
					else {
						gp_Vec p1p2(f1Dir.Location(), f2Dir.Location());
						if (p1p2.Dot(f1Dir.Direction())<0)
							contactType=ContactType::_Intersect;
						else 
							contactType = ContactType::_InGap;
					}

					f1VecSpot.push_back(f1Dir);
					f2VecSpot.push_back(f2.Plane().Axis());

					return true;
				}
				else if(f1.GetType()==GeomAbs_Cylinder){

					
					if(f1.Cylinder().Axis().IsCoaxial(f2.Cylinder().Axis(),AngTol(),LinTol())){
						if (Abs(f1.Cylinder().Radius() - f2.Cylinder().Radius())>SSGapSize)
							return true;
						
						_real u1i,u1s,v1i,v1s,u2i,u2s,v2i,v2s;

						BRepTools::UVBounds(f1.Face(), u1i, u1s, v1i, v1s);
						BRepTools::UVBounds(f2.Face(),u2i,u2s,v2i,v2s);

						if(f1.IsUPeriodic() && f2.IsUPeriodic()){
							gp_Pnt pnt1;
							gp_Vec tanU,tanV;
							_int NbStep = 4;
							_real V =0;
							_real Umin = 0;

							BRepAdaptor_Surface *surf;

							_bool IsReverce = false;
							if(f1.UPeriod()<f2.UPeriod()+AngTol()){
								surf = &f1;
								Umin = u1i;
								V	 = (v1s-v1i)/2;	
							}
							else {
								surf = &f2;
								Umin = u2i;
								V	 = (v2s-v2i)/2;
							IsReverce = true;
							}
							
							_real step = (surf->UPeriod())/NbStep;

							for(_int i=1;i<=NbStep;i++){

								surf->D1(Umin,V,pnt1,tanU,tanV);
								tanU.Cross(tanV);
								tanU.Normalize();
								if(IsReverce)
									f1VecSpot.push_back(gp_Ax1(pnt1,gp_Dir(tanU.Reversed())));
								else
									f1VecSpot.push_back(gp_Ax1(pnt1,gp_Dir(tanU)));

								Umin+=step;
							}
						}
						else 
							Standard_Failure::Raise("cylinder is V periodic");

					}
				}
				else if( f1.GetType()==GeomAbs_Cone){

					if(f1.Cone().Axis().IsCoaxial(f2.Cone().Axis(),AngTol(),LinTol())){

						_real u1i, u1s, v1i, v1s, u2i, u2s, v2i, v2s;

						BRepTools::UVBounds(f1.Face(), u1i, u1s, v1i, v1s);
						BRepTools::UVBounds(f2.Face(), u2i, u2s, v2i, v2s);

						_real R_sup1 = f1.Cone().RefRadius();
						_real R_inf1 = R_sup1 - std::sin(f1.Cone().SemiAngle())*(v1s-v1i);
						_real R_sup2 = f2.Cone().RefRadius();
						_real R_inf2 = R_sup2 - std::sin(f2.Cone().SemiAngle())*(v2s-v2i);
						
						if(R_sup1 < R_inf2 - LinTol() || R_sup2 < R_inf1-LinTol())
							return true;

						if(f1.IsUPeriodic() && f2.IsUPeriodic()){

							
							gp_Pnt pnt1;
							gp_Vec tanU,tanV;
							_int NbStep = 4;

							_real V =0;
							_real Umin = 0;
							
							BRepAdaptor_Surface *surf;
							_bool IsReverce = false;
							if(f1.UPeriod()<f2.UPeriod()+AngTol()){
								surf = &f1;
								Umin = u1i;
								V	 = (v1s-v1i)/2;	
							}
							else {
								surf = &f2;
								Umin = u2i;
								V	 = (v2s-v2i)/2;
								IsReverce = true;
							}

							_real step = (surf->UPeriod())/NbStep;

							

							for(_int i=0;i<NbStep;i++){
								surf->D1(Umin,V,pnt1,tanU,tanV);
								tanU.Cross(tanV);
								tanU.Normalize();

								if(IsReverce)
									tanU.Reverse();

								f1VecSpot.push_back(gp_Ax1(pnt1,gp_Dir(tanU)));	
								Umin += step;
							}

							if(f1VecSpot.size()){
								
								gp_Vec vec = f1.Cone().Axis().Direction();
								_real Dot =vec.Dot(f1VecSpot[0].Direction());
								
								if(Dot-AngTol()>0){
									f1VecSpot.push_back(f1.Cone().Axis());
								}
								else {
									f1VecSpot.push_back(f1.Cone().Axis().Reversed());
								}
		
							}
				
						}
				}
					return true;
				}

			}// if(f1.GetType()==f2.GetType())
	
		return false;
}
/*
inline _real ContactSpot::rand(_real min, _real max){
	std::uniform_real_distribution<>::param_type param(min,max);
	return dist(gen,param);
}
inline _real ContactSpot::rand(){
			return dist(gen);
}
*/
