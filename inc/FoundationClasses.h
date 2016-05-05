//#include"MakeBottle.h"
#ifndef _foundationClasses_
#define _foundationClasses_

#include "OCCT_Headers.h"
#include "STL_Headers.h"



Standard_PCharacter shapeTypeToString(TopAbs_ShapeEnum &shape);

class MakeString {
public:
    template<class T>
    MakeString& operator<< (const T& arg) {
        m_stream << arg;
        return *this;
    }
    operator std::string() const {
        return m_stream.str();
	}
	operator Standard_CString() {
		return m_stream.str().data();
	}
	
protected:
    std::stringstream m_stream;
};

 Standard_Boolean IsCodirectional(gp_Dir &D1, gp_Dir &D2);

//! Return Angular tolerance for my calculation
Standard_Real AngTol();

//!Return Linear tolerance for my calculation 
 Standard_Real LinTol();

 Standard_Real ParTol();

 Standard_Real SGap();

 _real AsmVecTol();
 
 _bool IsOneBlkOther(const gp_Dir &one, const gp_Dir& other);
 
 Standard_PCharacter typeToStr(GeomAbs_SurfaceType surfType);


//!Read Label attribute "Name" if it exists, else return NULL
//! Convert text from Unicode to CStirng UTF-8 type
Standard_PCharacter ReadName(const TDF_Label &lbl);

Handle(TopTools_HSequenceOfShape) importSTEP(
	char* fileName);

Handle(TopTools_HSequenceOfShape) importSTEPGeom(
									char *fileName);


Standard_Boolean SaveBREP (Standard_CString aFileName,
                                          const TopoDS_Shape& aShape);

Standard_Boolean ReadBREP(Standard_CString      aFileName,
                                        TopoDS_Shape& aShape);

void importStepWithAtt(const char *fileName, Handle(TDocStd_Document) &doc);

		
template<typename T>
T &isElementary(Handle_Standard_Transient & tns)
{
	T rtn = T::DownCast(tns);
	if(rnt.isNull())
		return nullptr;
	else 
		return rtn;
}
#endif