#include "ISession_Part.h"
#include <Prs3d_LineAspect.hxx>
#include <StdPrs_HLRPolyShape.hxx>
#include <StdPrs_ShadedSurface.hxx>
#include <StdPrs_WFPoleSurface.hxx>
#include <StdPrs_WFSurface.hxx>
#include <Prs3d_Text.hxx>



//IMPLEMENT_STANDARD_HANDLE(asp::ISession_Part, AIS_InteractiveObject)
//IMPLEMENT_STANDARD_RTTIEXT(asp::ISession_Part, AIS_InteractiveObject)

using namespace asp;
ISession_Part::ISession_Part(Part *part):AIS_InteractiveObject(),
myPart(part)
{
	PartName.Insert(0, part->GetName());
}

ISession_Part::~ISession_Part()
{
}
