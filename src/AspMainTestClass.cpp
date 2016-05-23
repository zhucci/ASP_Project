#include "AspMainTestClass.h"
#include <Geom_Axis1Placement.hxx>
#include <AIS_Point.hxx>
#include <AIS_LocalContext.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <Geom_CartesianPoint.hxx>
#include <qstatusbar.h>
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
#include "qapplication.h"
#include "MainFrame.h"
#include "DBGBuilder.h"
#include "OCCT_Headers.h"
#include "FoundationClasses.h"
#include "Assembly.h"
#include "Part.h"
#include "Viewer.h"
#include "AsmTreeBuilder.h"
#include "AspMainTool.h"
#include "DBGBuilder.h"
#include "qmessagebox.h"
#include "GoodColor.h"
#include "Voxel_BoolDS.hxx"
#include "Voxel_FastConverter.hxx"
#include "Voxel_CollisionDetection.hxx"

using namespace asp;


void AspMainTest::TestGraphIso(MainFrame * frame, AspMainTool *tool){
	
	Handle_AIS_InteractiveContext context = frame->myViewer->getIC();

	std::vector<Part *> selectedParts = tool->GetSelectedPart(context);

	AssemblyGraphBuilder graph;

	graph.Init(tool->product);

	//Handle_TopTools_HSequenceOfShape collectionOfParts;

	//auto partMap = tool->product->GetUnitMap();

	//std::string ImaleName = graph.GetPartGraphImageFileName(partMap->begin()->first);

	//QGraphicsScene scene;

}
void AspMainTest::TestFindPartsPointsFunction(MainFrame * frame, AspMainTool *tool){

	//assert(product != nullptr);

	Timer timer;
	timer.Start();
	_int fullAmount = 0;
	_int inPointsAmount = 0;

	auto iter = tool->product->UnitMap.begin();
	auto end = tool->product->UnitMap.end();


	for (_int i = 0; iter != end; ++iter, ++i){
		//Status updating
		QString status = "Pnt gen ";
		status += std::to_string(i).c_str();
		status += " / ";
		status += std::to_string(tool->product->UnitMap.size()).c_str();
		frame->SetStatus(status);
		Part *pPart = dynamic_cast<Part*>(iter->second);
		for (auto &face : *pPart){
			try{
				static _real dltU, dltV;
				static BRepClass_FaceClassifier classifier;
				static _bool notFound;
				notFound = true;
				dltU = face.surf.IsUPeriodic() ? M_PI / 6 : 4;
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
				frame->SetStatus(st);
			}
		}
		inPointsAmount += (_int) pPart->pntInPart.size();
	}
	timer.Stop();

	_real speed = fullAmount / timer.Seconds();

	QString status = std::to_string(inPointsAmount).c_str();
	status += " / ";
	status += std::to_string(fullAmount).c_str();
	status += " on speed = ";
	status += std::to_string(speed).c_str();

	frame->SetStatus(status);
	iter = tool->product->UnitMap.begin();

	//display points
	auto context = frame->myViewer->getIC();
	context->OpenLocalContext();
	//for (; iter != end; ++iter){
	Part *pPart = dynamic_cast<Part*>(iter->second);
	for (auto &pnt : pPart->pntInPart){
		Handle_Geom_CartesianPoint adaptPnt = new Geom_CartesianPoint(pnt);
		Handle_AIS_Point aisPoint = new AIS_Point(adaptPnt);

		context->LocalContext()->Display(aisPoint);
		context->SetColor(aisPoint, Quantity_NameOfColor::Quantity_NOC_RED, false);
	}
	//}
	frame->myViewer->getView()->Redraw();
}
void AspMainTest::TestSpartialDescriptorCalculation(MainFrame *frame, AspMainTool *tool){

	auto unitMap = tool->product->GetUnitMap();
	auto view = frame->myViewer;


	Handle_AIS_InteractiveContext context = view->getIC();
	if (context->HasOpenedContext())
		context->CloseAllContexts();
	context->EraseAll(true);

	Standard_Integer curLC = context->OpenLocalContext();

	for (auto iter = unitMap->begin(); iter != unitMap->end(); iter++){
		Part *part = dynamic_cast<Part *>(iter->second);
	
			//==========================================
			//			Visualise current Parts
			context->ClearLocalContext(AIS_ClearMode::AIS_CM_Interactive);

			Handle_AIS_Shape shape = tool->mapOfShapes.find(part->GetUri())->second;
			context->LocalContext()->Display(shape, AIS_Shaded);
			view->getView()->Redraw();

			AssemblyGraphBuilder builder;
			auto form = builder.getBodyDescriptor(part);
			
			frame->statusBar()->showMessage(std::to_string(form.MatVolRatio).c_str());

			auto end = part->colOfSurf.end();

			for (auto sp=part->colOfSurf.begin();sp!=end;sp++){
				auto op = sp;
				for (++op;  op != end; op++){

					//==========================================
					//			Visualise current surface

					context->ClearLocalContext(AIS_ClearMode::AIS_CM_Interactive);
					tool->ShowSurface(context, *sp);
					tool->ShowSurface(context, *op);
					view->getView()->Redraw();
					//==========================================
					
					//==========================================
					//			Test 
					//==========================================
					
					std::pair<gp_Pnt, gp_Vec> s1Desc;
					if(builder.GetTraitOfFace(*sp,s1Desc)){

						Handle_ISession_Direction axis = new ISession_Direction(s1Desc.first, s1Desc.second*10, 3);
						Handle_Geom_CartesianPoint adaptPnt = new Geom_CartesianPoint(s1Desc.first);
						
						axis->SetColor(Quantity_NOC_RED);

						Handle_AIS_Point aisPoint = new AIS_Point(adaptPnt);
						
						context->LocalContext()->Display(aisPoint);
						context->LocalContext()->Display(axis, AIS_WireFrame);
						
					}
					if (builder.GetTraitOfFace(*op, s1Desc)){
						Handle_Geom_CartesianPoint adaptPnt = new Geom_CartesianPoint(s1Desc.first);
						Handle_AIS_Point aisPoint = new AIS_Point(adaptPnt);

						context->LocalContext()->Display(aisPoint);
						Handle_ISession_Direction axis = new ISession_Direction(s1Desc.first, s1Desc.second*20, 5);
						axis->SetColor(Quantity_NOC_BLACK);
						context->LocalContext()->Display(axis, AIS_WireFrame);
						
					}
					auto FaceDesc = builder.getFaceSpartialDescriptor(*sp,*op);
					std::ostringstream message;
					message << "Ang = " << std::to_string(FaceDesc.AngDesc) << "  Dist  " << std::to_string(FaceDesc.DistDesc);
					frame->statusBar()->showMessage(message.str().c_str());
					view->getView()->Redraw();
					message.clear();
				}
			}
	}
	context->CloseLocalContext(curLC);
	
}
void AspMainTest::TestDescriptorOFSelectedPartCalculation(MainFrame *frame, AspMainTool *tool){

	auto aView = frame->myViewer;

	Handle_AIS_InteractiveContext context = aView->getIC();

	std::vector<Part *> selectedParts = tool->GetSelectedPart(context);

	if (selectedParts.size() < 1){
		QString status = "Firstly, choose one part then try again!";
		
		QMessageBox::information(frame, "Assembly Information", status);
		return;
	}
		

		AssemblyGraphBuilder builder;
		auto form = builder.getBodyDescriptor(selectedParts[0]);

		frame->statusBar()->showMessage(std::to_string(form.MatVolRatio).c_str());

}

void AspMainTest::TestIsoFaceForPartCalculation(MainFrame* frame, AspMainTool *tool){

	auto aView = frame->myViewer;

	Handle_AIS_InteractiveContext context = frame->myViewer->getIC();

	std::vector<Part *> selectedParts = tool->GetSelectedPart(context);

	if (selectedParts.size()<2)
		return;

	/*auto &PartMap = */selectedParts[0]->UnitMap;
	

	
	AssemblyGraphBuilder builder;
	//auto NewPart = Part(*(selectedParts[0]));
	//for (new)
	auto p1G = builder.GetPartGraphsSet(selectedParts[0],false);
	auto p2G = builder.GetPartGraphsSet(selectedParts[1], false);
	
	std::vector<PartIsomorphism> res;
	bool result;
	bool isReversed = false;
	if (p1G.first.vertex_set().size() > p2G.first.vertex_set().size() ||
		p1G.second.vertex_set().size() > p2G.second.vertex_set().size())
	{
		result = builder.IsomorphismOfPartCompare(p2G, p1G, &res);
		isReversed= true;
	}
	else{
		result = builder.IsomorphismOfPartCompare(p1G, p2G, &res);
	}
	context->ClearSelected(false);
	
	if(!result)
		return;

	context->EraseAll();
	context->OpenLocalContext(true, true);
	if (!isReversed){
		res[0].isoPart.first = selectedParts[0]->GetUri();
		res[0].isoPart.second = selectedParts[1]->GetUri();
	}
	else{
		res[0].isoPart.first = selectedParts[1]->GetUri();
		res[0].isoPart.second = selectedParts[0]->GetUri();
	}

	_int cColor=0;
	for (auto &iso : res){
	auto partIter=	std::find_if(selectedParts.begin(), selectedParts.end(),[&iso](Part* p)->bool{return iso.isoPart.first==p->GetUri();});
	auto coPartIter = std::find_if(selectedParts.begin(), selectedParts.end(), [&iso](Part* p)->bool{return iso.isoPart.second == p->GetUri(); });
	auto end = selectedParts.end();

	if (partIter == end && coPartIter == end)
		continue;

	auto FaceSetEnd = iso.isoFaceSet.end();
		for (auto &surfP1: *(*partIter)){
			auto surfP1Uri = surfP1.uri;

			auto isoFacesPair = find_if(iso.isoFaceSet.begin(), FaceSetEnd, [&surfP1Uri](std::pair<FaceUri, FaceUri> &f)-> bool { return f.first == surfP1Uri; });

			if (isoFacesPair == FaceSetEnd)
				continue;

			auto surfP2Uri = isoFacesPair->second;

			auto &coPartSurfCol = (*coPartIter)->colOfSurf;

			auto surfP2 = find_if(coPartSurfCol.begin(), coPartSurfCol.end(), [&surfP2Uri](const SurfaceAttribute &att)->bool{return att.uri==surfP2Uri;});

			if (surfP2 == coPartSurfCol.end())
					continue;

			Handle_AIS_Shape S1 = new AIS_Shape(surfP1.myShape);
			Handle_AIS_Shape S2 = new AIS_Shape(surfP2->myShape);

			Quantity_Color shapeColor = Color::name(++cColor);

			_int DisplayMode= AIS_Shaded;
			
		
		
		context->LocalContext()->Display(S1, DisplayMode);
		context->LocalContext()->Display(S2, DisplayMode);

		context->SetColor(S1, shapeColor.Name(), false);
		context->SetColor(S2, shapeColor.Name(), false);

		
		}
	}

	aView->Update();

}
void AspMainTest::TestContactSpotTimeCalc(MainFrame * frame, asp::AspMainTool *tool){
	
	if (!tool->product)
		return;
	_int fullAmount =(_int) tool->product->UnitMap.size();
	_int faceAmount = 0;
	for (auto &unit : tool->product->UnitMap){
		faceAmount += (_int) dynamic_cast<Part*>(unit.second)->colOfSurf.size();
	}
	Timer timer;
	
	timer.Start();

	DBGBuilder builder;
	builder.AddDBGRelation(tool->product);

	timer.Stop();

	QString status = "Parts# ";
	status += std::to_string(fullAmount).c_str();
	status += " & Faces# ";
	status += std::to_string(faceAmount).c_str();
		status += "T: ";
		status += timer.WhatTime().c_str();
		frame->SetStatus(status);
}
void AspMainTest::FillPartWithPoints(MainFrame * mainWindow, AspMainTool *tool){
	auto iter = tool->product->UnitMap.begin();
	auto end = tool->product->UnitMap.end();


	Timer timer;
	timer.Start();
	_int fullAmount = 0;
	_int inPointsAMount = 0;
	for (_int i = 0; iter != end; ++iter, ++i){
		//Status updating
		QString status = "Pnt gen ";
		status += std::to_string(i).c_str();
		status += " / ";
		status += std::to_string( tool->product->UnitMap.size()).c_str();
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

		_int nbX = std::ceil(XRange / step);
		_int nbY = std::ceil(YRange / step);
		_int nbZ = std::ceil(ZRange / step);
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
		inPointsAMount += (_int) pointsInsidePart.size();
	}
	timer.Stop();

	_real speed = fullAmount / timer.Seconds();

	QString status = std::to_string(inPointsAMount).c_str();
	status += " / ";
	status += std::to_string(fullAmount).c_str();
	status += " on speed = ";
	status += std::to_string(speed).c_str();

	mainWindow->SetStatus(status);
}
void AspMainTest::TestContactSpotProcess(MainFrame * frame, AspMainTool *tool){

	auto unitMap = tool->product->GetUnitMap();
	auto view = frame->myViewer;

	Standard_Real Gap = 0.5;
	Handle_AIS_InteractiveContext context = view->getIC();
	if (context->HasOpenedContext())
		context->CloseAllContexts();

	context->EraseAll(true);

	Standard_Integer curLC = context->OpenLocalContext();

	for (auto iter = unitMap->begin(); iter != unitMap->end(); iter++){
		Part *part = dynamic_cast<Part *>(iter->second);
		auto subItr = iter;
		for (++subItr; subItr != unitMap->end(); subItr++){
			Part *obstacle = dynamic_cast<Part *>(subItr->second);
			//==========================================
			//			Visualise current Parts
			context->ClearLocalContext(AIS_ClearMode::AIS_CM_Interactive);

			Handle_AIS_Shape shape = tool->mapOfShapes.find(part->GetUri())->second;
			context->LocalContext()->Display(shape, AIS_Shaded);
			context->SetTransparency(shape, 0.2, false);
			shape = tool->mapOfShapes.find(obstacle->GetUri())->second;;
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
					tool->ShowSurface(context, sp);
					tool->ShowSurface(context, obstSp);
					view->getView()->Redraw();
					//==========================================
					
					//==========================================
					//			Launch function Contact Spot

					if (asp::DBGBuilder::NotInContact(sp, obstSp, 1, Gap))
						continue;

					ContactSpot spot(sp, obstSp, part, obstacle);
					if (spot.IsDone()){

						//==========================================
						auto vecArray1 = spot.getF1ContactAxis();
						auto vecArray2 = spot.getF2ContactAxis();

						if (vecArray1.size() && vecArray2.size()){
							for (auto &dir : vecArray1){
								Handle_ISession_Direction axis = new ISession_Direction(dir.Location(), dir.Direction(), 10);
								context->LocalContext()->Display(axis, AIS_WireFrame);
							}

							for (auto &dir : vecArray2){
								Handle_ISession_Direction axis = new ISession_Direction(dir.Location(), dir.Direction(), 10);
								context->LocalContext()->Display(axis, AIS_WireFrame);
							}
							view->getView()->Redraw();
						}
						//}
					}
				}
			}

		}
	}
	context->CloseLocalContext(curLC);

}
void AspMainTest::TestVoxelBuilder(MainFrame* frame, AspMainTool *tool){
	Timer timer;
	timer.Start();

	auto assemblyShape = tool->product->ColSubUnit.front();
	//Amount of voxel calculation
	gp_Pnt min = assemblyShape->myBox->CornerMin();
	gp_Pnt max = assemblyShape->myBox->CornerMax();
	_real voxelSize = 3.;
	int ThreadAmount = 1;
	int nbX = (int)((max.X() - min.X())/voxelSize );
	int nbY = (int)((max.Y() - min.Y())/voxelSize);
	int nbZ = (int)((max.Z() - min.Z())/voxelSize);
	if (assemblyShape->myShapeType==TopAbs_COMPOUND){

		Voxel_BoolDS voxelShape;
		Voxel_FastConverter converter(assemblyShape->myshape, voxelShape, 0.1, nbX, nbY, nbZ, ThreadAmount);
		frame->ProgressBar->show();
		int status;
		converter.Convert(status,ThreadAmount);
		
	}
	timer.Stop();

	QString status = "Voxels# ";
	status += std::to_string(nbX*nbY*nbZ).c_str();
	status += "T: ";
	status += timer.WhatTime().c_str();
	frame->SetStatus(status);
	
}
