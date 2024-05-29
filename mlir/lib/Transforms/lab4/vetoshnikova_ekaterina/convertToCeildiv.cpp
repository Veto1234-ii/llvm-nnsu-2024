#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/Dialect/LLVMIR/LLVMDialect.h"
#include "mlir/IR/PatternMatch.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Tools/Plugins/PassPlugin.h"

using namespace mlir;

namespace {
enum OpType { SignedOp, UnsignedOp };

class VetoshnikovaConvertPass
    : public PassWrapper<VetoshnikovaConvertPass, OperationPass<ModuleOp>> {

public:
  StringRef getArgument() const final { return "vetoshnikova-convert-pass"; }
  StringRef getDescription() const final {
    return "splits the arith.ceildivui and arith.ceildivsi into arith "
           "operations";
  }

  void runOnOperation() override {
    getOperation()->walk([&](Operation *op) {
      if (auto ceilDivUI = dyn_cast<arith::CeilDivUIOp>(op)) {
        replaceCeilDiv(ceilDivUI, OpType::UnsignedOp);
      } else if (auto ceilDivSI = dyn_cast<arith::CeilDivSIOp>(op)) {
        replaceCeilDiv(ceilDivSI, OpType::SignedOp);
      }
    });
  }

private:
  template <typename CeilDivOpType>
  void replaceCeilDiv(CeilDivOpType op, OpType type) {
    OpBuilder builder(op);
    Location loc = op.getLoc();
    Value a = op.getLhs();
    Value b = op.getRhs();

    Value one =
        builder.create<arith::ConstantIntOp>(loc, 1, builder.getI32Type());
    Value add = builder.create<arith::AddIOp>(loc, a, b);
    Value sub = builder.create<arith::SubIOp>(loc, add, one);
    Value div;

    if (type == OpType::SignedOp) {
      div = builder.create<arith::DivSIOp>(loc, sub, b);
    } else {
      div = builder.create<arith::DivUIOp>(loc, sub, b);
    }

    op.replaceAllUsesWith(div);
    op.erase();
  }
};
} // anonymous namespace

MLIR_DECLARE_EXPLICIT_TYPE_ID(VetoshnikovaConvertPass)
MLIR_DEFINE_EXPLICIT_TYPE_ID(VetoshnikovaConvertPass)

PassPluginLibraryInfo getPluginInfo() {
  return {MLIR_PLUGIN_API_VERSION, "vetoshnikova-convert-pass", LLVM_VERSION_STRING,
          []() { PassRegistration<VetoshnikovaConvertPass>(); }};
}

extern "C" LLVM_ATTRIBUTE_WEAK PassPluginLibraryInfo mlirGetPassPluginInfo() {
  return getPluginInfo();
}
