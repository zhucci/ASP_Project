#include "Unit.h"
using namespace asp;


std::map<Standard_Integer,Unit *> Unit::UnitMap;

gp_Ax3 Unit::WCS;

_int Unit::UnitNumberCounter(1);

Unit::Unit(const TopoDS_Shape &shape, Unit *root):
IsCorrectBuild{ false }
{
	
	myshape = shape;
	uri = UnitNumberCounter++;
	bndBoxVolume = 0.;
	unitVolume = 0.;
	mass = 0.0;
	myShapeType = myshape.ShapeType();
	
	Name=nullptr;
	myLoc = shape.Location();
	myBox=nullptr;
	root=nullptr;
	if(myShapeType==TopAbs_COMPOUND)
	{ 
		myUnitType = unitType::_Assembly;	
	}
	else if (myShapeType==TopAbs_SOLID){
		myUnitType = unitType::_Part;	
	}
	else if(myShapeType == TopAbs_FACE){
		myUnitType = unitType::_Face;
	}
	else{
		printf("Error Unit type detect : %d!!!!!!\n", myShapeType);
		Standard_Failure::Raise("Shape type is unsuitable for unit");
	}
}


Unit::Unit(const  TDF_Label &label, Unit *root):
IsCorrectBuild{ false }, myLabel{label}
{
	
	if(label.IsNull()){
		Standard_Failure::Raise("Label is null");
	}
	
	uri = UnitNumberCounter++;
	bndBoxVolume = 0.;
	unitVolume = 0.;
	mass = 0.0;
	
	Name = nullptr;
	myBox = nullptr;
	root = nullptr;
		
	//Get shape from its label
	
	if (myLabel.Depth()>_RootShapeDepth_){

		tools = XCAFDoc_DocumentTool::ShapeTool(myLabel);

		if(tools->IsShape(myLabel)){
			myshape = XCAFDoc_ShapeTool::GetShape(myLabel);
			

			myShapeType = myshape.ShapeType();
			myLoc = myshape.Location();
			if(myShapeType==TopAbs_SOLID)
				myUnitType=unitType::_Part;
			else if(myShapeType==TopAbs_COMPOUND)
				myUnitType=unitType::_Assembly;	
		}
		else
			Standard_Failure::Raise("Label is not a shape");
	}
	else if(myLabel.Depth()==_RootShapeDepth_){
		tools==NULL;	 
		myShapeType = TopAbs_COMPOUND;
		myLoc = XCAFDoc_ShapeTool::GetLocation(myLabel);
		myUnitType = unitType::_Product;	
	}

	Name = ReadName(myLabel);

	myLoc = XCAFDoc_ShapeTool::GetLocation(myLabel);
	
	bndBoxVolume = 0.;
	unitVolume = 0.;
	mass = 0.0;


}

void Unit::Dump(Standard_OStream &stream, Standard_Integer rank)
{
		for(int i=0;i<rank;i++)
			stream<<"    ";
		if(rank)
		stream<<"|"<<endl;
		for(int i=0;i<rank;i++)
			stream<<"    ";
		if(rank)
			stream<<"\\_";
		
		
		if(Name!=NULL)
			stream<<Name;
}
/*
void TreeNodeExp()
{
	TDF_Label myLabel;
	Handle_TDF_Attribute att;
	if(myLabel.FindAttribute(TDataStd_TreeNode::GetDefaultTreeID(),att))
	{
		auto tree = Handle_TDataStd_TreeNode::DownCast(att);
		tree->Dump(std::cout);
	}
}
*/