//
// Created by advait on 10/29/19.
//

#include "llvm/IR/MEFEntry.h"
#include "llvm/IR/OperandTraits.h"
#include "llvm/IR/Module.h"


class User;
using namespace llvm;

MEFEntry::MEFEntry(FunctionType *Ty, const Twine &name, Module *ParentModule)
        : GlobalObject(Ty, Value::MEFEntryVal,
                OperandTraits<MEFEntry>::op_begin(this), 0,
                LinkageTypes::ExternalLinkage, name),
          NumArgs(Ty->getNumParams()) {
    assert(FunctionType::isValidReturnType(getReturnType()) &&
           "invalid return type");

    // ??
    setGlobalObjectSubClassData(0);

//    // We only need a symbol table for a function if the context keeps value names
//    if (!getContext().shouldDiscardValueNames())
//        SymTab = make_unique<ValueSymbolTable>();
//
    BuildArguments();
    if (ParentModule)
        ParentModule->getMEFEntryList().push_back(this);
}

//===----------------------------------------------------------------------===//
// Argument Implementation
//===----------------------------------------------------------------------===//

//Argument::Argument(Type *Ty, const Twine &Name, unsigned ArgNo)
//        : Value(Ty, Value::ArgumentVal), Parent(nullptr), ArgNo(ArgNo) {
//    setName(Name);
//}

void MEFEntry::Register(BasicBlock *block) {
    entryBlock = block;
    entryBlock->getParentMEF()->RegisterEntry(this);
}
void MEFEntry::BuildArguments() const {
    // Create the arguments vector, all arguments start out unnamed.
    auto *FT = getFunctionType();
    if (NumArgs > 0) {
        Arguments = std::allocator<Argument>().allocate(NumArgs);
        for (unsigned i = 0, e = NumArgs; i != e; ++i) {
            Type *ArgTy = FT->getParamType(i);
            assert(!ArgTy->isVoidTy() && "Cannot have void typed arguments!");
            new (Arguments + i) Argument(ArgTy, "", nullptr, i);
        }
    }

    // Clear the lazy arguments bit.
//    unsigned SDC = getSubclassDataFromValue();
//    const_cast<Function*>(this)->setValueSubclassData(SDC &= ~(1<<0));
//    assert(!hasLazyArguments());
}

/// Override destroyConstantImpl to make sure it doesn't get called on
/// MEFBody's because they shouldn't be treated like other constants.
//void MEFEntry::destroyConstantImpl() {
//    llvm_unreachable("You can't GV->destroyConstantImpl()!");
//}
//
//Value *MEFEntry::handleOperandChangeImpl(Value *From, Value *To) {
//    llvm_unreachable("Unsupported class for handleOperandChange()!");
//}


