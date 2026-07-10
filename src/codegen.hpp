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
      // TODO: generate all the statements
    }

    return output.str();
  }

  [[nodiscard]] inline std::string generate_exit() const {
    std::stringstream output;
    output << m_start; // start part

    // login the code for the exit
    output << "   li $v0, 4001\n";
    // output << "   li $a0, " << m_root.expr.int_lit.value.value() << "\n";
    output << "   " << m_syscall << "\n";

    return output.str();
  }

private:
  const NodeProgram m_root{};
  const std::string m_start = ".text\n.globl __start\n__start:\n";
  const std::string m_syscall = "syscall\n";
  int  m_label_count = 0;
  int  m_var_count = 0;
};

#endif // CODEGEN_H
