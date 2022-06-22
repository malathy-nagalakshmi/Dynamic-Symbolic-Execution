#include "Instrument.h"

using namespace llvm;

namespace instrument {

void instrumentDSEInit(Module *M, Function &F, Instruction &I) {
  // fill your instrumentation code here
    LLVMContext &C=M->getContext();
    IRBuilder<> IRB(C);
    Value* Fun = M->getOrInsertFunction(DSEInitFunctionName, IRB.getVoidTy());
    Function* Func=cast<Function>(Fun);    
    CallInst *Call;
    Call=CallInst::Create(Func,{},"",&I);
    Call->setCallingConv(CallingConv::C);
    Call->setTailCall(false);	
	
}

void instrumentDSEAlloca(Module *M, AllocaInst *AI) {
  LLVMContext &Ctx = M->getContext();
  std::vector<Value *> Args;
  Value *V = ConstantInt::get(Type::getInt32Ty(Ctx), getRegisterID(AI));
  Args.push_back(V);
  Args.push_back(AI);
  Type *ArgsTypes[] = {Type::getInt32Ty(Ctx), Type::getInt32PtrTy(Ctx)};
  FunctionType *FType = FunctionType::get(Type::getVoidTy(Ctx), ArgsTypes, false);
  Value *Fun = M->getOrInsertFunction(DSEAllocaFunctionName, FType);
  if (Function *F = dyn_cast<Function>(Fun)) {
      CallInst *Call = CallInst::Create(Fun, Args, "", AI->getNextNonDebugInstruction());
      Call->setCallingConv(CallingConv::C);
      Call->setTailCall(true);
  } else {
      errs() << "WARN: invalid function\n";
  }
}

void instrumentDSEStore(Module *M, StoreInst *SI) {
    LLVMContext &Ctx = M->getContext();
    std::vector<Value *> Args;
    Args.push_back(SI->getPointerOperand());
    Type *ArgsTypes[] = {Type::getInt32PtrTy(Ctx)};
    FunctionType *FType =
        FunctionType::get(Type::getVoidTy(Ctx), ArgsTypes, false);
    Value *Fun = M->getOrInsertFunction(DSEStoreFunctionName, FType);
    if (Function *F = dyn_cast<Function>(Fun)) {
        CallInst *Call = CallInst::Create(Fun, Args, "", SI);
        Call->setCallingConv(CallingConv::C);
        Call->setTailCall(true);
    } else {
        errs() << "WARN: invalid function\n";
    }
}

void instrumentDSELoad(Module *M, LoadInst *LI) {
  // fill your instrumentation code here
    LLVMContext &Ctx = M->getContext();
    std::vector<Value *> Args;
    Value *V = ConstantInt::get(Type::getInt32Ty(Ctx), getRegisterID(LI));
    Args.push_back(V);
    Args.push_back(LI->getPointerOperand());
    Type *ArgsTypes[] = {Type::getInt32Ty(Ctx),Type::getInt32PtrTy(Ctx)};
    FunctionType *FType =
        FunctionType::get(Type::getVoidTy(Ctx), ArgsTypes, false);
    Value *Fun = M->getOrInsertFunction(DSELoadFunctionName, FType);
    if (Function *F = dyn_cast<Function>(Fun)) {
        CallInst *Call = CallInst::Create(Fun, Args, "", LI);
        Call->setCallingConv(CallingConv::C);
        Call->setTailCall(true);
    } else {
        errs() << "WARN: invalid function\n";
    }

}

void instrumentDSEConst(Module *M, Value *V, Instruction *I) {
  // fill your instrumentation code here
    LLVMContext &Ctx = M->getContext();
    std::vector<Value *> Args;
    Args.push_back(V);
    Type *ArgsTypes[] = {Type::getInt32Ty(Ctx)};
    FunctionType *FType =
        FunctionType::get(Type::getVoidTy(Ctx), ArgsTypes, false);
    Value *Fun = M->getOrInsertFunction(DSEConstFunctionName, FType);
    if (Function *F = dyn_cast<Function>(Fun)) {
        CallInst *Call = CallInst::Create(Fun, Args, "", I);
        Call->setCallingConv(CallingConv::C);
        Call->setTailCall(true);
    } else {
        errs() << "WARN: invalid function\n";
    }
}

void instrumentDSERegister(Module *M, Value *V, Instruction *I) {
  // fill your instrumentation code here
    LLVMContext &Ctx = M->getContext();
    std::vector<Value *> Args;
    Value *V1 = ConstantInt::get(Type::getInt32Ty(Ctx), getRegisterID(V));
    Args.push_back(V1);
    Type *ArgsTypes[] = {Type::getInt32Ty(Ctx)};
    FunctionType *FType =
        FunctionType::get(Type::getVoidTy(Ctx), ArgsTypes, false);
    Value *Fun = M->getOrInsertFunction(DSERegisterFunctionName, FType);
    if (Function *F = dyn_cast<Function>(Fun)) {
        CallInst *Call = CallInst::Create(Fun, Args, "", I);
        Call->setCallingConv(CallingConv::C);
        Call->setTailCall(true);
    } else {
        errs() << "WARN: invalid function\n";
    }

}

void instrumentDSEICmp(Module *M, ICmpInst *I) {
  // fill your instrumentation code here
    LLVMContext &Ctx = M->getContext();
    std::vector<Value *> Args;
    Value *V = ConstantInt::get(Type::getInt32Ty(Ctx), getRegisterID(I));
    Args.push_back(V);
    Value *V1 = ConstantInt::get(Type::getInt32Ty(Ctx), I->getPredicate()); 
    Args.push_back(V1);
    Type *ArgsTypes[] = {Type::getInt32Ty(Ctx),Type::getInt32Ty(Ctx)};
    FunctionType *FType =
        FunctionType::get(Type::getVoidTy(Ctx), ArgsTypes, false);
    Value *Fun = M->getOrInsertFunction(DSEICmpFunctionName, FType);
    if (Function *F = dyn_cast<Function>(Fun)) {
        CallInst *Call = CallInst::Create(Fun, Args, "", I);
        Call->setCallingConv(CallingConv::C);
        Call->setTailCall(true);
    } else {
        errs() << "WARN: invalid function\n";
    }

}

void instrumentDSEBranch(Module *M, BranchInst *BI) {
  // fill your instrumentation code here
    LLVMContext &Ctx = M->getContext();
    std::vector<Value *> Args;
    Value *V = ConstantInt::get(Type::getInt32Ty(Ctx), getBranchID(BI));
    Value *V1 = ConstantInt::get(Type::getInt32Ty(Ctx), getRegisterID(BI->getOperand(0)));
    Args.push_back(V);
    Args.push_back(V1);   
    Args.push_back(BI->getOperand(0));
    Type *ArgsTypes[] = {Type::getInt32Ty(Ctx),Type::getInt32Ty(Ctx),Type::getInt1Ty(Ctx)};
    FunctionType *FType =
        FunctionType::get(Type::getVoidTy(Ctx), ArgsTypes, false);
    Value *Fun = M->getOrInsertFunction(DSEBranchFunctionName, FType);
    if (Function *F = dyn_cast<Function>(Fun)) {
        CallInst *Call = CallInst::Create(Fun, Args, "", BI);
        Call->setCallingConv(CallingConv::C);
        Call->setTailCall(true);
    } else {
        errs() << "WARN: invalid function\n";
    }

}

void instrumentDSEBinOp(Module *M, BinaryOperator *BO) {
  // fill your instrumentation code here
    LLVMContext &Ctx = M->getContext();
    std::vector<Value *> Args;
    Value *V = ConstantInt::get(Type::getInt32Ty(Ctx), getRegisterID(BO));
    Value *V1 = ConstantInt::get(Type::getInt32Ty(Ctx), BO->getOpcode());
    Args.push_back(V);
    Args.push_back(V1);
    Type *ArgsTypes[] = {Type::getInt32Ty(Ctx),Type::getInt32Ty(Ctx)};
    FunctionType *FType =
        FunctionType::get(Type::getVoidTy(Ctx), ArgsTypes, false);
    Value *Fun = M->getOrInsertFunction(DSEBinOpFunctionName, FType);
    if (Function *F = dyn_cast<Function>(Fun)) {
        CallInst *Call = CallInst::Create(Fun, Args, "", BO);
        Call->setCallingConv(CallingConv::C);
        Call->setTailCall(true);
    } else {
        errs() << "WARN: invalid function\n";
    }

}

/*
 * Implement your instrumentation for dynamic symbolic execution engine
 */
bool Instrument::runOnFunction(Function &F) {
  /* Add your code here */
  bool first = true;
  for (BasicBlock &B : F) {
     for (Instruction &I: B) {
	if(PHINode *Ph = dyn_cast<PHINode>(&I)){
	}
	else if (!first){
	}
	else{
		first=false;
		instrumentDSEInit(F.getParent(),F,I);
	}
        if(AllocaInst *AI = dyn_cast<AllocaInst>(&I))
		instrumentDSEAlloca(F.getParent(),AI);

	else if (StoreInst *SI = dyn_cast<StoreInst>(&I)){
		if(isa<Constant>(SI->getOperand(0)))
			instrumentDSEConst(F.getParent(),SI->getOperand(0),SI);
		else
			instrumentDSERegister(F.getParent(),SI->getOperand(0),SI);
		if(isa<Constant>(SI->getOperand(1)))
			instrumentDSEConst(F.getParent(),SI->getOperand(1),SI);

		instrumentDSEStore(F.getParent(),SI);
	}
	else if (LoadInst *LI = dyn_cast<LoadInst>(&I)){
		if(isa<Constant>(LI->getOperand(0)))
                        instrumentDSEConst(F.getParent(),SI->getOperand(0),SI);

		instrumentDSELoad(F.getParent(),LI);	
	}
	else if (BranchInst *BI = dyn_cast<BranchInst>(&I)){
		if(BI->isConditional())
			instrumentDSEBranch(F.getParent(),BI);
	}
	else if (ICmpInst *Cm = dyn_cast<ICmpInst>(&I)){
		if(isa<Constant>(Cm->getOperand(0)))
                       instrumentDSEConst(F.getParent(),Cm->getOperand(0),Cm);
		else
			instrumentDSERegister(F.getParent(),Cm->getOperand(0),Cm);
		if(isa<Constant>(Cm->getOperand(1)))
                       instrumentDSEConst(F.getParent(),Cm->getOperand(1),Cm);
		else
			instrumentDSERegister(F.getParent(),Cm->getOperand(1),Cm);

		instrumentDSEICmp(F.getParent(),Cm);
	
	}
	else if (BinaryOperator *BO = dyn_cast<BinaryOperator>(&I)){
	
                if(isa<Constant>(BO->getOperand(0)))
                        instrumentDSEConst(F.getParent(),BO->getOperand(0),BO);
		else
			instrumentDSERegister(F.getParent(),BO->getOperand(0),BO);
                if(isa<Constant>(BO->getOperand(1)))
                        instrumentDSEConst(F.getParent(),BO->getOperand(1),BO);
		else
			instrumentDSERegister(F.getParent(),BO->getOperand(1),BO);

		instrumentDSEBinOp(F.getParent(),BO);
	}
     }
  }
  return true;
}

char Instrument::ID = 1;
static RegisterPass<Instrument>
    X("Instrument", "Instrumentations for Dynamic Symbolic Execution", false,
      false);

} // namespace instrument
