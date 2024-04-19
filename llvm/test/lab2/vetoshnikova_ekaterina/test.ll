; RUN: opt -load-pass-plugin %llvmshlibdir/VetoshnikovaEInstrumentFunctions%pluginext\
; RUN: -passes=instrument_functions -S %s | FileCheck %s

; CHECK-LABEL: @func
; CHECK: call void @instrument_start()
; CHECK-NEXT: call void @instrument_end()
; CHECK-NEXT: ret void

define dso_local void @func() {
entry:
  ret void
}
