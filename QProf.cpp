#include <iostream>
#include <string>
#include "QProfUtil.h"
#include "QProf.h"
bool QProf::runOnModule(Module &M)
{
  LLVMContext& C=M.getContext();
  Type* int32Ty=Type::getInt32Ty(C);
  for (Module::iterator f = M.begin(), e=M.end(); f!=e; ++f)
  {
    map<int,int> op_id_counter; 
    Function* F=&(*f);
    LoopInfo* LI;
    if(F->isDeclaration())
    {
      continue;
    }
    //ScalarEvolution *SE = &getAnalysis<ScalarEvolution>(F);
    errs()<<"HERE\n";
    int opid=0;
    for (inst_iterator Inst = inst_begin(*F), E = inst_end(*F); Inst != E; ++Inst)
    {
      Instruction* I=&(*Inst);
      int op_id=getQProfIDMetadata(I,"op.id");
      if(op_id>-1)
      {

        errs()<<*I<<" "<<op_id<<" "<<op_id_counter[op_id]<<"\n";
        putTraceFunc(I, ConstantInt::get(int32Ty, op_id), ConstantInt::get(int32Ty, op_id_counter[op_id]));
        op_id_counter[op_id]++;
      }
    }
    // Unroll loop

    // Put profiler
  }
  return true;
}
char QProf::ID = 1;
static RegisterPass<QProf> X("QProf","quality profiling", false, false);
