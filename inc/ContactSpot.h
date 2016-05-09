#ifndef _ContactSpot_
#define _ContactSpot_

#include "OCCT_Headers.h"
#include "STL_Headers.h"
#include "FoundationClasses.h"
#include	"SurfaceAttribute.h"
#include "BRepExtrema_ExtPF.hxx"
namespace asp{

	class ContactSpot 
	{
	public:

		friend class AspMainTest;

			ContactSpot(const BRepAdaptor_Surface &surface1, 
				const BRepAdaptor_Surface &surface2);
			ContactSpot(const SurfaceAttribute &surface1,
				const SurfaceAttribute &surface2, Part* prt1, Part* prt2);
			_bool IsDone(){
				return isDone;
			}

			std::vector<gp_Ax1> &&getF1ContactAxis(){
				return std::move(f1VecSpot);
			}
			std::vector<gp_Ax1> &&getF2ContactAxis(){
				return std::move(f2VecSpot);
			}
			ContactType GetContactType(){return contactType;}
	private:
		_int AmtPntForCS{ 5 };
		_real MinContactSquare{ 4 };
		_real MinContactPntDistance{ 1 };
		_real MinContactPntAngDistance{ M_PI / 3 };
		_real ContDirParPrecision{ 0.05};
		Standard_Real SSGapSize { 4 };
		Standard_Real SSCrossSize{ 4 };
		Standard_Real SSContactGapSize{1e-1};

		struct facePntCorrespond{
			facePntCorrespond():correctPair{ false }{}
			std::pair<gp_Pnt,gp_Pnt> pnt3d;
			std::pair<gp_Pnt2d, gp_Pnt2d> pnt2d;
			std::pair<gp_Vec,gp_Vec> normal;
			Standard_Boolean correctPair;
			ContactType contType;
		};
		_bool PntOnSurfGenerate(BRepAdaptor_Surface *surf, TopAbs_Orientation surfOrient, std::list<facePntCorrespond> *corPnts);
		_bool PntOnFaceEdgesGenerate(BRepAdaptor_Surface *surf, TopAbs_Orientation surfOrient, std::list<facePntCorrespond> *corPnts);
		void Perform();

		void IsNear(_real *Direction, _real GapAllowSize,_int MaxIter);

		_bool IsSimpleContact();
		//!Hooke - Jeeves direct search
		_bool OverLayAreaPar(_int NbIter, _real delta []);

		_bool OverLayAreaEl();

		_bool OverLayAreaGen();

		_bool SameVectorInSetAlready(gp_Ax1 &axis, std::vector<gp_Ax1> &collection);

		_int CheckPntPairs(std::list<ContactSpot::facePntCorrespond> &mainPnts, _bool ShapeSwap, _int GoodPntRequirement);

		//!Compute distance between 2 points on the surface 
		inline _real DistFF(_real *X);

		//! Pattern search like Hooke - Jeeves 
		void ImportantSpotOfContact(BRepAdaptor_Surface &surf, Standard_Real uvProportion[]);

		_bool  IsOverlaySS(_int NbIter, Extrema_POnSurf &p1, Extrema_POnSurf &p2);

		inline _real rand(_real min, _real max);

		inline _real rand();


		 _bool IsBiggerSurface(BRepAdaptor_Surface &surf1, BRepAdaptor_Surface &surf2);
		 void InternalBndInit();
		
/*data*/

		BRepAdaptor_Surface f1,f2;
		ContactType contactType;
		SurfaceAttribute F1, F2;
		
		Part *prt1, *prt2;

		TopAbs_Orientation O_f1, O_f2;

		_bool ErrorInContactIdentification;

		Extrema_ExtSS extrema;

		//Extrema_ExtPS onf1;
		//Extrema_ExtPS onf2;

		Standard_Real bnd[8];
		Standard_Real u1i,u1s,v1i,v1s,u2i,u2s,v2i,v2s;
		std::mt19937 gen;
		std::uniform_real_distribution<> dist;

		_bool isSetGap;
		_bool isReverseStorage;
		_real gap;
		_real SquareGap;

		/*Contact spot area bounds*/
		_real ContactBnd[8];
		/*Step to explorer area*/
		_real delta[4];
		/*Exploretion result*/
		std::vector<gp_Ax1> f1VecSpot;
		std::vector<gp_Ax1> f2VecSpot;
		
		_real bestGap;
		_bool isDone;

	};
};
#endif