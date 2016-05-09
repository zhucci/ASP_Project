#include "Viewer.h"
#include "Timer.h"

#include <QApplication.h>
#include <QCursor.h>
#include <QMessagebox.h>

#include <QMouseEvent>

#include <WNT_Window.hxx>

#include <Voxel_Prs.hxx>
#include <AIS_ListOfInteractive.hxx>
#include <AIS_ListIteratorOfListOfInteractive.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <V3d_DirectionalLight.hxx>
#include <V3d_AmbientLight.hxx>
#include <qmenu.h>
#include <qaction.h>
#include <AIS_DisplayMode.hxx>
#include <AIS_LocalContext.hxx>


static Handle(Graphic3d_GraphicDriver) Viewer_aGraphicDriver;


Viewer::Viewer(QWidget* parent):QWidget(parent)
{
    if (myGraphicDriver.IsNull())
    {
      if (Viewer_aGraphicDriver.IsNull())
      {
	//Display connection requered only on Linux sistems
        Handle(Aspect_DisplayConnection) aDisplayConnection;
//Create a Graphic Driver from  the default Aspect_DisplayConnection
        Viewer_aGraphicDriver = new OpenGl_GraphicDriver (aDisplayConnection);
      }

      myGraphicDriver = Handle(OpenGl_GraphicDriver)::DownCast(Viewer_aGraphicDriver);
    }
 //Create a Viewer to this Driver 
	Handle(V3d_Viewer) aViewer = new V3d_Viewer(myGraphicDriver, TCollection_ExtendedString("Visu3D").ToExtString(), "",//Name and Domain
                                1000,// Space size
								V3d_XposYnegZpos, //Projection
								Quantity_NOC_GRAY40,//Background
								V3d_TypeOfVisualization::V3d_ZBUFFER,  // Type of  visualization
							   V3d_TypeOfShadingModel::V3d_GOURAUD,  // Shading  model  
								V3d_WAIT, // Update mode 
                                                true, true, V3d_TEX_NONE);

    myView = aViewer->CreateView();
	 
    myIC = new AIS_InteractiveContext(aViewer);
	myIC->SetZDetection(true);
    myIC->SetDeviationCoefficient(1.e-3);
//====================================
//Create a 3D view (a Windows example)
//====================================
//Windows dependent stuff	
    Aspect_Handle aWindowHandle = (Aspect_Handle )winId();
    Handle(WNT_Window) hWnd = new WNT_Window (aWindowHandle);
//Viewer derived from QWidget so we give V3d_Viewer control on Widget area
//And Viewer handle signals from UI and call some V3d_Viewer functions
    myView->SetWindow(hWnd);
 // Map this Window to  this screen
    if(!hWnd->IsMapped())
        hWnd->Map();
//End windows dependent stuff
//====================================
//Create a 3D view (a Windows example)
//====================================

    myView->MustBeResized();
    myView->SetSurfaceDetail(V3d_TEX_NONE);
    myView->SetSize(10000.0);
    myView->SetZSize(10000.0);
    myView->SetViewMappingDefault();

    myZoom = false;
    myPan = false;
    myRotate = false;
    setMouseTracking(true);

    setMinimumSize(400, 200);
    
    myView->ZBufferTriedronSetup();
    myView->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_BLACK, 0.08, V3d_ZBUFFER);
	 
    mySelector.Init(myView);

	setBackgroundRole( QPalette::NoRole );//NoBackground 
	// set focus policy to threat QContextMenuEvent from keyboard  
	setFocusPolicy( Qt::StrongFocus );
	setAttribute( Qt::WA_PaintOnScreen );
	setAttribute( Qt::WA_NoSystemBackground );
	
	aViewer->SetLightOn(new V3d_DirectionalLight(aViewer, V3d_XposYnegZneg, Quantity_NOC_WHITE, Standard_True));
	// aViewer->SetLightOn(new V3d_DirectionalLight(aViewer, V3d_XnegYnegZpos, Quantity_NOC_WHITE, Standard_True));
	// aViewer->SetLightOn(new V3d_DirectionalLight(aViewer, V3d_XnegYpos, Quantity_NOC_WHITE, Standard_True));
	// aViewer->SetLightOn(new V3d_DirectionalLight(aViewer, V3d_XnegYneg, Quantity_NOC_WHITE, Standard_True));
	// aViewer->SetLightOn(new V3d_DirectionalLight(aViewer, V3d_Xneg, Quantity_NOC_WHITE, Standard_True));
	// aViewer->SetLightOn(new V3d_DirectionalLight(aViewer, V3d_Xpos, Quantity_NOC_WHITE, Standard_True));
    aViewer->SetLightOn(new V3d_AmbientLight(aViewer, Quantity_NOC_WHITE));
	// aViewer->UpdateLights();
	 myIC->SetHilightColor(Quantity_NOC_BLUE2);
	
	 
}

Viewer::~Viewer()
{

}

void Viewer::paintEvent(QPaintEvent * pEvent)
{
    if (!myView.IsNull())
        myView->Redraw();
}


/*!
  Get paint engine for the OpenGL viewer. [ virtual public ]
*/
QPaintEngine* Viewer::paintEngine() const
{
//  return paintEngine();
  return 0;
}
void Viewer::Fit(){
	if(!myView.IsNull()){
		myView->FitAll(1.e-3);
	}
}
void Viewer::Update(){
	if (!myView.IsNull())
		myView->Redraw();
}
void Viewer::resizeEvent(QResizeEvent * e)
{
    if (!myView.IsNull())
    {
        myView->MustBeResized();
    }
}

void Viewer::mousePressEvent(QMouseEvent * mpEvent)
{
    // Memorize start point
    myStartPnt.setX(mpEvent->x());
    myStartPnt.setY(mpEvent->y());
    
    // Inform IC that the mouse cursor is at the point
  //  myIC->MoveTo(myStartPnt.x(), myStartPnt.y(), myView);

    // In case of rotation, define the start rotation point
    if (mpEvent->modifiers() & Qt::ControlModifier && (mpEvent->buttons() & Qt::RightButton)){
        myView->StartRotation(myStartPnt.x(), myStartPnt.y());
    }

    // Start degenerate mode
   // setDegenerateMode(true);

    emit mousePressed(mpEvent->modifiers(), mpEvent->x(), mpEvent->y());
}
/*
void Viewer::contextMenuEvent(QContextMenuEvent *menuEvent){

	QAction *shadingMode = new QAction(tr("Display mode"),this);
	connect(shadingMode, SIGNAL(triggered()), this, SLOT(ChangeShadingMode()));
	QAction *clearLocalContext = new QAction(tr("Clear Product"), this);
	connect(clearLocalContext, SIGNAL(triggered()), this, SLOT(CloseAllLocalContexts()));
	QMenu menu(tr("Context"), this);
	menu.addAction(shadingMode);
	menu.addAction(clearLocalContext);
	menu.exec(menuEvent->globalPos());
}
*/

void Viewer::mouseMoveEvent(QMouseEvent * mmEvent)
{
   // QPoint currentPnt(mmEvent->x(), mmEvent->y());

    if (mmEvent->modifiers() & Qt::ControlModifier)
    {
		if (mmEvent->buttons() & Qt::LeftButton)
        {
            myView->Pan(mmEvent->x() - myStartPnt.x(), myStartPnt.y() - mmEvent->y());
			myStartPnt.setX(mmEvent->x());
			myStartPnt.setY(mmEvent->y());
        }
		
        else if (mmEvent->buttons() & Qt::RightButton)
        {
            myView->Rotation(mmEvent->x(), mmEvent->y());
        }
		
    }
	else{
        myIC->MoveTo(mmEvent->x(), mmEvent->y(), myView);
    }

    emit mouseMoved(mmEvent->modifiers(), mmEvent->x(), mmEvent->y());
}
void Viewer::keyPressEvent(QKeyEvent *kEvent){
	if (kEvent->key() & Qt::Key_Tab){
		
		if (myIC->HasNextDetected()){
			myIC->HilightNextDetected(myView, true);
			myView->Redraw();
		}
	}
}
void Viewer::wheelEvent(QWheelEvent *event)
{
	if (event->delta() < 0)
		myView->SetScale(myView->Scale()/1.15);
	else
		myView->SetScale(myView->Scale() / 0.85);
	myView->Redraw();
	event->accept();
}
void Viewer::mouseReleaseEvent(QMouseEvent * mrEvent)
{
    if(mrEvent->button() == Qt::LeftButton)
    {
        if(!myZoom && !myPan && !myRotate)
        {
            if(mrEvent->modifiers() & Qt::ShiftModifier)
                myIC->ShiftSelect();
            else
                myIC->Select();

            // Select a voxel
            int ix = -1, iy = -1, iz = -1;
            bool detected = mySelector.Detect(mrEvent->x(), mrEvent->y(), ix, iy, iz);
            if (detected)
            {
                cout<<"("<<ix<<", "<<iy<<", "<<iz<<")"<<endl;
            }
            if (!myPrs.IsNull())
                myPrs->Highlight(ix, iy, iz);
        }
    }
    else if(mrEvent->button() == Qt::RightButton && !mrEvent->modifiers())
    {
		
		emit contextMenuSignal(mrEvent->globalPos());
	}

    // Finish degenerate mode
    setDegenerateMode(false);
    
    emit mouseReleased(mrEvent->modifiers(), mrEvent->x(), mrEvent->y());
}

void Viewer::mouseDoubleClickEvent(QMouseEvent * mdcEvent)
{
    emit mouseDoubleClick(mdcEvent->modifiers(), mdcEvent->x(), mdcEvent->y());
}

void Viewer::setDegenerateMode(const bool on)
{
    AIS_ListOfInteractive displayed;
    myIC->DisplayedObjects(displayed);
    AIS_ListIteratorOfListOfInteractive itri(displayed);
    for (; itri.More(); itri.Next())
    {
        if (itri.Value()->DynamicType() == STANDARD_TYPE(Voxel_Prs))
        {
            Handle(Voxel_Prs) prs = Handle(Voxel_Prs)::DownCast(itri.Value());
            prs->SetDegenerateMode(on);
            myView->Redraw();
            break;
        }
    }
}
void Viewer::ChangeShadingMode(){
	
	myIC->InitCurrent();
	Standard_Boolean isChanged = Standard_False;
	while(myIC->MoreCurrent()){
		
		Handle_AIS_InteractiveObject obj = myIC->Current();

		if (obj.IsNull())
			break;

		auto mode = obj->DisplayMode();
		switch(mode){
			case AIS_Shaded :	
				//myIC->LocalContext()->Display(obj, AIS_WireFrame);
				//obj->SetDisplayMode(AIS_WireFrame);
				myIC->SetDisplayMode(obj, AIS_WireFrame, false);
				break;
			case AIS_WireFrame:
				//myIC->LocalContext()->Display(obj, AIS_Shaded);
				//obj->SetDisplayMode(AIS_Shaded);
				myIC->SetDisplayMode(obj, AIS_Shaded, false);
				break;
			default :
				//myIC->LocalContext()->Display(obj, AIS_Shaded);
				//obj->SetDisplayMode(AIS_Shaded);
				myIC->SetDisplayMode(obj, AIS_Shaded, false);
				break;

		} 
	//	myIC->UnsetSelectionMode(myIC->Interactive());
	//	isChanged = Standard_True;
		
		
		myIC->NextCurrent();
	}
	
//	if (isChanged){
		
//		myIC->UnhilightCurrents(false);
		
//	}
	myView->Redraw();
}
void Viewer::CloseLocalContext(){
	myIC->CloseLocalContext();
	myIC->ActivateStandardMode(TopAbs_ShapeEnum::TopAbs_SOLID);
	if (!myIC->AutomaticHilight())
	myIC->SetAutomaticHilight(true);
	myView->Redraw();
}
void Viewer::DisplayAll(){
	
	myIC->DisplayAll(true);
}