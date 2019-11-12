
#include <llvm/IR/MEFBody.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRBuilder.h>
#include "llvm/IR/Module.h"

using namespace llvm;
MEFBody::MEFBody(LLVMContext& C, const Twine &N, Module *M)
    :Constant(Type::getLabelTy(C), Constant::MEFBodyVal, nullptr, 0) {
    if (M) {
        M->getMEFBodyList().push_back(this);
        setParent(M);
    }
    pseudoEntryBlock = BasicBlock::CreateMEF(C);
}

void MEFBody::registerEntry(BasicBlock* entry) {
    if (pseudoEntryBlock->getTerminator()) {
        SwitchInst* sw = (SwitchInst *) pseudoEntryBlock->getTerminator();
        sw->addCase(nullptr, entry);
    } else {
        auto builder = IRBuilder<>(pseudoEntryBlock);
        builder.CreateSwitch(nullptr, entry);
    }

}
unsigned MEFBody::getInstructionCount() const {
    unsigned NumInstrs = 0;
    for (const BasicBlock &BB : BasicBlocks)
        NumInstrs += std::distance(BB.instructionsWithoutDebug().begin(),
                                   BB.instructionsWithoutDebug().end());
    return NumInstrs;
}

LLVMContext &MEFBody::getContext() const {
    return getType()->getContext();
}

void MEFBody::dropAllReferences() {
    for (BasicBlock &BB : *this)
        BB.dropAllReferences();

    // Delete all basic blocks. They are now unused, except possibly by
    // blockaddresses, but BasicBlock's destructor takes care of those.
    // :TODO NEED to change this once getParent() has been updated to return MEFBody
    // Either that, or add another path for block deletion
//    while (!BasicBlocks.empty())
//        BasicBlocks.begin()->eraseFromParent();

    // Drop uses of any optional data (real or placeholder).
//    if (getNumOperands()) {
//        User::dropAllReferences();
//        setNumHungOffUseOperands(0);
//        setValueSubclassData(getSubclassDataFromValue() & ~0xe);
//    }

}
MEFBody::~MEFBody() {
    // After this it is safe to delete instructions.
    dropAllReferences();

}

/// Override destroyConstantImpl to make sure it doesn't get called on
/// MEFBody's because they shouldn't be treated like other constants.
void MEFBody::destroyConstantImpl() {
    llvm_unreachable("You can't GV->destroyConstantImpl()!");
}

Value *MEFBody::handleOperandChangeImpl(Value *From, Value *To) {
    llvm_unreachable("Unsupported class for handleOperandChange()!");
}

