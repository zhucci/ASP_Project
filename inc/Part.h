
#ifndef _Part_
#define _Part_

#include "Unit.h"
#include "surfaceAttribute.h"

#include <Geom_Axis1Placement.hxx>

namespace asp
{
	class AsmTreeNode;
//! class Part :public Unit
//! + vector<SurfaceAttribute *> colOFSurf
	class Part : public Unit
	{
	public:
		friend class AssemblyPartPriorityCompare;
		friend class AspMainTool;
		friend class AsmTreeBuilder;
		friend class AspMainTest;

		friend class AssemblyGraphBuilder;
		Part(TopoDS_Shape  &shape, Unit* root=NULL);
		Part(const TDF_Label &label, Unit* root=NULL);
		_int Init();
		~Part();

		void Dump(Standard_OStream &stream, Standard_Integer rank);

		std::vector< SurfaceAttribute>::iterator begin(){return colOfSurf.begin();}

		std::vector<SurfaceAttribute>::iterator end(){return colOfSurf.end();}

		SurfaceAttribute &GetSurfAtt(Standard_Integer index){return colOfSurf[index];}

		void SetContact(Contact &cont){colOfCont.push_back(cont);}

		void SetAsmDir(gp_Ax1 dir){
			PDir pdir(dir);
			PotentialDirs.push_back(pdir);
		}
		
	protected:
		
		std::vector<SurfaceAttribute> colOfSurf;

		std::vector<gp_Pnt> pntInPart;

		std::vector<Contact>  colOfCont; 
		struct PDir{
		public:
			PDir(gp_Ax1 ax):axis{ax}{}
			gp_Ax1 axis;
		};
		 std::vector<PDir> PotentialDirs;

//!calculate bounding box and update myBox variable
		void bndBoxCalc();
//! material volume calcuate and update 
		void MaterialVolumeUpdate();
//! bndBox Volume Calcuate
		void BoundVolumeUpdate();
//! Calculate adjacency of contact faces of part
		void AdjacencyCalc();
	
	};


};

#endif /*#define _Part_*/