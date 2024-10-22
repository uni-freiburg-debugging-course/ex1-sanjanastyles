#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <cctype>

// Step 1: Define token types
enum TokenType { NUMBER, OPERATOR, PAREN_OPEN, PAREN_CLOSE, SIMPLIFY };

// Structure to hold tokens
struct Token {
    TokenType type;
    std::string value;
};

// Function to tokenize input string
std::vector<Token> tokenize(const std::string &input) {
    std::vector<Token> tokens;
    std::string token;

    for (size_t i = 0; i < input.length(); ++i) {
        char c = input[i];

        // Ignore whitespace
        if (isspace(c)) {
            if (!token.empty()) {
                tokens.push_back({NUMBER, token});
                token.clear();
            }
            continue;
        } 
        // Handle opening parenthesis
        else if (c == '(') {
            if (!token.empty()) {
                tokens.push_back({NUMBER, token});
                token.clear();
            }
            tokens.push_back({PAREN_OPEN, "("});
        } 
        // Handle closing parenthesis
        else if (c == ')') {
            if (!token.empty()) {
                tokens.push_back({NUMBER, token});
                token.clear();
            }
            tokens.push_back({PAREN_CLOSE, ")"});
        } 
        // Handle operators
        else if (c == '+' || c == '-' || c == '*') {
            if (!token.empty()) {
                tokens.push_back({NUMBER, token});
                token.clear();
            }
            tokens.push_back({OPERATOR, std::string(1, c)});
        } 
        // Handle digits
        else if (isdigit(c)) {
            token += c; // Accumulate digits for a number
        } 
        // Handle the "simplify" keyword
        else if (input.substr(i, 8) == "simplify") {
            if (!token.empty()) {
                tokens.push_back({NUMBER, token});
                token.clear();
            }
            tokens.push_back({SIMPLIFY, "simplify"});
            i += 7; // Skip the rest of the "simplify" keyword
        } else {
            std::cerr << "Error: Unexpected character '" << c << "' encountered." << std::endl;
        }
    }

    // Finalize any remaining token
    if (!token.empty()) {
        tokens.push_back({NUMBER, token});
    }

    return tokens;
}

// Structure to represent an Abstract Syntax Tree (AST) node
struct ASTNode {
    std::string op;
    int left;
    int right;
};

// Function to parse tokens into an AST
ASTNode parse(const std::vector<Token>& tokens) {
    if (tokens[0].type != PAREN_OPEN || tokens[1].type != SIMPLIFY || 
        tokens[2].type != PAREN_OPEN || tokens[3].type != OPERATOR || 
        tokens[4].type != NUMBER || tokens[5].type != NUMBER || 
        tokens[6].type != PAREN_CLOSE || tokens[7].type != PAREN_CLOSE) {
        throw std::invalid_argument("Invalid syntax: Expected (simplify (op left right))");
    }

    std::string op = tokens[3].value; // The operator
    std::string leftStr = tokens[4].value; // The left operand as string
    std::string rightStr = tokens[5].value; // The right operand as string

    try {
        int left = std::stoi(leftStr); // Convert left operand to integer
        int right = std::stoi(rightStr); // Convert right operand to integer

        return {op, left, right};
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error parsing numbers: " << e.what() << std::endl;
        throw;
    }
}

// Function to evaluate the AST
int evaluate(const ASTNode &ast) {
    if (ast.op == "+") {
        return ast.left + ast.right;
    } else if (ast.op == "-") {
        return ast.left - ast.right;
    } else if (ast.op == "*") {
        return ast.left * ast.right;
    } else {
        throw std::invalid_argument("Unknown operator");
    }
}

// Function to generate random math problems
std::string random_expr() {
    const char *ops[] = {"+", "-", "*"};
    int left = rand() % 1000;
    int right = rand() % 1000;
    int op_idx = rand() % 3;

    std::stringstream ss;
    ss << "(simplify (" << ops[op_idx] << " " << left << " " << right << "))";

    return ss.str();
}

// Function to generate expressions and write them to a file
void generate_and_write_exprs(const std::string &filename, int count) {
    std::ofstream outfile(filename);
    if (!outfile) {
        std::cerr << "Error opening file for writing." << std::endl;
        return;
    }
    for (int i = 0; i < count; ++i) {
        outfile << random_expr() << std::endl;
    }
    outfile.close();
}

// Function to process the expressions and evaluate them
void process_file(const std::string &filename) {
    std::ifstream infile(filename);
    std::string line;

    while (std::getline(infile, line)) {
        std::cout << "Input: " << line << std::endl;

        try {
            std::vector<Token> tokens = tokenize(line);
            ASTNode ast = parse(tokens);
            int result = evaluate(ast);
            std::cout << "Result: " << result << std::endl;
        } catch (const std::exception &e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}

// Function to generate Z3 input from expressions
void generate_z3_input(const std::string &filename, const std::vector<std::string> &expressions) {
    std::ofstream outfile(filename);
    if (!outfile) {
        std::cerr << "Error opening file for writing Z3 input." << std::endl;
        return;
    }
    
    for (const auto& expr : expressions) {
        // Parse the expression to extract operator and operands
        std::vector<Token> tokens = tokenize(expr);
        ASTNode ast = parse(tokens);
        
        // Generate Z3 format
        outfile << "(assert (= " << evaluate(ast) << " " 
                << "(" << ast.op << " " << ast.left << " " << ast.right << ")))" << std::endl;
    }

    outfile.close();
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr))); 
    const int expr_count = 50;
    
    // Generate random math expressions and write to file
    generate_and_write_exprs("expressions.txt", expr_count);
    
    // Reading expressions from the file
    std::ifstream infile("expressions.txt");
    std::vector<std::string> expressions;
    std::string line;
    while (std::getline(infile, line)) {
        expressions.push_back(line);
    }

    // Process the expressions to evaluate and print results
    process_file("expressions.txt");

    // Generate Z3 input file based on expressions
    generate_z3_input("z3_input.smt2", expressions);
    
    return 0;
}
