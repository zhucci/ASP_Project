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
		friend class DBGBuilder;

		ContactSpot(SurfaceAttribute &surface1, SurfaceAttribute &surface2, Part* prt1, Part* prt2);

		_bool IsDone(){ return isDone;}

		std::vector<gp_Ax1> &&getF1ContactAxis(){ return std::move(f1VecSpot); }

		std::vector<gp_Ax1> &&getF2ContactAxis(){ return std::move(f2VecSpot);}

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
		
		_bool OverLayAreaPar(_int NbIter, _real delta []);

		_bool OverLayAreaEl();

		_bool OverLayAreaGen();

		_bool SameVectorInSetAlready(gp_Ax1 &axis, std::vector<gp_Ax1> &collection);

		_int CheckPntPairs(std::list<ContactSpot::facePntCorrespond> &mainPnts, _bool ShapeSwap, _int GoodPntRequirement);

	//For random points generation
		inline _real rand(_real min, _real max);

		inline _real rand();


		 _bool IsBiggerSurface(BRepAdaptor_Surface &surf1, BRepAdaptor_Surface &surf2);

		 void InternalBndInit();
		
/*data*/

		BRepAdaptor_Surface &f1,&f2;
		ContactType contactType;
		//SurfaceAttribute &F1, &F2;
		
		Part *prt1, *prt2;

		TopAbs_Orientation O_f1, O_f2;

		_bool ErrorInContactIdentification;

		std::mt19937 gen;
		std::uniform_real_distribution<> dist;

		//Contact spot area bounds
		Bnd_Box2d ContactBnd[2];

		//ContactSpot direction collection
		std::vector<gp_Ax1> f1VecSpot;
		std::vector<gp_Ax1> f2VecSpot;

		_bool isDone;

	};
};
#endif