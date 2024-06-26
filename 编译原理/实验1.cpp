#include <iostream>
#include <string.h>

using namespace std;

FILE* fin;
//读头行数
int row = 1;
//读头字符
char ch;
//读头字符串
char token[20];
int token_cnt = 0;
//关键字
const char keyWord[9][20] = { "void","int","float","char","if",
                              "else","while","do","for" };

int judge_token() {
    memset(token, NULL, sizeof(token));
    token_cnt = 0;
    //清除空格换行符
    while (ch == ' ' || ch == '\t' || ch == '\n') {
        if (ch == '\n')
            row++;
        ch = getc(fin);
    }
    //关键字或标识符
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
        while ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '_') {
            token[token_cnt++] = ch;
            ch = getc(fin);
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
            ch = getc(fin);
        }
        if (flag == 0) return 2;//整数
        else return 3;//浮点数
    }
    else {
        token[token_cnt++] = ch;
        switch (ch) {
            case '+': ch = getc(fin); return 5;
            case '-': ch = getc(fin); return 6;
            case '*': ch = getc(fin); return 7;
            case '/': ch = getc(fin); return 8;
            case '(': ch = getc(fin); return 19;
            case ')': ch = getc(fin); return 20;
            case '[': ch = getc(fin); return 21;
            case ']': ch = getc(fin); return 22;
            case '{': ch = getc(fin); return 23;
            case '}': ch = getc(fin); return 24;
            case ':': ch = getc(fin); return 25;
            case ';': ch = getc(fin); return 26;
            case ',': ch = getc(fin); return 27;
            case '=':
                ch = getc(fin);
                if (ch == '=') {
                    token[token_cnt++] = ch;
                    ch = getc(fin);
                    return 11;
                }
                else {
                    return 18;
                }
            case '>':
                ch = getc(fin);
                if (ch == '=') {
                    token[token_cnt++] = ch;
                    ch = getc(fin);
                    return 14;
                }
                else {
                    return 13;
                }
            case '<':
                ch = getc(fin);
                if (ch == '=') {
                    token[token_cnt++] = ch;
                    ch = getc(fin);
                    return 10;
                }
                else {
                    return 9;
                }
            case '!':
                ch = getc(fin);
                if (ch == '=') {
                    token[token_cnt++] = ch;
                    ch = getc(fin);
                    return 12;
                }
                else {
                    return -10;
                }
            case '&':
                ch = getc(fin);
                if (ch == '&') {
                    token[token_cnt++] = ch;
                    ch = getc(fin);
                    return 15;
                }
                else {
                    return 16;
                }
            case '|':
                ch = getc(fin);
                if (ch == '|') {
                    token[token_cnt++] = ch;
                    ch = getc(fin);
                    return 17;
                }
                else {
                    return -10;
                }
            case EOF: return -1;
            default: ch = getc(fin); return -10;
        }
    }
}

void analyse() {
    int temp;
    ch = getc(fin);
    cout << "输入串:\t对应编号:" << endl;
    while (1) {
        temp = judge_token();
        if (temp == -1)
            break;
        else if (temp == -10)
            cout << "line " << row << " error!" << endl;
        else
            cout << token << "\t" << temp << endl;
    }
}

int main() {
    fin = fopen("test.txt", "r");
    analyse();
    return 0;
}