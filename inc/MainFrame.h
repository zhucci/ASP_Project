#pragma once
#include <qmainwindow.h>
#include "ui_mainframe.h"
#include "AspMainTool.h"
#include "Viewer.h"

//UIC compiled mainFrame.ui file
//uic.exe src/mainFrame.ui -o win64/vc10/srcd/ui_mainFrame.h

namespace asp{
 class	AspMainTest;
};

class MainFrame :
	public QMainWindow, public Ui::MainFrame
{
	Q_OBJECT
public:

	friend class asp::AspMainTest;

	MainFrame(void);

	~MainFrame(void);

	void SetStatus(QString status);

private slots:
	void ViewModeChanged();
	void ShowAssemblyInfo();
	void SaveSequence();
	void ImportStep();
	void PlayBack();
	void StepBack();
	void Stop();
	void NextStep();
	void PlayFoward();
	void DBGShow();
	void ExportPhoto();
	void ShowFullProduct();
	void HideSelectedPart();
	void DisplayJustSelected();
	void SetShadedMode();
	void SetWireFrameMode();
	void about();
	void contextMenu(const QPoint &pos);
	void Exit();
//Debug session
	void Test1();
	void Test2();
	void Test3();
	void Test4();
	void Test5();
	void PartGraph();
	void TestVoxelGeneration();
	

	
protected:
	AIS_DisplayMode displayMode;
	asp::AspMainTool *aspTool{NULL};
	_bool justViewMode{false};
	Viewer* myViewer;

};

