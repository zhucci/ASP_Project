#pragma once
#include "AIS_InteractiveObject.hxx"
#include "OCCT_Headers.h"
#include "Part.h"



namespace asp{

//DEFINE_STANDARD_HANDLE(ISession_Part, AIS_InteractiveObject)
	class ISession_Part : public AIS_InteractiveObject
	{
	public:
		ISession_Part(Part *part);
		~ISession_Part();

//	DEFINE_STANDARD_RTTI(ISession_Part)
	private:

		Part *myPart;
		TCollection_ExtendedString PartName;
		
	};
};
