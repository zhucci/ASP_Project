
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

#include "assert.h"
//====================
//	aps namespace	 =
//====================
using namespace asp;
 
AspMainTool::AspMainTool(){
	ocafApplication = XCAFApp_Application::GetApplication();
	AsmVisualContextId = -1;
	showMustGoOn = true;
}
AspMainTool::~AspMainTool(){

}

void AspMainTool::Init(std::string stepfileName, Viewer *aView, MainFrame* mainWin){

	//	Standard_PCharacter file = new char[stepfileName.LengthOfCString()];
	mainWindow = mainWin;
	//	stepfileName.ToUTF8CString(file);
	if (ocafDocument.IsNull())
		ocafApplication->NewDocument("MDTV-XCAF", ocafDocument);
	else if (ocafDocument->IsOpened()){
		ocafDocument->Close();
		ocafApplication->NewDocument("MDTV-XCAF", ocafDocument);
		delete product;
		product = nullptr;
	}
	mainWindow->SetStatus("Reading STEP file is started");

	ReadStep(stepfileName.c_str());
	
	mainWindow->SetStatus("Reading STEP file has been finished");

	mainWindow->SetStatus("Building assembly view");

	BuildAssebly();
	
	PartToAisShapeMap();

	ShowProduct(aView);

	mainWindow->SetStatus("Finding assembly sequence is started");

	AsmTreeBuilding = true;
	AsmTreeInit();
	
	

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
	

#ifndef _DEBUG
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
						_int nStep = 40;
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
							Sleep(25);
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
					context->SetColor(helpShape,Quantity_NOC_GOLD3,false);
					context->SetDisplayMode(helpShape, AIS_WireFrame, false);
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
						context->SetDisplayMode(helpShape, AIS_WireFrame, false);
						context->SetColor(aisShape, Quantity_NOC_GOLD3,false);
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
	for (auto &aisPart : mapOfShapes){
		//aView->getIC()->LocalContext()->Display(aisPart.second, AIS_WireFrame);
		aView->getIC()->Display(aisPart.second, false);
		aView->getIC()->SetColor(aisPart.second, Quantity_NOC_GOLD3,false);
		aView->getIC()->SetDisplayMode(aisPart.second, AIS_WireFrame, false);
		
	}
	aView->Fit();
	//Set up current state of assembly/disassembly process (all part are display)
	if (asmSeq.IsDone())
	asmState = asmSeq.GetFullProductNode();
}
void AspMainTool::ShowInformAboutSelectedShape(Viewer* aView){
	Handle_AIS_InteractiveContext context = aView->getIC();

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
	context->ClearSelected();
	context->OpenLocalContext(true, true);
	for (auto &p : selectedParts){
		ShowBlkDirsOfPart(p, aView);
	}

	aView->Update();
}
void AspMainTool::ShowSurface(const Handle_AIS_InteractiveContext &context, SurfaceAttribute &surf){
	Handle_AIS_Shape S = new AIS_Shape(surf.myShape);
	Quantity_Color shapeColor;
	_int DisplayMode;	
	switch (surf.Func){
	case _Base:
		shapeColor = Quantity_NOC_RED;
		DisplayMode = AIS_Shaded;
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
		context->LocalContext()->Display(axis, AIS_WireFrame);
	}
}



Handle_TopTools_HSequenceOfShape AspMainTool::ListOfPart(){
	Handle_TopTools_HSequenceOfShape list = new TopTools_HSequenceOfShape();
	product->GetAllParts(list);
	return list;
}
void AspMainTool::TestContactSpotProcess(Viewer *view){

	auto unitMap = product->GetUnitMap();
	
	Standard_Real Gap = 0.5;
	Handle_AIS_InteractiveContext context = view->getIC();
	if (context->HasOpenedContext())
		context->CloseAllContexts();
	context->EraseAll(true);
	Standard_Integer curLC = context->OpenLocalContext();
		
	for (auto iter = unitMap->begin(); iter!=unitMap->end();iter++){
		Part *part = dynamic_cast<Part *>(iter->second);
		auto subItr = iter;

		for (subItr++; subItr!=unitMap->end();subItr++){
			Part *obstacle = dynamic_cast<Part *>(subItr->second);
			//==========================================
			//			Visualise current Parts
			context->ClearLocalContext(AIS_ClearMode::AIS_CM_Interactive);

			Handle_AIS_Shape shape = mapOfShapes.find(part->GetUri())->second;
			context->LocalContext()->Display(shape, AIS_Shaded);
			context->SetTransparency(shape, 0.2, false);
			shape = mapOfShapes.find(obstacle->GetUri())->second;;
			context->LocalContext()->Display(shape, AIS_WireFrame);
			view->getView()->Redraw();
			//==========================================
			if (obstacle->BndBox()->Distance(*part->BndBox()) < Gap)
			for (auto &sp : *part){
				if (sp.myBox.Distance(*obstacle->BndBox())<Gap)
				for (auto &obstSp : *obstacle){
					if (sp.myBox.Distance(obstSp.myBox)>Gap)
						continue;
				//==========================================
				//			Visualise current surface
				
					context->ClearLocalContext(AIS_ClearMode::AIS_CM_Interactive);
					ShowSurface(context, sp);
					ShowSurface(context, obstSp);
					view->getView()->Redraw();
				//==========================================
					TopAbs_Orientation orient1 = sp.myShape.Orientation();
					TopAbs_Orientation orient2 = obstSp.myShape.Orientation();
				//==========================================
				//			Launch function Contact Spot
				
					//if (NotInContact(sp, obstSp, 1, Gap))
					//	continue;

					ContactSpot spot(sp, obstSp, Gap);
					if (orient1 == TopAbs_REVERSED || orient2 == TopAbs_REVERSED)
						orient1 = TopAbs_FORWARD;
				//==========================================
				}
			}
		}
	}

}

void AspMainTool::TestFindPartsPointsFunction(Viewer* viewer){

	//assert(product != nullptr);
	
	Timer timer;
	timer.Start();
	_int fullAmount = 0;
	_int inPointsAmount = 0;
	
	auto iter = product->UnitMap.begin();
	auto end = product->UnitMap.end();
	

	for (_int i = 0; iter != end; ++iter, ++i){
		//Status updating
		QString status = "Pnt gen ";
		status += std::to_string(i).c_str();
		status += " / ";
		status += std::to_string(product->UnitMap.size()).c_str();
		mainWindow->SetStatus(status);
		Part *pPart = dynamic_cast<Part*>(iter->second);
		for (auto &face : *pPart){
			try{
				static _real dltU, dltV;
				static BRepClass_FaceClassifier classifier;
				static _bool notFound;
				notFound = true;
				dltU = face.surf.IsUPeriodic()? M_PI/6 : 4;
				dltV = face.surf.IsVPeriodic() ? M_PI / 6 : 4;

				static _real Urange, Vrange, uInf, vInf;
				uInf = face.surf.FirstUParameter();
				vInf = face.surf.FirstVParameter();
				Urange = std::abs(face.surf.LastUParameter() - uInf);
				Vrange = std::abs(face.surf.LastVParameter() - vInf);
				
				_int nbStpU = std::ceil(Urange / dltU);
				_int nbStpV = std::ceil(Vrange / dltV);

				dltU = Urange / nbStpU;
				dltV = Vrange / nbStpV;
				_real u = uInf;
				for (_int i = 0; i < nbStpU && notFound; ++i){
					static gp_Pnt2d pnt;
					u += dltU;

					pnt.SetX(u);
					_real v = vInf;
					for (_int j = 0; j < nbStpV && notFound; ++j){
						v += dltV;
						pnt.SetY(v);

						classifier.Perform(face.myShape, pnt, Precision::Confusion());

						++fullAmount;
						// ### Can be TopAbs_IN situation ?
						if (classifier.State() == TopAbs_ON || classifier.State() == TopAbs_IN){
							gp_Pnt pnt3d;
							face.surf.D0(pnt.X(), pnt.Y(), pnt3d);
							pPart->pntInPart.push_back(pnt3d);
							//notFound = false;

						}
					}
				}
				if (notFound)
					Standard_Failure::Raise("Can not find point on surface");
			}
			catch (Standard_Failure){
				QString st = "can not find points";
				mainWindow->SetStatus(st);
			}
		}
		inPointsAmount += pPart->pntInPart.size();
	}
	timer.Stop();

	_real speed = fullAmount / timer.Seconds();

	QString status = std::to_string(inPointsAmount).c_str();
	status += " / ";
	status += std::to_string(fullAmount).c_str();
	status += " on speed = ";
	status += std::to_string(speed).c_str();

	mainWindow->SetStatus(status);
	iter = product->UnitMap.begin();
	
//display points
	auto context = viewer->getIC();
	context->OpenLocalContext(); 
	//for (; iter != end; ++iter){
		Part *pPart = dynamic_cast<Part*>(iter->second);
		for (auto &pnt : pPart->pntInPart){
			Handle_Geom_CartesianPoint adaptPnt = new Geom_CartesianPoint(pnt);
			Handle_AIS_Point aisPoint = new AIS_Point(adaptPnt);
			
			context->LocalContext()->Display(aisPoint);
			context->SetColor(aisPoint, Quantity_NameOfColor::Quantity_NOC_RED,false);
		}
	//}
	viewer->getView()->Redraw();
}
void AspMainTool::FillPartWithPoints(Viewer * aViewer){
	auto iter = product->UnitMap.begin();
	auto end = product->UnitMap.end();

	
	Timer timer;
	timer.Start();
	_int fullAmount = 0;
	_int inPointsAMount = 0;
	for (_int i=0; iter != end; ++iter,++i){
//Status updating
		QString status = "Pnt gen ";
		status += std::to_string(i).c_str();
		status += " / ";
		status += product->UnitMap.size();
		mainWindow->SetStatus(status);

		static BRepClass3d_SolidClassifier solidClsf;

		TopoDS_Shape pShape = iter->second->getShape();
		gp_Pnt center = iter->second->GetCenter();
		Bnd_Box * pBox = iter->second->BndBox();
		gp_Pnt pBoxInf = pBox->CornerMin();
		gp_Pnt pBoxSup = pBox->CornerMax();

		//Points every 3 mm
		_real step = 2;
		_real XRange = pBoxSup.X() - pBoxInf.X();
		_real YRange = pBoxSup.Y() - pBoxInf.Y();
		_real ZRange = pBoxSup.Z() - pBoxInf.Z();

		_int nbX = std::ceil( XRange / step);
		_int nbY = std::ceil( YRange / step);
		_int nbZ = std::ceil( ZRange / step);
		_int amount = nbX*nbY*nbZ;
		fullAmount += amount;
		nbX = nbX ? nbX : 1;
		nbY = nbY ? nbY : 1;
		nbZ = nbZ ? nbZ : 1;

		_real stpX = XRange / nbX;
		_real stpY = YRange / nbY;
		_real stpZ = ZRange / nbZ;

		std::vector<gp_Pnt> pointsInsidePart;
		

		solidClsf.Load(pShape);

		for (_int i = 1; i <= nbX; ++i){
			static gp_XYZ tstPnt;
			static _real x = pBoxInf.X();
			x += stpX;
			tstPnt.SetX(x);
			_real y = pBoxInf.Y();
			for (_int j = 1; j <= nbY; ++j){
				y += stpY;
				tstPnt.SetY(y);
				_real z = pBoxInf.Z();
				for (_int k = 1; k <= nbZ; ++k){
					z += stpZ;
					tstPnt.SetZ(z);

					solidClsf.Perform(tstPnt, Precision::Confusion());

					if (solidClsf.State() == TopAbs_IN)
						pointsInsidePart.push_back(tstPnt);
				}	
			}
			status.clear();
			status += std::to_string(i*nbY*nbZ).c_str();
			status += " / ";
			status += std::to_string(amount).c_str();
			status += "T: ";
			status += timer.WhatTime().c_str();
			mainWindow->SetStatus(status);
		}
		inPointsAMount += pointsInsidePart.size();
	}
	timer.Stop();
	
	_real speed = fullAmount / timer.Seconds();
	
	QString status  = std::to_string(inPointsAMount).c_str();
			  status += " / ";
			  status += std::to_string(fullAmount).c_str();
			  status += " on speed = ";
			  status += std::to_string(speed).c_str();

	mainWindow->SetStatus(status);
}
