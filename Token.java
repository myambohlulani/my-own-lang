enum TokenType {
    I32, I64, F32, F64, STRING, BOOL, // data types
    IDENTIFIER, NUMBER, // variables and their value
    SEMI_COLON, ASSIGN, // end of line with semi-colon and assignment operator
    LEFT_BRACE, RIGHT_BRACE, LEFT_PAREN, RIGHT_PAREN,
    NEWLINE, DONE, DO, // escape sequence
    DIVIDE, MULTIPLY, ADD, SUBTRACT, // arithmetic operations
    EQUALS, NOT_EQUALS, LESS_THAN, GREATER_THAN, // same as up
    PRINT, PRINTF, // print into the output
    FOR, WHILE, SWITCH, // loops
    PUBLIC, PRIVATE, PROTECTED, // access modifiers
}

public class Token {

}