
#include"FoundationClasses.h"


_bool IsOneBlkOther(const gp_Dir &one, const gp_Dir& other){
	if (one.XYZ()*other.XYZ() - AsmVecTol() > 0)
		return true;
	return false;
}

 Standard_Boolean IsCodirectional(gp_Dir &D1, gp_Dir &D2){
	 if(D1.XYZ()*D2.XYZ()+ParTol()>1)
		 return Standard_True;
	 return Standard_False;
 }
Standard_PCharacter ReadName(const TDF_Label &lbl)
{
	if(lbl.IsNull())
		Standard_Failure::Raise("Null label");

	Handle(TDataStd_Name) n;
	
		if(lbl.FindAttribute(TDataStd_Name::GetID(),n))
		{
			TCollection_ExtendedString name = n->Get();
			Standard_Integer len=name.LengthOfCString()+1;
			Standard_PCharacter Name = new char[len];
			/* Standard_Integer ok = */ name.ToUTF8CString(Name);
			//Resource_Unicode::Convert(name,Name,len);
			return Name;
		}
		else
			return NULL;
}
Standard_Boolean SaveBREP (Standard_CString aFileName,
                                          const TopoDS_Shape& aShape)
{
  std::filebuf aFileBuf;
  std::ostream aStream (&aFileBuf);
  if (!aFileBuf.open (aFileName, ios::out))
  {
    return Standard_False;
  }

  BRepTools::Write (aShape, aStream); 
  return Standard_True;
}
Standard_Boolean ReadBREP(Standard_CString      aFileName,
                                        TopoDS_Shape& aShape)
{
  //aShape.Nullify();

  std::filebuf aFileBuf;
  std::istream aStream (&aFileBuf);
  if (!aFileBuf.open (aFileName, ios::in))
  {
    return Standard_False;
  }

  BRep_Builder aBuilder;
  BRepTools::Read(aShape, aFileName, aBuilder);
  return !aShape.IsNull();
}
Standard_PCharacter typeToStr(GeomAbs_SurfaceType surfType)
{
	Standard_PCharacter name = new char[35];
	
	Standard_Integer size = 34;
		switch((Standard_Integer)surfType)
		{
		case 0 : strcpy_s(name,size, "Plane");
						break;
			case 1 :strcpy_s(name,size,"Cylinder");
					break;
			case 2 : strcpy_s(name,size,"Cone");
					break;
			case 3 : strcpy_s(name,size,"Sphere");
					break;
			case 4 : strcpy_s(name,size,"Torus");
					break;
			case 5 : strcpy_s(name,size,"Bezier");
					break;
			case 6: strcpy_s(name,size,"BSpline");
					break;
			case 7: strcpy_s(name,size,"Revolution");
					break;
			case 8: strcpy_s(name,size,"Extrusion");
					break;
			case 9: strcpy_s(name,size,"OtherSurface");
					break;
			default : strcpy_s(name,size,"--------");
					break;
		}
		return name;
}

Standard_PCharacter shapeTypeToString(TopAbs_ShapeEnum &st)
{
	Standard_PCharacter name = new char[35];
	
	Standard_Integer size = 34;
		switch((Standard_Integer)st)
		{
		case 0 : strcpy_s(name,size, "TopAbs_Compound");
						break;
			case 1 :strcpy_s(name,size,"TopAbs_COMPSOLID");
					break;
			case 2 : strcpy_s(name,size,"TopAbs_SOLID");
					break;
			case 3 : strcpy_s(name,size,"TopAbs_SHELL");
					break;
			case 4 : strcpy_s(name,size,"TopAbs_FACE");
					break;
			case 5 : strcpy_s(name,size,"TopAbs_WIRE");
					break;
			case 6: strcpy_s(name,size,"TopAbs_EDGE");
					break;
			case 7: strcpy_s(name,size,"TopAbs_VERTEX");
					break;
			case 8: strcpy_s(name,size,"TopAbs_SHAPE");
					break;
			default : strcpy_s(name,size,"Unrecognize");
					break;
		}
		return name;
}
void shapeToGeom(TopoDS_Shape shape)
{

	TopLoc_Location loc;
	if(shape.ShapeType()==TopAbs_FACE)
	{
	Handle(Geom_Surface) sfc = BRep_Tool::Surface( TopoDS::Face(shape), loc);
	if(sfc->DynamicType()==STANDARD_TYPE(Geom_Plane))
	{
		Handle(Geom_Plane) geom = Handle(Geom_Plane)::DownCast(sfc) ;
		gp_Ax1 ax = geom->Axis();
	}else if(sfc->DynamicType()== STANDARD_TYPE	(Geom_CylindricalSurface))
	{
		Handle(Geom_CylindricalSurface) geom = Handle(Geom_CylindricalSurface)::DownCast(sfc);
		gp_Ax1 ax = geom->Axis();
	}
	else
	{
		sfc->DynamicType()->Print(std::cout);
	}
	
	Bnd_Box box;
	BRepAdaptor_Surface surf = BRepAdaptor_Surface(TopoDS::Face(shape));
	BndLib_AddSurface::Add(surf,surf.FirstUParameter(),surf.LastUParameter(),
		surf.FirstVParameter(), surf.LastVParameter(),surf.Tolerance(),box);
	
	}
	
}
void PrintBndBoxShape(const Bnd_Box box)
{
	Standard_Real xi, xs, yi, ys, zi, zs;
	box.Get(xi, yi, zi, xs, ys, zs);

	printf("bnd x=[%f,%f],y=[%f,%f],z=[%f,%f]\n",xi,xs,yi,ys,zi,zs);

}
//! Change return value
Bnd_Box SolidBndBox(TopoDS_Solid &sld)
{
	Bnd_Box box;
	TopExp_Explorer exp;
	exp.Init(sld,TopAbs_FACE);
	BRepAdaptor_Surface surf;

	for(;exp.More();exp.Next())
	{
		surf = BRepAdaptor_Surface(TopoDS::Face(exp.Current()));
		BndLib_AddSurface::Add(surf,surf.FirstUParameter(),surf.LastUParameter(),
		surf.FirstVParameter(), surf.LastVParameter(),surf.Tolerance(),box);
	}
	return box;
}

void SolidParam(TopoDS_Solid & sld)
{
	printf("Solid Param calculation...\n");
	Bnd_Box box = SolidBndBox(sld);
	PrintBndBoxShape(box);
	printf("Box dump...\n");
	box.Dump();
	
	GProp_GProps props;
	
	BRepGProp::VolumeProperties(sld, props, Standard_True);
	gp_Pnt cntr=props.CentreOfMass();
	printf("Mass:: %f\n Centre: [%f,%f,%f]\n", props.Mass(), cntr.X(), cntr.Y(), cntr.Z());
}

void shapeExplorer(Handle( TopTools_HSequenceOfShape) shapes)
{
	
	try
	{

	Standard_Integer nbs = shapes->Length();
	if(nbs<=0)
		Standard_DomainError::Raise("Shapes Collection is empty");
	TopExp_Explorer exSo,exSh;
	for(Standard_Integer i=1;i<=nbs;i++)
	{
		TopoDS_Shape shape = shapes->Value(i);
	
		int j=1,k=1;
		for(exSo.Init(shape,TopAbs_SOLID);exSo.More();exSo.Next())
		{
			for(exSh.Init(exSo.Current(),TopAbs_FACE);exSh.More();exSh.Next())
			{
					
				shapeToGeom(exSh.Current());

			}
			k=1;
			j++;
		}
		


	}
	}
	catch(Standard_DomainError)
	{
		cout<<"In function Shapeexplorer"<<std::endl;
		cout<<Standard_DomainError::Caught()<<endl;
	}
	
}

void labelsDump(const TDF_Label &lbl)
{
	TDF_IDFilter filter;
	TDF_AttributeIndexedMap map;
	if(lbl.IsNull())
		Standard_Failure::Raise(" FoundationClasses.cpp:178:label is null");

	//lbl.ExtendedDump(str,filter,map);
	//std::cout << lbl<<std::endl

	Standard_Integer rank=0;
	Standard_Integer maxRank=2;

			
	
Handle(XCAFDoc_ShapeTool) STool =		XCAFDoc_DocumentTool::ShapeTool(lbl);

	if(STool->IsAssembly(lbl))
	{
		rank=1;
	}
	else if(STool->IsCompound(lbl))
	{

		rank=2;
		
	}
	else if(STool->IsComponent(lbl))
	{
		rank=3;
	}

			Handle(TDataStd_Name) n;
		if(lbl.FindAttribute(TDataStd_Name::GetID(),n))
		{
			
			TCollection_ExtendedString name = n->Get();
			Standard_Integer len=name.LengthOfCString()+1;
			
			Standard_PCharacter normal_name = new char[len];
			Resource_Unicode::ConvertUnicodeToANSI(name,normal_name,len);
			  TCollection_AsciiString es; 
			  TDF_Tool::Entry(lbl,es); 

		}


		if(lbl.HasChild() && rank<=maxRank)
		for(TDF_ChildIterator iter(lbl,false);iter.More();iter.Next())
		{
				TDF_Label cld=iter.Value();
				Standard_Boolean isSub = XCAFDoc_ShapeTool::IsSubShape(cld);
				Standard_Boolean isShape = XCAFDoc_ShapeTool::IsShape(cld);
				Standard_Boolean isComponent = XCAFDoc_ShapeTool::IsComponent(cld);
				Standard_Boolean isCompound = XCAFDoc_ShapeTool::IsCompound(cld);
				Standard_Boolean isAssembly =  XCAFDoc_ShapeTool::IsAssembly(cld);
			
						labelsDump(cld);
					
			}

}
void  DumpChildren(const TDF_Label& lbl) 
{ 
  TDF_ChildIterator it; 
  TCollection_AsciiString es;
  Standard_PCharacter normal_name = new char[50];
  for (it.Initialize(lbl,Standard_True); it.More(); it.Next())
  { 
	  TDF_Label current=it.Value();
		TDF_Tool::Entry(current,es); 
		Handle(TDataStd_Name) n;
		
		if(current.FindAttribute(TDataStd_Name::GetID(),n))
		{
			TCollection_ExtendedString name = n->Get();
			Standard_Integer len=name.LengthOfCString()+1;
			
			
			Resource_Unicode::ConvertUnicodeToANSI(name,normal_name,len);
			  TCollection_AsciiString es; 
			  TDF_Tool::Entry(lbl,es); 
	
		}
		else{

		}
  } 

} 
void importStepWithAtt(
		const char *fileName, Handle(TDocStd_Document) &doc)
{


	STEPCAFControl_Reader aReader;
	IFSelect_ReturnStatus status = aReader.ReadFile(fileName);

	aReader.SetNameMode(Standard_True);
	//aReader.ChangeReader().WS()->MapReader()->SetTraceLevel(3); //Increase default level
	
	if(status!=IFSelect_RetDone)
		Standard_Failure::Raise(" FoundationClasses.cpp:275:reader error");

	Standard_Boolean ok = aReader.Transfer(doc);
	
	if(!ok)
		Standard_Failure::Raise(" FoundationClasses.cpp:280:Read Step file error\n");
	
#ifdef _DUMP_
	DumpChildren(root);
#endif
	
}


Handle(TopTools_HSequenceOfShape) importSTEP(
		char *fileName)
{
	
	Handle(TopTools_HSequenceOfShape) shapes;
	STEPControl_Reader aReader;
	IFSelect_ReturnStatus status = aReader.ReadFile(fileName);
	
	if(status!=IFSelect_RetDone)
		Standard_Failure::Raise(" FoundationClasses.cpp:304:Read Step file error\n");
	
		Standard_Boolean failsonly = false;
		aReader.PrintCheckLoad(failsonly,IFSelect_CountByItem);
		Standard_Integer nbr=aReader.NbRootsForTransfer();

		if(nbr==0)
			return NULL;

		shapes = new TopTools_HSequenceOfShape();
		shapes->Clear();

		for(Standard_Integer i=1;i<=nbr;i++)
		{
			Standard_Boolean ok = aReader.TransferRoot(i);
			Standard_Integer nbs = aReader.NbShapes();
			
			if(ok==Standard_True && nbs >0)
			{
				for(Standard_Integer j=1;j<=nbs;j++)
				{
					shapes->Append(aReader.Shape(j));
			
				}
			}
		}
	//	shapeExplorer(shapes);
		return shapes;
}



Bnd_Box BoundVolumeOfSolid(TopTools_HSequenceOfShape part)
{
	Bnd_Box box ;
	for (_int i = 1; i < part.Length(); i++)
		BRepBndLib::Add( part.Value(i), box, false);
	return box;
}
