#include "AsmTreeNode.h"
#include <cassert>
using namespace asp;

AssemblyPartPriorityCompare Compare;

AsmTreeNode::AsmTreeNode(const AsmTreeNode &node){
	myPart = node.myPart;
	operationType = node.operationType;
	MoveTraced = node.MoveTraced;
	rank = node.rank;
	for (auto iter = node.asmMove.begin(); iter != node.asmMove.end(); iter++){
		asmMove.push_back(*iter);
		if (node.moveIter == iter)
			moveIter = --asmMove.end();
	}
	endIter = asmMove.end();

	childNodes.insert(	childNodes.begin(),
								node.childNodes.begin(), 
								node.childNodes.end());

	parentNodes.insert(	parentNodes.begin(),
								node.parentNodes.begin(),
								node.parentNodes.end());
}
AsmTreeNode::AsmTreeNode(AsmTreeNode &&node):
operationType(std::move(node.operationType)),
MoveTraced(node.MoveTraced),
rank(node.rank),
childNodes(std::move(node.childNodes)),
asmMove(std::move(node.asmMove)),
parentNodes(std::move(node.parentNodes))
{
	myPart = node.myPart;
}
AsmTreeNode::AsmTreeNode(AsmTreeNode *root){
	if(root) parentNodes.push_back(root);
	myPart = nullptr;	
	rank=0;
	MoveTraced=false;
}

AsmTreeNode::AsmTreeNode(Part *part, Dir dsmDir, AsmTreeNode *root):
	myPart(part),
	rank(0),
	MoveTraced(false)
{

	if(root) 
		parentNodes.push_back(root);

	asmMove.push_back(dsmDir);
}

AsmTreeNode *AsmTreeNode::Next(){
	if (operationType == MOUNT){
		if (parentNodes.size())
			return *parentNodes.begin();
		else
			return nullptr;
	}
	else if (operationType == DISMANTLE){
		if (childNodes.size())
			return &(*childNodes.begin());
		else
			return nullptr;
	}
	return nullptr;
}
AsmTreeNode * AsmTreeNode::Prev(){
	if (operationType == MOUNT){
		if (childNodes.size())
			return &(*childNodes.begin());
		else
			return nullptr;
	}
	else if (operationType == DISMANTLE){
		if (parentNodes.size())
			return *parentNodes.begin();
		else
			return nullptr;
	}
	return nullptr;
}
Standard_Boolean AsmTreeNode::InTheEnd(){
	if (operationType == MOUNT){
		return parentNodes.size() == 0;
	}
	else if (operationType == DISMANTLE){
		return childNodes.size() == 0;
	}
	else
		return true;
}
bool AsmTreeNode::InitMove(){

	if (asmMove.size()){
		switch (operationType){
		case DISMANTLE :
			moveIter = asmMove.begin();
			endIter = asmMove.end();
			break;
		case MOUNT:
			moveIter = --(asmMove.end());
			endIter = --(asmMove.begin());
			break;
		default :
			Standard_Failure::Raise("Operation type unspecified");
			break;
		}
		
		return true;
	}
	
	return false;
}
bool AsmTreeNode::MoreMove(){
	switch (operationType){
	case MOUNT:
		moveIter--;
		if (moveIter == endIter)
			return false;
		break;
	case DISMANTLE:
		moveIter++;
		if (moveIter == endIter)
			return false;
		break;
	default:
		Standard_Failure::Raise("Operation type unspecified");
		break;
	}
	return true;
}

AsmTreeNode::Dir AsmTreeNode::Move(){
	if(operationType== MOUNT){
		return (*moveIter).Reversed();	
	}
	return (*moveIter); 
}

bool AssemblyPartPriorityCompare::operator()(const AsmTreeNode &node1, const AsmTreeNode &node2){
	//Goods part disassemble first!
	try{
		if (node1.myPart->GetUri() == node2.myPart->GetUri()){
			if (node1.rank!=node2.rank)
				return node1.rank>node2.rank;
			return node1.asmMove.begin()->Magnitude() < node1.asmMove.begin()->Magnitude();
		}
		else
		{

		//Force priority
		if (node1.rank != node2.rank)
			return node1.rank>node2.rank;
		_real val1,val2,dlt;

		//Smaller in priority
		val1 = node1.myPart->MaterialVolume();
		val2 = node2.myPart->MaterialVolume();
		if (val1<RealSmall() || val2<RealSmall())
			Standard_Failure::Raise("Extremely small part volume");
		
		dlt = val1 > val2 ? val1 / val2 : val2 / val1;
		
		if (dlt > 2)
			return val1<val2;

		if (node1.parentNodes.size() && node2.parentNodes.size() &&
			!(*node1.parentNodes.begin())->IsAbsNode() &&
			!(*node2.parentNodes.begin())->IsAbsNode()){
			gp_Pnt prevCenter1 = (*node1.parentNodes.begin())->myPart->GetCenter();
			gp_Pnt prevCenter2 = (*node2.parentNodes.begin())->myPart->GetCenter();
			val1 = node1.myPart->GetCenter().Distance(prevCenter1);
			val2 = node2.myPart->GetCenter().Distance(prevCenter2);
			dlt = val1 > val2 ? val1 / val2 : val2 / val1;
			if (dlt > 4)
				return val1<val2;
		}

		//Less contact priority
		val1 = (_int)node1.myPart->colOfCont.size();

		if (val1 < 1)
			return true;

		val1 = (_int)node2.myPart->colOfCont.size();

		if (val1 < 1)
			return false;

		dlt = val1 > val2 ? val1 / val2 : val2 / val1;

		if (dlt > 1.5)
			return val1 < val2;
		
		//Closer in priority
		val1 = node1.asmMove.begin()->Magnitude();
		val2 = node2.asmMove.begin()->Magnitude();
		dlt = val1 > val2 ? val1 / val2 : val2 / val1;
		if (dlt > 1.5)
			return val1 < val2;
		}
	}
	catch (Standard_Failure){
		return true;
	}

//default don't swap parts
	return true;
}
