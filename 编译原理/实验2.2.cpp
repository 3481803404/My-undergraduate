#include <iostream>
#include <cstring>
using namespace std;

char input[100];//���봮
int input_cnt=0;
char c1[100] ="";//���������봮
int c1_length=0;
char ch;//��ͷ�ַ�
char token[20];//��ͷ�ַ���
int token_cnt = 0;
const char keyWord[9][20] = { "void","int","float","char","if",
                              "else","while","do","for" };//�ؼ���
int judge_token() {
    memset(token, '\0', sizeof(token));
    token_cnt = 0;
    //�ؼ��ֻ��ʶ��
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
                return i + 28;//�ؼ���
            }
        }
        return 1;//��ʶ��
    }
        //�����򸡵���
    else if (ch >= '0' && ch <= '9') {
        int flag = 0;
        while ((ch >= '0' && ch <= '9') || ch == '.') {
            if (ch == '.')
                flag = 1;
            token[token_cnt++] = ch;
            ch = input[input_cnt++];
        }
        if (flag == 0) return 2;//����
        else return 3;//������
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
//�ʷ�����
int turn()
{
    int temp;
    ch = input[input_cnt++];
    cout << "�ʷ�����:\nvalue\tindex\tclass" << endl;
    while (true) {
        temp = judge_token();
        if (temp==-1)
            return 1;
        if (temp>=1&&temp<=3)
        {
            if(temp==1)
                cout << token << "\t" << temp << "\t��ʶ��" << endl;
            else if(temp==2)
                cout << token << "\t" << temp << "\t����" << endl;
            else if(temp==3)
                cout << token << "\t" << temp << "\t������" << endl;
            c1[c1_length++]='i';
        }
        else if(temp>=5&&temp<=8||temp==19||temp==20)
        {
            cout << token << "\t" << temp << "\t�����" << endl;
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
            cout << "�ʷ�����!" << endl;
            return 0;
        }
    }
}

int pos = 0; // ��ǰɨ�赽��λ��
// ��������
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
            cout<<"Error:ȱ��')'!"<<endl;
            return false;
        }
    } else {
        cout<<"Error:ȱ��һ������!"<<endl;
        return false;
    }
}
int main() {
    cout<<"������Ҫ�����ľ���:";
    cin >> input;
    while(turn())
    {
        // ��E��ʼ����
        cout<<"\n�﷨����:"<<endl;
        if (E() && pos == c1_length)
            cout << "���ַ��������ķ�!\n" << endl;
        else
            cout << "���ַ����������ķ�!\n" << endl;
        memset(input, '\0', sizeof(input));
        input_cnt=0;
        memset(c1, '\0', sizeof(c1));
        c1_length=0;
        pos=0;
        cout<<"������Ҫ�����ľ���:";
        cin >> input;
    }
    return 0;
}