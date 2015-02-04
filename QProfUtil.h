#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Debug.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/User.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/ADT/ilist.h"
#include "llvm/Analysis/InstructionSimplify.h"
#include "llvm/Analysis/LoopIterator.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/Utils/Cloning.h"
#include "llvm/Transforms/Utils/Local.h"
#include "llvm/Transforms/Utils/LoopUtils.h"
#include "llvm/Transforms/Utils/SimplifyIndVar.h"
#include "llvm/Transforms/Utils/UnrollLoop.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Metadata.h"
#include "llvm/IR/InstIterator.h"
#include <fstream>
#include <list>
#include <set>
#include <map>
#include <vector>
#include <math.h>
#include <stdlib.h>
using namespace std;
using namespace llvm;
bool isFloatTy(Type* type);
bool supportedInst(Instruction *I);
Constant* getTraceFunc(Module* m);
bool supportedInst(Instruction *I);
Value* castDouble(Instruction* I,Value* v);
Module* getModuleFromVal(Instruction* I);
void putTraceFunc(Instruction* I, Constant* id, Constant* idx);
template<class T>
void setQProfIDMetadata(T* I, string tag, int id)
{
  LLVMContext& C=I->getContext();
  Type* int32ty=Type::getInt32Ty(C);
  vector<Value*> metaarray(1);
  metaarray[0]=ConstantInt::get(int32ty, id, false);
  ArrayRef<Value*> metadata(metaarray);
  MDNode* N=MDNode::get(C,metadata);
  I->setMetadata(tag.c_str(),N);
  //DEBUG_MSG(errs()<<"Set ID = "<<id<<" "<<(*I)<<"\n";
  return;
}

template<class T>
int getQProfIDMetadata(T* I, string tag)
{
  int ret=-1;
  //LLVMContext& C=I->getContext();
  //Type* int32ty=Type::getInt32Ty(C);
  MDNode* N=I->getMetadata(tag.c_str());
  if(N!=NULL)
  {
    Value* id=(N->getOperand(0));
    ret=cast<ConstantInt>(id)->getZExtValue();
    //DEBUG_MSG(errs()<<"get ID = "<<ret<<" "<<(*I)<<"\n";
  }
  return ret;
}

