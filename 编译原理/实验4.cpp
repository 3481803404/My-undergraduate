#include <iostream>
#include <string>
#include <vector>
#include <stack>
using namespace std;
enum TokenType {
    IDENTIFIER,
    OPERATOR,
    NUMBER,
    ASSIGNMENT,
    END,
    LEFT_PAREN,
    RIGHT_PAREN
};
struct Token {
    TokenType type;
    string value;
};
struct Quadruple {
    string op;
    string arg1;
    string arg2;
    string result;
};
class Compiler {
public:
    explicit Compiler(string  input) : input_(move(input)) {}
    void compile() {
        lex();
        parse();
        generateIntermediateCode();
        generateAssemblyCode();
    }
private:
    string input_;
    vector<Token> tokens_;
    vector<Quadruple> quadruples_;
    void lex() {
        size_t pos = 0;
        while (pos < input_.size()) {
            if (isspace(input_[pos])) {
                pos++;
                continue;
            }
            Token token;
            if (isalpha(input_[pos])) {
                // 处理标识符
                string identifier;
                while (isalnum(input_[pos])) {
                    identifier += input_[pos];
                    pos++;
                }
                token.type = IDENTIFIER;
                token.value = identifier;
            }
            else if (isOperator(input_[pos])) {
                // 处理操作符
                token.type = OPERATOR;
                token.value = input_[pos];
                pos++;
            }
            else if (isdigit(input_[pos])) {
                // 处理数字
                string number;
                while (isdigit(input_[pos])) {
                    number += input_[pos];
                    pos++;
                }
                token.type = NUMBER;
                token.value = number;
            }
            else if (input_[pos] == '=') {
                // 处理赋值符号
                token.type = ASSIGNMENT;
                token.value = "=";
                pos++;
            }
            else if (input_[pos] == ';') {
                // 处理结束符号
                token.type = END;
                token.value = ";";
                pos++;
            }
            else if (input_[pos] == '(') {
                // 处理左括号
                token.type = LEFT_PAREN;
                token.value = "(";
                pos++;
            }
            else if (input_[pos] == ')') {
                // 处理右括号
                token.type = RIGHT_PAREN;
                token.value = ")";
                pos++;
            }
            else {
                cout << "Error: Invalid character encountered!" << endl;
                return;
            }
            tokens_.push_back(token);
        }
    }
    static bool isOperator(char c) {
        return (c == '+' || c == '-' || c == '*' || c == '/');
    }
    void parse() {
        if (tokens_.empty()) {
            cout << "Error: No tokens to parse!" << endl;
            return;
        }
        int currentToken = 0;
        while (currentToken < tokens_.size()) {
            Token token = tokens_[currentToken];
            if (token.type == END) {
                return;
            }
            if (token.type != IDENTIFIER) {
                cout << "Error: Expected identifier!" << endl;
                return;
            }
            string variable = token.value;
            currentToken++;
            if (currentToken >= tokens_.size() || tokens_[currentToken].type != ASSIGNMENT || tokens_[currentToken].value != "=") {
                cout << "Error: Expected assignment operator!" << endl;
                return;
            }
            currentToken++;
            stack<Token> postfix;
            stack<Token> operators;
            while (currentToken < tokens_.size() && tokens_[currentToken].type != END && tokens_[currentToken].value != ";") {
                Token current = tokens_[currentToken];
                if (current.type == IDENTIFIER || current.type == NUMBER) {
                    postfix.push(current);
                }
                else if (current.type == OPERATOR) {
                    while (!operators.empty() && operators.top().type == OPERATOR && precedence(current.value) <= precedence(operators.top().value)) {
                        postfix.push(operators.top());
                        operators.pop();
                    }
                    operators.push(current);
                }
                else if (current.type == LEFT_PAREN) {
                    operators.push(current);
                }
                else if (current.type == RIGHT_PAREN) {
                    while (!operators.empty() && operators.top().type != LEFT_PAREN) {
                        postfix.push(operators.top());
                        operators.pop();
                    }
                    if (operators.empty() || operators.top().type != LEFT_PAREN) {
                        cout << "Error: Unmatched parentheses!" << endl;
                        return;
                    }
                    operators.pop(); // 弹出左括号
                }
                else {
                    cout << "Error: Invalid token encountered!" << endl;
                    return;
                }
                currentToken++;
            }
            while (!operators.empty()) {
                if (operators.top().type != OPERATOR && operators.top().type != LEFT_PAREN) {
                    cout << "Error: Invalid expression!" << endl;
                    return;
                }
                postfix.push(operators.top());
                operators.pop();
            }
            processPostfix(variable, postfix);
        }
        if (currentToken == tokens_.size() && tokens_[currentToken].type != END)
            cout<< "Error: lack of ';'!" << endl;
    }
    static int precedence(const string& op) {
        if (op == "*" || op == "/") {
            return 2;
        }
        else if (op == "+" || op == "-") {
            return 1;
        }
        else {
            return 0;
        }
    }
    void processPostfix(const string& variable, stack<Token>& postfix) {
        vector<Token> expression;
        while (!postfix.empty()) {
            expression.push_back(postfix.top());
            postfix.pop();
        }
        stack<string> evaluation;
        for (int i = expression.size() - 1; i >= 0; i--) {
            Token current = expression[i];
            if (current.type == IDENTIFIER || current.type == NUMBER) {
                evaluation.push(current.value);
            }
            else if (current.type == OPERATOR) {
                if (evaluation.size() < 2) {
                    cout << "Error: Invalid expression!" << endl;
                    return;
                }
                string arg1 = evaluation.top();
                evaluation.pop();
                string arg2 = evaluation.top();
                evaluation.pop();
                string temp = generateTempVariable();
                Quadruple quad = { current.value, arg1, arg2, temp };
                quadruples_.push_back(quad);
                evaluation.push(temp);
            }
            else {
                cout << "Error: Invalid token encountered!" << endl;
                return;
            }
        }
        if (evaluation.size() != 1) {
            cout << "Error: Invalid expression!" << endl;
            return;
        }
        string result = evaluation.top();
        Quadruple assignment = { "=", result, "", variable };
        quadruples_.push_back(assignment);
    }
    static string generateTempVariable() { //产生中间变量
        static int tempCount = 0;
        return "T" + to_string(tempCount++);
    }
    void generateIntermediateCode() { //产生四元式
        cout << "中间代码四元式:" << endl;
        int cnt=1;
        for (const auto& quad : quadruples_) {
            if (quad.op == "+") {
                cout << cnt++ << ": (" << "+, " << quad.arg1 << ", " << quad.arg2 << ", " << quad.result << ")" << endl;
            }
            else if (quad.op == "-") {
                cout << cnt++ << ": (" << "-, " << quad.arg1 << ", " << quad.arg2 << ", " << quad.result << ")" << endl;
            }
            else if (quad.op == "*") {
                cout << cnt++ << ": (" << "*, " << quad.arg1 << ", " << quad.arg2 << ", " << quad.result << ")" << endl;
            }
            else if (quad.op == "/") {
                cout << cnt++ << ": (" << "/, " << quad.arg1 << ", " << quad.arg2 << ", " << quad.result << ")" << endl;
            }
            else if (quad.op == "=") {
                cout << cnt++ << ": (" << "=, " << quad.arg1 << ", " << "_" << ", " << quad.result << ")" << endl;
            }
        }
    }
    void generateAssemblyCode() { //产生汇编
        cout << "汇编指令:" << endl;
        for (const auto& quad : quadruples_) {
            if (quad.op == "+") {
                cout << "MOV R0," << quad.arg1 << endl;
                cout << "MOV R1," << quad.arg2 << endl;
                cout << "ADD R1,R0" << endl;
                cout << "MOV " << quad.result << ",R1" << endl;
            }
            else if (quad.op == "-") {
                cout << "MOV R0," << quad.arg1 << endl;
                cout << "MOV R1," << quad.arg2 << endl;
                cout << "SUB R1,R0" << endl;
                cout << "MOV " << quad.result << ",R1" << endl;
            }
            else if (quad.op == "*") {
                cout << "MOV AL," << quad.arg1 << endl;
                cout << "MUL " << quad.arg2 << endl;
                cout << "MOV " << quad.result << ",AX" << endl;
            }
            else if (quad.op == "/") {
                cout << "MOV AX," << quad.arg1 << endl;
                cout << "DIV " << quad.arg2 << endl;
                cout << "MOV " << quad.result << ",AL" << endl;
            }
            else if (quad.op == "=") {
                cout << "MOV R0," << quad.arg1 << endl;
                cout << "MOV " << quad.result << ", R0" << endl;
            }
        }
    }
};
int main() {
    // 输入赋值语句
    string input;
    cout << "输入赋值语句: " << endl;
    cin>>input;
    Compiler compiler(input);
    compiler.compile();
    return 0;
}