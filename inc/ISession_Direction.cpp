// ISession_Direction.cpp: implementation of the ISession_Direction class.
//
//////////////////////////////////////////////////////////////////////


#include "ISession_Direction.h"
#include <Prs3d_ArrowAspect.hxx>
#include <DsgPrs_LengthPresentation.hxx>
#include <Select3D_SensitiveSegment.hxx>
#include <Graphic3d_ArrayOfSegments.hxx>
#include <Prs3d_Root.hxx>
#include <Prs3d_Arrow.hxx>
#include <Prs3d_Drawer.hxx>
#include <Prs3d_LineAspect.hxx>
#include <Prs3d_Text.hxx>
#include <SelectMgr_EntityOwner.hxx>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

IMPLEMENT_STANDARD_HANDLE(ISession_Direction,AIS_InteractiveObject)
IMPLEMENT_STANDARD_RTTIEXT(ISession_Direction,AIS_InteractiveObject)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ISession_Direction::ISession_Direction()
{

}

ISession_Direction::ISession_Direction (const gp_Pnt& aPnt,
                                        const gp_Dir& aDir,
                                        Standard_Real aLength,
                                        Standard_Real anArrowLength)
: myPnt (aPnt),
  myDir (aDir),
  myLength (aLength),
  myArrowLength (anArrowLength)
{}

ISession_Direction::ISession_Direction (const gp_Pnt& aPnt,
                                        const gp_Vec& aVec,
                                        Standard_Real anArrowLength)
: myPnt (aPnt),
  myDir (aVec),
  myArrowLength (anArrowLength)
{
  myLength = aVec.Magnitude();
}

ISession_Direction::ISession_Direction (const gp_Pnt2d& aPnt2d,
                                        const gp_Dir2d& aDir2d,
                                        Standard_Real aLength)
: myPnt (gp_Pnt(aPnt2d.X(),aPnt2d.Y(),0.0)),
  myDir (gp_Dir(aDir2d.X(),aDir2d.Y(),0.0)),
  myLength (aLength)
{
  myArrowLength = myDrawer->ArrowAspect()->Length();
}

ISession_Direction::ISession_Direction (const gp_Pnt2d& aPnt2d,
                                        const gp_Vec2d& aVec2d)
: myPnt (gp_Pnt (aPnt2d.X(), aPnt2d.Y(), 0.0)),
  myDir (gp_Dir(aVec2d.X(), aVec2d.Y(), 0.0))
{
  myLength = aVec2d.Magnitude();
  myArrowLength = myDrawer->ArrowAspect()->Length();
}


ISession_Direction::~ISession_Direction()
{

}

void ISession_Direction::Compute (const Handle(PrsMgr_PresentationManager3d)& /*aPresentationManager*/,
                                  const Handle(Prs3d_Presentation)& aPresentation,
                                  const Standard_Integer /*aMode*/)
{
  // Set style for arrow
  Handle(Prs3d_ArrowAspect) anArrowAspect = myDrawer->ArrowAspect();
  anArrowAspect->SetLength (myArrowLength);

  gp_Pnt aLastPoint = myPnt;
  aLastPoint.Translate (myLength*gp_Vec(myDir));

  // Draw Line
  Handle(Graphic3d_ArrayOfSegments) aPrims = new Graphic3d_ArrayOfSegments (2);
  aPrims->AddVertex (myPnt);
  aPrims->AddVertex (aLastPoint);
  Prs3d_Root::CurrentGroup (aPresentation)->SetPrimitivesAspect (myDrawer->LineAspect()->Aspect());
  Prs3d_Root::CurrentGroup (aPresentation)->AddPrimitiveArray (aPrims);
  // Draw arrow
  Prs3d_Arrow::Draw (aPresentation,
                     aLastPoint,
                     myDir,
                     anArrowAspect->Angle(),
                     anArrowAspect->Length());

  // Draw text
  if (myText.Length() != 0)
  {
    gp_Pnt aTextPosition = aLastPoint;
    Prs3d_Text::Draw (aPresentation,
                      myDrawer->TextAspect(),
                      myText,
                      aTextPosition);
  }
}


void ISession_Direction::Compute (const Handle(Prs3d_Projector)& /*aProjector*/,
                                  const Handle(Prs3d_Presentation)& /*aPresentation*/) 
{
}

void ISession_Direction::ComputeSelection (const Handle(SelectMgr_Selection)&																aSelection,
                                           const Standard_Integer aMode) 
{
	Handle(SelectMgr_EntityOwner) eown = new SelectMgr_EntityOwner(this);
  eown -> SelectBasics_EntityOwner::Set(3);
  gp_Pnt last;
  last.SetX(myPnt.X()+myLength*myDir.X());
  last.SetY(myPnt.Y()+myLength*myDir.Y());
  last.SetZ(myPnt.Z()+myLength*myDir.Z());
  Handle(Select3D_SensitiveSegment) seg = new Select3D_SensitiveSegment(eown,
	  this->myPnt,
	  last);
  aSelection->Add(seg);
}

void ISession_Direction::SetText (TCollection_ExtendedString & theText)
{
  myText = theText;
}

void ISession_Direction::SetText (Standard_CString theText)
{
  myText = theText;
}

void ISession_Direction::SetLineAspect (const Handle(Prs3d_LineAspect)& theAspect)
{
  myDrawer->SetLineAspect (theAspect);
}