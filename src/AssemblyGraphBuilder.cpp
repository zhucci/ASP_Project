#include "AssemblyGraphBuilder.h"
#include "boost/graph/graphviz.hpp"
#include <iostream>
#include <cstdio>
#include <process.h>



int AssemblyGraphBuilder::Init(asp::Assembly *convertedAsm){
	//write_graphviz
	auto pShapeMap = convertedAsm->GetUnitMap();
	if (!pShapeMap->size())
		return -1;

	if(asmGraph)
		asmGraph = new assemblyGraph(pShapeMap->size());

	if(partGraphSet)
		partGraphSet = new partGraphMap;
	for (auto &p : *pShapeMap){
		//pushToPartSetGraph(p.first, dynamic_cast<Part*> (p.second));
		//addEdgeToAssemblyGraph(p.first, dynamic_cast<Part*> (p.second));
		;
	}
	
	
	
	// Create callback to print mappings


	std::ofstream graphStream("Graph.dot");
	if (graphStream.is_open()){
	//	write_graphviz(graphStream,graph1);
	//	execl("dot.exe -Tpng Graph.dot > output.png",NULL);
	//	execl("output.png",NULL);
	}
		
	return 0;
}

_int AssemblyGraphBuilder::pushToPartSetGraph(PartUri pUri, asp::Part* part){

	return 0;
}
_int AssemblyGraphBuilder::addEdgeToAssemblyGraph(PartUri pUri, asp::Part* part){

	return 0;
}
AssemblyGraphBuilder::FaceSpartial AssemblyGraphBuilder::getFaceSpartialDescriptor(const asp::SurfaceAttribute &surface1,
	const asp::SurfaceAttribute &surface2){
	
	FaceSpartial fSpartialDesc;
	

}

AssemblyGraphBuilder::BodyForm AssemblyGraphBuilder::getBodyDescriptor(asp::Part* part){
	
	BodyForm bForm;
}
AssemblyGraphBuilder::FaceType AssemblyGraphBuilder::getFaceType(const asp::SurfaceAttribute &surface1){
	FaceType fType;
	if (surface1.Func == asp::SurfaceFunction::_Base)
		fType.exect_match.push_back(1);
	else
		fType.exect_match.push_back(0);
}
AssemblyGraphBuilder::ContactType AssemblyGraphBuilder::getContactType(asp::Part* part1, asp::Part* part2){
	ContactType cType;
	

}
std::string AssemblyGraphBuilder::GetPartGraphImageFileName(_int PartUri){
	std::string FileName("djfdj");

	return FileName;
}