// Without RTTI
#include "DFG.h"
#include <memory>
#include <string>
class CDFG;
namespace llvm
{
  class Value;
  class BasicBlock;
}
/*
Graph* createGraph();


int addBBNode(Graph* g,  BBProperties* bp=NULL); // return BBID
int addInstNode(Graph* g, int bbid, InstProperties* ip=NULL); //return nodeID

void addInstEdge(Graph* g, llvm::Value* from, llvm::Value* to);
void addBBEdge(Graph* g, llvm::BasicBlock* from, llvm::BasicBlock* to);
*/
typedef std::shared_ptr<CDFG> cdfg_ptr;
cdfg_ptr createCDFG();
void addBBNode(cdfg_ptr cdfg, llvm::BasicBlock* bb);

void addInstNode(cdfg_ptr cdfg, llvm::BasicBlock* bb,
    llvm::Value* inst,IP& ip);

void addInstEdge(cdfg_ptr cdfg, llvm::Value* from,
    llvm::Value* to);
void addBBEdge(cdfg_ptr cdfg, llvm::BasicBlock* from,
    llvm::BasicBlock* to);
void print_cdfg(cdfg_ptr cdfg, std::string func_name);
