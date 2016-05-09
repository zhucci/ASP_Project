#ifndef _Unit_def
#define _Unit_def

#include "OCCT_Headers.h"
#include "STL_Headers.h"
#include "FoundationClasses.h"
#include "surfaceAttribute.h"

//! ASP - assembly sequence planning namespace for classes
namespace asp
{


//! Unit class has a common functionality for Assembly and Part classes 
//! Unit class has information about 
//! TopoDS_Shape
//! Bnd_box storing and calculation
//! Moving and rotating functions
	enum unitType { _Product, _Assembly, _Part, _Face, _Edge, _Vertex };

	class UnitInterface{
	public:
		virtual TopoDS_Shape getShape()=0;
		virtual unitType Type()=0;
		virtual Bnd_Box *BndBox()=0;
		virtual void Dump(Standard_OStream &str, Standard_Integer rank=0)=0;
		
	};

	
class Unit : public UnitInterface
{
	public:	
		friend class AspMainTool;
		friend class AspMainTest;
//! If start from Root label It will get a first founded shape and make all
//! assembly tree
	Unit(const TDF_Label &label, Unit* root=NULL);

	Unit::Unit(const TopoDS_Shape &shape, Unit *root=nullptr);

	~Unit(){ }

	TopoDS_Shape getShape(){return myshape;}

	std::map<Standard_Integer, Unit *> *GetUnitMap(){
		return &UnitMap;
	}
	Standard_Integer GetPartAmount(){
		return Standard_Integer(UnitMap.size());
	}
//! enum {_Product, _Assembly, _Part}
	unitType Type() { return myUnitType; }

				
	Bnd_Box *BndBox(){return myBox;}

	gp_Pnt GetCenter(){return ShapeCentre;}
	
//! Real volume of unit
//! Return -1 if Volume is not calculated
	Standard_Real MaterialVolume() { if(check)
									return unitVolume; 
										else return -1;}
//! Volume is wraped by bounding box
//! Return -1 if Volume is not calculated
	Standard_Real BoundVolume() { if(check) 
										return bndBoxVolume;
									  else return -1;}

//! TopoDS_Shape::Location()::Transformation()
	const gp_Trsf &GetTrsf(){return myLoc.Transformation();}

//! Universale resource identifier
//! Unique for all nodes in product
		Standard_Integer GetUri(){return uri;}

		Standard_PCharacter GetName(){return Name;}
	virtual void Dump(Standard_OStream &str, Standard_Integer rank=0);	
		
	protected:

	/*Functions*/

		Standard_Real ContactDist(){return RealSmall();}

		Standard_Real FaceDistance(){return Precision::Intersection();}
	/*DATA*/
		Unit *root;

		static _int UnitNumberCounter;//for uri calculation

		TDF_Label myLabel;

		static std::map<Standard_Integer,Unit *> UnitMap;

		static gp_Ax3 WCS;

		Handle(XCAFDoc_ShapeTool) tools;

		Standard_Integer uri;
		Standard_PCharacter Name;

		unitType myUnitType;
		TopAbs_ShapeEnum myShapeType;

		TopoDS_Shape myshape;
		TopLoc_Location myLoc;
//! Volume calculation status
		Standard_Boolean check;
		Bnd_Box *myBox;
		Standard_Real bndBoxVolume;
		Standard_Real unitVolume;
		gp_Pnt ShapeCentre;
		Standard_Real mass;

		
		
	};
};

#endif