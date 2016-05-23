
#include "Assembly.h"
#include "Part.h"
#include "XCAFDoc_ShapeTool.hxx"
#include "XCAFDoc_ShapeMapTool.hxx"

using namespace asp;
using namespace std;

Assembly::Assembly(TopoDS_Shape &shape, Unit* root):Unit(shape,root){

	TopoDS_Iterator iter(shape);
	
	for (; iter.More(); iter.Next()){
		auto newShape = iter.Value();
		Unit *subUnit;
		if (newShape.ShapeType()< TopAbs_SOLID){

			subUnit = new Assembly(newShape, this);
			if (!subUnit->IsDone())
				continue;
		}
		else if (newShape.ShapeType() == TopAbs_SOLID){
			subUnit = new asp::Part(newShape, this);
		}
		else {
			continue;
		}
		ColSubUnit.push_back(subUnit);

		if (subUnit->Type() == _Part)
			UnitMap.emplace(subUnit->GetUri(), subUnit);
	}

	if (!ColSubUnit.size())
		return;
	

	MaterialVolumeUpdate();
	BoundVolumeUpdate();
	IsCorrectBuild = true;
}

Assembly::Assembly(const TDF_Label &label, Unit* root/*=NULL*/):
	Unit(label, root)
{
	

	if(myUnitType > unitType::_Assembly)
		Standard_Failure::Raise("Shape must be COMPOUND for"
									"assembly structure");

	if(myUnitType==_Product){
		
		WCS = gp_Ax3(gp_Pnt(0,0,0),gp_Dir(0,0,1));
	
	
		TDF_ChildIterator iter;

		iter.Initialize(myLabel, Standard_False);
		
		//for (; iter.More(); iter.Next()){
		if (iter.More()){
			try{

			//_bool isRef =XCAFDoc_ShapeTool::IsReference(iter.Value());
			//	_bool isAssembly= XCAFDoc_ShapeTool::IsAssembly(iter.Value());
				_bool isShape = XCAFDoc_ShapeTool::IsShape(iter.Value());
				if (isShape)
				{
				TopoDS_Shape shape =XCAFDoc_ShapeTool::GetShape(iter.Value());
				
				//auto newLoc = shape.Location().Multiplied(myLoc);
				//shape.Move(myLoc);
				auto newShapeType = shape.ShapeType();
				
				Unit *subUnit;
				
				if (newShapeType<TopAbs_SOLID){
					subUnit = new Assembly(shape, this);
					
				}
				else if (newShapeType == TopAbs_SOLID){
					subUnit = new asp::Part(shape, this);
				}
				//else {
				///	continue;
				//}
				//
				//	continue;
				if (subUnit->IsDone()){

				ColSubUnit.push_back(subUnit);

					if (subUnit->Type()==_Part)
						UnitMap.emplace(subUnit->GetUri(),subUnit);
					}
				}
			}
			catch(Standard_Failure){
				Handle_Standard_Failure error = Standard_Failure::Caught();
				
					std::cerr<<"Caught in "<<__FILE__<<": "<<__LINE__<<endl;
					std::cerr<<"Msg: "<<error->GetMessageString();		
			}
		}
	}
	else {
		TopoDS_Iterator iter(myshape);
		for (; iter.More(); iter.Next()){
			auto newShape = iter.Value();
			Unit *subUnit;
			if (newShape.ShapeType()< TopAbs_SOLID)

				subUnit = new Assembly(newShape, this);
				
			else if (newShape.ShapeType() == TopAbs_SOLID)
				subUnit = new asp::Part(newShape, this);
			
			else 
					continue;
			
			if (!subUnit->IsDone())
				continue;

			ColSubUnit.push_back(subUnit);
			if (subUnit->Type() == _Part)
				UnitMap.emplace(subUnit->GetUri(), subUnit);
		}
		}
	
	if(!ColSubUnit.size())
		return;

	MaterialVolumeUpdate();
	BoundVolumeUpdate();
	IsCorrectBuild=true;
}
Assembly::~Assembly(){
	for (auto &part : ColSubUnit){
		delete part;
	}
}

std::vector<Unit *>::iterator asp::Assembly::begin()
{
			return ColSubUnit.begin();
}


std::vector<Unit *>::iterator asp::Assembly::end()
{
			return ColSubUnit.end();
}

void Assembly::Dump(Standard_OStream &stream, Standard_Integer rank)
{
	if (IsDone()){
		for(int i=0;i<rank;i++)
			stream<<"    ";
		if(rank)
		stream<<"|"<<endl;
		for(int i=0;i<rank;i++)
			stream<<"    ";
		if(rank)
			stream<<"\\_";
		
		gp_XYZ  c= ShapeCentre.Coord();
		if(!Name==NULL)
			stream<<Name;
		else
			stream<<shapeTypeToString(myShapeType);
		stream<<"  Center :["<<c.X()<<","<<c.Y()<<","<<c.Z();
		Standard_Real x_i,y_i,z_i,x_s,y_s,z_s;
		myBox->Get(x_i,y_i,z_i,x_s,y_s,z_s);
			stream<<"] Bnd:["<<x_i<<","<<x_s<<"],["<<y_i<<","<<y_s<<"],["<<z_i<<","<<z_s<<"]";

		
		stream<<" : ID = "<<myshape.HashCode(_HashValue_);
		stream<<endl;
	//	str << "BV =" << bndBoxVolume << std::endl;
	//	str << "MV=" << unitVolume << std::endl;

		
	for (auto u : ColSubUnit)
	{
		if(u->Type()==_Part)
			dynamic_cast<Part *>(u)->Dump(stream,rank+1);
		
		else 
			dynamic_cast<Assembly *>(u)->Dump(stream,rank+1);
			
	}
	}
}

void Assembly::bndBoxCalc()
{
	try{
		
		myBox = new Bnd_Box;
		auto iter = ColSubUnit.begin();
		auto end = ColSubUnit.end();

		Standard_Real xi, xs, yi, ys, zi, zs;
		Standard_Real Xmin, Xmax, Ymin, Ymax, Zmin, Zmax;
		
		auto box = (*iter)->BndBox();
		box->Get(Xmin, Ymin, Zmin, Xmax, Ymax, Zmax);
		
		for (iter++; iter != end; iter++)
		{
			box = (*iter)->BndBox();
			box->Get(xi, yi, zi, xs, ys, zs);
			Xmin = Xmin < xi ? Xmin : xi;
			Xmax = Xmax > xs ? Xmax : xs;
			Ymin = Ymin < yi ? Ymin : yi;
			Ymax = Ymax > ys ? Ymax : ys;
			Zmin = Zmin < zi ? Zmin : zi;
			Zmax = Zmax > zs ? Zmax : zs;
		}
			myBox->Update(Xmin, Ymin, Zmin, Xmax, Ymax, Zmax);
	}
	catch(Standard_Failure){

		std::cerr<<__FILE__<<": "<<__LINE__<<endl<<"Bnd Box Calc error"<<endl;
	std::cerr<<__FILE__<<": "<<__LINE__<<endl<<"TopAbsEnum: " <<this->myShapeType<<endl;
	
		this->myLabel.Dump(std::cerr);	
	}	
}
void Assembly::BoundVolumeUpdate()
{
	try{
	bndBoxCalc();
	Standard_Real xi, xs, yi, ys, zi, zs;
	myBox->Get(xi, yi, zi, xs, ys, zs);
	bndBoxVolume = (xs - xi)*(ys - yi)*(zs - zi);
	}
	catch(Standard_Failure)
	{
		
		std::cerr<<__FILE__<<": "<<__LINE__<<endl;
		std::cerr<<Standard_Failure::Caught()->GetMessageString()<<endl;
	}
};

void Assembly::MaterialVolumeUpdate()
{
		for (auto u : ColSubUnit)
		{
			unitVolume += u->MaterialVolume();
		}	
}


void Assembly::GetAllParts(Handle_TopTools_HSequenceOfShape col){

if (myUnitType == _Assembly){
	for (Standard_Integer i = 0; i < ColSubUnit.size(); i++){
			col->Append(ColSubUnit[i]->getShape());	
		}
}
else if(myUnitType == _Product){
	auto iter = UnitMap.begin();
		for(;iter!=UnitMap.end();iter++){
			col->Append((*iter).second->getShape());
		}
}
else {
	if (!myshape.IsNull())
		col->Append(myshape);

}
	
}

Part *Assembly::GetSamePart(TopoDS_Shape shape){
	for(auto unit: UnitMap){
		if(unit.second->Type()==_Part &&
			unit.second->getShape().IsEqual(shape))
			return dynamic_cast<Part *> (unit.second);
	}
	return nullptr;
}