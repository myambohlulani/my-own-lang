//
// Created by HLULANI on 2026/07/13.
// This header is similar to Codegen for mips, the only thing different is the registers and usage of stack instead of converting stuff
//

#ifndef X86_CODEGEN_H
#define X86_CODEGEN_H
#include "./parser.hpp"
#include "./lexer.hpp"
#include <sstream>

class CodeGenerator {
public:
  inline explicit CodeGenerator(NodeProgram root) : m_root(std::move(root)) {}

  [[nodiscard]] inline std::string generate() const {
    std::stringstream output;
    // Generating the first part of the code
    output <<  m_global;
    // for exit
    bool has_exit = false;

    for (const NodeStatement& stmt: m_root.statements) {
      if (std::holds_alternative<NodeExit>(stmt.var)) {
        has_exit = true;
      }

     output << generate_statements(stmt);
    }

    if (!has_exit) {
      output << generate_default_exit();
    }

    return output.str();
  }

private:
  NodeProgram m_root{};
  std::string m_int80h = "    int 80h\n";
  std::string m_syscall = "   syscall\n";
  std::string m_global = "    global _start\n_start:\n";

  [[nodiscard]] inline std::string generate_exit(const NodeExit& node) const {
    std::stringstream output;

    output << m_int80h << std::endl;
    output << m_syscall;
    return output.str();
  }

  /**
   * This method generates the default exit status with exit code of 0
   */
  [[nodiscard]] inline std::string generate_default_exit() const {
    std::stringstream output;
    output << "   move eax, 60\n";
    output << "   move rdi, 0\n";
    output << m_int80h;
    output << m_syscall;
    return output.str();
  }

  [[nodiscard]] inline std::string generate_statements(const NodeStatement& stmt) const {
    std::stringstream output;

    struct visit_statement {
      const CodeGenerator* gen;
      std::stringstream& output;

      void operator()(const NodeExit& node) const {
        output << gen -> generate_exit(node);
      }

      void operator()(const NodePrintf& node) const {
        output << gen -> generate_printf(node);
      }

      void operator()(const NodeVarDeclar& node) const {
        output << gen -> generate_variable_declar(node);
      }
    };

    std::visit(visit_statement{this, output}, stmt.var);

    return output.str();
  }

  /**
   * This method is responsible for variable declaration in assembly
   */
  [[nodiscard]] inline std::string generate_variable_declar(const NodeVarDeclar &node) const {
    std::stringstream output;
    output << "variable declaration"; // TODO: Implement variable declaration
    return output.str();
  }

  /**
   *This method is responsible for printing stuff using printf
   */
  [[nodiscard]] inline std::string generate_printf(const NodePrintf &node) const {
    std::stringstream output;
    //TODO: Implement generate printf
    return output.str();
  }

  [[nodiscard]] inline std::string generate_print_int() const {
    std::stringstream output;

    return output.str();
  }
};

#endif //X86_CODEGEN_H
