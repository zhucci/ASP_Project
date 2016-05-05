#include "DBGBuilder.h"

#include "Part.h"
#include "ContactSpot.h"

using namespace asp;

asp_Ax1::asp_Ax1(asp_Ax1 &&ax){
	SetDirection(ax.Direction());
	SetLocation(ax.Location());
	State = ax.State;
}
asp_Ax1::asp_Ax1(const asp_Ax1 &ax){
	SetDirection(ax.Direction());
	SetLocation(ax.Location());
	State = ax.State;
}

DBGBuilder::DBGBuilder(){
	
}
void DBGBuilder::SyncLists(closed_list<_int> *DisassembledPartList){
	closedList = DisassembledPartList;
}
DBGBuilder::~DBGBuilder(){

}
DBGBuilder::DBGBuilder(DBG &newDBG){
	myDBG = newDBG;
}
_bool DBGBuilder::AddDBGRelation(Unit* product){
	//Should be changed for adding new parts from product->GetUnitMap() to unitMap ?
	 unitMap = product->GetUnitMap();
	 //#ChangeThis
	for (auto iter=unitMap->begin(); iter!=unitMap->end(); iter++){
		
		auto subIter = unitMap->begin();
		for (; subIter != unitMap->end(); subIter++){
			if (subIter == iter)
				continue;
			
			DBGEdgeBuild(dynamic_cast<Part*>((*iter).second),
				dynamic_cast<Part*>((*subIter).second), true);
		}
	
	}
	return true;
}

void DBGBuilder::Add(DBGEdge &edge){
	std::list<DBGEdge>::iterator iter;
	if (FindSameEdge(edge, iter)){
		MergeCollectionOfDirs(edge.dirs, (*iter).dirs);
	}
	else{
		myDBG.push_back(edge);
	}
}
_bool DBGBuilder::FindSameEdge(DBGEdge &edge, std::list<DBGEdge>::iterator &pointer){
	auto predicate = [&](DBGEdge &E){return E.part == edge.part; };
	auto end = myDBG.end();
	auto iter = std::find_if(myDBG.begin(), end, predicate);
	while (iter != end){
		if ((*iter).obst == edge.obst){
			pointer = iter;
			return true;
		}
		iter++;
		iter = std::find_if(iter, end, predicate);
	}
	return false;
}
_bool DBGBuilder::Remove(_int NodeUri){
	return true;
}
_bool DBGBuilder::IsClose(_int PartUri){
	if (std::find(closedList->begin(), closedList->end(), PartUri) != closedList->end())
		return true;
	return false;
}
std::vector<asp_Ax1> DBGBuilder::GetUnBlkMoveDirs(_int PartUri){
	std::vector<asp_Ax1> unblockedDirs;
	for (auto iter = myDBG.begin(); iter != myDBG.end(); iter++){

		if ((*iter).part == PartUri && IsClose((*iter).obst)){
			for (auto &dir : iter->dirs){
				;
				if (dir.State == _MoveBlk && !IsMoveDirStillBlock(PartUri, dir)){	
					unblockedDirs.push_back(dir);
				}
			}

		}
	}
	
	return std::move(unblockedDirs);
}
_bool DBGBuilder::IsMoveDirStillBlock(_int PartUri, asp_Ax1 axis){
	
	for (auto iter = myDBG.begin(); iter != myDBG.end(); iter++){

		if ((*iter).part == PartUri && !IsClose((*iter).obst) ){
			for (auto &dir : iter->dirs){
				if (IsOneBlkOther(dir.Direction(),axis.Direction()))
					return true;
			}
		}
	}
		return false;	
}
std::vector<asp_Ax1> DBGBuilder::GetBlockedDirs(_int PartUri){

	std::vector<asp_Ax1> blkDirs;
	
	for (auto iter = myDBG.begin(); iter != myDBG.end(); iter++){
		
		if ((*iter).part == PartUri){
			if (!closedList->IsClose((*iter).obst)){
				blkDirs.insert(blkDirs.end(), (*iter).dirs.begin(), (*iter).dirs.end());
			}
		}
	}
	return std::move(blkDirs);
}
std::vector<asp_Ax1> DBGBuilder::GetFreeDirs(_int PartUri){

	std::vector<std::vector<asp_Ax1>::iterator> unBlkDirs;
	std::vector<std::vector<asp_Ax1>::iterator> blkDirs;
	std::vector<asp_Ax1> potentialDismountDirs;
	for (auto iter = myDBG.begin(); iter != myDBG.end(); iter++){
		if ((*iter).part == PartUri){
			_bool isClosed = closedList->IsClose((*iter).obst);
			for (auto dirIter = iter->dirs.begin();dirIter!= iter->dirs.end();++dirIter){
				if (isClosed && dirIter->State==_MoveBlk){
					unBlkDirs.push_back(dirIter);
				}
				else{
					blkDirs.push_back(dirIter);
				}
			}	
		}
	}
	for (auto & iter : unBlkDirs){
		_bool isBlocked = false;
		for (auto &blkIter : blkDirs){
			if (IsOneBlkOther(blkIter->Direction(), iter->Direction())){
				isBlocked = true;
				break;
			}

		}
		if (!isBlocked){
			potentialDismountDirs.push_back(asp_Ax1(iter->Location(),iter->Direction(),_MoveFree));
		}
	}
	for (auto & iter : blkDirs){
		_bool isBlocked = false;
		gp_Dir reversed = iter->Direction().Reversed();
		for (auto &blkIter : blkDirs){
			if (IsOneBlkOther(blkIter->Direction(), reversed)){
				isBlocked = true;
				break;
			}

		}
		if (!isBlocked){
			potentialDismountDirs.push_back(asp_Ax1(iter->Location(),reversed,_ContactFree));
		}
	}
	if (!blkDirs.size() && !unBlkDirs.size() && !potentialDismountDirs.size()){
		gp_Pnt center(0,0,0);
		potentialDismountDirs.push_back(asp_Ax1(gp_Ax1(center, gp_Dir(1, 0, 0)), _ContactFree));
		potentialDismountDirs.push_back(asp_Ax1(gp_Ax1(center, gp_Dir(0, 1, 0)), _ContactFree));
		potentialDismountDirs.push_back(asp_Ax1(gp_Ax1(center, gp_Dir(0, 0, 1)), _ContactFree));
		potentialDismountDirs.push_back(asp_Ax1(gp_Ax1(center, gp_Dir(-1, 0, 0)), _ContactFree));
		potentialDismountDirs.push_back(asp_Ax1(gp_Ax1(center, gp_Dir(0, -1, 0)), _ContactFree));
		potentialDismountDirs.push_back(asp_Ax1(gp_Ax1(center, gp_Dir(0, 0, -1)), _ContactFree));
	}
	return std::move(potentialDismountDirs);
}
DBGBuilder::operator DBG && (){
	return std::move(myDBG);
}

Standard_Boolean DBGBuilder::isEnvelopBndBox(Bnd_Box *box, Bnd_Box *wrapBox){

	Standard_Real eps = Precision::Confusion();

	if (wrapBox->IsOut(box->CornerMax()) ||
		wrapBox->IsOut(box->CornerMin()) ||
		!wrapBox->IsWhole() ||
		wrapBox->IsVoid())
		return Standard_False;
	else
		return Standard_True;
}



Standard_Boolean DBGBuilder::fastCheckIntersect(Part *part, Part *obst)
{
	try{

		//if (part->BndBox()->IsOut(*obst->BndBox()))
		//	return false;
		
		if (part->BndBox()->Distance(*obst->BndBox()) > SGap())
			return false;

			for (SurfaceAttribute sp : *part){

				for (SurfaceAttribute obstSp : *obst){
					//Create Collection of Contacts
					if (obstSp.myBox.IsOut(sp.myBox)){
						continue;
					}
					else{

						BRepExtrema_ExtFF cheker(sp.myShape, obstSp.myShape);
						if (!cheker.IsDone())
							Standard_Failure::Raise("cheker is not  done");

						if (cheker.NbExt() && cheker.SquareDistance(1)<RealSmall())
							return Standard_True;
					}
				}
			}
		
		return Standard_False;
	}
	catch (Standard_Failure)
	{
		std::cerr << __FILE__ << ":" << __LINE__ << "Msg:" << Standard_Failure::Caught()->GetMessageString();
		Standard_Failure::Raise("fastCheckIntersect function error");
		return false;
	}

}

//===============================
//=	function : NotOverlay		=
//=	purpose  : may contact exist=
//===============================
_bool DBGBuilder::NotOverlay(Bnd_Box *box1, Bnd_Box *box2, _real OverLaySize){

	_real eps = LinTol();
	if (box1->IsThin(eps) || box2->IsThin(eps))
		return true;

	gp_Pnt b1_i = box1->CornerMin();
	gp_Pnt b1_s = box1->CornerMax();
	gp_Pnt b2_i = box2->CornerMin();
	gp_Pnt b2_s = box2->CornerMax();


	Standard_Boolean bRet = false;
	Standard_Real delta;

	delta = box2->GetGap() + box2->GetGap() + OverLaySize;
	_int count = 0;
	if (!box1->IsXThin(eps) && !box2->IsXThin(eps) &&
		((b2_s.X() - b1_i.X() > delta) && (b1_s.X() - b2_i.X() > delta)))
		count++;

	if (!box1->IsYThin(eps) && !box2->IsYThin(eps) &&
		((b2_s.Y() - b1_i.Y() > delta) && (b1_s.Y() - b2_i.Y() > delta)))
		count++;
	if (!box1->IsZThin(eps) && !box2->IsZThin(eps) &&
		((b2_s.Z() - b1_i.Z() > delta) && (b1_s.Z() - b1_i.Z() > delta)))
		count++;
	return count<2;

}
_bool DBGBuilder::NotInContact(SurfaceAttribute &sp, SurfaceAttribute &obstSp, _real OverLaySize, _real gap){

	if ((0<sp.Type && sp.Type<4 || sp.Type == GeomAbs_SurfaceOfRevolution) && (0<obstSp.Type && obstSp.Type<4 || sp.Type == GeomAbs_SurfaceOfRevolution))
		return obstSp.myBox.IsOut(sp.myBox) || NotOverlay(&sp.myBox, &obstSp.myBox, OverLaySize);
	else {
		_bool Dist = sp.myBox.Distance(obstSp.myBox)>gap;
		_bool Overlay = NotOverlay(&sp.myBox, &obstSp.myBox, OverLaySize);
		return  Dist || Overlay;

	}
}
void DBGBuilder::MergeCollectionOfDirs(std::vector<gp_Ax1> &colOfDir, StateOfDir dirType, std::vector<asp_Ax1> &container){

	for (auto &&dir : colOfDir)
		container.push_back(asp_Ax1(dir,dirType));
	
}
void DBGBuilder::MergeCollectionOfDirs(std::vector<asp_Ax1> &colOfDir, std::vector<asp_Ax1> &container){

	for (auto &&dir : colOfDir)
		container.push_back(dir);
	
}
Standard_Integer DBGBuilder::DBGEdgeBuild(Part *part, Part *obst, Standard_Boolean allowGap)
{
	try{

		_real Gap;
		_real Overlay = 1;
		if (allowGap)
			Gap = SGap();
		else
			Gap = LinTol();

		// ### need some optimization until excluded
		//if (!fastCheckIntersect(part, obst))
		//	return 0;
		const Bnd_Box &partBox = *part->BndBox();
		const Bnd_Box &obstBox = *obst->BndBox();
		if (partBox.Distance(obstBox) > Gap)
			return 0;

		_int i = 0, j = 0;
		_int PartUri = part->GetUri();
		_int ObstUri = obst->GetUri();

		std::vector<asp_Ax1> blkDirsForPart;
		std::vector<asp_Ax1> blkDirsForObstacle;

		for (auto &sp : *part){
			if (sp.myBox.Distance(obstBox)<Gap)
			for (auto &obstSp : *obst){
				if (obstSp.myBox.Distance(sp.myBox)>Gap)
					continue;
				//Posibility be at contact check
				if (NotInContact(sp, obstSp, Overlay, Gap))
					continue;
				
				ContactSpot spot(sp, obstSp, Gap);

				if (spot.IsDone()){

					std::vector <gp_Ax1> colOfDir = spot.getF1ContactAxis();

					if (colOfDir.size()){
						MergeCollectionOfDirs(colOfDir,_ContactBlk, blkDirsForPart);
						//colOfDir = spot.getF2ContactAxis();
						//MergeCollectionOfDirs(colOfDir,_ContactBlk, blkDirsForObstacle);

						sp.Func = obstSp.Func = _Base;
						part->SetContact(Contact(i, ObstUri, j,ContactType::_Contact));
						//obst->SetContact(Contact(colOfDir, j, PartUri, i, _Contact, true, true));
					}
				}

				j++; /*increase j*/
			}
			j = 0; /*nullify j*/
			i++;
		}
		if (blkDirsForPart.size()){
			DBGEdge newEdge(PartUri, ObstUri,std::move( blkDirsForPart));
			myDBG.push_back(newEdge);
		}
		if (blkDirsForObstacle.size()){
			DBGEdge newEdge(PartUri, ObstUri, std::move(blkDirsForObstacle));
			myDBG.push_back(newEdge);
		}

		return 0;
	}
	catch (Standard_Failure){
		std::cerr << __FILE__ << ":" << __LINE__ << "Msg:" << Standard_Failure::Caught()->GetMessageString();
		return 0;
	}
	return 0;
}

