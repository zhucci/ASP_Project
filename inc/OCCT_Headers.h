
#ifndef _OCCT_Headers_
#define _OCCT_Headers_

//********************************
//			Package OSD
//********************************
#include <OSD_Chronometer.hxx>


//******************************
//			Algorithm
//******************************
#include <Extrema_ExtSS.hxx>
#include <Extrema_ExtElSS.hxx>


#include <ProjLib_ProjectOnPlane.hxx>
#include <ProjLib.hxx>
#include <ProjLib_Projector.hxx>
#include <ProjLib_ProjectOnSurface.hxx>

//**********************************
//			Geom_
//**********************************
//#include <GeomAbs_SurfaceType.hxx>
#include <Geom_ConicalSurface.hxx>
#include <Geom_SphericalSurface.hxx>
#include <Geom_ToroidalSurface.hxx>
#include <GeomAdaptor.hxx>
#include <GeomAdaptor_Surface.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <Geom_Plane.hxx>
#include <Geom_Surface.hxx>
#include <Geom_TrimmedCurve.hxx>

#include <Geom2d_Ellipse.hxx>
#include <Geom2d_TrimmedCurve.hxx>

#include <GeomAPI_ExtremaSurfaceSurface.hxx>
#include <GeomAPI.hxx>
#include <GeomAPI_ProjectPointOnSurf.hxx>
#include <Geom_Axis2Placement.hxx>
//**********************************
//			TopoDS_
//**********************************
#include <TopTools_HSequenceOfShape.hxx>
#include <TopoDS_Solid.hxx>
#include <Handle_TopoDS_TSolid.hxx>
#include <TopTools.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>

#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Compound.hxx>

#include <ShapeAnalysis_Surface.hxx>
#include <Adaptor3d_HSurface.hxx>
#include <LProp3d_SLProps.hxx>
#include <GeomLProp_SLProps.hxx>
#include <GeomAdaptor_HSurface.hxx>
//**********************************
//			Standard_Types
//**********************************
#include <Standard_PCharacter.hxx>

#include<Standard_CString.hxx>
#include <TCollection_HAsciiString.hxx>
#include <TCollection.hxx>
#include <TColGeom_Array1OfSurface.hxx>

#include <TColStd_HSequenceOfTransient.hxx>
//**********************************
//		.stp Import/Export
//**********************************
#include<StepData.hxx>

#include <XSControl_WorkSession.hxx>
#include <StepData_StepModel.hxx>
#include <Interface_InterfaceModel.hxx>
#include<STEPControl_Reader.hxx>
#include <StepToGeom_MakePlane.hxx>


#include <Bnd_Box.hxx>
#include <BndLib.hxx>
#include <BndLib_AddSurface.hxx>

//**********************************
//		BrepTools Brep_Tool
//**********************************

#include <TopOpeBRep_PointClassifier.hxx>


#include <BRepExtrema_DistShapeShape.hxx>
#include <BRepExtrema_ExtFF.hxx>

#include <HLRBRep_SurfaceTool.hxx>
#include <HLRBRep_CurveTool.hxx>


#include <BRepAdaptor_Surface.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <BRepTools.hxx>

#include <StepData_StepReaderData.hxx>
#include <BRep_Tool.hxx>
#include <BRep_Curve3D.hxx>
#include <BRep_Tool.hxx>
#include <BRepBndLib.hxx>
#include <BRepGProp_Vinert.hxx>
#include <BRepAlgoAPI_Fuse.hxx>


#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepMesh_Triangle.hxx>
#include <BRepMesh_IncrementalMesh.hxx>

#include <BRepFilletAPI_MakeFillet.hxx>

#include <BRepGProp.hxx>
#include <BRepLProp.hxx>
#include <GProp_GProps.hxx>

#include <BRepLib.hxx>
#include <BRepExtrema_DistanceSS.hxx>

#include <BRepOffsetAPI_MakeThickSolid.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>

#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakePrism.hxx>

//**********************************
//		Comlicated geometry 
//**********************************
#include <GC_MakeArcOfCircle.hxx>
#include <GC_MakeSegment.hxx>

#include <GCE2d_MakeSegment.hxx>

//**********************************
//		Primary Geometry
//**********************************
#include <gp.hxx>
#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>
#include <gp_Ax2d.hxx>
#include <gp_Dir.hxx>
#include <gp_Dir2d.hxx>
#include <gp_Pnt.hxx>
#include <gp_Pnt2d.hxx>
#include <gp_Trsf.hxx>
#include <gp_Vec.hxx>

#include <gp_Pln.hxx>

//**********************************
//Application Interractive servise
//**********************************
#include<AIS.hxx>

#include <TopTools_ListOfShape.hxx>
#include <TopTools.hxx>
#include <AIS_Shape.hxx>
#include <AIS_InteractiveContext.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <V3d_Viewer.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <Graphic3d_GraphicDriver.hxx>
#include <WNT_Window.hxx>
#include <Graphic3d_Group.hxx>
#include <V3d_View.hxx>
#include <Xw_Window.hxx>
#include <TCollection_ExtendedString.hxx>
#include <V3d_AmbientLight.hxx>
#include <V3d_DirectionalLight.hxx>
#include <WNT_WClass.hxx>
#include <WNT_Window.hxx>
#include <InterfaceGraphic_Graphic3d.hxx>
#include <Standard_Failure.hxx>
#include <AIS_Trihedron.hxx>
#include <AIS_Axis.hxx>


//*****************************
//****   XCAF headers   *******
//*****************************
#include <XCAFApp_Application.hxx>
#include <TDocStd_Document.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <XCAFDoc_ColorType.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <TDF_LabelSequence.hxx>
#include <TDF_ChildIterator.hxx>
#include <TDF_IDFilter.hxx>
#include <TDF_AttributeIndexedMap.hxx>
#include <TDF_Tool.hxx>
#include <TDataXtd_Shape.hxx>
#include <TNaming_UsedShapes.hxx>
#include <TNaming_NamedShape.hxx>
#include <TDF_AttributeIndexedMap.hxx>

#include <Transfer_TransientProcess.hxx>
//*****************************
//****   TDataStd	   *******
//*****************************
#include <TDataStd.hxx>
#include <TDataStd_Name.hxx>
#include <TDataStd_NamedData.hxx>
#include <TDataStd_TreeNode.hxx>

//*****************************
//****   Units		   *******
//*****************************
#include <UnitsAPI.hxx>
#include <Resource_ConvertUnicode.hxx>
#include <Resource_Unicode.hxx>


//*****************************
//****   Standard Exceptions   *******
//*****************************
#include <StepFile_CallFailure.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_DomainError.hxx>

//*****************************
//	 ------DEFINE------
//*****************************
#define _RootShapeDepth_ 2
#define _HashValue_ 1000000

//*****************************
//	 ------Typedef------
//*****************************

typedef Standard_Real _real;
typedef Standard_Boolean _bool;
typedef Standard_Integer _int;



#endif //_OCCT_Headers_