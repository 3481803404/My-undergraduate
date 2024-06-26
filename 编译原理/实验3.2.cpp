#include <iostream>
#include <stack>
#include <algorithm>
using namespace std;
int f[7], g[7];//存储优先级函数的值
char p[7][8] = {">><<<>>", ">><<<>>", ">>>><>>", ">>>><>>", "<<<<<=@", ">>>>@>>", "<<<<<@=" };
char s[] = "+-*/()#";
char get_p(char a, char b) {
    int i = 0, j = 0;
    while (s[i] != a) i++;
    while (s[j] != b) j++;
    return p[i][j];
}
void get_priority() {
    bool changed;//标记本轮是否发生了变化
    int n=sizeof(f)/sizeof(int);//终结符的数量
    for (int i = 0; i < n; i++)
        f[i] = g[i] = 1;
    do {
        changed = false;//初始化本轮未发生变化
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (get_p(s[i],s[j])=='>' && f[i] <= g[j]) {
                    f[i] = g[j] + 1;
                    changed = true;
                } else if (get_p(s[i],s[j])=='<' && f[i] >= g[j]) {
                    g[j] = f[i] + 1;
                    changed = true;
                } else if (get_p(s[i],s[j])=='=' && f[i] != g[j]) {
                    f[i] = g[j] = max(f[i], g[j]);
                    changed = true;
                }
            }
        }
    } while (changed);//直到本轮没有变化为止
    for (int i = 0; i < n; i++)
        if (f[i] >= 2 * n + 1 || g[i] >= 2 * n + 1) {
            cout << "优先函数不存在" << endl;
            exit(0);
        }
    cout<<"优先函数:\n\t";
    for (int i = 0; i < n; i++)
        cout << s[i] << "\t";
    cout<<endl;
    cout<<"f\t";
    for (int i = 0; i < n; i++)
        cout << f[i] << "\t";
    cout<<endl;
    cout<<"g\t";
    for (int i = 0; i < n; i++)
        cout << g[i] << "\t";
    cout<<endl;
}
int priority(char a, char b) {
    int i = 0, j = 0;
    while (s[i] != a) i++;
    while (s[j] != b && j != 7) j++;
    if (j == 7) {
        cout<<"error!"<<endl;
        exit(0);
    }
    return f[i]-g[j];
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
        else {
            int flag=priority(sign.top(), expression[i]);
            if(flag<0)
                sign.push(expression[i++]);
            else if(flag>0){
                int b = num.top();
                num.pop();
                int a = num.top();
                num.pop();
                char op = sign.top();
                sign.pop();
                num.push(calculate(a, b, op));
            }
            else{
                sign.pop();
                i++;
            }
        }
    }
    return num.top();
}
int main() {
    get_priority();
    string expression;
    cout <<"请输入表达式:";
    cin >> expression;
    cout << expression << " = " << evaluate(expression+'#') << endl;
    return 0;
}
