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
      output << generate_statements(stmt);
    }

    // TODO: add a generate default_exit method,
    output << generate_default_exit();
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

    // login the code for the exit
    output << "   li $v0, 4001\n";
    output << "   li $a0, " << node.expr.int_lit.value.value() << "\n";
    output << "   " << m_syscall << "\n";

    return output.str();
  }

  [[nodiscard]] inline std::string generate_statements(const NodeStatement& stmt) const {
    std::stringstream output;

    struct visit_statement {
      const Generator* gen;
      std::stringstream& output;

      void operator()(const NodeExit& node) const {
        output << gen -> generate_exit(node);
      }

      void operator()(const NodePrintf& node) const {
        output << gen -> generate_print_int_stmt(node);
      }

      // TODO: Create if statements and more
    };

    std::visit(visit_statement{this, output}, stmt.var);
    return output.str();
  }

  [[nodiscard]] inline std::string generate_default_exit() const {
    std::stringstream output;

    output << "   li $v0, 4001\n"; // exit sys
    output << "   li $a0, 1\n";
    output << "   li $a1, 0\n";
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
