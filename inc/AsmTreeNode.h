#ifndef _AsmTreeNode_
#define _AsmTreeNode_
#include "STL_Headers.h"
#include "OCCT_Headers.h"
#include "Part.h"

namespace asp{

#ifndef _AsmTreeNode
#define _AsmTreeNode

	class AsmTreeNode;

	class AssemblyPartPriorityCompare{
	public:
	 bool operator()(const AsmTreeNode &node1, const AsmTreeNode &node2);
	};
#endif
	

	class AsmTreeNode{

	public:
		enum		AsmMoveType{UNKNOWN, MOUNT, DISMANTLE };
		typedef  gp_Vec Dir;

		friend	class		AsmTreeBuilder;
		friend	class		AssemblyPartPriorityCompare;
		
		AsmTreeNode( AsmTreeNode *root = nullptr );
		AsmTreeNode( const AsmTreeNode &node );
		AsmTreeNode( AsmTreeNode &&node );
		AsmTreeNode(Part *part, Dir dsmDir, AsmTreeNode *root);

	//!next assemble or disassemble operation
	//! To get assemble operation to use "SetOperationType(MOUNT);
	//! To get disassemble operation to use "SetOperationType(DISMANTLE);
	//! Changing OperationType not unvalidate Move Iteration
		AsmTreeNode*			Next();
		AsmTreeNode*			Prev();
	//! type : MOUNT, DISMANTLE
		void						SetOperationType(AsmMoveType type){ operationType = type; }

		bool						InitMove();

		bool						MoreMove();

		void						NextMove();

		Dir						Move();

		Standard_Boolean		InTheEnd();

		Standard_Integer		MovedPartUri()	{ return myPart == nullptr ? -1 : myPart->GetUri(); }

		Part*						MovedPart()		{ return myPart; }

	//!Assembly tree is not actualy a tree it's a lattice 
	//!because we have many parts from whome we may start product assembly or disaasembly operation
	//! If return <true> just set Assembly operation -> Check is it end -> Get Next Element 
		Standard_Boolean		IsAbsNode(){ return myPart == nullptr; }
	protected:
		Part* myPart;
	std::list<AsmTreeNode*>	parentNodes;

		AsmMoveType				operationType;

		Standard_Integer		rank;

		Standard_Boolean		MoveTraced;

		std::vector<Dir>::iterator		moveIter;

		std::vector<Dir>::iterator		endIter;

		std::vector<Dir>					asmMove;
		
		std::list<AsmTreeNode>			childNodes;

	};
	
};
#endif