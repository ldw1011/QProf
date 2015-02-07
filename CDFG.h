#pragma once
#include "DFG.h"
#include "CFG.h"
#include "Graph.hpp"
#include <map>
  using namespace std;
struct CDFG
{
  CDFG():tot_inst(0), tot_bb(0)
  {
  }
  dfg_t dfg;
  cfg_t cfg;
  int tot_inst;
  int tot_bb;
  map<llvm::Value*, dfg_t::Vertex> InstMap;
  map<llvm::BasicBlock*, cfg_t::Vertex> BBMap;
};

