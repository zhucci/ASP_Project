
#include "AspMainTool.h"
#include "qapplication.h"
#include "MainFrame.h"
//=======================
//	Local includes		=
//=======================
#include <Geom_Axis1Placement.hxx>
#include <AIS_Point.hxx>
#include <AIS_LocalContext.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <Geom_CartesianPoint.hxx>

#include "ISession_Direction.h"
#include "ISession_Text.h"
#include "Viewer.h"
#include "Timer.h"
#include "BRepClass3d_SolidClassifier.hxx"
#include "BRepClass_FaceClassifier.hxx"
#include "BRepBuilderAPI_MakeVertex.hxx"
#include "Geom_Point.hxx"
#include "AssemblyGraphBuilder.h"
#include "assert.h"
#include <AIS_InteractiveObject.hxx>
#include "qmessagebox.h"

extern Quantity_NameOfColor colorVar[];
extern int ColorAmount;
//====================
//	aps namespace	 =
//====================
using namespace asp;
 
AspMainTool::AspMainTool():
AsmVisualContextId{ -1 },
showMustGoOn{true},
displayMode{AIS_DisplayMode::AIS_WireFrame}{
	ocafApplication = XCAFApp_Application::GetApplication();
}

AspMainTool::~AspMainTool(){

	if (!ocafDocument.IsNull() && ocafDocument->IsOpened()){
		ocafApplication->Close(ocafDocument);
		ocafApplication->NewDocument("MDTV-XCAF", ocafDocument);
		delete product;
		product = nullptr;
	}
}

void AspMainTool::Init(std::string stepfileName, Viewer *aView, MainFrame* mainWin){

	//	Standard_PCharacter file = new char[stepfileName.LengthOfCString()];
	mainWindow = mainWin;
	
	//	stepfileName.ToUTF8CString(file);
	if (ocafDocument.IsNull())
		ocafApplication->NewDocument("MDTV-XCAF", ocafDocument);
	
	if (!ocafDocument->IsEmpty()){
		ocafDocument->Close();
		ocafApplication->NewDocument("MDTV-XCAF", ocafDocument);
	}
	mainWindow->SetStatus("Reading STEP file is started");

	ReadStep(stepfileName.c_str());
	
	mainWindow->SetStatus("Reading STEP file has been finished");

	mainWindow->SetStatus("Building assembly view");

	BuildAssebly();
	
	PartToAisShapeMap();

	ShowProduct(aView);

	mainWindow->SetStatus("Finding assembly sequence is started");
	if (AsmTreeCalculation){
		AsmTreeBuilding = true;
		AsmTreeInit();
	}
	
	

}
void AspMainTool::PartToAisShapeMap(){
	mapOfShapes.clear();

	for (auto & part : product->UnitMap){
			Handle_AIS_Shape shape = new AIS_Shape(part.second->getShape());
			mapOfShapes.emplace(part.first, shape);	
	}
}
_bool AspMainTool::AsmTreeInit()
{
	asmSeq.Init(product,mainWindow);
	if (asmSeq.IsDone())
		asmState = asmSeq.GetFullProductNode();

	return asmSeq.IsDone();
}

IFSelect_ReturnStatus AspMainTool::ReadStep(Standard_CString file){
	try{
		OCC_CATCH_SIGNALS
	importStepWithAtt(file, ocafDocument);
	}
	catch(Standard_Failure)
	{
		std::cerr<<__FILE__<<":"<<__LINE__<<endl;
		std::cerr<<Standard_Failure::Caught()->GetMessageString()<<endl;
	}
	
	return IFSelect_RetDone;
}

IFSelect_ReturnStatus AspMainTool::BuildAssebly()
{
	try{
//!handle system signals as ordinary exceptions
		OCC_CATCH_SIGNALS

	TDF_Label shapelbl =  XCAFDoc_DocumentTool::ShapesLabel 
											(ocafDocument->Main());
		
		product = new asp::Assembly(shapelbl);
	

#ifdef _LOGOUTPUT_
			LogFile.unsetf ( std::ios::floatfield );
			LogFile.unsetf(std::ios::scientific);
			LogFile.setf(std::ios::fixed);
			LogFile.precision(1);
			
			product->Dump(LogFile);
#endif
				
		
	}
	catch(Standard_Failure)
	{
		std::cerr<<__FILE__<<":"<<__LINE__<<endl;
		std::cerr<<Standard_Failure::Caught()->GetMessageString()<<endl;
			
		return IFSelect_RetFail;
	}
	return IFSelect_RetDone;
}


IFSelect_ReturnStatus AspMainTool::SequenceOptimize(){

	return IFSelect_RetDone;
}
std::string &&AspMainTool::GetLog(){
	
	std::string str(LogFile.str().c_str());
	LogFile.clear();
	return std::move(str);
}
std::string AspMainTool::GetSequence(AsmTreeNode::AsmMoveType type){

	if (asmSeq.IsDone()){
		std::string response;

		AsmTreeNode *sequenceTree;

		if (type == AsmTreeNode::DISMANTLE)
			sequenceTree = asmSeq.GetFullProductNode();
		else
			sequenceTree = asmSeq.GetWholeProductNode();

		while (sequenceTree != nullptr){
			sequenceTree->SetOperationType(type);

			response += std::to_string(sequenceTree->MovedPartUri());

			if (sequenceTree->InitMove()){
				auto vec = sequenceTree->Move();
				response += " To [" + std::to_string(vec.X()) + "," + std::to_string(vec.Y()) + "," + std::to_string(vec.Z()) + "] dist =" + std::to_string(vec.Magnitude());
			}
			response += "\n";
			sequenceTree = sequenceTree->Next();
		}
		return response;
	}
	return std::string("No sequence was obtained\n");
}
void AspMainTool::HideDisassembled( Viewer* aViewer){
	auto context = aViewer->getIC();
	for (auto &dismounted : asmSeq.closedList){
		auto hided = this->mapOfShapes.find(dismounted);
		if (hided!= mapOfShapes.end())
		context->Erase(hided->second,false);
	}
	aViewer->Update();
	asmState = asmSeq.GetWholeProductNode();
}
void AspMainTool::PlayAssemblyAnimation(AsmTreeNode::AsmMoveType processType, Viewer* aViewer){
	try{
		
		if (!asmSeq.IsDone())
			return;
		showMustGoOn = true;
		Handle_V3d_View myView = aViewer->getView();
		Handle_AIS_InteractiveContext context = aViewer->getIC();

		while (showMustGoOn){
		//Check current state of assembly process
			asmState->SetOperationType(processType);
			
			if (!asmState->IsAbsNode()){
				//Show Animation
				Part* movedPart = asmState->MovedPart();
				
				
				auto partAISShape = mapOfShapes.find(movedPart->GetUri())->second;
				if (partAISShape.IsNull())
					Standard_Failure::Raise("Unknow part shape was found");
				if (context->IsDisplayed(partAISShape))
					context->Erase(partAISShape);
				
				
				TopoDS_Shape partShape((const TopoDS_Shape&) movedPart->getShape());
				gp_Trsf move;

				if (processType == AsmTreeNode::MOUNT){
					asmState->SetOperationType(AsmTreeNode::DISMANTLE);
					
					if (asmState->InitMove()){
						do {
							
							move.SetTranslation(asmState->Move());
							partShape.Move(move);
						}
						while (asmState->MoreMove());
					}	
				}
				

				//Set up shape to initial position
				Handle_AIS_Shape  animeShape = new AIS_Shape(partShape);

				context->Display(animeShape, false);
				context->SetDisplayMode(animeShape, AIS_Shaded, false);
				context->SetTransparency(animeShape, 0.4, true);

				//For all assembly translations
				asmState->SetOperationType(processType);
				if (asmState->InitMove()) 
				do {
						gp_Vec moveVec = asmState->Move();
						_real distance = moveVec.Magnitude();
						_int nStep = 80;
						gp_Vec stepMove = moveVec.Normalized();
						stepMove.Scale(distance / nStep);
						move.SetTranslation(stepMove);
						_int step = 1;

						while (step <= nStep && showMustGoOn){

							//Change part location
							//move.SetTranslation(stepMove);
							move.SetTranslation(stepMove*step);
							//Redisplay moved part shape

							animeShape->Set(partShape.Moved(move));
							context->Redisplay(animeShape);
							myView->AutoZFit();
							//Sleep(10);
							++step;
							if (step%10)
								qApp->processEvents();
						
						}
						
						
									
				} while (asmState->MoreMove() && showMustGoOn);
				
				if (processType == AsmTreeNode::DISMANTLE){
					context->Erase(animeShape,false);
				}
				else{
					context->Erase(animeShape,false);
					context->Display(partAISShape, false);	
				}
				
				
			}
			if (asmState->InTheEnd())
				break;
			asmState = asmState->Next();
			aViewer->getView()->Redraw();
		}
		

	}
	catch (Standard_Failure){
		std::cerr << __FILE__ << " " << __LINE__ << Standard_Failure::Caught()->GetMessageString() << std::endl;
	}
}

void AspMainTool::ShowAssemblyStep(AsmTreeNode::AsmMoveType processType, Viewer * aViewer){
	try{
		if (!asmSeq.IsDone())
			return;
		if (asmState == nullptr)
			return;

		auto context = aViewer->getIC();
		asmState->SetOperationType(processType);
		//Close prev context with some garbage	
		if (AsmVisualContextId!=-1){

			aViewer->getIC()->CloseLocalContext(AsmVisualContextId);
			AsmVisualContextId = -1;

			if (!helpShape.IsNull()){

				if (context->IsDisplayed(helpShape)){
					//context->SetColor(helpShape,Quantity_NOC_GOLD3,false);
					//context->SetDisplayMode(helpShape, AIS_WireFrame, false);
					context->Erase(helpShape, false);
				}
			}

			if (asmState && !asmState->IsAbsNode()){

				auto aisShape = mapOfShapes.find(asmState->MovedPartUri())->second;

				if (!aisShape.IsNull()){
					if (processType == AsmTreeNode::DISMANTLE){
						if (context->IsDisplayed(aisShape)){
							//context->Unhilight(aisShape, false);
							context->Erase(aisShape, false);
						}
					}
					else if (processType == AsmTreeNode::MOUNT){
						
						if (context->IsDisplayed(aisShape)){
							//context->SetDisplayMode(helpShape, AIS_WireFrame, false);
							//context->SetColor(aisShape, Quantity_NOC_GOLD3);
							//context->Unhilight(aisShape,false);
						}
						else{
							//context->Unhilight(aisShape, false);					
						context->Display(aisShape, false);
						//context->SetDisplayMode(helpShape, AIS_WireFrame, false);
						//context->SetColor(aisShape, Quantity_NOC_GOLD3,false);
						}
							
					}
				}
			
			}
		}

		
		if (!asmState->InTheEnd()){
			asmState = asmState->Next();
			asmState->SetOperationType(processType);
		}

		//Display assembly operation
		
		if (!asmState->IsAbsNode()){
			auto pPart = asmState->MovedPart();
				if (pPart){

					gp_Pnt partCenter = pPart->GetCenter();

					if (processType == AsmTreeNode::MOUNT){

						auto curDispShape = mapOfShapes.find(pPart->GetUri())->second;
						if (curDispShape && curDispShape.IsNull())
							context->Erase(curDispShape);

						TopoDS_Shape partShape((const TopoDS_Shape&)pPart->getShape());
						gp_Trsf move;

						asmState->SetOperationType(AsmTreeNode::DISMANTLE);

						if (asmState->InitMove()){
							do {

								move.SetTranslation(asmState->Move());
								partShape.Move(move);
								partCenter.Translate(asmState->Move());

							} while (asmState->MoreMove());
						}

						helpShape = new AIS_Shape(partShape);
						
					}
					else
					{
						helpShape = mapOfShapes.find(pPart->GetUri())->second;
					}

					
					if (!helpShape.IsNull()){
						if (context->IsDisplayed(helpShape)){
							context->SetDisplayMode(helpShape, AIS_Shaded, false);
							context->SetColor(helpShape, Quantity_NOC_RED,false);
						}
						else{
							context->Display(helpShape, false);
							context->SetDisplayMode(helpShape, AIS_Shaded, false);
							context->SetColor(helpShape, Quantity_NOC_RED, false);
							//context->Hilight(helpShape, false);
						}
						//Open local context for axis visualization
						AsmVisualContextId = aViewer->getIC()->OpenLocalContext(false, false);


						if (asmState->InitMove()){
							auto trsf = asmState->Move();
							if (processType == AsmTreeNode::MOUNT)
								trsf.Reverse();
							//This arrow will be added to local contact and deleteded with one	
							ShowArrow(aViewer->getIC(), gp_Ax1(partCenter, trsf), trsf.Magnitude());
						}
					}
				}	
		}
		

		aViewer->Update();
		
	}
	catch (Standard_Failure){
		std::cerr << __FILE__ << " " << __LINE__ << Standard_Failure::Caught()->GetMessageString() << std::endl;
	}
}

Handle_TDocStd_Document AspMainTool::GetStdDoc(){
	return ocafDocument;
}
void AspMainTool::ShowProduct(Viewer* aView){
	//aView->getIC()->CloseAllContexts();
	//aView->getIC()->OpenLocalContext();
	int i =0;
	for (auto &aisPart : mapOfShapes){
		if (i>=ColorAmount)
			i=0;
		//aView->getIC()->LocalContext()->Display(aisPart.second, AIS_WireFrame);
		aView->getIC()->Display(aisPart.second, false);
		aView->getIC()->SetColor(aisPart.second, colorVar[++i], false);
		aView->getIC()->SetDisplayMode(aisPart.second, displayMode, false);
		
	}
	aView->Fit();
	//Set up current state of assembly/disassembly process (all part are display)
	if (asmSeq.IsDone())
	asmState = asmSeq.GetFullProductNode();
}
std::vector<Part *> AspMainTool::GetSelectedPart(Handle_AIS_InteractiveContext context){

	std::vector<Part *> selectedParts;
	for (context->InitSelected();
		context->MoreSelected();
		context->NextSelected()){

		auto ais_shape = Handle_AIS_Shape::DownCast(context->SelectedInteractive());
		if (ais_shape.IsNull())
			continue;

		Part *selectedPart = product->GetSamePart(ais_shape->Shape());
		if (selectedPart){
			selectedParts.push_back(selectedPart);
		}
		//context->Remove(context->Interactive(),false);
	}
	
	return selectedParts;
	
}
void AspMainTool::SetDisplayMode(AIS_DisplayMode dispMode, Viewer* aView){
	displayMode = dispMode;
	ShowProduct(aView);

}
void AspMainTool::ShowJustSelectedShape(Viewer* aView){

	Handle_AIS_InteractiveContext context = aView->getIC();

	std::vector<Part *> selectedParts = GetSelectedPart(context);

	context->ClearSelected(false);
	context->EraseAll(false);
	//context->OpenLocalContext(true, true);

	for (auto &p : selectedParts){
		auto aisPart = this->mapOfShapes.find(p->GetUri());
		if (aisPart != mapOfShapes.end())
		aView->getIC()->Display(aisPart->second, false);
		aView->getIC()->SetColor(aisPart->second, Quantity_NOC_GOLD3, false);
		aView->getIC()->SetDisplayMode(aisPart->second, AIS_WireFrame, false);
	}
	aView->Fit();
	aView->Update();
}
void AspMainTool::ShowInformAboutSelectedShape(Viewer* aView){
	Handle_AIS_InteractiveContext context = aView->getIC();

	std::vector<Part *> selectedParts = GetSelectedPart(context);
	
	context->ClearSelected(false);
	context->OpenLocalContext(true, true);

	for (auto &p : selectedParts){
		ShowBlkDirsOfPart(p, aView);
	}
	aView->Update();
}
void AspMainTool::ShowAssemblyInfo(MainFrame* appWindow){

	if (!product)
		return;

	_int fullAmount = product->UnitMap.size();
	_int faceAmount = 0;
	_int ContactAmount = 0;
	for (auto &unit : product->UnitMap){
		faceAmount += dynamic_cast<Part*>(unit.second)->colOfSurf.size();
		ContactAmount += dynamic_cast<Part*>(unit.second)->colOfCont.size();
	}
	ContactAmount /=2;
	QString status = "Parts# ";
	status += std::to_string(fullAmount).c_str();
	status += " & Faces# ";
	status += std::to_string(faceAmount).c_str();
	status += " & Contacts# ";
	status += std::to_string(ContactAmount).c_str();
	auto widget = QMessageBox::information(appWindow,"Assembly Information",status);
	
}
void AspMainTool::ShowSurface(const Handle_AIS_InteractiveContext &context, SurfaceAttribute &surf){
	Handle_AIS_Shape S = new AIS_Shape(surf.myShape);
	Quantity_Color shapeColor;
	_int DisplayMode;	
	switch (surf.Func){
	case _Base:
		shapeColor = Quantity_NOC_RED;
		DisplayMode = AIS_Shaded;
		//DisplayMode = AIS_WireFrame;
		break;
	case _UnknownFunction:
		shapeColor = Quantity_NOC_BLUE1;
		DisplayMode = AIS_WireFrame;
		break;
	default:
		//shapeColor = Quantity_NOC_GRAY;
		shapeColor = Quantity_NOC_BLACK;
		DisplayMode = AIS_WireFrame;
		break;
	}
	context->SetColor(S, shapeColor.Name(), false);
	//context->LocalContext()->Display(S, DisplayMode);
	context->LocalContext()->Display(S, DisplayMode);
	//context->SetDisplayMode(S,DisplayMode,false);
}
void AspMainTool::ShowArrow(const Handle_AIS_InteractiveContext &context, gp_Ax1 &axis, Standard_Integer arrowSize){

	if (context->LocalContext().IsNull())
		Standard_Failure::Raise("Arrow should be displayed into Local context");

	Handle_ISession_Direction ais_axis = new ISession_Direction(axis.Location(), axis.Direction(), arrowSize);

	context->LocalContext()->Display(ais_axis, AIS_WireFrame);
}
void AspMainTool::HideSelectedPart(Viewer *aView){
	Handle_AIS_InteractiveContext context = aView->getIC();

	std::vector<Part *> selectedParts = GetSelectedPart(context);
	context->ClearSelected(false);

	/*
	for (auto &p : selectedParts){
		auto aisShape = mapOfShapes.find(p->GetUri());
		if (aisShape != mapOfShapes.end())
			context->Erase(aisShape->second,false);	
	}
	*/
	aView->Update();
}
void AspMainTool::ShowBlkDirsOfPart(Part *part, Viewer *aView){

	auto context = aView->getIC();

	if (context->LocalContext().IsNull())
		Standard_Failure::Raise("Arrow should be displayed into Local context");


	for (SurfaceAttribute &surf : *part)
		ShowSurface(context, surf);
	//Display Part Name
	Handle_ISession_Text uri = new ISession_Text(std::to_string(part->GetUri()).c_str(), part->BndBox()->CornerMax());
	context->LocalContext()->Display(uri, AIS_WireFrame);

	gp_Pnt center = part->GetCenter();
	Standard_Real arrowSize = part->BndBox()->CornerMin().Distance(part->BndBox()->CornerMax());

	for (auto dir : asmSeq.getDirBlkGraph()->GetBlockedDirs(part->GetUri())){
		Handle_ISession_Direction axis = new ISession_Direction(center, dir.Direction(), arrowSize);
		axis->SetArrowColor(Quantity_NOC_BLACK);
		context->Display(axis,true);
		context->LocalContext()->Display(axis, AIS_WireFrame);
	}
}



Handle_TopTools_HSequenceOfShape AspMainTool::ListOfPart(){
	Handle_TopTools_HSequenceOfShape list = new TopTools_HSequenceOfShape();
	product->GetAllParts(list);
	return list;
}

