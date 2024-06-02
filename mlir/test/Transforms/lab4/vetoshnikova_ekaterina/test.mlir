// RUN: mlir-opt -load-pass-plugin=%mlir_lib_dir/VetoshnikovaConvertPlugin%shlibext --pass-pipeline="builtin.module(llvm.func(vetoshnikova-convert-pass))" %s | FileCheck %s

module {
llvm.func @testceildivui(%arg0: i32, %arg1: i32) -> i32 attributes {llvm.linkage = #llvm.linkage<external>} {
    // CHECK: llvm.func @testceildivui(%arg0: i32, %arg1: i32) -> i32 attributes {llvm.linkage = #llvm.linkage<external>} {
    // CHECK-NEXT: %c1_i32 = arith.constant 1 : i32
    // CHECK-NEXT: %0 = arith.addi %arg0, %arg1 : i32
    // CHECK-NEXT: %1 = arith.subi %0, %c1_i32 : i32
    // CHECK-NEXT: %2 = arith.divui %1, %arg1 : i32
    // CHECK-NOT: %0 = arith.ceildivui %arg0, %arg1 : i32
    %0 = arith.ceildivui %arg0, %arg1 : i32
    llvm.return %0 : i32
}
llvm.func @testceildivsi(%arg0: i32, %arg1: i32) -> i32 attributes {llvm.linkage = #llvm.linkage<external>} {
    // CHECK: llvm.func @testceildivsi(%arg0: i32, %arg1: i32) -> i32 attributes {llvm.linkage = #llvm.linkage<external>} {
    // CHECK-NEXT: %c1_i32 = arith.constant 1 : i32
    // CHECK-NEXT: %0 = arith.addi %arg0, %arg1 : i32
    // CHECK-NEXT: %1 = arith.subi %0, %c1_i32 : i32
    // CHECK-NEXT: %2 = arith.divsi %1, %arg1 : i32
    // CHECK-NOT: %0 = arith.ceildivsi %arg0, %arg1 : i32
    %0 = arith.ceildivsi %arg0, %arg1 : i32
    llvm.return %0 : i32
}
llvm.func @testceildivui64(%arg0: i64, %arg1: i64) -> i64 attributes {llvm.linkage = #llvm.linkage<external>} {
    // CHECK: llvm.func @testceildivui64(%arg0: i64, %arg1: i64) -> i64 attributes {llvm.linkage = #llvm.linkage<external>} {
    // CHECK-NEXT: %c1_i64 = arith.constant 1 : i64
    // CHECK-NEXT: %0 = arith.addi %arg0, %arg1 : i64
    // CHECK-NEXT: %1 = arith.subi %0, %c1_i64 : i64
    // CHECK-NEXT: %2 = arith.divui %1, %arg1 : i64
    // CHECK-NOT: %0 = arith.ceildivui %arg0, %arg1 : i64
    %0 = arith.ceildivui %arg0, %arg1 : i64
    llvm.return %0 : i64
}
llvm.func @testceildivsi64(%arg0: i64, %arg1: i64) -> i64 attributes {llvm.linkage = #llvm.linkage<external>} {
    // CHECK: llvm.func @testceildivsi64(%arg0: i64, %arg1: i64) -> i64 attributes {llvm.linkage = #llvm.linkage<external>} {
    // CHECK-NEXT: %c1_i64 = arith.constant 1 : i64
    // CHECK-NEXT: %0 = arith.addi %arg0, %arg1 : i64
    // CHECK-NEXT: %1 = arith.subi %0, %c1_i64 : i64
    // CHECK-NEXT: %2 = arith.divsi %1, %arg1 : i64
    // CHECK-NOT: %0 = arith.ceildivsi %arg0, %arg1 : i64
    %0 = arith.ceildivsi %arg0, %arg1 : i64
    llvm.return %0 : i64
}
llvm.func @testceildivui8(%arg0: i8, %arg1: i8) -> i8 attributes {llvm.linkage = #llvm.linkage<external>} {
    // CHECK: llvm.func @testceildivui8(%arg0: i8, %arg1: i8) -> i8 attributes {llvm.linkage = #llvm.linkage<external>} {
    // CHECK-NEXT: %c1_i8 = arith.constant 1 : i8
    // CHECK-NEXT: %0 = arith.addi %arg0, %arg1 : i8
    // CHECK-NEXT: %1 = arith.subi %0, %c1_i8 : i8
    // CHECK-NEXT: %2 = arith.divui %1, %arg1 : i8
    // CHECK-NOT: %0 = arith.ceildivui %arg0, %arg1 : i8
    %0 = arith.ceildivui %arg0, %arg1 : i8
    llvm.return %0 : i8
}
llvm.func @testceildivsi8(%arg0: i8, %arg1: i8) -> i8 attributes {llvm.linkage = #llvm.linkage<external>} {
    // CHECK: llvm.func @testceildivsi8(%arg0: i8, %arg1: i8) -> i8 attributes {llvm.linkage = #llvm.linkage<external>} {
    // CHECK-NEXT: %c1_i8 = arith.constant 1 : i8
    // CHECK-NEXT: %0 = arith.addi %arg0, %arg1 : i8
    // CHECK-NEXT: %1 = arith.subi %0, %c1_i8 : i8
    // CHECK-NEXT: %2 = arith.divsi %1, %arg1 : i8
    // CHECK-NOT: %0 = arith.ceildivsi %arg0, %arg1 : i8
    %0 = arith.ceildivsi %arg0, %arg1 : i8
    llvm.return %0 : i8
}
}