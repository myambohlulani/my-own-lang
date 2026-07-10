#ifndef CODEGEN_H
#define CODEGEN_H
#include "./parser.hpp"

class Generator {
public:
  inline explicit Generator(NodeProgram root) : m_root(std::move(root)) {}

  [[nodiscard]] inline std::string generate() const {
    std::stringstream output;
    output << m_start;

    for (const NodeStatement& stmt : m_root.statements) {
      //TODO: Generate all the statements
      //output << generate_statements(stmt);
    }

    // TODO: add a generate default_exit method, output << generate_default_exit();
    return output.str();
  }

private:
  const NodeProgram m_root{};
  const std::string m_start = ".text\n.globl __start\n__start:\n";
  const std::string m_syscall = "syscall\n";
  int  m_label_count = 0;
  int  m_var_count = 0;

  [[nodiscard]] inline std::string generate_exit(const NodeExit& node) const {
    std::stringstream output;
    output << m_start; // start part

    // login the code for the exit
    output << "   li $v0, 4001\n";
    output << "   li $a0, " << node.expr.int_lit.value.value() << "\n";
    output << "   " << m_syscall << "\n";

    return output.str();
  }

  [[nodiscard]] inline std::string generate_print_int_stmt(const NodePrintf& node) const {
    std::stringstream output;
    output << "   li $v0, 4004\n";
    output << "   li $a0, 1\n"; // file descriptor stdout
    output << "   li $a1, " << node.expr.int_lit.value.value() << "\n" ; // TODO: print the integer only
    output << "   li $a2, 8\n"; // byte for the number
    output << "   " << m_syscall << "\n";
    return output.str();
  }

};

#endif // CODEGEN_H
