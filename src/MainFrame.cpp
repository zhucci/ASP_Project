#include "MainFrame.h"
#include "qfiledialog.h"
#include <qmessagebox.h>
#include <Graphic3d_BufferType.hxx>
#include <AIS_LocalContext.hxx>
#include "AsmTreeNode.h"
#include "AspMainTestClass.h"
MainFrame::MainFrame(void):QMainWindow(0)
{
	setupUi(this);
	displayMode = AIS_DisplayMode::AIS_WireFrame;
	this->setWindowIconText("AsmPlan");
	myViewer = new Viewer(this);
	myViewer->setFocus();

	setCentralWidget(myViewer);
	connect(actionEXIT, SIGNAL(triggered()),this,SLOT(Exit()));
	connect(actionSTEP, SIGNAL(triggered()),this,SLOT(ImportStep()));
	connect(actionABOUT, SIGNAL(triggered()),this,SLOT(about()));
	connect(actionSTEPBack, SIGNAL(triggered()),this,SLOT(StepBack()));
	connect(actionPlayBack, SIGNAL(triggered()),this,SLOT(PlayBack()));
	connect(actionStop, SIGNAL(triggered()),this,SLOT(Stop()));
	connect(actionNextStep, SIGNAL(triggered()),this,SLOT(NextStep()));
	connect(actionPlayFoward, SIGNAL(triggered()),this,SLOT(PlayFoward()));
	connect(actionFoto, SIGNAL(triggered()),this,SLOT(ExportPhoto()));
	connect(actionDBGShow, SIGNAL(triggered()),this,SLOT(DBGShow()));
	connect(myViewer, SIGNAL(contextMenuSignal(const QPoint &)), this, SLOT(contextMenu(const QPoint &)));
//Test panel functions
	connect(action_ContactTest, SIGNAL(triggered()), this, SLOT(TestContactSpotVerbose()));
	connect(action_PointOnFace, SIGNAL(triggered()), this, SLOT(TestPointOnPart()));
	connect(action_FaceTrait, SIGNAL(triggered()), this, SLOT(TestSpartialDesc()));
	connect(action_PartIso, SIGNAL(triggered()), this, SLOT(TestIsoFaceForPart()));
	connect(action_DBGTime, SIGNAL(triggered()), this, SLOT(TestDBGTime()));
	connect(actionPartGraph, SIGNAL(triggered()), this, SLOT(PartGraph()));
	connect(actionViewMode, SIGNAL(triggered()), this, SLOT(ViewModeChanged()));
	connect(action_AssemlyInfoRequest, SIGNAL(triggered()), this, SLOT(ShowAssemblyInfo()));
	connect(action_SetFrameDisplayMode, SIGNAL(triggered()), this, SLOT(SetWireFrameMode()));
	connect(action_SetShadedDisplayMode, SIGNAL(triggered()), this, SLOT(SetShadedMode()));
	connect(actionBodyDescriptor, SIGNAL(triggered()), this, SLOT(TestBodyDescriptor()));
	connect(action_VoxelGeneration, SIGNAL(triggered()), this, SLOT(TestVoxelGeneration()));
	
	connect(action_ShowFullProduct, SIGNAL(triggered()), this, SLOT(ShowFullProduct()));
	connect(action_HideDisassembled, SIGNAL(triggered()), this, SLOT(HideDisaasembledPart()));
}
void MainFrame::ViewModeChanged(){
	justViewMode=!justViewMode;
}
MainFrame::~MainFrame(){

}
void MainFrame::HideDisaasembledPart(){
	if (aspTool){
		aspTool->HideDisassembled(myViewer);
	}
}
void MainFrame::ShowAssemblyInfo(){
	if (aspTool){
		aspTool->ShowAssemblyInfo(this);
	}
}
void MainFrame::SaveSequence(){

}
	
void MainFrame::Exit(){
	
	close();
}
void MainFrame::Stop(){
	aspTool->StopAssemblyAnimation();
}
void MainFrame::ImportStep(){

//	myViewer->getIC()->RemoveAll(false);
//	myViewer->getIC()->CloseAllContexts(false);
//	myViewer->getIC()->OpenLocalContext(false, false, false);
	
	
	/*
	TCollection_ExtendedString fileName;
	QString fn = "C:\\StepModels\\Box.stp";
	std::fstream file;
	file.open(fn.toStdString().c_str(), std::ios::in);
	if (!file.is_open()){
	QString fn = QFileDialog::getOpenFileName(this, QString::null, QString("c:\\StepModels"), "*.stp");
	}
	else{
	file.close();
	}
	*/
	QString fn = QFileDialog::getOpenFileName(this, QString::null, QString("c:\\StepModels"), "*.stp");

	 if ( !fn.isEmpty() ){
		 if (!aspTool){
			aspTool = new asp::AspMainTool;
		 }
		 else{
			 delete aspTool;
			 delete myViewer;
			 aspTool=NULL;
			 myViewer = NULL;
			 myViewer = new Viewer(this);
			 myViewer->setFocus();
			 setCentralWidget(myViewer);
			 aspTool = new asp::AspMainTool;
		 }
		 aspTool->SetAsmTreeCalcStatus(!justViewMode);

		 aspTool->Init(fn.toStdString(), myViewer, this);
		 statusBar()->showMessage(tr("Ready"));
	}
	 
}
void MainFrame::ShowFullProduct(){

	aspTool->ShowProduct(myViewer);
}
void MainFrame::about(){
	    QMessageBox::about( this, tr("AssemPlan"),
			tr("This demo project to test functions of automation assembly planning systems"));
}

void MainFrame::ExportPhoto(){
	QString fn = QFileDialog::getSaveFileName(this, tr("Save Image"), QString::null, "*.jpg");
	if (!fn.isEmpty())
		myViewer->getView()->Dump(fn.toStdString().c_str(), Graphic3d_BufferType::Graphic3d_BT_RGBA);
	
}
void MainFrame::NextStep(){

	aspTool->ShowAssemblyStep(asp::AsmTreeNode::DISMANTLE, myViewer);
	statusBar()->showMessage("StepFoward");
}
void MainFrame::StepBack(){
	aspTool->ShowAssemblyStep(asp::AsmTreeNode::MOUNT, myViewer);
	statusBar()->showMessage("StepBack");
}
void MainFrame::PlayBack(){
	aspTool->PlayAssemblyAnimation(asp::AsmTreeNode::MOUNT, myViewer);
	//std::string sequence = aspTool->GetSequence(asp::AsmTreeNode::MOUNT);
	//QMessageBox::about(this, "Sequence", sequence.c_str());
}
void MainFrame::PlayFoward(){
	if (aspTool){
		aspTool->PlayAssemblyAnimation(asp::AsmTreeNode::DISMANTLE,myViewer);
	}
}
void MainFrame::DBGShow(){
	if (aspTool){
	aspTool->ShowInformAboutSelectedShape(myViewer);
	}
}

void MainFrame::TestContactSpotVerbose(){
	if (aspTool){
		asp::AspMainTest test;
		test.TestContactSpotProcess(this, aspTool);
	}
	
}

void MainFrame::TestPointOnPart(){
	if (aspTool){
		asp::AspMainTest test;
		test.FillPartWithPoints(this, aspTool);
	}
}
void MainFrame::TestSpartialDesc(){
	if (aspTool){
		asp::AspMainTest test;
		test.TestSpartialDescriptorCalculation(this, aspTool);
	}
}
void MainFrame::TestIsoFaceForPart(){
	if (aspTool){
		asp::AspMainTest test;
		test.TestIsoFaceForPartCalculation(this, aspTool);
	}
}
void MainFrame::TestDBGTime(){
	if (aspTool){
		asp::AspMainTest test;
		test.TestContactSpotTimeCalc(this, aspTool);
	}
	
}
void MainFrame::TestVoxelGeneration(){
	if (aspTool){
		asp::AspMainTest test;
		test.TestVoxelBuilder(this,aspTool);
	}
}
void MainFrame::TestBodyDescriptor(){
	if (aspTool){
		asp::AspMainTest test;
		test.TestDescriptorOFSelectedPartCalculation(this, aspTool);
	}
}
void MainFrame::PartGraph(){
	if (aspTool){
		asp::AspMainTest test;
		test.TestGraphIso(this,aspTool);
	}
}
void MainFrame::HideSelectedPart(){
	if (aspTool){
		aspTool->HideSelectedPart(myViewer);
	}
}
void MainFrame::SetShadedMode(){
	displayMode =  AIS_Shaded;

	if (aspTool)
		aspTool->SetDisplayMode(displayMode, myViewer);
	
}
void MainFrame::SetWireFrameMode(){

	displayMode = AIS_WireFrame;

	if (aspTool)
		aspTool->SetDisplayMode(displayMode,myViewer);
	
}
void MainFrame::DisplayJustSelected(){
	if (aspTool){
		aspTool->ShowJustSelectedShape(myViewer);
	}
}
void MainFrame::contextMenu(const QPoint &pos){
	QAction *shadingMode = new QAction(tr("Display mode"), this);
	connect(shadingMode, SIGNAL(triggered()), myViewer, SLOT(ChangeShadingMode()));
	QAction *clearLocalContext = new QAction(tr("Clear Product"), this);
	connect(clearLocalContext, SIGNAL(triggered()), myViewer, SLOT(CloseLocalContext()));
	QAction *displayHiddenPart = new QAction(tr("Display hidden parts"), this);
	connect(displayHiddenPart, SIGNAL(triggered()), this, SLOT(ShowFullProduct()));
	QAction *HidePart = new QAction(tr("Hide part"), this);
	connect(HidePart, SIGNAL(triggered()), myViewer, SLOT(HideSelectedShape()));
	QAction *displayJustSelected = new QAction(tr("Keep just this"), this);
	connect(displayJustSelected, SIGNAL(triggered()), this, SLOT(DisplayJustSelected()));
	QMenu menu(tr("Context"), this);

	menu.addAction(shadingMode);
	menu.addAction(HidePart);
	menu.addAction(displayJustSelected);
	menu.addAction(clearLocalContext);
	menu.addAction(displayHiddenPart);
	menu.exec(pos);
}
void MainFrame::SetStatus(QString status){
	statusBar()->showMessage(status);
	qApp->processEvents();
}