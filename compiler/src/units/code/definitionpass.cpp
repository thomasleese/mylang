#include "units/code.h"

using namespace Code;

DefinitionPass::DefinitionPass(Generator *gen) :
	Pass(gen) {
	
}

void DefinitionPass::generateCode() {
	
}

/*
	llvm::BasicBlock *entry = llvm::BasicBlock::Create(llvm::getGlobalContext(), "entry", func);

	if (decl->getName()->getValue() == "main") {
		this->builder->SetInsertPoint(entry);
	}
*/
