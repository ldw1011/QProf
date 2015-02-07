#include "ExtractGraphModel.h"
#include "llvm/IR/User.h"

bool ExtractGraphModel::runOnModule(Module &M)
{
  for (Module::iterator F = M.begin(), EF=M.end(); F!=EF; ++F)
  {
    if(F->isDeclaration())
    {
      continue;
    }
    
    /*
       Argument node 
    for (Function::arg_iterator ab=F->arg_begin(), ae=F->arg_end(); ab!=ae; ++ab)
    {
       Value* op=&(*ab);
       addInstEdge(cdfg, op, inst);
    }*/


    cdfg_ptr cdfg=createCDFG();
    for (Function::iterator B = F->begin(), BE = F->end(); B != BE; ++B)
      // Print out the name of the basic block if it has one, and then the
      // number of instructions that it contains
    {
      BasicBlock* bb=&(*B);
      addBBNode(cdfg,bb);
      errs() << "Basic block (name=" << B->getName() << ") has "
      << B->size() << " instructions.\n";
      
      for (BasicBlock::iterator I = B->begin(), IE = B->end(); I != IE; ++I)
      {
        errs() << *I << "\n";
        Value* inst=&(*I);
        IP ip(inst);
        addInstNode(cdfg,bb,inst,ip);
      }
    }
    for (Function::iterator B = F->begin(), BE = F->end(); B != BE; ++B)
      // Print out the name of the basic block if it has one, and then the
      // number of instructions that it contains
    {
      BasicBlock* bb=&(*B);
      addBBNode(cdfg,bb);
      errs() << "Basic block (name=" << B->getName() << ") has "
      << B->size() << " instructions.\n";
      
      for (BasicBlock::iterator I = B->begin(), IE = B->end(); I != IE; ++I)
      {
        errs() << *I << "\n";
        Value* inst=&(*I);
        if(isa<GetElementPtrInst>(inst))
        {
          GetElementPtrInst* gep=cast<GetElementPtrInst>(I);
          for (User::op_iterator op_b=gep->idx_begin(), op_e=gep->idx_end(); op_b!=op_e; op_b++)
          {
            Value* op=op_b->get();
            if(isa<Instruction>(op) || isa<Argument>(op))
            {
              errs()<<"\t"<<*op<<"\n";
              addInstEdge(cdfg, op, inst);
            }
          }
        }
        else
        {
          for (User::op_iterator op_b=I->op_begin(), op_e=I->op_end(); op_b!=op_e; op_b++)
          {
            Value* op=op_b->get();
            if(isa<Instruction>(op))
            {
              errs()<<"\t"<<*op<<"\n";
              addInstEdge(cdfg, op, inst);
            }
          }
        }
      }
    }
    print_cdfg(cdfg);


  }
  return true;
}

char ExtractGraphModel::ID = 1;
static RegisterPass<ExtractGraphModel> X("ExtractGraphModel","Extract Graph Model", false, false);
