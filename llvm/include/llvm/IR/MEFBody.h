//
// Created by advait on 10/24/19.
//

#ifndef LLVM_MEFBODY_H
#define LLVM_MEFBODY_H

#include "llvm/ADT/Twine.h"
#include "llvm/ADT/ilist_node.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/SymbolTableListTraits.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/Compiler.h"
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

namespace llvm {

class Constant;
class LLVMContext;

class MEFBody: public Constant, public ilist_node<MEFBody> {
public:
    using BasicBlockListType = SymbolTableList<BasicBlock>;
    // BasicBlock iterators...
    using iterator = BasicBlockListType::iterator;
    using const_iterator = BasicBlockListType::const_iterator;


    MEFBody(LLVMContext& C, const Twine &N = "", Module *M = nullptr);
    MEFBody(MEFBody& x) = delete;
    MEFBody operator=(MEFBody& other) = delete;
    ~MEFBody();
private:
    BasicBlockListType BasicBlocks;         ///< The basic blocks

    friend class SymbolTableListTraits<MEFBody>;
protected:
    Module *Parent;             // The containing module.

    // Used by SymbolTableListTraits.
    void setParent(Module *parent) {
        Parent = parent;
    }

public:
    static MEFBody *Create(LLVMContext& C, const Twine &N = "", Module *M = nullptr) {
        return new MEFBody(C, N, M);
    }

    /// Get the module that this global value is contained inside of...
    Module *getParent() { return Parent; }
    const Module *getParent() const { return Parent; }

    /// Get the underlying elements of the Function... the basic block list is
    /// empty for external functions.
    ///
    const BasicBlockListType &getBasicBlockList() const { return BasicBlocks; }
    BasicBlockListType &getBasicBlockList()       { return BasicBlocks; }

    static BasicBlockListType MEFBody::*getSublistAccess(BasicBlock*) {
        return &MEFBody::BasicBlocks;
    }

    //===--------------------------------------------------------------------===//
    // BasicBlock iterator forwarding functions
    //
    iterator                begin()       { return BasicBlocks.begin(); }
    const_iterator          begin() const { return BasicBlocks.begin(); }
    iterator                end  ()       { return BasicBlocks.end();   }
    const_iterator          end  () const { return BasicBlocks.end();   }

    size_t                   size() const { return BasicBlocks.size();  }
    bool                    empty() const { return BasicBlocks.empty(); }
    const BasicBlock       &front() const { return BasicBlocks.front(); }
    BasicBlock       &front()       { return BasicBlocks.front(); }
    const BasicBlock        &back() const { return BasicBlocks.back();  }
    BasicBlock        &back()       { return BasicBlocks.back();  }


    //===--------------------------------------------------------------------===//
    // Virtual Method implementations from Constant super class
    //
    /// Override destroyConstantImpl to make sure it doesn't get called on
    /// MEFBody's because they shouldn't be treated like other constants.
    void destroyConstantImpl();
    Value *handleOperandChangeImpl(Value *From, Value *To);

    //===--------------------------------------------------------------------===//
    // Methods that participate in this object's destruction
    //
    bool hasGC() const {
        return false;
    }
    void clearGC();
    void dropAllReferences();
    LLVMContext& getContext() const;

};
}
#endif //LLVM_MEFBODY_H
