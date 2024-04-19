#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

namespace {

struct InstrumentFunctions : llvm::PassInfoMixin<InstrumentFunctions> {

  llvm::PreservedAnalyses run(llvm::Function &F,
                              llvm::FunctionAnalysisManager &AM) {

    llvm::LLVMContext &context = F.getContext();

    llvm::IRBuilder<> builder(context);

    auto module = F.getParent();

    llvm::FunctionType *type =
        llvm::FunctionType::get(llvm::Type::getVoidTy(context), false);

    llvm::FunctionCallee instrumentStart =
        module->getOrInsertFunction("instrument_start", type);

    llvm::FunctionCallee instrumentEnd =
        module->getOrInsertFunction("instrument_end", type);

    builder.SetInsertPoint(&F.getEntryBlock().front());

    builder.CreateCall(instrumentStart);

    for (llvm::BasicBlock &b : F) {

      if (auto *reInst = llvm::dyn_cast<llvm::ReturnInst>(b.getTerminator())) {
        builder.SetInsertPoint(b.getTerminator());
        builder.CreateCall(instrumentEnd);
      }
    }

    return llvm::PreservedAnalyses::all();
  }

  static bool isRequired() { return true; }
};
} // namespace

extern "C" LLVM_ATTRIBUTE_WEAK::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "instrument_functions", "0.1",
          [](llvm::PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](llvm::StringRef name, llvm::FunctionPassManager &FPM,
                   llvm::ArrayRef<llvm::PassBuilder::PipelineElement>) -> bool {
                  if (name == "instrument_functions") {
                    FPM.addPass(InstrumentationPass{});
                    return true;
                  }
                  return false;
                });
          }};
}
