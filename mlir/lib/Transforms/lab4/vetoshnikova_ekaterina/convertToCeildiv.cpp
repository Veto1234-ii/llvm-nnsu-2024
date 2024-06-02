#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/Dialect/LLVMIR/LLVMDialect.h"
#include "mlir/IR/PatternMatch.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Tools/Plugins/PassPlugin.h"

using namespace mlir;

namespace {

class VetoshnikovaConvertPass
    : public PassWrapper<VetoshnikovaConvertPass,
                         OperationPass<LLVM::LLVMFuncOp>> {

public:
  StringRef getArgument() const final { return "vetoshnikova-convert-pass"; }
  StringRef getDescription() const final {
    return "pass that breaks arith.ceildivui and arith.ceildivsi operations "
           "into calculation following the rule: ceildiv(a, b) = (a + b - 1) / "
           "b ";
  }

  void runOnOperation() override {
    getOperation()->walk([&](Operation *op) {
      if (isa<arith::CeilDivUIOp, arith::CeilDivSIOp>(op)) {
        replaceCeildiv(op);
      }
    });
  }

private:
  void replaceCeildiv(Operation *op) {
    OpBuilder builder(op);
    Location loc = op->getLoc();
    Value a = op->getOperand(0);
    Value b = op->getOperand(1);

    Value one = builder.create<arith::ConstantIntOp>(loc, 1, a.getType());
    Value add = builder.create<arith::AddIOp>(loc, a, b);
    Value sub = builder.create<arith::SubIOp>(loc, add, one);
    Value div;

    if (isa<arith::CeilDivSIOp>(op)) {
      div = builder.create<arith::DivSIOp>(loc, sub, b);
    } else {
      div = builder.create<arith::DivUIOp>(loc, sub, b);
    }

    op->replaceAllUsesWith(ValueRange(div));
    op->erase();
  }
};
} // anonymous namespace

MLIR_DECLARE_EXPLICIT_TYPE_ID(VetoshnikovaConvertPass)
MLIR_DEFINE_EXPLICIT_TYPE_ID(VetoshnikovaConvertPass)

PassPluginLibraryInfo getPluginInfo() {
  return {MLIR_PLUGIN_API_VERSION, "vetoshnikova-convert-pass",
          LLVM_VERSION_STRING,
          []() { PassRegistration<VetoshnikovaConvertPass>(); }};
}

extern "C" LLVM_ATTRIBUTE_WEAK PassPluginLibraryInfo mlirGetPassPluginInfo() {
  return getPluginInfo();
}
