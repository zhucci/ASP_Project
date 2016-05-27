#include "surfaceAttribute.h"

using namespace asp;

Standard_Integer SurfaceAttribute::counter = 0;

SurfaceAttribute::SurfaceAttribute(const TopoDS_Shape &surfShape)
{
	surf.Initialize(TopoDS::Face(surfShape));
	Func=_Default;
	uri = counter++;
	BRepBndLib::Add(surfShape,myBox);
}

SurfaceAttribute::SurfaceAttribute(const SurfaceAttribute &att):
	myBox(att.myBox),surf(att.surf),Func(att.Func),
	uri(att.uri)
{}

SurfaceAttribute &SurfaceAttribute::operator=(const SurfaceAttribute &att)
{
	uri=att.uri;
	myBox=att.myBox;
	surf=att.surf;
	Func=att.Func;
	return *this;
}