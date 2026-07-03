#ifndef CODEGEN_H
#define CODEGEN_H

class Generator {
	public:
		inline explicit Generator(ParserNode::NodeExit root) m_root(std::move(root)) {}
		
		[[nodiscard]] std::string generate() const {
			std::stringstream output;
			output << m_start; // start part
			output << "	mov rax, 60\n";
			output << " mov rdi, " << m_root.expr.INT_LIT.value.value() << "\n";
			output << " " << m_syscall;

			return output.str();
		}

	private:
		const ParserNode::NodeExit m_root;
		const std::string m_start = "global _start\n_start:\n";
		const std::string m_syscall = "syscall\n";
		
};

#endif // CODEGEN_H
