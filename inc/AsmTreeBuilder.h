#ifndef _AsmTreeBuilder_
#define _AsmTreeBuilder_

#include "OCCT_Headers.h"
#include "Part.h"
#include "ContactSpot.h"
#include "surfaceAttribute.h"
#include "DBGBuilder.h"
#include "AsmTreeNode.h"
class MainFrame;
namespace asp{

//!Class for assembly sequence building
class AsmTreeBuilder{
public:
						AsmTreeBuilder();
		
						~AsmTreeBuilder();

		void			Init(Unit* product, MainFrame* mainWin);

		operator		AsmTreeNode && ();

		AsmTreeNode *GetFullProductNode();

		AsmTreeNode *GetWholeProductNode();

		_bool			IsDone(){ return done; }
		
		void			Stop(){backTraceIterCounter=backTraceMaxAmount+1;done=true;}

		DBGBuilder	*getDirBlkGraph(){ return &myDBGBuilder; }

private:								//Functions
		void							InitDirBlockGraph			();

		void							InitAssemblySequence		();

		_bool							continueDisassembly		(AsmTreeNode * root);

		_bool							rejectDisassembly			(AsmTreeNode * node);

		_bool							acceptedDisassembly		(AsmTreeNode * node);

		void							ExploreNode					(AsmTreeNode * node);

		std::list<AsmTreeNode>  FreePartFromDBGView		(AsmTreeNode * root);
		
		std::vector<asp_Ax1>		FreeDirs						(_int PartUri, 
																		 AsmTreeNode *curNodeInTree);

		_bool							IsBlocked					(asp_Ax1 dir, 
																		 std::vector<asp_Ax1> *colOfBlkDir);

		_real							DistanceToBndBorder		(gp_Ax1 axis, 
																		Bnd_Box *targetBorder);

		_real							DistanceToBndBorder		(Part *part, 
																		 gp_Ax1 axis, 
																		 Bnd_Box *bnd);

		void							FindPointsOnPartSurface	(Part *pPart);

		_bool							MoveFeasibilityCheck		(AsmTreeNode &asmProc);

		_bool							IsMoveAlongDirBlocked	(Part *part, 
																		 gp_Ax1 axis, 
																		 _int Distance, 
																		 std::vector<_int> &ObstacleURI);

		static	_bool				IsLineIntersectPart		(Handle(Geom_TrimmedCurve) line, Part* const part);

		
										//DATA

//product structure
		Unit							*myAssembly;
//For calculation process feedback
		MainFrame					*mainWindow;
//disassembled parts
		closed_list<_int>			closedList;
//DBG Structure with it's builder
		DBGBuilder					myDBGBuilder;
//Assembly sequence representation
		AsmTreeNode					asmSequence;
//Final state	
		_bool							done;
		_int							backTraceIterCounter;
		const _int					backTraceMaxAmount=50000;
		
};

};
#endif /*_UnitTools*/
