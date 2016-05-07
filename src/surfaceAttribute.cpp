#include "surfaceAttribute.h"

using namespace asp;

Standard_Integer SurfaceAttribute::counter = 0;

SurfaceAttribute::SurfaceAttribute(const TopoDS_Shape &surfShape)
{
	
	myShape = TopoDS::Face(surfShape);

	surf.Initialize(myShape);
	
	Func=_Default;

	Type = surf.GetType();

	//uri= myShape.HashCode(_HashValue_);
	uri = counter++;
	//dir=surf.Direction();
	BRepBndLib::Add(surfShape,myBox);
}

SurfaceAttribute::SurfaceAttribute(const SurfaceAttribute &att):
	myBox(att.myBox),Type(att.Type),surf(att.surf),myShape(att.myShape),Func(att.Func),
	uri(att.uri)
{
	for(auto con : att.colOfAdjSurf)
		colOfAdjSurf.push_back(con);
	
}

SurfaceAttribute &SurfaceAttribute::operator=(const SurfaceAttribute &att)
{
	uri=att.uri;
	myShape=att.myShape;
	myBox=att.myBox;
	Type=att.Type;
	surf=att.surf;
	Func=att.Func;
	for(auto con : att.colOfAdjSurf)
		colOfAdjSurf.push_back(con);
	return *this;
}