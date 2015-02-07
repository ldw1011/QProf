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
  InstProperties(llvm::Value* _inst)
    :inst(_inst)
  {
  }
  int id;
  llvm::Value* inst;
};
typedef InstProperties IP;
