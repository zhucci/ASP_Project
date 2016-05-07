#include "MainFrame.h"
#include "qfiledialog.h"
#include <qmessagebox.h>
#include <Graphic3d_BufferType.hxx>
#include <AIS_LocalContext.hxx>
#include "AsmTreeNode.h"

MainFrame::MainFrame(void):QMainWindow(0)
{
	setupUi(this);
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
//debug functions
	connect(actionT1, SIGNAL(triggered()), this, SLOT(Test1()));
	connect(actionT2, SIGNAL(triggered()), this, SLOT(Test2()));
	connect(actionT3, SIGNAL(triggered()), this, SLOT(Test3()));
	connect(actionT4, SIGNAL(triggered()), this, SLOT(Test4()));
	connect(actionT5, SIGNAL(triggered()), this, SLOT(Test5()));
	connect(actionPartGraph, SIGNAL(triggered()), this, SLOT(PartGraph()));
	connect(actionViewMode, SIGNAL(triggered()), this, SLOT(ViewModeChanged()));

}
void MainFrame::ViewModeChanged(){
	justViewMode=!justViewMode;
}
MainFrame::~MainFrame(){

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

	//std::string sequence = aspTool->GetSequence(asp::AsmTreeNode::DISMANTLE);
	aspTool->PlayAssemblyAnimation(asp::AsmTreeNode::DISMANTLE, myViewer);
	//QMessageBox::about(this, "Sequence", sequence.c_str());
}
void MainFrame::DBGShow(){
	
	aspTool->ShowInformAboutSelectedShape(myViewer);
}

void MainFrame::Test1(){
	aspTool->TestContactSpotProcess(myViewer);
}

void MainFrame::Test2(){
	aspTool->FillPartWithPoints(myViewer);
}
void MainFrame::Test3(){
	aspTool->TestFindPartsPointsFunction(myViewer );
}
void MainFrame::Test4(){

}
void MainFrame::Test5(){

}
void MainFrame::TestVoxelGeneration(){

}
void TestVoxelGeneration(){

}
void MainFrame::PartGraph(){
	if (!aspTool){
		aspTool = new asp::AspMainTool;
	}
	aspTool->TestGraphIso(myViewer);
}
void MainFrame::contextMenu(const QPoint &pos){
	QAction *shadingMode = new QAction(tr("Display mode"), this);
	connect(shadingMode, SIGNAL(triggered()), myViewer, SLOT(ChangeShadingMode()));
	QAction *clearLocalContext = new QAction(tr("Clear Product"), this);
	connect(clearLocalContext, SIGNAL(triggered()), myViewer, SLOT(CloseLocalContext()));
	QAction *displayHiddenPart = new QAction(tr("Display hidden parts"), this);
	connect(displayHiddenPart, SIGNAL(triggered()), this, SLOT(ShowFullProduct()));
	QMenu menu(tr("Context"), this);
	menu.addAction(shadingMode);
	menu.addAction(clearLocalContext);
	menu.addAction(displayHiddenPart);
	menu.exec(pos);
}
void MainFrame::SetStatus(QString status){
	statusBar()->showMessage(status);
	qApp->processEvents();
}