#ifndef CODEGEN_H
#define CODEGEN_H
#include "./parser.hpp"

class Generator {
public:
  inline explicit Generator(NodeProgram root) : m_root(std::move(root)) {}

  [[nodiscard]] inline std::string generate() const {
    std::stringstream output;
    output << m_start;

    // allowing only 1 exit, the first one
    bool has_exit = false;
    bool uses_print_int = false;

    for (const NodeStatement &stmt : m_root.statements) {
      if (std::holds_alternative<NodeExit>(stmt.var)) {
        has_exit = true;
      }

      if (std::holds_alternative<NodePrintf>(stmt.var)) {
        uses_printf_int = true;
      }

      output << generate_statements(stmt);
    }

    if (!has_exit) {
      output << generate_default_exit();
    }

    if (uses_print) {
      output << m_data;
      output << "		buffer: .space 12\n";
      output << "		newline: .asciiz \"\n\"\n";
    }

    return output.str();
  }

private:
  const NodeProgram m_root{};
  const std::string m_start = ".text\n.globl __start\n__start:\n";
  const std::string m_syscall = "syscall\n";
  const std::string m_data = ".data\n";
  const std::string m_newline = "\n";

  int m_label_count = 0;
  int m_var_count = 0;

  [[nodiscard]] inline std::string generate_exit(const NodeExit &node) const {
    std::stringstream output;

    // login the code for the exit
    output << "   li $v0, 4001\n";
    output << "   li $a0, " << node.expr.int_lit.value.value() << "\n";
    output << "   " << m_syscall << "\n";

    return output.str();
  }

  [[nodiscard]] inline std::string
  generate_statements(const NodeStatement &stmt) const {
    std::stringstream output;

    struct visit_statement {
      const Generator *gen;
      std::stringstream &output;

      void operator()(const NodeExit &node) const {
        output << gen->generate_exit(node);
      }

      void operator()(const NodePrintf &node) const {
        output << gen->generate_print_int_stmt(node);
      }

      // TODO: Create if statements and more
    };

    std::visit(visit_statement{this, output}, stmt.var);
    return output.str();
  }

  [[nodiscard]] inline std::string generate_default_exit() const {
    std::stringstream output;

    output << "   li $v0, 4001\n"; // exit sys
    output << "   li $a0, 0\n";
    output << "   " << m_syscall << "\n";

    return output.str();
  }

  [[nodiscard]] inline std::string
  generate_print_int_stmt(const NodePrintf &node) const {
    std::stringstream output;
    output << "print_int:\n";
    output << "   addi $sp, $sp, -4\n";
    output << "   sw   $ra, 0($sp)\n";
    output << "\n";
    output << "   move $t0, $a0\n";
    output << "   la   $t1, buffer + 11\n";
    output << "   li   $t2, 10\n";
    output << "   li   $t3, 0\n";
    output << "\n";
    output << "convert_loop:\n";
    output << "   div  $t0, $t2\n";
    output << "   mfhi $t4\n";
    output << "   mflo $t0\n";
    output << "   addi $t4, $t4, '0'\n";
    output << "   sb   $t4, 0($t1)\n";
    output << "   addi $t1, $t1, -1\n";
    output << "   addi $t3, $t3, 1\n";
    output << "   bnez $t0, convert_loop\n";
    output << "\n";
    output << "   addi $t1, $t1, 1\n";
    output << "\n";
    output << "   li   $v0, 4004\n";
    output << "   li   $a0, 1\n";
    output << "   move $a1, $t1\n";
    output << "   move $a2, $t3\n";
    output << "   " << m_syscall << "\n";
    output << "\n";
    output << "   li   $v0, 4004\n";
    output << "   li   $a0, 1\n";
    output << "   la   $a1, newline\n";
    output << "   li   $a2, 1\n";
    output << "   " << m_syscall << "\n";
    output << "\n";
    output << "   lw   $ra, 0($sp)\n";
    output << "   addi $sp, $sp, 4\n";
    output << "   jr   $ra\n";
    return output.str();
  }
};

#endif // CODEGEN_H
