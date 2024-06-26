#include <iostream>
#include <stack>
#include <string>
using namespace std;
char p[7][8] = {">><<<>>", ">><<<>>", ">>>><>>", ">>>><>>", "<<<<<=@", ">>>>@>>", "<<<<<@=" };
char s[] = "+-*/()#";
char priority(char a, char b) {
    int i = 0, j = 0;
    while (s[i] != a) i++;
    while (s[j] != b && j != 7) j++;
    if (j == 7) {
        cout<<"error!"<<endl;
        exit(0);
    }
    return p[i][j];
}
int calculate(int a, int b, char op) {
    switch (op) {
        case '+':return a + b;
        case '-':return a - b;
        case '*':return a * b;
        default:return a / b;
    }
}
int evaluate(string expression) {
    stack<int> num;
    stack<char> sign;
    sign.push('#');
    int i = 0;
    while (!sign.empty()) {
        if (isdigit(expression[i])) {
            int operand = 0;
            while (i < expression.length() && isdigit(expression[i])) {
                operand = operand * 10 + expression[i] - '0';
                i++;
            }
            num.push(operand);
        }
        else
            switch (priority(sign.top(), expression[i])) {
                case'<':sign.push(expression[i++]);break;
                case'>':{
                    int b = num.top();
                    num.pop();
                    int a = num.top();
                    num.pop();
                    char op = sign.top();
                    sign.pop();
                    num.push(calculate(a, b, op));
                    break;
                }
                case'=':sign.pop();i++;break;
                default:cout<<"error!"<<endl;exit(0);
            }
    }
    return num.top();
}
int main() {
    string expression;
    cout <<"请输入表达式:";
    cin >> expression;
    cout << expression << " = " << evaluate(expression+'#') << endl;
    return 0;
}