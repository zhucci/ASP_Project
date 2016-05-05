#include "ContactSpot.h"


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
	if(OverLayAreaEl(delta))
		ErrorInContactIdentification=false;
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

		
			OverLayAreaGen(delta);	
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

_bool ContactSpot::OverLayAreaGen(_real delta[]){

	/*We have some points right now*/
	try{
	for(_int i=1;i<=extrema.NbExt();i++)
	{
				if(extrema.SquareDistance(i)>SquareGap){
					continue;
				}
				else {
				Extrema_POnSurf pnt1 ,pnt2;
				extrema.Points(i,pnt1,pnt2);
				gp_Vec tanU,tanV;
				_real U,V;
				gp_Pnt pntOnF1;
				pnt1.Parameter(U,V);
				f1.D1(U,V,pntOnF1,tanU,tanV);

				if(extrema.SquareDistance(i)-LinTol()>0){
					gp_Vec blockDir(pnt1.Value(),pnt2.Value());
					if(!tanU.Crossed(tanV).IsParallel(gp_Vec(pnt1.Value(),pnt2.Value()),AsmVecTol()))
						continue;
					else{
						/*Area investigation*/
						f1VecSpot.push_back(gp_Ax1(pnt1.Value(),gp_Dir(blockDir)));
					}
				}
				else {
					gp_Vec tanU2,tanV2;
					_real U2,V2;
					gp_Pnt pntOnF2;
					pnt2.Parameter(U2,V2);
					f2.D1(U2,V2,pntOnF2,tanU2,tanV2);
					tanU.Cross(tanV);
					if(!tanU2.Crossed(tanV2).IsParallel(tanU,AsmVecTol()))
						continue;
					else 
					{
						//Area investigation
						f1VecSpot.push_back(gp_Ax1(pntOnF2,gp_Dir(tanU)));

						ContactBnd[0]=ContactBnd[1]=U;
						ContactBnd[2]=ContactBnd[3]=V;
						ContactBnd[4]=ContactBnd[5]=U2;
						ContactBnd[6]=ContactBnd[7]=V2;

					}
				}
				}
				

		
	}
	}
	catch(Standard_Failure)
	{
		std::cerr<<__FILE__<<": "<<__LINE__<<":"<<Standard_Failure::Caught()->GetMessageString()<<endl;
	}
	return true;
	
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