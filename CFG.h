#pragma once
#include <vector>
#include "DFG.h"
#include "Graph.hpp"
namespace llvm
{
  class Value;
  class BasicBlock;
};
typedef Graph < IP, EG > dfg_t;
struct BasicBlockProperties
{
  BasicBlockProperties()
    :bb(NULL) {}
  BasicBlockProperties(llvm::BasicBlock* _bb)
    :bb(_bb) {}
  int id;
  llvm::BasicBlock* bb;
  std::vector<dfg_t::Vertex> insts;
};
typedef BasicBlockProperties BP;
typedef Graph <BP, EG > cfg_t;
