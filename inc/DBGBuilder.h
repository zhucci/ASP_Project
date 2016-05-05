#ifndef _DBGBuilder_
#define _DBGBuilder_
#include "OCCT_Headers.h"
#include "STL_Headers.h"
#include "Unit.h"
namespace asp{

	class asp_Ax1 :public gp_Ax1{
	public:
		asp_Ax1(const gp_Pnt& P, const gp_Dir& V, StateOfDir state) :gp_Ax1(P, V), State(state){}
		asp_Ax1(const gp_Ax1 &axis, StateOfDir state) :gp_Ax1(axis), State(state){}
		asp_Ax1(asp_Ax1 &&ax);
		asp_Ax1(const asp_Ax1 &ax);
		asp_Ax1()= delete;
		StateOfDir State;
	};
//!DBG relation codding
//! Part::Uri-- std::list<asp_Ax1> -->Part::Uri
	struct DBGEdge{
		Standard_Integer part;
		Standard_Integer obst;
		std::vector<asp_Ax1> dirs;
		DBGEdge(Standard_Integer blockedPart, Standard_Integer obstacle, std::vector<asp_Ax1> &&colOfDirs):
			dirs(colOfDirs),
			part(blockedPart),
			obst(obstacle)
			{}
		DBGEdge(Standard_Integer blockedPart, Standard_Integer obstacle, asp_Ax1 &Dir) :
			part(blockedPart),
			obst(obstacle)
		{
			dirs.push_back(Dir);
		}
		~DBGEdge(){}
	};

//! Directional blocking graph representation
//! std::list<DBGEdge> DBG
	typedef std::list<DBGEdge> DBG;

	template<typename T>
	class closed_list : public std::list<T>{
	public :
	//! mark PartUri as close
		void  Close(T PartUri){
			if (!IsClose(PartUri))
				push_back(PartUri);
		}
	//!Return previosly closed partUri
		T OpenPrev(){
			if (size()){
			T bck = this->back();
				pop_back();
				return bck;
			}
		}
	//! Mark PartUri as open
		void  Open(T PartUri){
			auto marked = std::find(begin(), end(), PartUri);
			if (marked != end())
				erase(marked);
		}
		_bool IsClose(T PartUri){
			if (std::find(begin(), end(), PartUri) != end())
				return true;
			return false;
		}
		void SetTarget(_int target){
			targetAmount = target;
		}
		_bool IsTargetDone(){
			return size() == targetAmount;
		}
		_int GetTarget(){
			return targetAmount;
		}
	private:
		_int targetAmount;

	};

	

//!DBG builder class
//! Build DBG from contact situation
//! Has members for adding and deleting nodes and edges from DBG
	class DBGBuilder{
	public:

		DBGBuilder();

		void SyncLists(closed_list<_int> *DisassembledPartList);

		DBGBuilder(DBG &newDBG);

		DBGBuilder(DBGBuilder &builder)=delete;
		
		~DBGBuilder();
		
		_bool AddDBGRelation(Unit* product);

		void Add(DBGEdge &edge);

		_bool Remove(_int NodeUri);
		
		//_bool IsOpenListEmpty();
		//_bool IsClosedListEmpty();
		std::vector<asp_Ax1>			GetBlockedDirs	(_int PartUri);
		std::vector<asp_Ax1>			GetFreeDirs		(_int PartUri);
		
		std::vector<asp_Ax1> GetUnBlkMoveDirs(_int PartUri);

		friend _bool operator<<(std::vector<_int> &FreeParts, DBGBuilder &builder);

		operator DBG && ();


	protected:

		_bool FindSameEdge(DBGEdge &edge, std::list<DBGEdge>::iterator &pointer);
		_bool DBGBuilder::IsMoveDirStillBlock(_int PartUri, asp_Ax1 axis);

		std::list<DBGEdge> myDBG;
		std::map<_int, Unit*> *unitMap;

	//!Unactive nodes e.i. parts have disassembled
		closed_list<_int> *closedList;
		_bool IsClose(_int PartUri);
		//std::list<_int> openedList;

		Standard_Integer DBGEdgeBuild(Part *part, Part *obst, Standard_Boolean allowGap=Standard_True);

		//!function: isEnvelopBndBox
		//! purpose: check if 'box' is wraped by 'wrapBox'
		Standard_Boolean isEnvelopBndBox(Bnd_Box *box, Bnd_Box *wrapBox);

		_bool NotInContact(SurfaceAttribute &sp, SurfaceAttribute &obstSp, _real OverLaySize, _real gap);
		_bool NotOverlay(Bnd_Box *box1, Bnd_Box *box2, _real OverLaySize);
		


		//! For Intersected boxes it searches cross direction
		Standard_Boolean isCrossedBndBox(Bnd_Box *box, Bnd_Box *crossBox, gp_Dir &dir);
		Standard_Boolean fastCheckIntersect(Part *part, Part *obst);

		void MergeCollectionOfDirs(std::vector<gp_Ax1> &colOfDir, StateOfDir dirType, std::vector<asp_Ax1> &container);
		void MergeCollectionOfDirs(std::vector<asp_Ax1> &colOfDir, std::vector<asp_Ax1> &container);
	};
};
#endif //_DBGBuilder