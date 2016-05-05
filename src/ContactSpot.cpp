#include "ContactSpot.h"
#include "BRepExtrema_ExtPF.hxx"
#include "TopoDS_Vertex.hxx"
#include "BRepBuilderAPI_MakeVertex.hxx"
#include "LProp3d_SLProps.hxx"
#include "BRepLProp_SLProps.hxx"
#include "Geom2dAdaptor.hxx"
#include "BRepTools.hxx"
#include "Extrema_ExtPS.hxx"
using namespace asp;

ContactSpot::ContactSpot(const BRepAdaptor_Surface &surface1, const BRepAdaptor_Surface &surface2, _real gapSize)
{
	if(gapSize<0)
		gap=SGap();
	else 
		gap = gapSize;

			f1 =surface1;
			f2 = surface2;
			BoundInit(bnd,f1);
			BoundInit(&bnd[4],f2);
			InternalBndInit();
	SquareGap = gap*gap;

	std::random_device init;
		
	gen.seed(init());

		//onf1.Initialize(f1,bnd[0],bnd[1],bnd[2],bnd[3],LinTol(),LinTol());
		//onf2.Initialize(f2,bnd[4],bnd[5],bnd[6],bnd[7],LinTol(),LinTol());
		
	Perform();
}
_real ContactSpot::DistFF(_real *X)
{
	return f1.Value(X[0], X[1]).Distance(f2.Value(X[2], X[3]));
}
ContactSpot::ContactSpot(const SurfaceAttribute &surface1,
	const SurfaceAttribute &surface2, _real gapSize){
	f1 = surface1.surf;
	f2 = surface2.surf;
	O_f1 = surface1.myShape.Orientation();
	O_f2 = surface2.myShape.Orientation();
	
	if (gapSize<0)
		gap = SGap();
	else
		gap = gapSize;

	BoundInit(bnd, f1);
	BoundInit(&bnd[4], f2);
	InternalBndInit();
	SquareGap = gap*gap;

	Perform();
}
void ContactSpot::Perform(){

	_real delta[4];
	
	ImportantSpotOfContact(f1,delta);
	ImportantSpotOfContact(f2,&delta[2]);

	//Try simple some lighter algorithm 
	//if(OverLayAreaEl(delta))
	//	ErrorInContactIdentification=false;

	if(OverLayAreaGen())
		isDone=true;
	//else 
	//	ErrorInContactIdentification=true;
	/*
	extrema.Initialize(f2,bnd[4],bnd[5],bnd[6],bnd[7],LinTol());
	extrema.Perform(f1,bnd[4],bnd[5],bnd[6],bnd[7],LinTol());
	
	if(extrema.IsDone())
		if(extrema.IsParallel()){

			_real theorDist = extrema.SquareDistance(1);

			if(theorDist<SquareGap)
				return;

			

		//General algorithm for parallel surface
				OverLayAreaPar(10,delta);

			
		}

		else{  //Not parallel surfaces

		
			
		} 
		*/
}
_bool ContactSpot::OverLayAreaPar(_int NbIter, _real delta []){
	_real point[2];
	try{
				point[0] = rand(bnd[0],bnd[1]);
				point[1] = rand(bnd[2],bnd[3]);

				gp_Pnt pnt = f1.Value(point[0],point[1]);
				Extrema_ExtPS ext(pnt,f2,bnd[4],bnd[5],bnd[6],bnd[7],0.1,0.1,Extrema_ExtFlag_MIN);
				if(ext.IsDone()){
					
					_int NbExtrem = ext.NbExt();
					if(NbExtrem ){
						_real U,V;
					ext.Point(1).Parameter(U,V);
					
					TopOpeBRep_PointClassifier classif;
					//classif.Load(f1.Face);
					TopAbs_State state = classif.Classify(f1.Face(),gp_Pnt2d(U,V),LinTol());
					if(state == TopAbs_IN || state == TopAbs_ON )
						if(ext.SquareDistance(1)+LinTol()>Max(gap,extrema.SquareDistance(1)))
						{
						_real d = ext.SquareDistance(1);
						}
					Extrema_ExtPS ext2(ext.Point(1).Value(),f1,0.1,0.1,Extrema_ExtFlag_MIN);
					if(ext2.IsDone())
						ext2.Point(1).Parameter(U,V);
					classif.Load(f1.Face());
					state = classif.Classify(f1.Face(),gp_Pnt2d(U,V),LinTol());
					if(state == TopAbs_IN || state == TopAbs_ON )
					if(ext2.SquareDistance(1)+LinTol()>Max(gap,extrema.SquareDistance(1)))
						{
						_real d = ext.SquareDistance(1);
						}
					}
				}
				return true;
	}
	catch(Standard_Failure)
	{
		std::cerr<<__FILE__<<": "<<__LINE__<<":"<<Standard_Failure::Caught()->GetMessageString()<<endl;
	}
	}

_bool ContactSpot::OverLayAreaGen(){

	/*We have some points right now*/
	//try{

		//f1 - surface 1
		//f2 - surface 2
		//which surface has the longest edge perimeter 

		_real f1_perimeter =0;
		_real f2_perimeter =0;
		_int NeedPntAmount =5;
		GProp_GProps propsTester;
		BRepGProp::LinearProperties(f1.Face(),propsTester);
		f1_perimeter=propsTester.Mass();
		
		BRepGProp::LinearProperties(f2.Face(), propsTester);
		f2_perimeter = propsTester.Mass();
		BRepAdaptor_Surface * smlSurf;
		BRepAdaptor_Surface * bgSurf;
		Standard_Boolean smlFaceOrientation, bgFaceOrientation;
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
		

		TopExp_Explorer edgeExp;
		for (edgeExp.Init(smlSurf->Face(), TopAbs_EDGE); edgeExp.More(); edgeExp.Next()){
			TopLoc_Location loc;
			_real ui, us;
			
			//Handle_Geom_Curve curv = BRep_Tool::Curve(TopoDS::Edge(edgeExp.Current()), loc, ui, us);
			Handle_Geom2d_Curve faceCurve = BRep_Tool::CurveOnSurface(TopoDS::Edge(edgeExp.Current()), smlSurf->Face(), ui, us);
			
			_real dlt = (us - ui) / AmtPntForCS();
			for (_int k = 0; k<AmtPntForCS(); k++){
				ContactSpot::facePntCorrespond corPnts;
				faceCurve->D0(dlt*k,corPnts.pnt2d.first);

				gp_Vec tanU, tanV;
				
				smlSurf->D1(corPnts.pnt2d.first.X(), corPnts.pnt2d.first.X(), corPnts.pnt3d.first, tanU, tanV);
						tanU.Cross(tanV);

					if (smlFaceOrientation != TopAbs_FORWARD)
							tanU.Reverse();
					corPnts.normal.first=tanU;
			
				mainPnts.push_back(corPnts);
				}
			}
		/*goto End;
		//Extremum points on bigger face

		BRepExtrema_ExtPF extBuilder;
		extBuilder.Initialize(bgSurf->Face(),Extrema_ExtFlag_MIN,Extrema_ExtAlgo_Grad);
		for (auto p = mainPnts.begin(); p != mainPnts.end();){
			extBuilder.Perform(BRepBuilderAPI_MakeVertex(p->pnt3d.first),bgSurf->Face());
			if (extBuilder.IsDone() && extBuilder.NbExt()){
				
				p->pnt3d.second = extBuilder.Point(1);
				_real U, V;
				extBuilder.Parameter(1, U, V);
				gp_Pnt2d pnt2d(U,V);
				p->pnt2d.second=pnt2d;

				gp_Pnt pntOnF1;
				gp_Vec tanU,tanV;
				bgSurf->D1(U, V, pntOnF1, tanU, tanV);
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

	
		//Extremum points back to initial face
		_int GoodPntAmount = 0;
		extBuilder.Initialize(smlSurf->Face(), Extrema_ExtFlag_MIN, Extrema_ExtAlgo_Grad);
		for (auto p = mainPnts.begin(); p != mainPnts.end();){
			gp_Vec p1p2(p->pnt3d.first,p->pnt3d.second);

			_real magnitude = p1p2.Magnitude();
			if (magnitude<Precision::Confusion()){
				GoodPntAmount++;
				p->correctPair = true;
				p++;
				continue;
			}
			extBuilder.Perform(BRepBuilderAPI_MakeVertex(p->pnt3d.second), smlSurf->Face());
			if (extBuilder.IsDone() && extBuilder.NbExt()){
				_real U, V;
				extBuilder.Parameter(1, U, V);
				gp_Pnt2d pnt2d(U, V);
				gp_Pnt pntOnF1;
				gp_Vec tanU, tanV;
				bgSurf->D1(U, V, pntOnF1, tanU, tanV);
				tanU.Cross(tanV);
				if (smlFaceOrientation != TopAbs_FORWARD)
					tanU.Reverse();
				tanU.Normalize();

				p->normal.first = tanU;
				p->pnt3d.first = pntOnF1;
				p->pnt2d.first = pnt2d;

				p++;
			}
			else
				p = mainPnts.erase(p);
		}

		if (GoodPntAmount<NeedPntAmount)
	for (auto p = mainPnts.begin(); p != mainPnts.end();p++){
		if (p->correctPair)
			continue;
		gp_Vec p1p2(p->pnt3d.first, p->pnt3d.second);

		_real magnitude = p1p2.Magnitude();

		if (magnitude<RealSmall()){
			p->correctPair=true;
			GoodPntAmount++;
			continue;
		}
		else{
			_real p1p2_n1 = p1p2.Dot(p->normal.first);
			p1p2.Reverse();
			_real p2p1_n2 = p->normal.second.Dot(p1p2); 
			if (magnitude<SGap() && p1p2_n1 + ParTol()>1 && p2p1_n2+ParTol()>1){
				p->correctPair = true;
				GoodPntAmount++;
				continue;
			}
			if (p1p2_n1 - ParTol()<-1 && p2p1_n2 - ParTol()<-1){
				p->correctPair = true;
				GoodPntAmount++;
				continue;
			}

		}	
	}
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

		if (Square1 > MinContactSquare() && Square2 > MinContactSquare()){
			*/
		//End :


				for (auto p = mainPnts.begin(); p != mainPnts.end();p++){
				//	if (p->correctPair){
							gp_Ax1 sS(p->pnt3d.first,p->normal.first);
							gp_Ax1 gS(p->pnt3d.first, p->normal.first);
							gp_Ax1 *f1Axis = AreFacesSwaped ? &sS : &gS;
							gp_Ax1 *f2Axis = AreFacesSwaped ? &gS : &sS;
						//	if (!SameVectorInSetAlready(*f1Axis, f1VecSpot)){
								f1VecSpot.push_back(*f1Axis);
								f2VecSpot.push_back(*f2Axis);
						//	}
						
				//}
			}

			return true;
	//	}
			
	//}

	//}
	//catch(Standard_Failure)
	//{
	//	std::cerr<<__FILE__<<": "<<__LINE__<<":"<<Standard_Failure::Caught()->GetMessageString()<<endl;
	//}
	//return false;
	
}
_bool ContactSpot::SameVectorInSetAlready(gp_Ax1 &axis, std::vector<gp_Ax1> &collection){
	for (auto &ax : collection){
		if (ax.Direction().Dot(axis.Direction())+AsmVecTol()>1)
			return true;
	}
}
_bool  ContactSpot::IsOverlaySS(_int NbIter, Extrema_POnSurf &p1, Extrema_POnSurf &p2){
	try{
	_real theorDist =0;
	_real SSPntPair[4];

	_real delta[4];
	
	ImportantSpotOfContact(f1,delta);
	ImportantSpotOfContact(f2,&delta[2]);

	extrema.Initialize(f2,bnd[4],bnd[5],bnd[6],bnd[7],LinTol());
	extrema.Perform(f1,bnd[4],bnd[5],bnd[6],bnd[7],LinTol());
	
	if(extrema.IsDone())
		if(extrema.IsParallel()){

			_real theorDist = sqrt(extrema.SquareDistance(1));

			if(theorDist<gap)
				return false;

			if(IsSimpleContact()){
				/*Try simple some lighter algorithm */
			
			}
			
		/*General algorithm for parallel surface*/
			_real point[2];

				point[0] = rand(bnd[0],bnd[1]);
				point[1] = rand(bnd[2], bnd[3]);
				
		
			
				gp_Pnt pnt = f1.Value(point[0],point[1]);
				Extrema_ExtPS ext(pnt,f2,0.1,0.1,Extrema_ExtFlag_MIN);
				if(ext.IsDone()){
					_real U,V;
					
					ext.Point(1).Parameter(U,V);
					
					TopOpeBRep_PointClassifier classif;
					//classif.Load(f1.Face);
					TopAbs_State state = classif.Classify(f1.Face(),gp_Pnt2d(U,V),LinTol());
					if(state == TopAbs_IN || state == TopAbs_ON )
						if(ext.SquareDistance(1)+LinTol()>Min(gap,theorDist))
						{
						_real d = ext.SquareDistance(1);
						}
					Extrema_ExtPS ext2(ext.Point(1).Value(),f1,0.1,0.1,Extrema_ExtFlag_MIN);
					if(ext2.IsDone())
						ext2.Point(1).Parameter(U,V);
					classif.Load(f1.Face());
					state = classif.Classify(f1.Face(),gp_Pnt2d(U,V),LinTol());
					if(state == TopAbs_IN || state == TopAbs_ON )
					if(ext2.SquareDistance(1)+LinTol()>Min(gap,theorDist))
						{
						_real d = ext.SquareDistance(1);
						}

				}
		}
	
	
		/*We find some extrema right now*/
		
	for(_int i=1;i<=extrema.NbExt();i++)
	{
		if(extrema.SquareDistance(i)>SquareGap)
			continue;
		Extrema_POnSurf pnt1 ,pnt2;
		extrema.Points(i,pnt1,pnt2);
		gp_Vec tanU,tanV;
		_real U,V;
		gp_Pnt pntOnSurf;
		pnt1.Parameter(U,V);
		f1.D1(U,V,pntOnSurf,tanU,tanV);
		gp_Vec blockDir(pnt1.Value(),pnt2.Value());

		if(tanU.Crossed(tanV).IsParallel(gp_Vec(pnt1.Value(),pnt2.Value()),ParTol()))
			f1VecSpot.push_back(gp_Ax1(pntOnSurf,gp_Dir(blockDir)));
		
	}
	if(!f1VecSpot.size())
		return false;
		
	}
	catch(Standard_Failure)
	{
		
		std::cerr<<__FILE__<<": "<<__LINE__<<":"<<Standard_Failure::Caught()->GetMessageString()<<endl;
	}
	return true;
}

_bool ContactSpot:: OverLayAreaEl( _real delta []){

	
	_real SSPntPair[4];

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
						return true;
					if (!extEl.IsParallel())
						return true;
					_real dist = extEl.SquareDistance();
					if (dist - LinTol() > SquareGap)
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
					
					if(	!f1Dir.Direction().IsOpposite(f2Dir.Direction(), AngTol()))
						return true;

					//Blocking direction
					
					f1VecSpot.push_back(f1Dir);
					//f2VecSpot.push_back(f2.Plane().Axis());

					return true;
				}
				else if(f1.GetType()==GeomAbs_Cylinder){

					
					if(f1.Cylinder().Axis().IsCoaxial(f2.Cylinder().Axis(),AngTol(),LinTol())){
						if(Abs(f1.Cylinder().Radius()-f2.Cylinder().Radius())>gap)
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
						_real R_sup1 = f1.Cone().RefRadius();
						_real R_inf1 = R_sup1 - std::sin(f1.Cone().SemiAngle())*(v1s-v1i);
						_real R_sup2 = f2.Cone().RefRadius();
						_real R_inf2 = R_sup2 - std::sin(f2.Cone().SemiAngle())*(v2s-v2i);
						
						if(R_sup1 <R_inf2 - LinTol() || R_sup2 < R_inf1-LinTol())
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

void ContactSpot::ImportantSpotOfContact(BRepAdaptor_Surface &surf, Standard_Real uvProportion[]){
	 _real minLin = 1;/*for 1 mm part */

	 _real minRad = 0.175; 
	
	 switch (surf.GetType()){
		 case GeomAbs_Plane :
			 uvProportion[0] = minLin;
			 uvProportion[1] = minLin;
			 break;
			case GeomAbs_Cylinder :
				uvProportion[0] = minRad;
				uvProportion[1] = minLin;
			 break;
			case GeomAbs_Cone :
				uvProportion[0] = minRad;
				uvProportion[1] = minLin;
				break;
			case GeomAbs_Sphere :
				uvProportion[0] = minRad;
				uvProportion[1] = minRad;
				break;
			case GeomAbs_Torus :
				uvProportion[0] = minRad;
				uvProportion[1] = minRad;
				break;
			case GeomAbs_SurfaceOfExtrusion :
				uvProportion[0] = minRad;
				uvProportion[1] = minLin;
				break;
			default :
				uvProportion[0] = minLin;
				uvProportion[1] = minLin;
				break;
	 }
	
 }
inline _real ContactSpot::rand(_real min, _real max){
	std::uniform_real_distribution<>::param_type param(min,max);
	return dist(gen,param);
}
inline _real ContactSpot::rand(){
			return dist(gen);
}

_real ContactSpot::Value(_real *X,_bool *IsOnBound /*bool [8]*/){
				if(X[0]-LinTol()<bnd[0])
					X[0]=bnd[0], IsOnBound[0]=true;
				else if(X[0]+LinTol()>bnd[1])
					X[0]=bnd[1],IsOnBound[1] = true;
				if(X[1]<bnd[2])
					X[1]=bnd[2], IsOnBound[2] = true;
				else if(X[1]>bnd[3])
					X[1]=bnd[3], IsOnBound[3] = true;
				if(X[2]<bnd[4])
					X[2]=bnd[4], IsOnBound[4]=true;
				else if(X[2]>bnd[5])
					X[2]=bnd[5],IsOnBound[5] = true;
				if(X[3]<bnd[6])
					X[3]=bnd[6], IsOnBound[6] = true;
				else if(X[3]>bnd[7])
					X[3]=bnd[7], IsOnBound[7] = true;
					
				return f1.Value(X[0],X[1]).Distance(f2.Value(X[3],X[4]));
}

 inline	_real ContactSpot::Value(_real *X)
{
	return f1.Value(X[0],X[1]).Distance(f2.Value(X[2],X[3]));
}
 _bool ContactSpot::IsSimpleContact()
 {
	 Standard_Failure::Raise("Not implement function use");
	 return true;
 }
 void ContactSpot::InternalBndInit(){
	u1i = f1.FirstUParameter(); //U parameter from first surface infimum value
	u1s = f1.LastUParameter(); // U supremum
	v1i = f1.FirstVParameter();
	v1s = f1.LastVParameter();
	u2i = f2.FirstUParameter(); 
	u2s = f2.LastUParameter(); 
	v2i = f2.FirstVParameter();
	v2s = f2.LastVParameter();
}
 _bool ContactSpot::IsBiggerSurface(BRepAdaptor_Surface &f1, BRepAdaptor_Surface &f2)
 {
	 //it is incorrect version but simple
	if(f1.LastUParameter()-f1.FirstUParameter()+f1.LastVParameter()-f1.FirstVParameter() >
		f2.LastUParameter()-f2.FirstUParameter()+f2.LastVParameter()-f2.FirstVParameter())
		return true;
	return false; 		
 }

void ContactSpot::BoundInit(_real *bnd, BRepAdaptor_Surface &f1){
	
	bnd[0] = f1.FirstUParameter();
	bnd[1] = f1.LastUParameter();
	bnd[2] =  f1.FirstVParameter();
	bnd[3] = f1.LastVParameter();
 }