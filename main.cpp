#include <stdlib.h>
#include <iostream>
#include "llvm/Support/SourceMgr.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Pass.h"
#include "llvm/PassManager.h"
#include "QProf.h"
#include "QTag.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Target/TargetLibraryInfo.h"
using namespace llvm;
int main(int argc, char** argv)
{
  if(argc < 3)
  {
    errs() << "Expected an arguement - IR file name\n";
    exit(1);
  }
  SMDiagnostic Err;
  std::cout<<argv[1]<<std::endl;
  Module *Mod = ParseIRFile(argv[1], Err, getGlobalContext());
  
  Err.print(argv[0],errs());
  std::string ErrInfo="";
  raw_fd_ostream out(argv[2],ErrInfo,llvm::sys::fs::F_None);
  if (Mod)
  {
    PassManager PM;
    PM.add(new QTag());
    PM.add(new TargetLibraryInfo());
    PM.add(createPromoteMemoryToRegisterPass());
    PM.add(createLCSSAPass());
    PM.add(createLoopRotatePass());
    PM.add(createLCSSAPass());
    PM.add(createLoopSimplifyPass());
    PM.add(createSCCPPass());
    PM.add(createLCSSAPass());
    PM.add(createLoopUnrollPass(10000000));
    PM.add(new QProf());
    PM.add(createCFGSimplificationPass());
    PM.add(createFlattenCFGPass());
    PM.add(createStructurizeCFGPass());
    PM.add(createSCCPPass());
    PM.run(*Mod);
    WriteBitcodeToFile(Mod,out); 
  }
  else
  {
    std::cout<<"Mod is null"<<std::endl;
  }

}
