//
// Created by advait on 10/29/19.
//

#ifndef LLVM_MEFENTRY_H
#define LLVM_MEFENTRY_H

#include "GlobalObject.h"
#include "SymbolTableListTraits.h"
#include "llvm/IR/OperandTraits.h"
#include "BasicBlock.h"
#include "llvm/IR/Argument.h"

namespace llvm {

class MEFEntry : public GlobalObject, public ilist_node<MEFEntry> {
public:
    using arg_iterator = Argument *;
    using const_arg_iterator = const Argument *;

private:
    friend class SymbolTableListTraits<MEFEntry>;
    mutable Argument *Arguments = nullptr;  ///< The formal arguments
    size_t NumArgs;
    BasicBlock *entryBlock;
//    std::unique_ptr<ValueSymbolTable> SymTab;  ///< Symbol table of args/instructions

    //
    // Following methods Participate in destruction on Entry object
    //
//    void clearArguments();
    MEFEntry(FunctionType *Ty, const llvm::Twine &N, llvm::Module *M);
    void BuildArguments() const;
public:

    MEFEntry(const MEFEntry&) = delete;
    void operator=(const MEFEntry&) = delete;
    ~MEFEntry() = default;

    static MEFEntry *Create(FunctionType *Ty, const Twine &N = "", Module *M = nullptr) {
        return new MEFEntry(Ty, N, M);
    }

    // Provide fast operand accessors.
    DECLARE_TRANSPARENT_OPERAND_ACCESSORS(Value);

    /// Returns the FunctionType for me.
    FunctionType *getFunctionType() const {
        return cast<FunctionType>(getValueType());
    }

    /// Returns the type of the ret val.
    Type *getReturnType() const { return getFunctionType()->getReturnType(); }

    static bool classof(const Value *V) {
        return V->getValueID() == Value::MEFEntryVal;
    }

    void Register(BasicBlock *block) {
        entryBlock = block;
        entryBlock->getParentMEF()->RegisterEntry(entryBlock);
    }

    const BasicBlock       &getEntryBlock() const   { return *entryBlock; }
    BasicBlock             &getEntryBlock()         { return *entryBlock; }

/// @name Function Argument Iteration
/// @{

        arg_iterator arg_begin() {
            return Arguments;
        }
        const_arg_iterator arg_begin() const {
            return Arguments;
        }

        arg_iterator arg_end() {
            return Arguments + NumArgs;
        }
        const_arg_iterator arg_end() const {
            return Arguments + NumArgs;
        }

        iterator_range<arg_iterator> args() {
            return make_range(arg_begin(), arg_end());
        }
        iterator_range<const_arg_iterator> args() const {
            return make_range(arg_begin(), arg_end());
        }

/// @}

        size_t arg_size() const { return NumArgs; }
        bool arg_empty() const { return arg_size() == 0; }

};

template <>
struct OperandTraits<MEFEntry> : public HungoffOperandTraits<3> {};

DEFINE_TRANSPARENT_OPERAND_ACCESSORS(MEFEntry, Value)

}
#endif
