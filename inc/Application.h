#ifndef APPLICATION_H
#define APPLICATION_H

#include <qmainwindow.h>
#include <AIS_Shape.hxx>
#include <Voxel_Prs.hxx>
#include <Voxel_BoolDS.hxx>
#include <Voxel_ColorDS.hxx>
#include "Viewer.h"
#include "ui_mainwindow.h"
#include "AspMainTool.h"

class Application: public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

public:
    Application();
    ~Application();

protected:
    void closeEvent( QCloseEvent* );
	
private slots:
    void box();
    void cylinder();
    void torus();
    void sphere();
    void choose();
    void load( const QString & );
    void load( const TopoDS_Shape & );
    void save();
    void open();

    void testBoolDS();
    void testColorDS();
    void testFloatDS();
    void testOctBoolDS();
    void testROctBoolDS();

    void testFuseBoolDS();
    void testFuseColorDS();
    void testFuseFloatDS();
    void testCutBoolDS();
    void testCutColorDS();
    void testCutFloatDS();
    
    void convert2bool();
    void convert2color();
    void convert(const int );

    void setNbX();
    void setNbY();
    void setNbZ();

    void setScanSide();

    void setVolumicBoolValue();
    void setVolumicColorValue();

    void setQuadrangleSize();
    void setPointSize();

    void setColorMinValue();
    void setColorMaxValue();

    void setUsageOfGLlists();
    
    void setDisplayedXMin();
    void setDisplayedXMax();
    void setDisplayedYMin();
    void setDisplayedYMax();
    void setDisplayedZMin();
    void setDisplayedZMax();

    void displayPoints();
    void displayNearestPoints();
    void displayBoxes();
    void displayNearestBoxes();

    void displayColorScale();
    
    void displayWaves();
    void displayCut();
    void displayCollisions();

    void about();

private:
    void display(Voxel_VoxelDisplayMode );
    void initPrs();


	Viewer* myViewer;
	asp::AspMainTool aspTool;
	Handle(AIS_Shape) myShape;
	Handle(Voxel_Prs) myVoxels;
	Voxel_BoolDS* myBoolVoxels;
	Voxel_ColorDS* myColorVoxels;
	int myNbX;
	int myNbY;
	int myNbZ;
    unsigned char myScanSide;
    bool myVolumicBoolValue;
    unsigned char myVolumicColorValue;
    int myQuadrangleSize;
    int myPointSize;
    unsigned char myColorMinValue;
    unsigned char myColorMaxValue;
    double myDisplayedXMin;
    double myDisplayedXMax;
    double myDisplayedYMin;
    double myDisplayedYMax;
    double myDisplayedZMin;
    double myDisplayedZMax;
};

#endif // APPLICATION_H
