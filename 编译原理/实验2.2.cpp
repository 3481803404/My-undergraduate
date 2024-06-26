#include <iostream>
#include <cstring>
using namespace std;

char input[100];//输入串
int input_cnt=0;
char c1[100] ="";//处理后的输入串
int c1_length=0;
char ch;//读头字符
char token[20];//读头字符串
int token_cnt = 0;
const char keyWord[9][20] = { "void","int","float","char","if",
                              "else","while","do","for" };//关键字
int judge_token() {
    memset(token, '\0', sizeof(token));
    token_cnt = 0;
    //关键字或标识符
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
        token[token_cnt++] = ch;
        ch = input[input_cnt++];
        while ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '_') {
            token[token_cnt++] = ch;
            ch = input[input_cnt++];
        }
        token[token_cnt++] = '\0';
        for (int i = 0; i < 9; i++) {
            if (strcmp(token, keyWord[i]) == 0) {
                return i + 28;//关键字
            }
        }
        return 1;//标识符
    }
        //整数或浮点数
    else if (ch >= '0' && ch <= '9') {
        int flag = 0;
        while ((ch >= '0' && ch <= '9') || ch == '.') {
            if (ch == '.')
                flag = 1;
            token[token_cnt++] = ch;
            ch = input[input_cnt++];
        }
        if (flag == 0) return 2;//整数
        else return 3;//浮点数
    }
    else {
        token[token_cnt++] = ch;
        switch (ch) {
            case '+': ch = input[input_cnt++]; return 5;
            case '-': ch = input[input_cnt++]; return 6;
            case '*': ch = input[input_cnt++]; return 7;
            case '/': ch = input[input_cnt++]; return 8;
            case '(': ch = input[input_cnt++]; return 19;
            case ')': ch = input[input_cnt++]; return 20;
            case '\0': return -1;
            default: ch = ch = input[input_cnt++]; return -10;
        }
    }
}
//词法分析
int turn()
{
    int temp;
    ch = input[input_cnt++];
    cout << "词法分析:\nvalue\tindex\tclass" << endl;
    while (true) {
        temp = judge_token();
        if (temp==-1)
            return 1;
        if (temp>=1&&temp<=3)
        {
            if(temp==1)
                cout << token << "\t" << temp << "\t标识符" << endl;
            else if(temp==2)
                cout << token << "\t" << temp << "\t整数" << endl;
            else if(temp==3)
                cout << token << "\t" << temp << "\t浮点数" << endl;
            c1[c1_length++]='i';
        }
        else if(temp>=5&&temp<=8||temp==19||temp==20)
        {
            cout << token << "\t" << temp << "\t运算符" << endl;
            switch (temp) {
                case 5:c1[c1_length++]='+';break;
                case 6:c1[c1_length++]='-';break;
                case 7:c1[c1_length++]='*';break;
                case 8:c1[c1_length++]='/';break;
                case 19:c1[c1_length++]='(';break;
                case 20:c1[c1_length++]=')';break;
                default:break;
            }
        }
        else
        {
            cout << "词法错误!" << endl;
            return 0;
        }
    }
}

int pos = 0; // 当前扫描到的位置
// 函数声明
bool E();
bool e();
bool T();
bool t();
bool F();
// E -> Te
bool E() {
    return T() && e();
}
// e -> +Te | -Te | $
bool e() {
    if (pos < c1_length && (c1[pos] == '+' || c1[pos] == '-')) {
        pos++;
        return T() && e();
    } else {
        return true;
    }
}
// T -> Ft
bool T() {
    return F() && t();
}
// t -> *Ft | /Ft | $
bool t() {
    if (pos < c1_length && (c1[pos] == '*' || c1[pos] == '/')) {
        pos++;
        return F() && t();
    } else {
        return true;
    }
}
// F -> i | (E)
bool F() {
    if (pos < c1_length && c1[pos] == 'i') {
        pos++;
        return true;
    } else if (pos < c1_length && c1[pos] == '(') {
        pos++;
        bool res = E();
        if (pos < c1_length && c1[pos] == ')') {
            pos++;
            return res;
        } else {
            cout<<"Error:缺少')'!"<<endl;
            return false;
        }
    } else {
        cout<<"Error:缺少一个因子!"<<endl;
        return false;
    }
}
int main() {
    cout<<"请输入要分析的句型:";
    cin >> input;
    while(turn())
    {
        // 从E开始分析
        cout<<"\n语法分析:"<<endl;
        if (E() && pos == c1_length)
            cout << "该字符串符合文法!\n" << endl;
        else
            cout << "该字符串不符合文法!\n" << endl;
        memset(input, '\0', sizeof(input));
        input_cnt=0;
        memset(c1, '\0', sizeof(c1));
        c1_length=0;
        pos=0;
        cout<<"请输入要分析的句型:";
        cin >> input;
    }
    return 0;
}