# Converting BNF to C Code - Complete Guide

## The Core Pattern

**Every BNF rule becomes a C function.**

```
BNF Rule → C Function
```

## Pattern 1: Simple Terminal Matching

**BNF:**
```bnf
<break_statement> ::= "break" ";"
```

**C Code:**
```c
ASTNode* parse_break_statement(Parser* parser) {
    expect(parser, TOKEN_BREAK);    // Match "break"
    expect(parser, TOKEN_SEMICOLON); // Match ";"

    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_BREAK;
    return node;
}
```

## Pattern 2: Sequence of Elements

**BNF:**
```bnf
<declaration> ::= <data_type> <identifier> "=" <expression> ";"
```

**C Code:**
```c
ASTNode* parse_declaration(Parser* parser) {
    ASTNode* node = malloc(sizeof(ASTNode));

    char* type = parse_data_type(parser);     // <data_type>
    char* name = parser->current.value;        // <identifier>
    expect(parser, TOKEN_IDENTIFIER);
    expect(parser, TOKEN_ASSIGN);              // "="
    ASTNode* expr = parse_expression(parser);  // <expression>
    expect(parser, TOKEN_SEMICOLON);           // ";"

    node->data.declaration.type = type;
    node->data.declaration.name = name;
    node->data.declaration.value = expr;

    return node;
}
```

**Rule: Parse elements left-to-right in order.**

## Pattern 3: Choices (|)

**BNF:**
```bnf
<data_type> ::= "i32" | "i64" | "string" | "bool"
```

**C Code:**
```c
char* parse_data_type(Parser* parser) {
    if (check(parser, TOKEN_I32)) {
        advance(parser);
        return "i32";
    }
    else if (check(parser, TOKEN_I64)) {
        advance(parser);
        return "i64";
    }
    else if (check(parser, TOKEN_STRING)) {
        advance(parser);
        return "string";
    }
    else if (check(parser, TOKEN_BOOL)) {
        advance(parser);
        return "bool";
    }
    else {
        error("Expected data type");
    }
}
```

**Rule: Use if/else chain for choices.**

## Pattern 4: Zero or More (*)

**BNF:**
```bnf
<block> ::= "{" <statement>* "}"
```

**C Code:**
```c
ASTNode* parse_block(Parser* parser) {
    expect(parser, TOKEN_LBRACE);

    // Dynamic array for statements
    int capacity = 10;
    int count = 0;
    ASTNode** statements = malloc(sizeof(ASTNode*) * capacity);

    // Loop while NOT at closing brace
    while (!check(parser, TOKEN_RBRACE)) {
        // Resize array if needed
        if (count >= capacity) {
            capacity *= 2;
            statements = realloc(statements, sizeof(ASTNode*) * capacity);
        }

        statements[count++] = parse_statement(parser);
    }

    expect(parser, TOKEN_RBRACE);

    ASTNode* node = malloc(sizeof(ASTNode));
    node->data.block.statements = statements;
    node->data.block.count = count;
    return node;
}
```

**Rule: Use while loop + dynamic array for repetition.**

## Pattern 5: One or More (+)

**BNF:**
```bnf
<case_clauses> ::= <case_clause>+
```

**C Code:**
```c
CaseClause** parse_case_clauses(Parser* parser, int* count) {
    int capacity = 10;
    *count = 0;
    CaseClause** clauses = malloc(sizeof(CaseClause*) * capacity);

    // Parse at least one (required)
    clauses[(*count)++] = parse_case_clause(parser);

    // Then zero or more
    while (check(parser, TOKEN_CASE)) {
        if (*count >= capacity) {
            capacity *= 2;
            clauses = realloc(clauses, sizeof(CaseClause*) * capacity);
        }
        clauses[(*count)++] = parse_case_clause(parser);
    }

    return clauses;
}
```

**Rule: Parse one, then loop for the rest.**

## Pattern 6: Optional (?)

**BNF:**
```bnf
<return_statement> ::= "return" <expression>? ";"
```

**C Code:**
```c
ASTNode* parse_return_statement(Parser* parser) {
    expect(parser, TOKEN_RETURN);

    ASTNode* expr = NULL;

    // Check if expression exists
    if (!check(parser, TOKEN_SEMICOLON)) {
        expr = parse_expression(parser);
    }

    expect(parser, TOKEN_SEMICOLON);

    ASTNode* node = malloc(sizeof(ASTNode));
    node->data.return_stmt.expression = expr;  // Could be NULL
    return node;
}
```

**Rule: Use if statement to check for optional element.**

## Pattern 7: Complex Statement Routing

**BNF:**
```bnf
<statement> ::= <if_statement>
              | <while_loop>
              | <for_loop>
              | <declaration>
              | <assignment>
```

**C Code:**
```c
ASTNode* parse_statement(Parser* parser) {
    // Look at current token and route to correct parser

    if (check(parser, TOKEN_IF)) {
        return parse_if_statement(parser);
    }
    else if (check(parser, TOKEN_WHILE)) {
        return parse_while_loop(parser);
    }
    else if (check(parser, TOKEN_FOR)) {
        return parse_for_loop(parser);
    }
    else if (is_data_type(parser->current.type)) {
        return parse_declaration(parser);
    }
    else if (check(parser, TOKEN_IDENTIFIER)) {
        return parse_assignment(parser);
    }
    else {
        error("Unexpected statement");
    }
}
```

**Rule: Look ahead at current token to decide which path.**

## Essential Helper Functions

```c
// Move to next token
void advance(Parser* parser) {
    if (parser->position < parser->token_count - 1) {
        parser->position++;
        parser->current = parser->tokens[parser->position];
    }
}

// Check if current token matches type (doesn't consume)
int check(Parser* parser, TokenType type) {
    return parser->current.type == type;
}

// Expect token and consume it (or error)
void expect(Parser* parser, TokenType type) {
    if (!check(parser, type)) {
        fprintf(stderr, "Parse error: expected %d but got %d\n",
                type, parser->current.type);
        exit(1);
    }
    advance(parser);
}

// Check multiple types
int check_any(Parser* parser, int count, ...) {
    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; i++) {
        TokenType type = va_arg(args, TokenType);
        if (check(parser, type)) {
            va_end(args);
            return 1;
        }
    }

    va_end(args);
    return 0;
}
```

## Memory Management in C

**Important: Always allocate AST nodes**

```c
ASTNode* create_node(NodeType type) {
    ASTNode* node = malloc(sizeof(ASTNode));
    if (node == NULL) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }
    node->type = type;
    return node;
}
```

**Remember to free later:**

```c
void free_ast(ASTNode* node) {
    if (node == NULL) return;

    switch (node->type) {
        case NODE_DECLARATION:
            free(node->data.declaration.name);
            free(node->data.declaration.type);
            free_ast(node->data.declaration.value);
            break;
        // ... other cases
    }

    free(node);
}
```

## Complete Example: If Statement

**BNF:**
```bnf
<if_statement> ::= "if" "(" <expression> ")" ":" "then" "{"
                       <statement>*
                   "}" "fi" ";"
```

**Complete C Implementation:**
```c
typedef struct {
    ASTNode* condition;
    ASTNode** then_body;
    int then_count;
} IfStatement;

ASTNode* parse_if_statement(Parser* parser) {
    // Create node
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_IF_STATEMENT;

    // Match "if"
    expect(parser, TOKEN_IF);

    // Match "("
    expect(parser, TOKEN_LPAREN);

    // Parse condition
    node->data.if_stmt.condition = parse_expression(parser);

    // Match ")"
    expect(parser, TOKEN_RPAREN);

    // Match ":"
    expect(parser, TOKEN_COLON);

    // Match "then"
    expect(parser, TOKEN_THEN);

    // Match "{"
    expect(parser, TOKEN_LBRACE);

    // Parse statements (zero or more)
    int capacity = 10;
    node->data.if_stmt.then_body = malloc(sizeof(ASTNode*) * capacity);
    node->data.if_stmt.then_count = 0;

    while (!check(parser, TOKEN_RBRACE)) {
        if (node->data.if_stmt.then_count >= capacity) {
            capacity *= 2;
            node->data.if_stmt.then_body =
                realloc(node->data.if_stmt.then_body,
                       sizeof(ASTNode*) * capacity);
        }

        node->data.if_stmt.then_body[node->data.if_stmt.then_count++] =
            parse_statement(parser);
    }

    // Match "}"
    expect(parser, TOKEN_RBRACE);

    // Match "fi"
    expect(parser, TOKEN_FI);

    // Match ";"
    expect(parser, TOKEN_SEMICOLON);

    return node;
}
```

## Operator Precedence

For expressions with operators, use cascading functions:

**BNF:**
```bnf
<additive> ::= <multiplicative> (("+" | "-") <multiplicative>)*
<multiplicative> ::= <primary> (("*" | "/") <primary>)*
```

**C Code:**
```c
ASTNode* parse_additive(Parser* parser) {
    ASTNode* left = parse_multiplicative(parser);

    while (check(parser, TOKEN_PLUS) || check(parser, TOKEN_MINUS)) {
        char* op = parser->current.value;
        advance(parser);

        ASTNode* right = parse_multiplicative(parser);

        ASTNode* binary = malloc(sizeof(ASTNode));
        binary->type = NODE_BINARY_OP;
        binary->data.binary_op.left = left;
        binary->data.binary_op.operator = op;
        binary->data.binary_op.right = right;

        left = binary;  // Left-associative
    }

    return left;
}

ASTNode* parse_multiplicative(Parser* parser) {
    ASTNode* left = parse_primary(parser);

    while (check(parser, TOKEN_STAR) || check(parser, TOKEN_SLASH)) {
        char* op = parser->current.value;
        advance(parser);

        ASTNode* right = parse_primary(parser);

        ASTNode* binary = malloc(sizeof(ASTNode));
        binary->type = NODE_BINARY_OP;
        binary->data.binary_op.left = left;
        binary->data.binary_op.operator = op;
        binary->data.binary_op.right = right;

        left = binary;
    }

    return left;
}
```

## Quick Reference Card

| BNF Pattern   | C Code Pattern                           |
| ------------- | ---------------------------------------- |
| `"keyword"`   | `expect(parser, TOKEN_KEYWORD);`         |
| `<rule>`      | `parse_rule(parser);`                    |
| `A B C`       | Parse A, then B, then C in sequence      |
| `A \| B \| C` | `if/else if/else` chain                  |
| `A*`          | `while (!stop_condition) { parse_A(); }` |
| `A+`          | Parse once, then `while` for rest        |
| `A?`          | `if (condition) { parse_A(); }`          |

## Next Steps

1. **Start with lexer** - Tokenize source code first
2. **Build simple parsers** - Start with `break`, `continue`
3. **Add complexity** - Then expressions, statements
4. **Test incrementally** - Test each parser function
5. **Add error handling** - Better error messages
6. **Build interpreter/compiler** - Execute or generate code

## Common Mistakes to Avoid

❌ **Forgetting to advance()**
```c
expect(parser, TOKEN_IF);  // This advances
if (check(parser, TOKEN_IF)) {
    // Need to advance() here!
}
```

❌ **Not allocating memory**
```c
ASTNode node;  // Stack allocation - BAD
ASTNode* node = malloc(sizeof(ASTNode));  // Heap - GOOD
```

❌ **Memory leaks**
```c
// Always free what you malloc
free_ast(ast);
```

❌ **Wrong operator precedence**
```c
// Must parse higher precedence first
// multiplication before addition
```

## Resources

- Look at real compiler code: GCC, Clang source
- Read "Crafting Interpreters" book (free online)
- Study your grammar file as the blueprint
- Test with simple examples first