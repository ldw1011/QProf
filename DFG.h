#pragma once
#include <iostream>
#include <vector>
namespace llvm
{
  class Value;
};
struct InstProperties
{
  InstProperties()
    :inst(NULL)
  {
  }
  InstProperties(llvm::Value* _inst,int op_id, int op_idx, std::string opcode, std::string text)
    :inst(_inst),op_id(op_id),op_idx(op_idx),opcode(opcode),text(text)
  {
    // First Get instruction inst_id and inst_id_idx
    // Label
  }
  std::ostream& label(std::ostream& os);
  int id;
  int op_id;
  int op_idx;
  std::string opcode;
  std::string text;
  llvm::Value* inst;
};
typedef InstProperties IP;
