#include "X86.h"
#include "X86InstrInfo.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/Register.h"
#include <utility>
#include <vector>

using namespace llvm;

namespace {
class X86PassVetoshnikova : public MachineFunctionPass {
public:
  static char ID;

  X86PassVetoshnikova() : MachineFunctionPass(ID) {}

bool X86PassVetoshnikova::runOnMachineFunction(MachineFunction &Mfunc) {

    const TargetInstrInfo *info = Mfunc.getSubtarget().getInstrInfo();
    bool ismodified = false;
    std::vector<std::pair<MachineInstr *, MachineInstr *>> replace;

    for (auto &basicblock : Mfunc) {

      MachineInstr *mulinstr = nullptr;
      MachineInstr *addinstr = nullptr;

      for (auto instruction = basicblock.begin();
           instruction != basicblock.end(); ++instruction) {

        if (instruction->getOpcode() == X86::MULPDrr ||
            instruction->getOpcode() == X86::MULPDrm) {

          mulinstr = &instruction;

          for (auto next = instruction; next != basicblock.end(); ++next) {

            if (next->getOpcode() == X86::ADDPDrr) {

              addinstr = &*next;

              if (mulinstr->getOperand(0).getReg() ==
                  addinstr->getOperand(1).getReg()) {

                replace.emplace_back(mulinstr, addinstr);

                ismodified = true;

                break;
              }
            } else if (next->definesRegister(
                           mulinstr->getOperand(0).getReg())) {
              break;
            }
          }
        }
      }
    }

    for (const auto &[mul, add] : replace) {

      MachineInstrBuilder builder =
          BuildMI(*mul->getParent(), *mul, mul->getDebugLoc(),
                  info->get(X86::VFMADD213PDZ128r));

      builder.addReg(add->getOperand(0).getReg(), RegState::Define);
      builder.addReg(mul->getOperand(1).getReg());
      builder.addReg(mul->getOperand(2).getReg());
      builder.addReg(add->getOperand(2).getReg());

      mul->eraseFromParent();
      add->eraseFromParent();
    }

    return ismodified;
  }
};
} // namespace

char X86PassVetoshnikova::ID = 0;

static RegisterPass<X86BendPass> X("x86-pass-vetoshnikova",
                                   "X86 Pass Vetoshnikova", false, false);
