#include <iostream>
#include <string>
#include "QTag.h"
#include "QProfUtil.h"
bool QTag::runOnModule(Module &M)
{
  errs()<<"QTAG\n";
  for (Module::iterator f = M.begin(), e=M.end(); f!=e; ++f)
  {
    Function* F=&(*f);
    LoopInfo* LI;
    if(F->isDeclaration())
    {
      errs()<<"F?\n";
      continue;
    }
    //ScalarEvolution *SE = &getAnalysis<ScalarEvolution>(F);
    errs()<<"HERE\n";
    // Put tag
    int opid=0;
    for (inst_iterator Inst = inst_begin(*F), E = inst_end(*F); Inst != E; ++Inst)
    {
      Instruction* I=&(*Inst);
      if( supportedInst(I))
      {
        errs()<<*I<<"\n";
        setQProfIDMetadata(I,"op.id",opid);
        opid++;
      }
    }

    // Unroll loop

    // Put profiler
  }
  return true;
}
char QTag::ID = 1;
static RegisterPass<QTag> X("QTag","quality profiling", false, false);
bool QTagPost::runOnModule(Module &M)
{
  LLVMContext& C=M.getContext();
  Type* int32Ty=Type::getInt32Ty(C);
  for (Module::iterator f = M.begin(), e=M.end(); f!=e; ++f)
  {
    map<int,int> op_id_counter;
    Function* F=&(*f);

    if(F->isDeclaration())
    {
      continue;
    }
    //ScalarEvolution *SE = &getAnalysis<ScalarEvolution>(F);
    errs()<<"HERE\n";

    for (inst_iterator Inst = inst_begin(*F), E = inst_end(*F); Inst != E; ++Inst)
    {
      Instruction* I=&(*Inst);
      int op_id=getQProfIDMetadata(I,"op.id");
      if(op_id>-1)
      {
        errs()<<*I<<" "<<op_id<<" "<<op_id_counter[op_id]<<"\n";
        setQProfIDMetadata(I,"op.idx",op_id_counter[op_id]);
        op_id_counter[op_id]++;
      }
    }
  }
  return true;
}
char QTagPost::ID = 2;
static RegisterPass<QTagPost> X2("QTagPost","quality profiling post", false, false);
