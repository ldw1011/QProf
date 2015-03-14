#pragma once
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/graphviz.hpp>
using namespace boost;
using namespace std;
/* definition of basic boost::graph properties */
enum vertex_properties_t { vertex_properties };
enum edge_properties_t { edge_properties };
namespace boost {
  BOOST_INSTALL_PROPERTY(vertex, properties);
  BOOST_INSTALL_PROPERTY(edge, properties);
}
struct EG
{
  int i;
};
template<typename G>
class vertexwriter
{
  public:
  vertexwriter(G* _g):g(_g) {}
  void operator()(ostream& os, typename G::Vertex& v) const
  {
    g->properties(v).label(os);
  }
  G* g;
};

/* the graph base class template */
template < typename VERTEXPROPERTIES, typename EDGEPROPERTIES >
class Graph
{
  public:

    /* an adjacency_list like we need it */
    typedef adjacency_list<
      setS, // disallow parallel edges
      listS, // vertex container
      bidirectionalS, // directed graph
      property<vertex_properties_t, VERTEXPROPERTIES>,
      property<edge_properties_t, EDGEPROPERTIES>
        > GraphContainer;


    /* a bunch of graph-specific typedefs */
    typedef typename graph_traits<GraphContainer>::vertex_descriptor Vertex;
    typedef typename graph_traits<GraphContainer>::edge_descriptor Edge;
    typedef std::pair<Edge, Edge> EdgePair;

    typedef typename graph_traits<GraphContainer>::vertex_iterator vertex_iter;
    typedef typename graph_traits<GraphContainer>::edge_iterator edge_iter;
    typedef typename graph_traits<GraphContainer>::adjacency_iterator adjacency_iter;
    typedef typename graph_traits<GraphContainer>::out_edge_iterator out_edge_iter;

    typedef typename graph_traits<GraphContainer>::degree_size_type degree_t;

    typedef std::pair<adjacency_iter, adjacency_iter> adjacency_vertex_range_t;
    typedef std::pair<out_edge_iter, out_edge_iter> out_edge_range_t;
    typedef std::pair<vertex_iter, vertex_iter> vertex_range_t;
    typedef std::pair<edge_iter, edge_iter> edge_range_t;


    /* constructors etc. */
    Graph()
    {}

    Graph(const Graph& g) :
      graph(g.graph)
  {}

    virtual ~Graph()
    {}


    /* structure modification methods */
    void Clear()
    {
      graph.clear();
    }

    Vertex AddVertex(const VERTEXPROPERTIES& prop)
    {
      Vertex v = add_vertex(graph);
      properties(v) = prop;
      return v;
    }

    void RemoveVertex(const Vertex& v)
    {
      clear_vertex(v, graph);
      remove_vertex(v, graph);
    }

    EdgePair AddEdge(const Vertex& v1, const Vertex& v2, const EDGEPROPERTIES& prop_12, const EDGEPROPERTIES& prop_21)
    {
      /* TODO: maybe one wants to check if this edge could be inserted */
      Edge addedEdge1 = add_edge(v1, v2, graph).first;
      Edge addedEdge2 = add_edge(v2, v1, graph).first;

      properties(addedEdge1) = prop_12;
      properties(addedEdge2) = prop_21;

      return EdgePair(addedEdge1, addedEdge2);
    }
    void print()
    {
      typename property_map<GraphContainer, vertex_properties_t>::type param = get(vertex_properties, graph);
      print_graph(graph,param);
      print_edges2(graph,param, get(edge_properties, graph));
    }


    /* property access */
    VERTEXPROPERTIES& properties(const Vertex& v)
    {
      typename property_map<GraphContainer, vertex_properties_t>::type param = get(vertex_properties, graph);
      return param[v];
    }

    const VERTEXPROPERTIES& properties(const Vertex& v) const
    {
      typename property_map<GraphContainer, vertex_properties_t>::const_type param = get(vertex_properties, graph);
      return param[v];
    }

    EDGEPROPERTIES& properties(const Edge& v)
    {
      typename property_map<GraphContainer, edge_properties_t>::type param = get(edge_properties, graph);
      return param[v];
    }

    const EDGEPROPERTIES& properties(const Edge& v) const
    {
      typename property_map<GraphContainer, edge_properties_t>::const_type param = get(edge_properties, graph);
      return param[v];
    }


    /* selectors and properties */
    const GraphContainer& getGraph() const
    {
      return graph;
    }

    vertex_range_t getVertices() const
    {
      return vertices(graph);
    }

    adjacency_vertex_range_t getAdjacentVertices(const Vertex& v) const
    {
      return adjacent_vertices(v, graph);
    }

    int getVertexCount() const
    {
      return num_vertices(graph);
    }

    int getVertexDegree(const Vertex& v) const
    {
      return out_degree(v, graph);
    }


    /* operators */
    Graph& operator=(const Graph &rhs)
    {
      graph = rhs.graph;
      return *this;
    }
    void write_graph(std::string filename)
    {
      std::cout<<"Filename: "<<filename<<"\n";
      std::ofstream out(filename);
      vertexwriter<Graph<VERTEXPROPERTIES,EDGEPROPERTIES> > vpw(this);
      typedef typename graph_traits<GraphContainer>::directed_category cat_type;
      typedef graphviz_io_traits<cat_type> Traits;
      std::string name = "G";
      out << Traits::name() << " " << escape_dot_string(name) << " {" << std::endl;


      vertex_iter i, end;
      typedef typename property_map<GraphContainer, vertex_properties_t>::type vertex_map_t;
      for(boost::tie(i,end) = getVertices();i != end; ++i) {
        auto param = properties(*i);
        out << escape_dot_string(param);
        vpw(out, *i); //print vertex attributes
        out << ";" << std::endl;
      }
      edge_iter ei, edge_end;
      for(boost::tie(ei, edge_end) = edges(graph); ei != edge_end; ++ei) {
        auto snode = source(*ei, graph);
        auto sparam = properties(snode);
        auto tparam = properties(target(*ei,graph));
        out << escape_dot_string(sparam)
            << Traits::delimiter()
            << escape_dot_string(tparam) << " ";
        out << ";" << std::endl;
      }
      out << "}" << std::endl;
      out.close();
    }
  protected:
    GraphContainer graph;
};
/* how to use
   struct VertexProperties {
   int i;
   };

   struct EdgeProperties {
   };

   typedef Graph<VertexProperties, EdgeProperties> MyGraph;

   MyGraph g;

   VertexProperties vp;
   vp.i = 42;

   MyGraph::Vertex v = g.AddVertex(vp);

   g.properties(v).i = 23;


 */
