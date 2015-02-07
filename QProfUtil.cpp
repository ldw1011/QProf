#include "QProfUtil.h"
bool isFloatTy(Type* type)
{
  if(type->isHalfTy() || type->isFloatTy() || type->isDoubleTy()
      || type->isX86_FP80Ty() || type->isFP128Ty())
  {
    if(type->isHalfTy() || type->isX86_FP80Ty() || type->isFP128Ty())
    {
      errs()<<"ERROR: "<<*type<<"is not supported yet\n";
    }
    return true;
  }
  else
  {
    return false;
  }
}
bool supportedInst(Instruction *I)
{
  if(
      I->getOpcode()==Instruction::FAdd||
      I->getOpcode()==Instruction::FSub||
      I->getOpcode()==Instruction::FMul
    )
  {
    return true;
  }
  else
  {
    return false;
  }
}
// For FADD,FMUL, PHINODE -> not supported yet
Constant* getTraceFunc(Module* m)
{
  vector<Type*> traceArgs;
  LLVMContext& C=m->getContext();
  Type* dbTy=Type::getDoubleTy(C);
  Type* voidTy=Type::getVoidTy(C);
  Type* int32Ty=Type::getInt32Ty(C);
  traceArgs.push_back(int32Ty); // operator map id
  traceArgs.push_back(int32Ty); // operator id
  for (int i=0; i<3; i++)
    traceArgs.push_back(dbTy);  // op0, op1, return

  ArrayRef<Type*> argsRef(traceArgs);
  FunctionType *traceType=
    FunctionType::get(voidTy,argsRef, false);
  return m->getOrInsertFunction("trace",traceType);
}
Value* castDouble(Instruction* I,Value* v, Instruction* insertPtr)
{
  IRBuilder<> builder(insertPtr);
  LLVMContext& C = I->getContext();
  Type* dbTy=Type::getDoubleTy(C);
  if(v->getType()!=dbTy)
  {
    Value* casted=builder.CreateFPCast(v,dbTy,"_ct");
    return casted;
  }
  else
  {
    return v;
  }
}
Module* getModuleFromVal(Instruction* I)
{
  Function *M = I->getParent() ? I->getParent()->getParent() : NULL;
  return M ? M->getParent() : NULL;
}
void putTraceFunc(Instruction* I, Constant* id, Constant* idx, Instruction* insertPtr)
{
  IRBuilder<> builder(insertPtr);
  assert(isFloatTy(I->getType())==true);
  LLVMContext& C=I->getContext();
  Function* trace=cast<Function>(getTraceFunc(getModuleFromVal(I)));
  std::vector<Value*> args;
  args.push_back(id);
  args.push_back(idx);
  args.push_back(castDouble(I,I,insertPtr));
  for(int i=0,e=I->getNumOperands(); i<e; i++)
  {
    Value* arg=I->getOperand(i);
    args.push_back(castDouble(I,arg,insertPtr));
  }
  ArrayRef<Value*> argRef(args);
  Value* callinst=builder.CreateCall(trace, argRef);
}
