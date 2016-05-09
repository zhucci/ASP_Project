#ifndef _Assembly_
#define _Assembly_

#include "Unit.h"
#include "Handle_TDocStd_Document.hxx"


namespace asp
{
//! This class is used to present product structure as tree where nodes is parts or sub-assembly
//!and arc means including relations	
	class Assembly : public Unit
	{		
	public:

		friend class AspMainTest;

		Assembly(Handle(TDocStd_Document) &document);
		Assembly(TopoDS_Shape &shape, Unit* root=NULL);
		Assembly(const TDF_Label &label, Unit* root=NULL);
		~Assembly();
		
//! Print Assembly structure to 'stream'
	void Dump(Standard_OStream &stream, Standard_Integer rank=0);
//! Walk through all parts in assembly and calculate their contacts
//! using discoverConnection func to fill colOfCont in Parts
	 void GetAllParts(Handle_TopTools_HSequenceOfShape collection); 
	
//! For Range base loop
	std::vector<Unit *>::iterator begin();
//! For Range base loop
	std::vector<Unit *>::iterator end();

	Part * GetSamePart(TopoDS_Shape shape);
	protected:
		std::vector<Unit *> ColSubUnit;

//!calculate bounding box and update myBox variable
		void bndBoxCalc();
//! material volume calcuate and update 
		void MaterialVolumeUpdate();
//! bndBox Volume Calcuate
		void BoundVolumeUpdate();
	};
};

#endif