#include "AsmTreeBuilder.h"
#include "DBGBuilder.h"
#include "BRepPrimAPI_MakeBox.hxx"
#include "GeomAPI_IntCS.hxx"
#include "gp_Lin.hxx"
#include "Geom_Line.hxx"
#include "GC_MakeLine.hxx"
#include "Geom_Surface.hxx"
#include "BRepClass_FaceClassifier.hxx"
#include "MainFrame.h"
using namespace asp;

extern AssemblyPartPriorityCompare Compare;

AsmTreeBuilder::AsmTreeBuilder(){
	done = false;
}
AsmTreeBuilder::~AsmTreeBuilder(){

}
AsmTreeBuilder::operator AsmTreeNode && (){
	
	return std::move(asmSequence);
}
AsmTreeNode *AsmTreeBuilder::GetFullProductNode(){
	if (done){
		asmSequence.SetOperationType(AsmTreeNode::DISMANTLE);
		return &asmSequence;
	}
	return nullptr;
}
AsmTreeNode *AsmTreeBuilder::GetWholeProductNode(){
	if (done){
		auto leafNode = asmSequence.childNodes.begin();
		while (leafNode->childNodes.size())
			leafNode = leafNode->childNodes.begin();
		leafNode->SetOperationType(AsmTreeNode::MOUNT);
		return &(*leafNode);
	}
	return nullptr;
}
void AsmTreeBuilder::Init(Unit* product, MainFrame* mainWin){

	mainWindow = mainWin;
// Simplest Lattice Structure

//		TopLatticeNode
//		 /		|		\
//		N1		N2		N3
//		 \		|	  /
//		endLatticeNode (has a lot of parents) but has got no childs

// #### Now all nodes has just one parents 
	myAssembly = product;

	myDBGBuilder.SyncLists(&closedList);
	
	mainWindow->SetStatus(QString("Start for general DBG creating"));

	myDBGBuilder.AddDBGRelation(myAssembly);

	mainWindow->SetStatus(QString("Complite DBG"));

	closedList.SetTarget(myAssembly->GetPartAmount());
	backTraceIterCounter=0;
	continueDisassembly(&asmSequence);
		done = true;
	
	//closedList.clear();
	
}

_bool AsmTreeBuilder::continueDisassembly(AsmTreeNode * root){
	
	if (acceptedDisassembly(root)) return true;

	ExploreNode(root);

	_bool backTrack = false;
	_int partUri=-1;
	_int childAmount =root->childNodes.size();
	
	while( root->childNodes.begin()!=root->childNodes.end()){

		if (backTrack && root->childNodes.begin()->MovedPartUri() == partUri){
				root->childNodes.pop_front();
				continue;
			}

		QString status = "Dismounted ";
		status += std::to_string(closedList.size()).c_str();
		status += " / ";
		status += std::to_string(closedList.GetTarget()).c_str();
		status += " Current ";
		status += std::to_string(childAmount - root->childNodes.size()).c_str();
		status += " / ";
		status += std::to_string(childAmount).c_str();

		mainWindow->SetStatus(status);

		AsmTreeNode *current = &(*root->childNodes.begin());
		if (rejectDisassembly(current)){
			root->childNodes.pop_front();
			continue;
		}
		partUri = current->MovedPartUri();
		closedList.Close(current->myPart->GetUri());

		if (continueDisassembly(current)){
			return true;
		}
		
		backTrack= true;

		if (++backTraceIterCounter > backTraceMaxAmount)
			return false;
	//Mark previosly closed part as open
		closedList.OpenPrev();
		root->childNodes.pop_front();
	}
	return false;
}

_bool AsmTreeBuilder::rejectDisassembly(AsmTreeNode * node){
	if (node->IsAbsNode())
		return false;
	else
		//Check collitions along move direction
		return !MoveFeasibilityCheck(*node);
}

_bool AsmTreeBuilder::acceptedDisassembly(AsmTreeNode * node){

	if (closedList.IsTargetDone()){
		node->childNodes.push_back(AsmTreeNode(node));
		return true;
	}
	return false;
}

void AsmTreeBuilder::ExploreNode(AsmTreeNode *node){

	//Find free dirs which opposite to blocking directions
	auto &&aspDirs = FreePartFromDBGView(node);

	//Sort new nodes
	aspDirs.sort(Compare);

	//Add disassembly step as child to node
	for (auto &newchildNode : aspDirs){
		//Add new child node
		node->childNodes.push_back(newchildNode);
	}

	
	//std::sort(node->childNodes.begin(), node->childNodes.end(), comparePart);
}

_bool AsmTreeBuilder::MoveFeasibilityCheck(AsmTreeNode &asmProc){
	//_bool feasible = true;
	if (!asmProc.IsAbsNode() && !asmProc.MoveTraced){
		
		Part *myPart = asmProc.myPart;

		FindPointsOnPartSurface(myPart);//do nothing if have done already

		for (gp_Pnt &tracePnt : myPart->pntInPart){
			
			gp_Vec move = *asmProc.asmMove.begin();
			gp_Ax1 axis(tracePnt, move);
			_real Dist = DistanceToBndBorder(myPart, axis,myAssembly->BndBox());
			
			std::vector<_int > obstList;
			if (IsMoveAlongDirBlocked(myPart, axis, Dist, obstList)){
				for (auto &obstUri : obstList){
					DBGEdge newEdge(myPart->GetUri(), obstUri, asp_Ax1(axis, _MoveBlk));
					myDBGBuilder.Add(newEdge);
				}
				return false;
			}
		}
		asmProc.MoveTraced = true;
	}
	return true;
}
std::list<AsmTreeNode> AsmTreeBuilder::FreePartFromDBGView(AsmTreeNode *root){

	auto unitMap = myAssembly->GetUnitMap();
	std::list<AsmTreeNode> possibleDsmSteps;

	for (auto iter = unitMap->begin(); iter != unitMap->end(); iter++){
		if (!closedList.IsClose((*iter).first)){

			auto freeDirs = FreeDirs((*iter).first, root);
			if (freeDirs.size()){
				auto myPart = dynamic_cast<Part*>((*iter).second);
				for (auto &dsmDir : freeDirs){
					gp_Vec moveVec(dsmDir.Direction());
					_real Dist = DistanceToBndBorder(myPart, gp_Ax1(myPart->GetCenter(),moveVec), myAssembly->BndBox());
					moveVec.Multiply(Dist);
					AsmTreeNode newNode(myPart, moveVec, root);
					if (dsmDir.State == _MoveFree){
					//	newNode.rank= 1;
					//	newNode.MoveTraced=true;
					}
					possibleDsmSteps.push_back(std::move(newNode));
				}
			}
		}
	}

	return possibleDsmSteps;

}
void IsPartMoveBlock(){
	
}
std::vector<asp_Ax1> AsmTreeBuilder::FreeDirs(_int PartUri,AsmTreeNode *curNodeInTree){
	
	auto blkDirs = myDBGBuilder.GetUnBlkMoveDirs(PartUri);
	if (blkDirs.size()){
		for (auto &dir : blkDirs){
			dir.State = _ContactFree;
		}
		return blkDirs;
	}

	std::vector<asp_Ax1> freeDirs;
	blkDirs = myDBGBuilder.GetBlockedDirs(PartUri);
	if (blkDirs.size()){
		//if (std::find_if(blkDirs.begin(), blkDirs.end(),
		//	[](const asp_Ax1& ax){return ax.State == StateOfDir::_MoveBlk; }) != blkDirs.end())
		//	return freeDirs;

		for (auto &dir : blkDirs){
			asp_Ax1 dsmDir = asp_Ax1(dir.Reversed(), _ContactFree);
			if (!IsBlocked(dsmDir, &blkDirs)){
				freeDirs.push_back(dsmDir);
			}
			
		}
	}
	else{
		
		if (!curNodeInTree->IsAbsNode()){
			curNodeInTree->SetOperationType(AsmTreeNode::DISMANTLE);
			freeDirs.push_back(asp_Ax1(myAssembly->GetUnitMap()->find(PartUri)->second->GetCenter(), *curNodeInTree->asmMove.begin(), _ContactFree));
		}
		
			auto center = myAssembly->GetUnitMap()->find(PartUri)->second->GetCenter();
			freeDirs.push_back(asp_Ax1(gp_Ax1(center, gp_Dir(1, 0, 0)), _ContactFree));
			freeDirs.push_back(asp_Ax1(gp_Ax1(center, gp_Dir(0, 1, 0)), _ContactFree));
			freeDirs.push_back(asp_Ax1(gp_Ax1(center, gp_Dir(0, 0, 1)), _ContactFree));
			freeDirs.push_back(asp_Ax1(gp_Ax1(center, gp_Dir(-1, 0, 0)), _ContactFree));
			freeDirs.push_back(asp_Ax1(gp_Ax1(center, gp_Dir(0, -1, 0)), _ContactFree));
			freeDirs.push_back(asp_Ax1(gp_Ax1(center, gp_Dir(0, 0, -1)), _ContactFree));

			
	}

	return freeDirs;
}
_bool AsmTreeBuilder::IsBlocked(asp_Ax1 dir, std::vector<asp_Ax1> *colOfBlkDir){

	for (auto iter = colOfBlkDir->begin(); iter != colOfBlkDir->end(); iter++){

		if (IsOneBlkOther(iter->Direction(),dir.Direction()))
			return true;
	}
	return false;
}

void AsmTreeBuilder::FindPointsOnPartSurface(Part *pPart){
	std::vector< int> ForbidenFaces;
	for (auto &contact : pPart->colOfCont){
		if (contact.Type == _Intersect){
			ForbidenFaces.push_back(contact.MySurfNumber);
		}
	}

	if (!pPart->pntInPart.size()){
		for (auto &face : *pPart){
			if (ForbidenFaces.size() && std::find(ForbidenFaces.begin(),ForbidenFaces.end(),face.uri)!=ForbidenFaces.end())
				continue;
		try{
			static _real dltU, dltV;
			static BRepClass_FaceClassifier classifier;
			static _bool notFound;
			notFound = true;
			dltU = face.surf.IsUPeriodic() ? M_PI / 8 : 2;
			dltV = face.surf.IsVPeriodic() ? M_PI / 8 : 2;

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


					if (classifier.State() == TopAbs_ON || classifier.State() == TopAbs_IN){
						gp_Pnt pnt3d;
						gp_Vec vU,vV;
						face.surf.D1(pnt.X(), pnt.Y(), pnt3d,vU,vV);
						
						gp_Vec norm = vU.Crossed(vV);
						
						if (face.myShape.Orientation()==TopAbs_FORWARD)
							norm.Reverse();

						norm.Normalize();
						norm.Multiply(1);
						pnt3d.Translate(norm);
						pPart->pntInPart.push_back(pnt3d);
						//One point per surface (Search stop flag)
						notFound = false;
					}
				}
			}
			if (notFound)
				Standard_Failure::Raise("Can not find point on surface");
		}
		catch (Standard_Failure){

			//QString st = "Can not find point on surface";
		}
	}
	auto &col = pPart->pntInPart;
	for (_int i = 0; i < col.size(); ++i){
		for (_int j = i+1; j < col.size();){
			if (col[i].Distance(col[j]) < 2)
				col.erase(col.begin() + j);
			else
				++j;
		}
	}
	}
}


_bool AsmTreeBuilder::IsMoveAlongDirBlocked(Part *part, gp_Ax1 axis, _int Distance, std::vector<_int> &ObstacleURI){
	_bool findBlocks = false;
	_int partUri = part->GetUri();

	
	gp_Pnt p1 = axis.Location();
	gp_XYZ myDir = axis.Direction().XYZ()*Distance;
	gp_Pnt p2 = p1.XYZ() + myDir;

	Handle(Geom_TrimmedCurve) mLine = GC_MakeSegment(p1, p2);
	mLine->SetTrim(2, mLine->LastParameter());

	for (auto obst : *(myAssembly->GetUnitMap())){
		Part * obstPart = dynamic_cast<Part *> (obst.second);
		_int Uri = obst.second->GetUri();
		if (Uri == partUri || std::find(closedList.begin(), closedList.end(), Uri) != closedList.end())
			continue;
		
		if (IsLineIntersectPart(mLine, obstPart)){
			ObstacleURI.push_back(obst.first);
			findBlocks = true;
		}
		
	}
	
	return findBlocks;
}
_bool AsmTreeBuilder::IsLineIntersectPart(Handle(Geom_TrimmedCurve) line, Part* const part){

	Handle_Geom_Line moveLine = Handle_Geom_Line::DownCast(line->BasisCurve());
	if (moveLine.IsNull())
		Standard_Failure::Raise("Curve is not simply line");

	if (part->BndBox()->IsOut(moveLine->Lin()))
		return false;

	for (auto &surf : *part){
		//Does Axis intersect Bnd_Box
		if (surf.myBox.IsOut(moveLine->Lin()))
			continue;
		Handle_Geom_Surface testedSurf = Handle_Geom_Surface::DownCast(surf.surf.Surface().Surface()->Transformed(surf.surf.Trsf()));
		
		if (testedSurf){
			GeomAPI_IntCS CS(line, testedSurf);
			if (CS.IsDone() && CS.NbPoints()){
				
				BRepClass_FaceClassifier classifyThis;
				for (_int i = 1; i <= CS.NbPoints(); i++){
					classifyThis.Perform(surf.myShape, CS.Point(i), Precision::Confusion());
					TopAbs_State state = classifyThis.State();
					
					if (state==TopAbs_State::TopAbs_IN)
						return true;
				}
				
			}
		}
	}
	return false;
}
_real AsmTreeBuilder::DistanceToBndBorder(Part *part, gp_Ax1 axis, Bnd_Box *bnd){
	gp_Pnt target = axis.Location();
	gp_Vec move = axis.Direction().XYZ().Normalized();
	move.Multiply(30);
	_real PartSize = part->BndBox()->CornerMin().Distance(part->BndBox()->CornerMax());
	while (!bnd->IsOut(target)){
		target.Translate(move);
	}
	_real dist = target.Distance(axis.Location());
	if (dist){
		dist = PartSize;
	}
	else if (dist > 1.3* PartSize){
		dist = PartSize;
	}
	return dist;
}
_real AsmTreeBuilder::DistanceToBndBorder(gp_Ax1 axis, Bnd_Box *bnd){
	gp_Pnt target = axis.Location();
	gp_Vec move = axis.Direction().XYZ().Normalized();
	move.Multiply(30);

	while (!bnd->IsOut(target)){
		target.Translate(move);
	}
	_real dist = target.Distance(axis.Location());
	
	return dist ? dist : 30;
	/*
	gp_Pnt sphereCenter = bnd->CornerMin();
	sphereCenter.BaryCenter(1, bnd->CornerMax(), 1);

	//O is center of bounding sphere 
	//A is a axis location
	//B point on the sphere
	//phi is angele(AO ^ AB)
	//psi (BA ^ BO)
	//alpha (OA ^ OB)
	_real radius = bnd->CornerMin().Distance(bnd->CornerMax()) / 2;
	gp_Vec vOA(sphereCenter, axis.Location());
	_real sin_phi = vOA.CrossMagnitude(axis.Direction());
	_real OA = vOA.Magnitude();
	if (sin_phi < ParTol())
		return std::abs(radius - OA);
	
	_real sin_psi = sin_phi * OA / radius;
	_real alpha = M_PI - std::asin(sin_phi) - std::asin(sin_psi);
	_real distMove= radius * sin(alpha) / sin_phi;
	if (!(distMove == distMove))
		return radius/2.;
	return distMove;
	*/

}

