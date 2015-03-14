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
    // map<int,int> op_id_counter;
    Function* F=&(*f);

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
      int op_idx=getQProfIDMetadata(I,"op.idx");
      if(op_id>-1)
      {

        errs()<<*I<<" "<<op_id<<" "<<op_idx<<"\n";
        inst_iterator insertPtr=Inst;
        insertPtr++;
        if(I->getOpcode()==Instruction::FAdd
            || I->getOpcode()==Instruction::FMul)
        {
          // putBinaryTraceFunc(I, ConstantInt::get(int32Ty, op_id), ConstantInt::get(int32Ty, op_id_counter[op_id]),&(*insertPtr));
          putBinaryTraceFunc(I, ConstantInt::get(int32Ty, op_id), ConstantInt::get(int32Ty, op_idx),&(*insertPtr));
        }
        else if(I->getOpcode()==Instruction::FCmp)
        {
          // putFCMPTraceFunc(I, ConstantInt::get(int32Ty, op_id), ConstantInt::get(int32Ty, op_id_counter[op_id]),&(*insertPtr));
          putFCMPTraceFunc(I, ConstantInt::get(int32Ty, op_id), ConstantInt::get(int32Ty, op_idx),&(*insertPtr));
        }
        else if(I->getOpcode()==Instruction::Select)
        {
          // putSelectTraceFunc(I, ConstantInt::get(int32Ty, op_id), ConstantInt::get(int32Ty, op_id_counter[op_id]),&(*insertPtr));
          putSelectTraceFunc(I, ConstantInt::get(int32Ty, op_id), ConstantInt::get(int32Ty, op_idx),&(*insertPtr));
        }
        else
        {
          errs()<<"WHAT\n";
        }
        // op_id_counter[op_id]++;
      }
    }
    // Unroll loop

    // Put profiler
  }
  return true;
}
char QProf::ID = 1;
static RegisterPass<QProf> X("QProf","quality profiling", false, false);
