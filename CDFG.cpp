#include "CDFG.h"
#include <boost/graph/subgraph.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/graphviz.hpp>
#include <memory>

typedef std::shared_ptr<CDFG> cdfg_ptr;
cdfg_ptr createCDFG()
{
  return cdfg_ptr(std::make_shared<CDFG>());
}
void addBBNode(cdfg_ptr cdfg, llvm::BasicBlock* bb)
{
  BP bp(bb);
  bp.id=cdfg->tot_bb;
  cfg_t::Vertex v=cdfg->cfg.AddVertex(bp);
  cdfg->tot_bb++;
  cdfg->BBMap[bb]=v;
}
void addInstNode(cdfg_ptr cdfg, llvm::BasicBlock* bb,
    llvm::Value* inst,IP& ip)
{
  ip.id=cdfg->tot_inst;
  dfg_t::Vertex v=cdfg->dfg.AddVertex(ip);

  cdfg->tot_inst++;
  BP& bp=cdfg->cfg.properties(cdfg->BBMap[bb]);
  bp.insts.push_back(v);
  cdfg->InstMap[inst]=v;
}
void addInstEdge(cdfg_ptr cdfg, llvm::Value* from,
    llvm::Value* to)
{
  dfg_t::Vertex fromVertex=cdfg->InstMap[from];
  dfg_t::Vertex toVertex=cdfg->InstMap[to];
  cdfg->dfg.AddEdge(fromVertex,toVertex,EG(),EG());
}
void addBBEdge(cdfg_ptr cdfg, llvm::BasicBlock* from,
    llvm::BasicBlock* to)
{
  cfg_t::Vertex fromVertex=cdfg->BBMap[from];
  cfg_t::Vertex toVertex=cdfg->BBMap[to];
  cdfg->dfg.AddEdge(fromVertex,toVertex,EG(),EG());
}
ostream& operator<<(ostream& os, const IP& ip)
{
  return os<<ip.id;
}
ostream& operator<<(ostream& os, const BP& bp)
{
  return os<<bp.id;
}
ostream& operator<<(ostream& os, const EG& bp)
{
  return os;
}
void print_cdfg(cdfg_ptr cdfg,string func_name)
{
  cout<<"Print CFG\n";
  cdfg->cfg.print();
  cout<<"Print DFG\n";
  cdfg->dfg.print();
  cout<<"Print DOT\n";
  cdfg->cfg.write_graph(string("./dot_")+func_name+string("_cfg.dot"));
  cout<<"Print DOT\n";
  cdfg->dfg.write_graph(string("./dot_")+func_name+string("_dfg.dot"));
   // print_edges2(cdfg->cfg, get(BP, cdfg->cfg), get(edge_index, cdfg->cfg));
}
std::ostream& InstProperties::label(std::ostream& os)
{
  os<<"[op_id=" <<boost::escape_dot_string(op_id) <<",\n"
    <<"op_idx=" <<boost::escape_dot_string(op_idx)<<",\n"
    <<"opcode=" <<boost::escape_dot_string(opcode)<<",\n"
    <<"asm="    <<boost::escape_dot_string(text)  <<"]\n";
  return os;
}

std::ostream& BasicBlockProperties::label(std::ostream& os)
{
  os<<"[op_id=" <<id<<"]\n";
  return os;
}
/*
struct InstPropertiesWriter
{
  InstPropertiesWriter(dfg_t& d):dfg(d){}
  void operator()(ostream& os, dfg_t::Vertex& v)
    const
  {
    os<<dfg.properties(v);
  }
  dfg_t& dfg;
};
struct BasicBlockPropertiesWriter
{
  BasicBlockPropertiesWriter(cfg_t& d):cfg(d){}
  void operator()(ostream& os, cfg_t::Edge& v)
    const
  {
    os<<cfg.properties(v);
  }
  cfg_t& cfg;
};*/
