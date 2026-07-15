//
// Created by HLULANI on 2026/07/13.
// This header is similar to Codegen for mips, the only thing different is the
// registers and usage of stack instead of converting stuff
//

#ifndef X86_CODEGEN_H
#define X86_CODEGEN_H
#include "./lexer.hpp"
#include "./parser.hpp"
#include <sstream>

// macros for the constants
#define DATA_SECTION "\nsection .data\n"
#define GLOBAL_SECTION "\nsection .text\nglobal _start\n_start:\n";
#define EXIT_STATUS_CODE "   mov rax, 60\n"
#define SUCCESS_STATUS_CODE "   mov rdi, 0\n"
#define EXTERNAL_C_FUNCTIONS "extern printf\nextern exit\n"
#define SYSTEM_CALL_ "   syscall\n\n"

// responsible for pushing and popping from the stack
#define STACK_POP "   pop"
#define STACK_PUSH "  push"

class CodeGenerator {
public:
  inline explicit CodeGenerator(NodeProgram root) : m_root(std::move(root)) {}

  [[nodiscard]] inline std::string generate() const {
    std::stringstream output;
    // Generating the first part of the code
    output << EXTERNAL_C_FUNCTIONS;
    output << GLOBAL_SECTION;
    // for exit
    bool has_exit = false;

    for (const NodeStatement &stmt : m_root.statements) {
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

  [[nodiscard]] inline static std::string generate_exit(const NodeExit &node) {
    /**
     * This method generates the exit code for the file
     */
    std::stringstream output;
    const auto &exit_stmt = std::get<NodeLiteral>(node.expr.value);
    output << EXIT_STATUS_CODE;

    if (exit_stmt.int_lit.value.has_value())
      output << "   mov rdi, " << exit_stmt.int_lit.value.value() << "\n";
    else {
      output << SUCCESS_STATUS_CODE;
    }

    output << SYSTEM_CALL_;
    return output.str();
  }

  /**
   * This method generates the default exit status with exit code of 0
   */
  [[nodiscard]] inline std::string generate_default_exit() const {
    std::stringstream output;
    output << EXIT_STATUS_CODE;
    output << SUCCESS_STATUS_CODE;
    output << SYSTEM_CALL_;
    return output.str();
  }

  [[nodiscard]] inline std::string
  generate_statements(const NodeStatement &stmt) const {
    /**
     * This method generates stataments into x86
     */
    std::stringstream output;

    struct visit_statement {
      const CodeGenerator *gen;
      std::stringstream &output;

      void operator()(const NodeExit &node) const {
        output << gen->generate_exit(node);
      }

      void operator()(const NodePrintf &node) const {
        output << gen->generate_printf(node);
      }

      void operator()(const NodeVarDeclar &node) const {
        output << gen->generate_variable_declar(node);
      }
    };

    std::visit(visit_statement{this, output}, stmt.var);

    return output.str();
  }

  /**
   * This method is responsible for variable declaration in assembly
   */
  [[nodiscard]] inline std::string
  generate_variable_declar(const NodeVarDeclar &node) const {
    std::stringstream output;
    // output << "variable declaration"; // TODO: Implement variable declaration
    return output.str();
  }

  /**
   *This method is responsible for printing stuff using printf
   */
  [[nodiscard]] inline std::string
  generate_printf(const NodePrintf &node) const {
    std::stringstream output;
    // TODO: Implement generate printf
    return output.str();
  }

  [[nodiscard]] inline std::string generate_print_int() const {
    /**
     * This method is responsible for printing integers
     */
    std::stringstream output;

    return output.str();
  }

  [[nodiscard]] inline std::string generate_print_string(std::string &value) const {
    /**
     * This method is responsible for printing strings
     */
    std::stringstream output;

    return output.str();
  }
};

#endif // X86_CODEGEN_H
