#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "llvm/Support/raw_ostream.h"

class Visitor : public clang::RecursiveASTVisitor<Visitor> {
public:
  explicit Visitor(clang::ASTContext *Context) : Context(Context) {}
  bool VisitCXXRecordDecl(clang::CXXRecordDecl *Declaration) {
    llvm::outs() << Declaration->getNameAsString() << "\n";
    for (const auto &field : Declaration->fields()) {
      llvm::outs() << " |_" << field->getNameAsString() << "\n";
    }
    return true;
  }

private:
  clang::ASTContext *Context;
};

class Consumer : public clang::ASTConsumer {
public:
  explicit Consumer(clang::ASTContext *Context) : vis(Context) {}

  virtual void HandleTranslationUnit(clang::ASTContext &Context) {

    // Traversing the translation unit decl via a RecursiveASTVisitor
    // will visit all nodes in the AST.

    vis.TraverseDecl(Context.getTranslationUnitDecl());
  }

private:
  Visitor vis;
};

class Plugin : public clang::PluginASTAction {
public:
    virtual std::unique_ptr<clang::ASTConsumer>
    CreateASTConsumer(clang::CompilerInstance &Compiler, 
        llvm::StringRef InFile) {
    return std::unique_ptr<clang::ASTConsumer>(
        new Consumer(&Compiler.getASTContext()));
  }

  //Create the AST consumer object for this action, if supported.
  // Parameters
  //CI - The current compiler instance
  //InFile - The current input file
  //Returns The new AST consumer, or null on failure.

  bool ParseArgs(const clang::CompilerInstance &CI,
                 const std::vector<std::string> &args) {
    if (args.size() && args[0] == "help")
      PrintHelp(llvm::errs());
    return true;
    }
  void PrintHelp(llvm::raw_ostream &ros) {
    ros << "This plugin prints the names of all classes and their fields\n";
  }
};

static clang::FrontendPluginRegistry::Add<Plugin>
    X("vetoshnikova-plugin-print-classes",
      "Printing the names of all classes and their fields");