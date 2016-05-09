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
#include <Prs3d_TextAspect.hxx>
#include <Graphic3d_AspectLine3d.hxx>

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
	arrowColor = Quantity_NameOfColor::Quantity_NOC_RED;
}

ISession_Direction::ISession_Direction (const gp_Pnt& aPnt,
                                        const gp_Dir& aDir,
                                        Standard_Real aLength,
                                        Standard_Real anArrowLength)
: myPnt (aPnt),
  myDir (aDir),
  myLength (aLength),
  myArrowLength (anArrowLength)
  
{
	arrowColor = Quantity_NameOfColor::Quantity_NOC_RED;
}

ISession_Direction::ISession_Direction (const gp_Pnt& aPnt,
                                        const gp_Vec& aVec,
                                        Standard_Real anArrowLength)
: myPnt (aPnt),
  myDir (aVec),
  myArrowLength (anArrowLength)
{
  myLength = aVec.Magnitude();
  arrowColor = Quantity_NameOfColor::Quantity_NOC_RED;
  
}

ISession_Direction::ISession_Direction (const gp_Pnt2d& aPnt2d,
                                        const gp_Dir2d& aDir2d,
                                        Standard_Real aLength)
: myPnt (gp_Pnt(aPnt2d.X(),aPnt2d.Y(),0.0)),
  myDir (gp_Dir(aDir2d.X(),aDir2d.Y(),0.0)),
  myLength (aLength)
{
  myArrowLength = myDrawer->ArrowAspect()->Length();
  arrowColor = Quantity_NameOfColor::Quantity_NOC_RED;
}

ISession_Direction::ISession_Direction (const gp_Pnt2d& aPnt2d,
                                        const gp_Vec2d& aVec2d)
: myPnt (gp_Pnt (aPnt2d.X(), aPnt2d.Y(), 0.0)),
  myDir (gp_Dir(aVec2d.X(), aVec2d.Y(), 0.0))
{
  myLength = aVec2d.Magnitude();
  myArrowLength = myDrawer->ArrowAspect()->Length();
  arrowColor = Quantity_NameOfColor::Quantity_NOC_RED;
}


ISession_Direction::~ISession_Direction()
{

}

void ISession_Direction::Compute (const Handle(PrsMgr_PresentationManager3d)& /*aPresentationManager*/,
                                  const Handle(Prs3d_Presentation)& aPresentation,
                                  const Standard_Integer /*aMode*/)
{
  // Set style for arrow
	Handle(Prs3d_ArrowAspect) anArrowAspect = new Prs3d_ArrowAspect(); // myDrawer->ArrowAspect();  
   anArrowAspect->Aspect() = new Graphic3d_AspectLine3d(arrowColor,Aspect_TypeOfLine::Aspect_TOL_SOLID,2);
  anArrowAspect->SetLength (myArrowLength);
  anArrowAspect->SetColor(arrowColor);

  SetColor(arrowColor);

  gp_Pnt aLastPoint = myPnt;

  aLastPoint.Translate (myLength*gp_Vec(myDir));

  myDrawer->SetArrowAspect(anArrowAspect);

  gp_Pnt LastPoint = myPnt;
  LastPoint.Translate(myLength*gp_Vec(myDir));
  
  if (myText.Length() == 0)
	  DsgPrs_LengthPresentation::Add(aPresentation, myDrawer, myPnt, aLastPoint, DsgPrs_AS_LASTAR);

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
	 Handle_Prs3d_TextAspect textAspect = myDrawer->TextAspect();
	 textAspect->SetColor(arrowColor);
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

void ISession_Direction::ComputeSelection (const Handle(SelectMgr_Selection)&	aSelection,
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
void ISession_Direction::SetArrowColor(Quantity_NameOfColor color)
{
	arrowColor = color;
}
