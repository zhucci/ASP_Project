#ifndef _AsmShowManager_
#define _AsmShowManager_
#include "OCCT_Headers.h"

namespace asp{
	class AsmShowManager{
	public:
		AsmShowManager();
		~AsmShowManager();
		void Next();
		void Prev();
		void Current();
		Standard_Boolean MoreFoward();
		Standard_Boolean MorePrev();

	protected:
	private:
		
	};
};
#endif