// ISession_Surface.h: interface for the ISession_Surface class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _ISESSION_SURFACE_
#define _ISESSION_SURFACE_


#include "OCCT_Headers.h"


DEFINE_STANDARD_HANDLE(ISession_Surface,AIS_InteractiveObject)
class ISession_Surface : public AIS_InteractiveObject  
{
public:
	ISession_Surface();
	ISession_Surface(Handle(Geom_Surface)& aSurface);
	virtual ~ISession_Surface();

DEFINE_STANDARD_RTTI(ISession_Surface)
private:

Standard_EXPORT virtual  void Compute(const Handle(PrsMgr_PresentationManager3d)& aPresentationManager,const Handle(Prs3d_Presentation)& aPresentation,const Standard_Integer aMode = 0);
Standard_EXPORT virtual  void Compute(const Handle(Prs3d_Projector)& aProjector,const Handle(Prs3d_Presentation)& aPresentation);
void ComputeSelection(const Handle(SelectMgr_Selection)& aSelection,const Standard_Integer aMode);

Handle(Geom_Surface) mySurface;

};

#endif // !defined(_ISESSION_SURFACE_)
