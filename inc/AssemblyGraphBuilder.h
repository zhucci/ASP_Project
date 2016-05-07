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
enum edge_ContactType_t {};

namespace boost {

	template <> struct property_kind<edge_FaceSpartial_t> {
		typedef edge_property_tag type; 
};

template <> struct property_kind<vertex_PartUri_t> {
	typedef vertex_property_tag type;
};

template <> struct property_kind<vertex_BodyForm_t> {
	typedef vertex_property_tag type;
};
template <> struct property_kind<vertex_FaceType_t> {
	typedef vertex_property_tag type;
};

template <> struct property_kind<edge_ContactType_t> {
	typedef vertex_property_tag type;
};
}
typedef _int PartUri;
typedef _int FaceUri;
struct PartIsomorphism{
	std::pair<PartUri, PartUri> isoPart;
	std::vector<std::pair<FaceUri, FaceUri>> isoFaceSet;
};
typedef std::vector<PartIsomorphism> isomorphismPartFaceMap;

class AssemblyGraphBuilder {
public :

	typedef undirectedS graph_orientationType;

	struct FaceSpartial{
		std::vector<double> exect_match;
		std::vector<double> tol_match;
	};
	struct BodyForm{
		PartUri partUri;
		std::vector<double> exect_match;
		std::vector<double> tol_match;
	};
	struct FaceType{
		FaceUri faceUri;
		std::vector<double> exect_match;
		std::vector<double> tol_match;
	};
	struct ContactType{
		std::vector<double> exect_match;
		std::vector<double> tol_match;
	};

	template<typename Graph1, typename Graph2>
	struct user_callback {

		typedef typename graph_traits<Graph1>::vertex_descriptor Vertex1ID;

		typedef typename graph_traits<Graph2>::vertex_descriptor Vertex2ID;

		user_callback(const Graph1& graph1, const Graph2& graph2)
		: graph1_(graph1), graph2_(graph2) {}

		template <typename CorrespondenceMap1To2,
			typename CorrespondenceMap2To1>
			bool operator()(CorrespondenceMap1To2 f, CorrespondenceMap2To1) const {

				// Print (sub)graph isomorphism map
				BGL_FORALL_VERTICES_T(v, graph1_, Graph1){
					char *faceIndex = get(vertex_index_t(), graph1_, v);
					Vertex2ID corFaceIndex = get(vertex_index_t(), graph2_, get(f, v))
				}

				return true;
			}

	private:
		const Graph1& graph1_;
		const Graph2& graph2_;
		std::map<Vertex1ID, Vertex2ID> FaceToFaceMap;

	};

	typedef adjacency_list<vecS, vecS, graph_orientationType,
	property<vertex_FaceType_t,BodyForm> ,
	property<edge_FaceSpartial_t, FaceSpartial>>  partGraph;

	typedef adjacency_list<vecS, vecS, graph_orientationType,
		property<vertex_BodyForm_t, BodyForm>,
		property<edge_ContactType_t, ContactType >> assemblyGraph;

	typedef property_map<partGraph,edge_FaceSpartial_t>::type edgePropMap;

	typedef property_map<partGraph, vertex_BodyForm_t>::type vertexPropMap;

	typedef _int PartUri;
	
	typedef std::map<PartUri,std::pair<partGraph, partGraph>> partGraphMap;



	AssemblyGraphBuilder(){};

	_int Init(asp::Assembly *convertedAsm);

	std::string GetPartGraphImageFileName(_int PartUri);

private:
	_int LastRetrivale{0};
	_int pushToPartSetGraph(PartUri pUri, asp::Part* part);

	_int addEdgeToAssemblyGraph(PartUri pUri, asp::Part* part);

	_int AssemblyGraphBuilder::addEdgeToAssemblyGraph(PartUri pUri, asp::Part* part){

		return 0;
	}
	FaceSpartial getFaceSpartialDescriptor(const SurfaceAttribute &surface1,
		const SurfaceAttribute &surface2);

	BodyForm getBodyDescriptor(Part* part);
	FaceType getFaceType(const SurfaceAttribute &surface1);
	ContactType getContactType(Part* part1, Part* part2);

	partGraphMap *partGraphSet{NULL};
	assemblyGraph	*asmGraph{NULL};
	

	//Map of parts
	//Part contact graph
	//Part contact face graph
	//Part full graph
	//Part node descriptor (vector of double)
	//Contact face descriptor (2x double){contact type and ContactSqnare/SquareFace}
	
	//Spartial relationship (2x double){Distance ; Angle}
};
#endif // !_AsmGraphBuilder
