#include <vector>
namespace llvm
{
  class Value;
  class BasicBlock;
};
struct InstProperties
{
  Value* inst;
};
struct BBProperties
{
  std::list<Value*> insts;
};
