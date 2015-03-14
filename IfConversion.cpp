#include "IfConversion.h"
#include <unordered_set>
#include <functional>
struct ifStruct
{
  enum if_type_t {EMPTY_IF, DIAMOND_IF, TRIANGLE_IF};
  ifStruct()
    :if_type(EMPTY_IF),head(NULL),tail(NULL)
  {
    body[0]=NULL;body[1]=NULL;
  }
  ifStruct(BasicBlock* _head, BasicBlock* _body0,
    BasicBlock* _tail)
    :if_type(TRIANGLE_IF),head(_head),tail(_tail)
  {
    body[0]=_body0;
    body[1]=NULL;
  }
  ifStruct(BasicBlock* _head, BasicBlock* _body0, BasicBlock* _body1,
    BasicBlock* _tail)
    :if_type(DIAMOND_IF),head(_head),tail(_tail)
  {
    body[0]=_body0;
    body[1]=_body1;
  }
  bool operator==(const ifStruct& rhs) const
  {
    if(this->head == rhs.head && this->body[0]==rhs.body[0] && this->body[1]==rhs.body[1]
        && this->tail == rhs.tail)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  if_type_t if_type;
  BasicBlock* head;
  BasicBlock* body[2];
  BasicBlock* tail;
};
namespace  std
{
  template<>
    struct hash<ifStruct>
    {
      typedef ifStruct argument_type;
      typedef std::size_t result_type;
      result_type operator()(argument_type const& s) const
      {
        result_type const h1 ( std::hash<BasicBlock*>()(s.head));
        result_type const h2 ( std::hash<BasicBlock*>()(s.body[2]));
        result_type const h3 ( std::hash<BasicBlock*>()(s.body[1]));
        result_type const h4 ( std::hash<BasicBlock*>()(s.tail));
        return h1 ^ (h2 <<1 ) ^ (h3 <<2) ^ (h4 <<3);
      }
    };
}
/*
ostream& operator<<(ostream& os,const ifStruct& i)
{
  return os<<"head: \n"<<*(i.head)<<"\n"<<"tail: \n"<<*(i.tail)<<"\n";
}*/
BasicBlock* getSingleSuccesor(BasicBlock* B)
{
  succ_iterator SI = succ_begin(B);
  succ_iterator E=succ_end(B);
  if(SI == E) return NULL;
  BasicBlock* SuccBB = *SI;
  ++SI;
  if(SI!=E) return NULL;
  return SuccBB;
}
bool isLoop(BasicBlock* head, BasicBlock* tail)
{
  succ_iterator SI = succ_begin(tail);
  succ_iterator E=succ_end(tail);
  for(;SI!=E; ++SI)
  {
    if(*SI==head)
      return true;
  }
  return false;
}
ifStruct findIfStruct(Function& F)
{
  BasicBlock* body0=NULL;
  BasicBlock* body1=NULL;
  BasicBlock* tail=NULL;
  BasicBlock* head=NULL;
  //check first candidate
  for(Function::iterator B = F.begin(), BE=F.end(); B!=BE; ++B)
  {
    tail=getSingleSuccesor(&(*B));
    head=B->getSinglePredecessor();
    
    if(tail != NULL && head != NULL and !isLoop(head,tail))
    {
      body0=&(*B);
      break;
    }
  }
  if(body0!=NULL)
  {
    //check is it diamond?
    for(Function::iterator B = F.begin(), BE=F.end(); B!=BE; ++B)
    {
      if(tail==getSingleSuccesor(&(*B)) && head==B->getSinglePredecessor())
      {
        body1=&(*B);
        return ifStruct(head,body0,body1,tail);
      }
    }
    return ifStruct(head,body0,tail);
  }
  else
  {
    return ifStruct();
  }
}
Instruction* moveStoreInst(StoreInst* I, BasicBlock* to)
{
  BranchInst* tm=cast<BranchInst>(to->getTerminator());
  if(tm==NULL) 
    errs()<<"Error: moveStoreInst\n";
  Value* cond=tm->getCondition();
  Value* ptr=I->getPointerOperand();
  Value* value=I->getValueOperand();
  Instruction* ld=new LoadInst(ptr,".tmp.ld",tm);
  Value* tValue=NULL;
  Value* fValue=NULL;
  if(tm->getSuccessor(0)==to)
  {
    tValue=ld;
    fValue=value;
  }
  else
  {
    tValue=value;
    fValue=ld;
  }
  Instruction* sel=SelectInst::Create(cond,tValue,fValue,".if.sel",tm);
  Instruction* st=new StoreInst(sel,ptr);
  return st;
}
Instruction* moveInst(Instruction* I, BasicBlock* to)
{
  errs()<<"moveInst: "<<*I<<"\n";
  Instruction* clone=NULL;
  if(isa<StoreInst>(I))
  {
    clone=moveStoreInst(cast<StoreInst>(I), to);
  }
  else
  {
    clone=I->clone();
  }
  errs()<<"Clone: "<<*I<<"\n";
  errs()<<"To: "<<*(to->getTerminator())<<"\n";
  clone->insertBefore(to->getTerminator());
  errs()<<"insert done: "<<*clone<<"\n";
  for(User *u : I->users())
  {
    errs()<<"\t"<<"User before: "<<*u<<"\n";
    u->replaceUsesOfWith(I,clone);
    errs()<<"\t"<<"User after: "<<*u<<"\n";
  }
  errs()<<"replace done\n";
  return clone;
}

void mergeBasicBlock(BasicBlock* B,BasicBlock* head)
{
  errs()<<"mergeBasicBlock\n";
  errs()<<"BODY: "<<*B<<"\n";
  errs()<<"HEAD: "<<*head<<"\n";
  list<Instruction*> delList;
  for(BasicBlock::iterator I=B->begin(), IE=B->end();I!=IE;++I)
  {
    Instruction* inst=&(*I);
    errs()<<"inst: "<<*inst<<"\n";
    if(isa<TerminatorInst>(inst))
    {
      break;
    }
    else
    {
      Instruction* to=moveInst(inst,head);
      delList.push_front(inst);
    }
  }
  for(list<Instruction*>::iterator i=delList.begin(),e=delList.end(); i!=e;i++)
  {
    errs()<<"Erase: "<<*(*i)<<"\n";
    for(User* u : (*i)->users())
    {
      errs()<<"U: "<<*u<<"\n";
    }
    (*i)->eraseFromParent();
  }
}
void eliminateTriangleIfStruct(ifStruct& ifstruct)
{
  errs()<<"eliminateTriangleIfStruct\n";
  mergeBasicBlock(ifstruct.body[0],ifstruct.head);
}
void eliminateDiamondIfStruct(ifStruct& ifstruct)
{
  errs()<<"eliminateDiamondIfStruct\n";
  mergeBasicBlock(ifstruct.body[0],ifstruct.head);
  mergeBasicBlock(ifstruct.body[1],ifstruct.head);
}
bool IfConversion::runOnFunction(Function &F)
{
  std::unordered_set<ifStruct> ifSet;
  int prev_setsize=0;
  do
  {
    prev_setsize=ifSet.size();
    ifSet.insert(findIfStruct(F));
    errs()<<"Set size: "<<ifSet.size()<<"\n";
  }
  while(prev_setsize==ifSet.size());
  for(auto I : ifSet)
  {
    if(I.if_type==ifStruct::DIAMOND_IF)
    {
      eliminateDiamondIfStruct(I);
    }
    if(I.if_type==ifStruct::TRIANGLE_IF)
    {
      eliminateTriangleIfStruct(I);
    }

  }
  return true;
} 
char IfConversion::ID = 1;
static RegisterPass<IfConversion> X("IfConversion","IfConversion", false, false);
