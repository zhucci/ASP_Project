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
enum edge_ContactDesc_t { edge_ContactDesc };

namespace boost {

	BOOST_INSTALL_PROPERTY(edge, FaceSpartial);
	BOOST_INSTALL_PROPERTY(edge, ContactDesc);
	BOOST_INSTALL_PROPERTY(vertex, FaceType);
	BOOST_INSTALL_PROPERTY(vertex, PartUri);
	BOOST_INSTALL_PROPERTY(vertex, BodyForm);
};

typedef _int PartUri;
typedef _int FaceUri;

struct PartIsomorphism{
	std::pair<PartUri, PartUri> isoPart;
	std::vector<std::pair<FaceUri, FaceUri>> isoFaceSet;
};
typedef std::vector<PartIsomorphism> isomorphismPartFaceMap;

namespace asp{
class AssemblyGraphBuilder {
public :

	friend class AspMainTest;

	typedef undirectedS graph_orientationType;

	struct FaceSpartial{
		_real DistDesc;
		_real AngDesc;
		friend bool operator==(const FaceSpartial& f1, const FaceSpartial& f2){
			return std::abs(f1.DistDesc - f2.DistDesc)<0.1 &&
				std::abs(f1.AngDesc - f2.AngDesc)<0.05 ;
		}
	};
	struct BodyForm{
		PartUri partUri;
		_real MatVolRatio;
		friend bool operator==(const BodyForm& b1, const BodyForm& b2){
			return std::abs(b1.MatVolRatio - b2.MatVolRatio)<0.2;
		}
	};
	struct FaceType{
		FaceUri faceUri;
		_int FaceFunctionType;
		_int FaceFormType;
		friend bool operator==(const FaceType& ft1, const FaceType& ft2){
			if (ft1.FaceFunctionType == ft2.FaceFunctionType &&
				ft1.FaceFormType == ft2.FaceFormType)
				return true;
			else 
				return false;	
		}
		
	};
	
	struct ContactDesc{
		_int contactType;
		friend _bool operator==(const ContactDesc &desc1,const ContactDesc &desc2){
			return desc1.contactType==desc2.contactType;
		}
	};

	template<typename Graph1, typename Graph2>
	struct user_callback {
		friend class AssemblyGraphBuider;

		user_callback(const Graph1& graph1, const Graph2& graph2)
		: graph1_(graph1), graph2_(graph2) {}

		template <typename CorrespondenceMap1To2,
			typename CorrespondenceMap2To1>
			bool operator()(CorrespondenceMap1To2 f, CorrespondenceMap2To1) const {

				// Print (sub)graph isomorphism map
				BGL_FORALL_VERTICES_T(v, graph1_, Graph1){
					BodyForm part1 = get(vertex_BodyFormDesc, graph1_, v);
					BodyForm part2 = get(vertex_BodyFormDesc, graph2_, get(f, v));
					PartToPartMap.emplace(part1.partUri, part2.partUri);
				}

				return true;
			}

		const Graph1& graph1_;
		const Graph2& graph2_;
		std::map<PartUri, PartUri> PartToPartMap;

	};

	
	typedef adjacency_list<vecS, vecS, graph_orientationType,
		property<vertex_FaceType_t, BodyForm>,
		property<edge_FaceSpartial_t, FaceSpartial >> partGraph;

	
	typedef adjacency_list<vecS, vecS, graph_orientationType,
		property<vertex_BodyForm_t, BodyForm>,
		property<edge_ContactDesc_t, ContactDesc >> assemblyGraph;

//	typedef property_map<partGraph, edge_FaceSpartial_t>::type PartGraph_edgePropMap;

//	typedef property_map<partGraph, vertex_BodyForm_t>::type AssemblyGraph_vertexPropMap;

// Binary function object that returns true if BodyForm of one part is similar to other

	struct AssemblyGraph_Vertex_equivalent {

		AssemblyGraph_Vertex_equivalent(const property_map<assemblyGraph, vertex_BodyForm_t> property_map1,
		const property_map<assemblyGraph, vertex_BodyForm_t> property_map2) :
		m_property_map1(property_map1),
		m_property_map2(property_map2) { }

		template <typename ItemFirst,
			typename ItemSecond>
			bool operator()(const ItemFirst item1, const ItemSecond item2) {
				BodyForm form1 = get(m_property_map1, item1);
				BodyForm form2 = get(m_property_map2, item2);
				return std::abs(form1.MatVolRatio-form2.MatVolRatio)<0.2;		
			}

	private:
		const property_map<assemblyGraph, vertex_BodyForm_t> m_property_map1;
		const property_map<assemblyGraph,vertex_BodyForm_t> m_property_map2;
	};




	typedef _int PartUri;
	
	typedef std::map<PartUri,std::pair<partGraph, partGraph>> partGraphMap;


	AssemblyGraphBuilder(){};

	_int Init(asp::Assembly *convertedAsm);

	std::string GetPartGraphImageFileName(_int PartUri);

private:
	_int LastRetrivale{0};
	_int pushToPartSetGraph(PartUri pUri, asp::Part* part);

	_int addEdgeToAssemblyGraph(PartUri pUri, asp::Part* part);

	FaceSpartial getFaceSpartialDescriptor(asp::SurfaceAttribute &surface1,
		asp::SurfaceAttribute &surface2);
	_bool GetTraitOfFace(asp::SurfaceAttribute &s1, std::pair<gp_Pnt, gp_Vec> &traits);
	BodyForm getBodyDescriptor(asp::Part* part);
	FaceType getFaceType(const asp::SurfaceAttribute &surface1);
	_int getContactDesc(asp::Part* part1, asp::Part* part2, AssemblyGraphBuilder::ContactDesc &desc);

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
};
#endif // !_AsmGraphBuilder
