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
