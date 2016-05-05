#ifndef _surfaceAttribute
#define _surfaceAttribute

#include "OCCT_Headers.h"
#include "STL_Headers.h"
#include "FoundationClasses.h"
namespace asp
{
	
	class Part;

	enum ContactType {_Out, _InGap, _Contact, _Intersect, _Into, _WrapIt, _UnknownContact};
	enum RelationType {_Precedence, _BlockDir, _Kinematic_Plane, _Kinematic_Cylinder, _Kinematic_Cone};
	enum StateOfDir {_MoveBlk, _MoveFree,_ContactBlk,_ContactFree};
	enum SurfaceFunction {_UnknownFunction, _Base, _Grip, _Kinematic, _Default};

//!Surface to Surface adjacency 
//! 'cos'  = cos(Norm_s1 ^ Norm_s2)
	struct AdjacencySS{
		AdjacencySS(Standard_Real _cos, Standard_Integer surfAttributeNumber){
			cos=_cos;
			surf=surfAttributeNumber;
		}

		//AdjacencySS() = delete;
		//AdjacencySS(AdjacencySS &adj)=delete;
		~AdjacencySS(){}
		Standard_Real cos;
		Standard_Integer surf;
	};
//! 'surf' (Surface) -- is a --> TopoDS_Shape 'myShape'
//! 'myBox' this Bnd_Box alighed to WCS  
//! 'dir' is a main assemble direction
//! 'Type' is a surface type (Plane, Cylinder, Cone ...)
//! 'colOfAdjSurf' vector of contacts with adjacence faces
//! 'Func' is a SurafaceFunction enum (_Unknown, _Base, _Grip, ...)
	struct SurfaceAttribute {

		SurfaceAttribute(const TopoDS_Shape &surfShape);
		SurfaceAttribute(SurfaceAttribute &att);
		SurfaceAttribute &SurfaceAttribute::operator=(SurfaceAttribute &att);
	
		BRepAdaptor_Surface surf;
		TopoDS_Face myShape;
		Bnd_Box myBox;
		gp_Dir dir;
		Standard_Integer uri;
		GeomAbs_SurfaceType Type;
		
		std::vector<AdjacencySS> colOfAdjSurf;
		SurfaceFunction Func;
	private:
		static Standard_Integer counter;
	};

//! SurfaceNumber (from colOfSurf) -- Type --> SurfNum (from obstacle::colOfCont)*/
	struct Contact{
		
//! S1 - this part surface
//! P1 - obstacle part 
//! S2 - surface from obstacle part
//! 'type' - contact type
		Contact(_int S1, _int P1, _int S2, ContactType type){
			Type=type;
			obstURI=P1;
			MySurfNumber=S1;
			ObstSurfNumber=S2;
		}

		ContactType Type;
		Standard_Integer obstURI;
		Standard_Integer MySurfNumber;
		Standard_Integer ObstSurfNumber;
	};


}; /*asp namespace*/
#endif /*ifndef _surfaceAttribute*/