// ISession_Curve.h: interface for the ISession_Curve class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_ISESSION_CURVE_)
#define _ISESSION_CURVE_

#include <Standard_Macro.hxx>
#include <Standard_DefineHandle.hxx>
#include "OCCT_Headers.h"

DEFINE_STANDARD_HANDLE(ISession_Curve,AIS_InteractiveObject)

class ISession_Curve : public AIS_InteractiveObject  
{
public:
  ISession_Curve(Handle(Geom_Curve)& aCurve);
  virtual ~ISession_Curve();

  DEFINE_STANDARD_RTTI(ISession_Curve)

private:

  Standard_EXPORT virtual  void Compute(const Handle(PrsMgr_PresentationManager3d)& aPresentationManager,const Handle(Prs3d_Presentation)& aPresentation,const Standard_Integer aMode = 0) ;
  Standard_EXPORT virtual  void Compute(const Handle(Prs3d_Projector)& aProjector,const Handle(Prs3d_Presentation)& aPresentation) ;
  void ComputeSelection(const Handle(SelectMgr_Selection)& aSelection,const Standard_Integer aMode) ;

  Handle(Geom_Curve) myCurve;
};

#endif // !defined(_ISESSION_CURVE_)
