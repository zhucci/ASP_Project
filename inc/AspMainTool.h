#ifndef _AspMainTool_
#define _AspMainTool_

#include "OCCT_Headers.h"
#include "FoundationClasses.h"
#include "Assembly.h"
#include "Part.h"
#include "Viewer.h"
#include "AsmTreeBuilder.h"
class Viewer;
class MainFrame;
namespace asp{

#define _CRT_SECURE_NO_WARNINGS


class AspMainTool
{
public:

	friend class AspMainTest;

	 AspMainTool();
	~AspMainTool();

	void Init(std::string stepfileName, Viewer *aView, MainFrame* appWindow=nullptr);

	void ShowInformAboutSelectedShape(Viewer *aViewer);

	void AspMainTool::HideSelectedPart(Viewer *aView);

	void ShowAssemblyStep(AsmTreeNode::AsmMoveType operType, Viewer * aViewer);

	void ShowAssemblyInfo(MainFrame* appWindow);

	void ShowJustSelectedShape(Viewer* aView);

	void SetDisplayMode(AIS_DisplayMode dispMode, Viewer* aView);

	void PlayAssemblyAnimation(AsmTreeNode::AsmMoveType processType, Viewer* aViewer);

	void								StopAssemblyAnimation(){ if (AsmTreeBuilding) asmSeq.Stop(); showMustGoOn = false; }

	void								ShowProduct(Viewer* aViewer);

	void								HideDisassembled(Viewer* aViewer);
	
	void								SetAsmTreeCalcStatus(Standard_Boolean status){AsmTreeCalculation = status;}

	std::string &&					GetLog();
	
	std::string						GetSequence(AsmTreeNode::AsmMoveType type);
	
protected:

	MainFrame *mainWindow;

	void								PartToAisShapeMap();

	std::vector<Part *>  		AspMainTool::GetSelectedPart(Handle_AIS_InteractiveContext context);

	void								ShowBlkDirsOfPart(Part *part, Viewer *aViewer);

	

	void								ShowSurface(const Handle_AIS_InteractiveContext &context, SurfaceAttribute &surf);

	void								ShowArrow(const Handle_AIS_InteractiveContext &context, gp_Ax1 &axis, Standard_Integer arrowSize);

	_bool								AsmTreeInit();

	IFSelect_ReturnStatus		ReadStep(Standard_CString filename);
	
	Handle_TDocStd_Document		GetStdDoc();
	Handle_TopTools_HSequenceOfShape ListOfPart();
	IFSelect_ReturnStatus		BuildAssebly();
	
	IFSelect_ReturnStatus		SequenceOptimize();
private:
	Standard_Boolean				showMustGoOn; //On and on, does anybody know what we are looking for...
	Standard_Boolean				AsmTreeCalculation{false};
	Standard_Boolean				FullAsmTreeCalc{false};
	Standard_Boolean				AsmTreeBuilding;
	Viewer*							myViewer;
	std::stringstream				LogFile;
	Handle(TDocStd_Document)	ocafDocument;
	Handle(XCAFApp_Application) ocafApplication;

	TopTools_HSequenceOfShape	productShapes;
//!Part Uri --> AIS_Shape
	std::map<_int, Handle_AIS_Shape> mapOfShapes;

	AIS_DisplayMode displayMode;

	_int AsmVisualContextId;
	struct PrevAsmStep{
		PrevAsmStep() :PartShape{ NULL },
		PartsHelpedShape{NULL},
		PartUri{0},
		ContextId{0}{}

		AsmTreeNode::AsmMoveType type;
		_int PartUri;
		_int ContextId;
		Handle_AIS_Shape PartShape;
		Handle_AIS_Shape PartsHelpedShape;

	} prevStep;
	
	_int ProductContext;
	Assembly *product;
	AsmTreeBuilder asmSeq;
	AsmTreeNode *asmState;
};


};
#endif