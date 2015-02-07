#include "CDFG.h"
#include <memory>
typedef std::shared_ptr<CDFG> cdfg_ptr;
cdfg_ptr createCDFG()
{
  return cdfg_ptr(make_shared<CDFG>());
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
void print_cdfg(cdfg_ptr cdfg)
{
  cdfg->cfg.print();
  cdfg->dfg.print();
   // print_edges2(cdfg->cfg, get(BP, cdfg->cfg), get(edge_index, cdfg->cfg));
}
