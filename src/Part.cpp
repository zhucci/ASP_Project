
#include "Part.h"

using namespace asp;

Part::Part(const TDF_Label &label, Unit* root/*=NULL*/):
	Unit(label,root)
{
	

	if(myUnitType!=unitType::_Part)
		Standard_Failure::Raise("shape not suitable for part");


	/*Part surface exploration */
	TopExp_Explorer exp(myshape,TopAbs_FACE);
		for(;exp.More();exp.Next())
		{
			//auto aSurface =	BRep_Tool::Surface(TopoDS::Face(exp.Current()));
			//auto srf = Handle_Geom_ElementarySurface::DownCast(aSurface);
			//if(!srf.IsNull()){
			
			colOfSurf.push_back(SurfaceAttribute(exp.Current()));
		//	}
			
		}

	/*Volume calculation*/
	MaterialVolumeUpdate();
	BoundVolumeUpdate();

}

void Part::AdjacencyCalc()
{
	#ifdef _DEBUG
		OSD_Chronometer chron;
		chron.Start();
#endif 
		for(Standard_Integer i=0;i<colOfSurf.size();i++){

			//GeomAdaptor_HSurface adaptorS1(colOfSurf[i].surf);
			auto &S1 = colOfSurf[i].myShape;
			GeomLProp_SLProps prop_S1(BRep_Tool::Surface(S1),1,LinTol());
			ShapeAnalysis_Surface toolS1(BRep_Tool::Surface(S1));

			for(Standard_Integer j=i+1;j<colOfSurf.size();j++)

				if(colOfSurf[i].myShape.IsPartner(colOfSurf[j].myShape)){

					Standard_Real CosFi=-2;

					auto &S2 = colOfSurf[j].myShape;
					//GeomAdaptor_HSurface adaptorS2(colOfSurf[j].surf);
					ShapeAnalysis_Surface toolS2(BRep_Tool::Surface(S2));
					GeomLProp_SLProps prop_S2(BRep_Tool::Surface(S2),1,LinTol());
					
					TopExp_Explorer exp1(colOfSurf[i].myShape,TopAbs_EDGE);
					TopExp_Explorer exp2(colOfSurf[j].myShape,TopAbs_EDGE);

					for(;exp1.More();exp1.Next()){
						for(;exp2.More();exp2.Next())

							if(exp1.Current().IsEqual(exp2.Current())){
								
								Standard_Real Vmin,Vmax;
								Handle_Geom_Curve C1=  BRep_Tool::Curve(TopoDS::Edge(exp1.Current()),Vmin,Vmax);
								gp_Pnt pnt = C1->Value(Vmin);
								
								gp_Pnt2d pnt2d_S1 = toolS1.ValueOfUV(pnt,LinTol());
								gp_Pnt2d pnt2d_S2 = toolS2.ValueOfUV(pnt,LinTol());

								
								prop_S1.SetParameters(pnt2d_S1.Coord().X(),pnt2d_S1.Coord().Y());
								prop_S2.SetParameters(pnt2d_S2.Coord().X(),pnt2d_S2.Coord().Y());
								CosFi = prop_S1.Normal().XYZ()*prop_S2.Normal().XYZ();
								
								goto _exit_;
							}
					}
						
_exit_:
				if(CosFi<-1)
					Standard_Failure::Raise("Cos of Partner Surfaces less -1");

				colOfSurf[i].colOfAdjSurf.push_back(AdjacencySS(CosFi,j));
				colOfSurf[j].colOfAdjSurf.push_back(AdjacencySS(CosFi,i));
				
			}

	}
#ifdef _DEBUG
	
		chron.Stop();
		chron.Show(std::cout);
#endif
}
Part::~Part()
{
}
void Part::bndBoxCalc()
{
	try{
		
		myBox = new Bnd_Box;
		BRepBndLib::Add(myshape, *myBox, false);
	}
	catch(Standard_Failure)
	{
		std::cerr<<__FILE__<<": "<<__LINE__<<endl<<"Bnd Box Calc error"<<endl;
	std::cerr<<__FILE__<<": "<<__LINE__<<endl<<"TopAbsEnum: " <<this->myShapeType<<endl;
	
		this->myLabel.Dump(std::cerr);	
	}	
}
void Part::BoundVolumeUpdate()
{
	try{
	bndBoxCalc();
	Standard_Real xi, xs, yi, ys, zi, zs;
	myBox->Get(xi, yi, zi, xs, ys, zs);
	bndBoxVolume = (xs - xi)*(ys - yi)*(zs - zi);
	}
	catch(Standard_Failure)
	{
		
		std::cerr<<__FILE__<<": "<<__LINE__<<endl;
		std::cerr<<Standard_Failure::Caught()->GetMessageString()<<endl;
	}
}

void Part::MaterialVolumeUpdate()
{

		GProp_GProps props;

		BRepGProp::VolumeProperties(myshape, props, Standard_True);
		unitVolume = props.Mass();
		ShapeCentre = props.CentreOfMass();
		check = true;
}
void Part::Dump(Standard_OStream &stream, Standard_Integer rank)
{
		for(int i=0;i<rank;i++)
			stream<<"   ";
		if(rank)
		stream<<"|"<<endl;
		for(int i=0;i<rank;i++)
			stream<<"   ";
		if(rank)
			stream<<"\\_";
		stream<<"part : ";
		
		if(Name!=NULL)
			stream<<Name;
		
		Standard_Real x_i,y_i,z_i,x_s,y_s,z_s;
		myBox->Get(x_i,y_i,z_i,x_s,y_s,z_s);
		
			stream<<" Bnd:["<<x_i<<","<<x_s<<"],["<<y_i<<","<<y_s<<"],["<<z_i<<","<<z_s<<"]";
		
			stream<<"--ID:"<<uri;
		stream<<endl;

		
		rank++;
			for(auto s: colOfSurf){
				for(int i=0;i<rank;i++)
				stream<<"   ";
			if(rank)
				stream<<"|"<<endl;
			for(int i=0;i<rank;i++)
				stream<<"   ";
			if(rank)
				stream<<"\\_";

				stream<<"Type:"<<static_cast<const char *> (typeToStr(s.Type));
				stream<<"U ["<<s.surf.FirstUParameter()<<","<<s.surf.LastUParameter()<<"] ";
				stream<<"V ["<<s.surf.FirstVParameter()<<","<<s.surf.LastVParameter()<<"] ";
				//stream<<" Dir:["<<s.dir.X()<<","<<s.dir.Y()<<","<<s.dir.Z()<<"] ";
				stream<<"Hash: "<<s.uri;
				//s.myBox.Get(x_i,y_i,z_i,x_s,y_s,z_s);
				//stream<<"Bnd:["<<x_i<<","<<x_s<<"],["<<y_i<<","<<y_s<<"],["<<z_i<<","<<z_s<<"]";
				stream<<endl;		
			}
				
}
