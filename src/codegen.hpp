#ifndef CODEGEN_H
#define CODEGEN_H
#include "./parser.hpp"
#include <unordered_map>
#include <vector>

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
        const NodePrintf &printf_node = std::get<NodePrintf>(stmt.var);
        if (std::holds_alternative<NodeExpr>(printf_node.expr)) {
          uses_print_int = true;
        }
      }

      output << generate_statements(stmt);
    }

    if (!has_exit) {
      output << generate_default_exit();
    }

    if (uses_print_int) {
      output << generate_print_int_func();
    }

    if (uses_print_int || !m_string_literals.empty()) {
      output << m_data;
      output << "		buffer: .space 12\n";
      output << "		newline: .asciiz \"\\n\"" << m_newline;

      for (size_t i = 0; i < m_string_literals.size(); ++i) {
        output << "		str" << i << ": .asciiz \""
               << m_string_literals[i] << "\\n\"";
        output << "\n		str" << i << "_len = . - str" << i << "\n";
      }
    }

    return output.str();
  }

private:
  const NodeProgram m_root{};
  const std::string m_start = ".text\n.globl __start\n__start:\n";
  const std::string m_syscall = "		syscall\n";
  const std::string m_data = ".data\n";
  const std::string m_newline = "\n";
  mutable std::vector<std::string> m_string_literals; // for print values
  int m_label_count = 0;
  mutable int m_var_count = 0;

  // variables maps
  mutable std::unordered_map<std::string,std::string> m_str_vars; // for strings
  mutable std::unordered_map<std::string,std::string> m_int_vars; // for ints
  mutable std::vector<std::string> m_int_var_declarations;

  [[nodiscard]] inline std::string generate_exit(const NodeExit &node) const {
    std::stringstream output;

    // login the code for the exit
    output << "		li $v0, 4001\n";
    output << emit_load_int(node.expr, "$a0");
    output << m_syscall << "\n";

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
        output << gen->generate_printf(node);
      }

      // TODO: Create if statements and more
    };

    std::visit(visit_statement{this, output}, stmt.var);
    return output.str();
  }

  [[nodiscard]] inline std::string generate_default_exit() const {
    std::stringstream output;

    output << "		li $v0, 4001\n"; // exit sys
    output << "		li $a0, 0\n";
    output << m_syscall << "\n";

    return output.str();
  }

  [[nodiscard]] inline std::string
  generate_print_string(const std::string &value) const {
    std::stringstream output;

    int idx = m_var_count++;
    m_string_literals.push_back(value);
    output << "		li $v0, 4004\n";
    output << "		li $a0, 1\n";
    output << "		la $a1, str" << idx << "\n";
    output << "		li $a2, str" << idx << "_len\n";
    output << m_syscall;
    output << m_newline;

    return output.str();
  }

  // this method loads a value
  [[nodiscard]] inline std::string emit_load_int(const NodeExpr &expr, const std::string &reg) const {
    std::stringstream out;

    std::visit([&](auto &&v) {
          using T = std::decay_t<decltype(v)>;
          if constexpr (std::is_same_v<T, NodeLiteral>) {
            out << "		li " << reg << ", " << v.int_lit.value.value() << "\n";
          } else if constexpr (std::is_same_v<T, NodeIdentExpr>) {
            const std::string name = v.ident.value.value();
            const auto it = m_int_vars.find(name);
            if (it == m_int_vars.end()) {
              std::cerr << "Undeclared (or non-numeric) variable used: " << name << "\n";
              exit(EXIT_FAILURE);
            }
            out << "		lw " << reg << ", " << it->second << "\n";
          }
        },
        expr.value);

    return out.str();
  }

  [[nodiscard]] inline std::string generate_print_int_func() const {
    std::stringstream output;
    output << "print_int:\n";
    output << "		addi $sp, $sp, -4\n";
    output << "		sw $ra, 0($sp)\n";
    output << m_newline;
    output << "		move $t0, $a0\n";
    output << "		la  $t1, buffer + 11\n";
    output << "		li  $t2, 10\n";
    output << "		li  $t3, 0\n";
    output << m_newline;
    output << "convert_loop:\n";
    output << "		div $t0, $t2\n";
    output << "		mfhi $t4\n";
    output << "		mflo $t0\n";
    output << "		addi $t4, $t4, '0'\n";
    output << "		sb $t4, 0($t1)\n";
    output << "		addi $t1, $t1, -1\n";
    output << "		addi $t3, $t3, 1\n";
    output << "		bnez $t0, convert_loop\n";
    output << m_newline;
    output << "		addi $t1, $t1, 1\n";
    output << m_newline;
    output << "		li $v0, 4004\n";
    output << "		li $a0, 1\n";
    output << "		move $a1, $t1\n";
    output << "		move $a2, $t3\n";
    output << m_syscall;
    output << m_newline;
    output << "		li $v0, 4004\n";
    output << "		li $a0, 1\n";
    output << "		la $a1, newline\n";
    output << "		li $a2, 1\n";
    output << m_syscall;
    output << m_newline;
    output << "		lw $ra, 0($sp)\n";
    output << "		addi $sp, $sp, 4\n";
    output << "		jr $ra\n";

    return output.str();
  }

  [[nodiscard]] inline std::string
  generate_print_int_stmt(const NodePrintf &node) const {
    std::stringstream output;

    if (!std::holds_alternative<NodeExpr>(node.expr)) {
      // not an integer
      exit(EXIT_FAILURE);
    }

    const NodeExpr &expr = std::get<NodeExpr>(node.expr);
    output << emit_load_int(expr, "$a0");
    output << "		jal print_int\n";
    output << m_newline;

    return output.str();
  }

  [[nodiscard]] inline std::string
  generate_printf(const NodePrintf &node) const {
    if (std::holds_alternative<NodeExpr>(node.expr)) {
      return generate_print_int_stmt(node);
    }

    if (std::holds_alternative<NodeStr>(node.expr)) {
      const NodeStr &str = std::get<NodeStr>(node.expr);
      return generate_print_string(str.string_lit.value.value());
    }

    std::cerr << "print(): unsupported expression type\n";
    exit(EXIT_FAILURE);
  }
};

#endif // CODEGEN_H
