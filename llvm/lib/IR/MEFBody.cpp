
#include <llvm/IR/MEFBody.h>
#include "llvm/IR/Module.h"

using namespace llvm;
MEFBody::MEFBody(const Twine &N, Module *M)
    :Constant((Type *) Type::VoidTyID, Constant::MEFBodyVal, nullptr, 0) {
    if (M) {
        M->getMEFBodyList().push_back(this);
        setParent(M);
    }

}

/// Override destroyConstantImpl to make sure it doesn't get called on
/// MEFBody's because they shouldn't be treated like other constants.
void MEFBody::destroyConstantImpl() {
    llvm_unreachable("You can't GV->destroyConstantImpl()!");
}


Value *MEFBody::handleOperandChangeImpl(Value *From, Value *To) {
    llvm_unreachable("Unsupported class for handleOperandChange()!");
}

