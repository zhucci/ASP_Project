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
#include <Graphic3d_ArrayOfPolylines.hxx>

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

void ISession_Direction::Compute (const Handle(PrsMgr_PresentationManager3d)& aPresentationManager,
                                  const Handle(Prs3d_Presentation)& aPresentation,
                                  const Standard_Integer aMode)
{
  // Set style for arrow
	Handle(Prs3d_ArrowAspect) anArrowAspect = new Prs3d_ArrowAspect(); // myDrawer->ArrowAspect();  
   anArrowAspect->Aspect() = new Graphic3d_AspectLine3d(arrowColor,Aspect_TypeOfLine::Aspect_TOL_SOLID,1);
  anArrowAspect->SetLength (myArrowLength);
  anArrowAspect->SetColor(arrowColor);
  anArrowAspect->SetAngle(0.16);
  myDrawer->SetArrowAspect(anArrowAspect);

  this->SetColor(arrowColor);
  //SetColorInstraction
  this->myOwnColor=arrowColor;
  this->hasOwnColor = true;

  gp_Pnt aLastPoint = myPnt;

  aLastPoint.Translate (myLength*gp_Vec(myDir));
  
  myDrawer->SetArrowAspect(anArrowAspect);

 // if (myText.Length() == 0)
//	  DsgPrs_LengthPresentation::Add(aPresentation, myDrawer, myPnt, aLastPoint, DsgPrs_AS_LASTAR);

  Handle(Graphic3d_ArrayOfSegments) aPrims = new Graphic3d_ArrayOfSegments (2);
  aPrims->AddVertex (myPnt);
  aPrims->AddVertex (aLastPoint);
  aPrims->SetVertexColor(1,arrowColor);
  aPrims->SetVertexColor(2, arrowColor);

  Prs3d_Root::CurrentGroup(aPresentation)->SetPrimitivesAspect(anArrowAspect->Aspect());
  Prs3d_Root::CurrentGroup (aPresentation)->AddPrimitiveArray (aPrims);
  
  // Draw arrow
  

  auto anAngle = anArrowAspect->Angle();
  auto aLength  = myLength*0.15;
  auto aLocation = aLastPoint;

  Quantity_Length dx, dy, dz;  myDir.Coord(dx, dy, dz);
  //
  // Point of the arrow:
  Quantity_Length xo, yo, zo;  aLocation.Coord(xo, yo, zo);

  // Center of the base circle of the arrow:
  Quantity_Length xc = xo - dx * aLength;
  Quantity_Length yc = yo - dy * aLength;
  Quantity_Length zc = zo - dz * aLength;

  // Construction of i,j mark for the circle:
  Quantity_Length xn = 0., yn = 0., zn = 0.;

  if (Abs(dx) <= Abs(dy) && Abs(dx) <= Abs(dz)) xn = 1.;
  else if (Abs(dy) <= Abs(dz) && Abs(dy) <= Abs(dx)) yn = 1.;
  else zn = 1.;
  Quantity_Length xi = dy * zn - dz * yn;
  Quantity_Length yi = dz * xn - dx * zn;
  Quantity_Length zi = dx * yn - dy * xn;

  Quantity_Length Norme = sqrt(xi*xi + yi*yi + zi*zi);
  xi = xi / Norme; yi = yi / Norme; zi = zi / Norme;

  const Quantity_Length  xj = dy * zi - dz * yi;
  const Quantity_Length  yj = dz * xi - dx * zi;
  const Quantity_Length  zj = dx * yi - dy * xi;

  const Standard_Integer NbPoints = 15;

  Handle(Graphic3d_ArrayOfSegments) aPrims1 = new Graphic3d_ArrayOfSegments(2 * NbPoints);
  Handle(Graphic3d_ArrayOfPolylines) aPrims2 = new Graphic3d_ArrayOfPolylines(NbPoints + 1);
  
  gp_Pnt p1;
  const Standard_Real Tg = tan(anAngle);
  _int color = 0;
  color = (color<<16) *207;
 
  for (Standard_Integer i = 1; i <= NbPoints; i++)
  {

	  const Standard_Real cosinus = cos(2 * M_PI / NbPoints * (i - 1));
	  const Standard_Real sinus = sin(2 * M_PI / NbPoints * (i - 1));

	  const gp_Pnt pp(xc + (cosinus * xi + sinus * xj) * aLength * Tg,
		  yc + (cosinus * yi + sinus * yj) * aLength * Tg,
		  zc + (cosinus * zi + sinus * zj) * aLength * Tg);

	  aPrims1->AddVertex(aLocation);
	  aPrims1->AddVertex(pp);
	  if (i == 1) p1 = pp;
	  aPrims2->AddVertex(pp);

	  aPrims1->SetVertexColor(i, color);
	  
  }
  aPrims2->AddVertex(p1);
  aPrims2->SetVertexColor(1,color);
  aPrims2->SetBoundColor(1,arrowColor);

  Prs3d_Root::CurrentGroup(aPresentation)->AddPrimitiveArray(aPrims1);
  Prs3d_Root::CurrentGroup(aPresentation)->AddPrimitiveArray(aPrims2);
  // Draw text
  /*
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
  */
  
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
