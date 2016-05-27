#ifndef _AspMainTest_
#define _AspMainTest_

class Viewer;
class MainFrame;
namespace asp{
class AspMainTool;
};
namespace asp{

#define _CRT_SECURE_NO_WARNINGS

	class AspMainTest
	{
	public:
		AspMainTest(){}
		~AspMainTest(){}

		//Test functions
		void	TestContactSpotProcess(MainFrame * frame, asp::AspMainTool *tool);
		
		void TestFindPartsPointsFunction(MainFrame * frame, asp::AspMainTool *tool);
		void FillPartWithPoints(MainFrame * frame, asp::AspMainTool *tool);
		void TestContactSpotTimeCalc(MainFrame * frame, asp::AspMainTool *tool);
//Graph feature testing
		void	TestGraphIso(MainFrame * frame, asp::AspMainTool *tool);
		void TestSpartialDescriptorCalculation(MainFrame *frame, AspMainTool *tool);
		void TestDescriptorOFSelectedPartCalculation(MainFrame *frame, AspMainTool *tool);
//Color isomarph faces of selected two part
		void TestIsoFaceForPartCalculation(MainFrame* frame, AspMainTool *tool);
		void TestVoxelBuilder(MainFrame*frame, AspMainTool *tool);
		void TestCollitionDetecton(MainFrame *frame, AspMainTool *tool);
		void TestCurvePartIntersectionFunction(MainFrame *frame, AspMainTool *tool);
	};


};

#endif