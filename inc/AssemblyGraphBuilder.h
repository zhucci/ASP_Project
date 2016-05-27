#ifndef _AsmGraphBuilder
#define _AsmGraphBuilder

#include "Assembly.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/vf2_sub_graph_iso.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/utility.hpp>
#include <boost/config.hpp>
#include "surfaceAttribute.h"
#include "Part.h"
#include <sstream>

using namespace boost;

//Graph express spartial relationship between part's faces
//Faces are divided into two groups: contact and ordinary faces
//Each group is divided on surface classes too
//So, all classes amount is 2x(surf types amount)
//There are some kind of graphs
//First one is contact graph (where all nodes are contact faces)
//Add there are one graph for each part in assembly with all its faces with spartial reletationships

using namespace boost;
using namespace std;

enum edge_FaceSpartial_t { edge_faceSpartialDesc };
enum vertex_BodyForm_t{ vertex_BodyFormDesc };
enum vertex_FaceType_t{vertex_FaceType};
enum vertex_PartUri_t { vertex_PartUri};
enum edge_ContactDesc_t { edge_ContactDesc };

namespace boost {

	BOOST_INSTALL_PROPERTY(edge, FaceSpartial);
	BOOST_INSTALL_PROPERTY(edge, ContactDesc);
	BOOST_INSTALL_PROPERTY(vertex, FaceType);
	BOOST_INSTALL_PROPERTY(vertex, PartUri);
	BOOST_INSTALL_PROPERTY(vertex, BodyForm);
};

namespace asp{

typedef _int PartUri;
typedef _int FaceUri;

struct PartIsomorphism{
	PartIsomorphism() :isoPart{ std::pair<PartUri, PartUri>(-1, -1) }{}
	std::pair<PartUri, PartUri> isoPart;
	std::vector<std::pair<FaceUri, FaceUri>> isoFaceSet;
};
typedef std::vector<PartIsomorphism> isomorphismPartFaceMap;

//-------------------------------------------------------------- =
//		Assembly Isomorphism controller
//---------------------------------------------------------------=
//	function: Launch isomorphism checking for subPart of assembly
//---------------------------------------------------------------=

template<typename Graph1, typename Graph2>
struct Iso_AssemblyCallback {

	friend class AssemblyGraphBuider;

	Iso_AssemblyCallback(AssemblyGraphBuilder *graphBuilder, const Graph1& graph1, const Graph2& graph2, bool SaveResultFlag)
		: graph1_(graph1), graph2_(graph2), builder{ graphBuilder }, NeedToSaveResult{ SaveResultFlag }{

		PartToPartMap = new std::map<int, int>();
		BGL_FORALL_VERTICES_T(v, graph1_, Graph1){
			BodyForm part1 = graph1_[v];
			PartToPartMap->emplace(std::pair<int, int>(part1.partUri, 0));
		}
	}
	template <typename CorrespondenceMap1To2,
		typename CorrespondenceMap2To1>
		bool operator()(CorrespondenceMap1To2 f, CorrespondenceMap2To1) const {

			// Print (sub)graph isomorphism map
			//	output << "-------------Isomorphism-------------"<<std::endl;
			//	output << "-------------------------------------"<<std::endl;
			BGL_FORALL_VERTICES(v, graph1_, Graph1){
				graph_traits<Graph1>::vertex_descriptor w = get(f, v);
				
				auto v1Desc = graph1_[v];
				//auto v2Desc = graph2_[w];
				PartToPartMap->find(v1Desc.partUri)->second = graph2_[w].partUri;
				//	output << "#" << v1Desc.partUri << "-->"<<" #"<<v2Desc.partUri<<std::endl;
			}

			if (builder->IsomorphismOfPartCompare(PartToPartMap,NeedToSaveResult))
				return false;
			return true;
		}
	const Graph1& graph1_;
	const Graph2& graph2_;
	bool NeedToSaveResult;
	std::map<int, int> *PartToPartMap;
	AssemblyGraphBuilder *builder;
	//	std::stringstream output;

};

//---------------------------
//Face isomorphism controller
//---------------------------

template<typename Graph1, typename Graph2>
struct Iso_FaceCallback {

	friend class AssemblyGraphBuider;

	Iso_FaceCallback(const Graph1& graph1, const Graph2& graph2, bool AllIsomorphismSearch, std::vector<PartIsomorphism> *isoFaceContainer)
		: graph1_(graph1), graph2_(graph2), allIsomorphismSearch{ AllIsomorphismSearch }, isomorphismFace{ isoFaceContainer }{}
	
	template <typename CorrespondenceMap1To2,
		typename CorrespondenceMap2To1>
		bool operator()(CorrespondenceMap1To2 f, CorrespondenceMap2To1) const {

			// Print (sub)graph isomorphism map
			//	output << "-------------Isomorphism-------------"<<std::endl;
			//	output << "-------------------------------------"<<std::endl;
			if (isomorphismFace){
				
				PartIsomorphism iso;
				BGL_FORALL_VERTICES(v, graph1_, Graph1){
					auto w = get(f, v);
					auto v1Desc = graph1_[v];
					auto v2Desc = graph2_[w];
					iso.isoFaceSet.push_back(std::pair<FaceUri,FaceUri>(v1Desc.faceUri, v2Desc.faceUri));
					//	output << "#" << v1Desc.partUri << "-->"<<" #"<<v2Desc.partUri<<std::endl;
				}
				isomorphismFace->push_back(iso);
			}
			return allIsomorphismSearch;
		}

	
	const Graph1& graph1_;
	const Graph2& graph2_;
	bool allIsomorphismSearch;
	bool needSaveResult;
	std::vector<PartIsomorphism> *isomorphismFace;

	//	std::stringstream output;

};

//-------------------------------------------------
//Face isomorphism  always false controller
//-------------------------------------------------


struct Iso_FaceFalseCallback {

	friend class AssemblyGraphBuider;

	Iso_FaceFalseCallback(){}

	template <typename CorrespondenceMap1To2,
		typename CorrespondenceMap2To1>
		bool operator()(CorrespondenceMap1To2, CorrespondenceMap2To1) const {
			//Stop search
			return false;
		}

};


typedef undirectedS graph_orientationType;

//-------------------------
//			FaceSpartial
//-------------------------
struct FaceSpartial{
	_real DistDesc;
	_real AngDesc;
};

//-------------------------
//			BodyForm
//-------------------------
struct BodyForm{
	int Vertex_Index;
	PartUri partUri;
	_real MatVolRatio;
};
//-------------------------
//			FaceType
//-------------------------

struct FaceType{
	int Vertex_Index;
	FaceUri faceUri;
	_int FaceFunctionType;
	_int FaceFormType;
};
//-------------------------
//			ContactDesc
//-------------------------
struct ContactDesc{
	_int contactType;
	friend _bool operator==(const ContactDesc &desc1, const ContactDesc &desc2){
		return desc1.contactType == desc2.contactType;
	}
};

//Assembly Graph
typedef adjacency_list<vecS, vecS, graph_orientationType,
	BodyForm, ContactDesc > assemblyGraph;

//PartGraph
typedef adjacency_list<vecS, vecS, graph_orientationType,
	FaceType, FaceSpartial> partGraph;


// Binary function object that returns true if BodyForm of one part is similar to other	
struct BodyFormCompare{
	BodyFormCompare(const assemblyGraph& graph1, const assemblyGraph & graph2)
	: sGraph(graph1), bGraph(graph2) {}

	bool operator ()(const graph_traits<assemblyGraph>::vertex_descriptor &v1,
		const graph_traits<assemblyGraph>::vertex_descriptor &v2){
		return std::abs(sGraph[v1].MatVolRatio - bGraph[v2].MatVolRatio)<0.2;
	}

private:
	assemblyGraph sGraph;
	assemblyGraph bGraph;
};

// Binary function 
struct ContactDescCompare{
	ContactDescCompare(const assemblyGraph& graph1, const assemblyGraph & graph2)
	: sGraph(graph1), bGraph(graph2) {}

	bool operator ()(const graph_traits<assemblyGraph>::edge_descriptor &v1,
		const graph_traits<assemblyGraph>::edge_descriptor &v2){
		return sGraph[v1].contactType == bGraph[v2].contactType;
	}

private:
	assemblyGraph sGraph;
	assemblyGraph bGraph;
};

//Binary compare function
struct FaceTypeCompare{

	FaceTypeCompare(const partGraph& graph1, const partGraph & graph2)
		: sGraph(graph1), bGraph(graph2) {}

	bool operator()(const graph_traits<partGraph>::vertex_descriptor &v1,
		const graph_traits<partGraph>::vertex_descriptor &v2){
		if (sGraph[v1].FaceFunctionType == bGraph[v2].FaceFunctionType //&& sGraph[v1].FaceFormType == bGraph[v2].FaceFormType
			)
			return true;
		else
			return false;
	}
	
private:
	partGraph sGraph;
	partGraph bGraph;
};

//Binary compare function
struct FaceSpartialCompare{

	FaceSpartialCompare(const partGraph& graph1, const partGraph & graph2)
	: sGraph(graph1), bGraph(graph2) {}

	bool operator()(const graph_traits<partGraph>::edge_descriptor &v1,
		const graph_traits<partGraph>::edge_descriptor &v2){
		return std::abs(sGraph[v1].DistDesc - bGraph[v2].DistDesc)<0.3 &&
			std::abs(sGraph[v1].AngDesc - bGraph[v2].AngDesc)<0.05;
	}

private:
	partGraph sGraph;
	partGraph bGraph;
};
//-------------------------
//-------------------------
//AssemblyGraphBuilderClass
//-------------------------
//-------------------------

class AssemblyGraphBuilder {
public :

	friend class AspMainTest;

	typedef _int PartUri;
	
	typedef std::map<PartUri,std::pair<partGraph, partGraph>> partGraphMap;


	AssemblyGraphBuilder(){};
	void ShowAssemblyIsomorphism(std::map<int, int> *PartToPartMap);
	

	bool IsomorphismOfPartCompare(std::map<int, int> *currentIsoResult, bool SaveResultFlag);

	bool IsomorphismOfPartCompare(std::pair<partGraph, partGraph> smallGSet, std::pair<partGraph, partGraph> largeG,std::vector<PartIsomorphism> *result);

	bool IsomorphismOfPartCompare(partGraph smallGSet, partGraph largeGSet);

	_int Init(asp::Assembly *convertedAsm);

	assemblyGraph GetAssemblyGraph(std::vector<Part*> *parts);

	_int GripFacesIsomorphism(std::vector<Part*> *partsWithGripFaces,std::vector<PartIsomorphism> isomorphism,bool ReturnFirstResult);

	std::string GetPartGraphImageFileName(_int PartUri);

private:

	bool IsDone{ false };

	_int LastRetrivale{0};

	std::pair<partGraph, partGraph> GetPartGraphsSet( asp::Part* part, bool justBaseAndGripFaces);


	FaceSpartial getFaceSpartialDescriptor(asp::SurfaceAttribute &surface1,
		asp::SurfaceAttribute &surface2);
	_bool GetTraitOfFace(asp::SurfaceAttribute &s1, std::pair<gp_Pnt, gp_Vec> &traits);
	BodyForm getBodyDescriptor(asp::Part* part);
	FaceType getFaceType(const asp::SurfaceAttribute &surface1);
	_int getContactDesc(asp::Part* part1, asp::Part* part2, ContactDesc &desc);

	partGraphMap *partGraphSet{NULL};

	partGraphMap *currentGraphSetForMatch{NULL};
	assemblyGraph	*asmGraph{NULL};
	
	std::vector<PartIsomorphism> currentIsoPart;
	PartIsomorphism currentIso;
	
	//Map of parts
	//Part contact graph
	//Part contact face graph
	//Part full graph
	//Part node descriptor (vector of double)
	//Contact face descriptor (2x double){contact type and ContactSqnare/SquareFace}
	
	//Spartial relationship (2x double){Distance ; Angle}
};
};
#endif // !_AsmGraphBuilder
