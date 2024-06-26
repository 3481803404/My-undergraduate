#include<iostream>
#include<cstring>
using namespace std;

char input[100];//输入串
int input_cnt=0;
char c1[100] ="";//处理后的输入串
int c1_cnt=0;
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
        {
            c1[c1_cnt++]='#';
            return 1;
        }
        if (temp>=1&&temp<=3)
        {
            if(temp==1)
                cout << token << "\t" << temp << "\t标识符" << endl;
            else if(temp==2)
                cout << token << "\t" << temp << "\t整数" << endl;
            else if(temp==3)
                cout << token << "\t" << temp << "\t浮点数" << endl;
            c1[c1_cnt++]='i';
        }
        else if(temp>=5&&temp<=8||temp==19||temp==20)
        {
            cout << token << "\t" << temp << "\t运算符" << endl;
            switch (temp) {
                case 5:c1[c1_cnt++]='+';break;
                case 6:c1[c1_cnt++]='-';break;
                case 7:c1[c1_cnt++]='*';break;
                case 8:c1[c1_cnt++]='/';break;
                case 19:c1[c1_cnt++]='(';break;
                case 20:c1[c1_cnt++]=')';break;
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

char vt[100]="i+-*/()";//终结符
char vn[100]="EeTtF";//非终结符
char grammer[100][100]={"E->Te","e->+Te","e->-Te","e->$","T->Ft",
                        "t->*Ft","t->/Ft","t->$","F->i","F->(E)"};//产生式
int vtnum, vnnum, pronum=10;//终结符，非终结符以及产生式的个数
char firstSet[100][100];//first集
char followSet[100][100];//follow集
int M[100][100];//分析表

int VT(char c) {
    int i;
    for (i = 0; i < vtnum; i++)
        if (vt[i] == c)
            break;
    return i;
}
int VN(char c) {
    int i;
    for (i = 0; i < vnnum; i++)
        if (vn[i] == c)
            break;
    return i;
}
//判断first集中是否包含空字符
bool haveEmpty(char c) {
    int i = VT(c);
    if (i < vtnum)
        return false;
    else
        return firstSet[VN(c)][vtnum] == 1;
}
//加入first集
void addfirstSet(char X, char c) {
    int i = VT(c);
    int j = VN(X);
    if (i < vtnum)//c是终结符则c属于first(X)
        firstSet[j][i] = 1;
    else {//c是非终结符则first(c)属于first(X)
        i = VN(c);
        for (int k = 0; k <= vtnum; k++)
            if (firstSet[i][k] == 1)
                firstSet[j][k] = 1;
    }
}
//求非终结符的first集
void first(char X) {
    for (int i = 0; i < pronum; i++) {
        if (grammer[i][0] == X) {//找X所对应的产生式
            if (grammer[i][3] == '$')//产生式右部为空
                firstSet[VN(X)][vtnum] = 1;
            else {//产生式右部非空
                int p = 3;
                while (grammer[i][p] != '\0') {
                    int q = VT(grammer[i][p]);
                    if (q < vtnum) {//产生式右部第一个字符为终结符
                        addfirstSet(X, grammer[i][p]);
                        break;
                    }
                    else {//产生式右部第一个字符为非终结符
                        q = VN(grammer[i][p]);
                        int flag = 0;
                        for (int m = 0; m <= vtnum; m++)
                            if (firstSet[q][m] == 1)
                            {
                                flag = 1;
                                break;
                            }
                        if (flag == 0)//First集未被求过
                            first(grammer[i][p]);
                        addfirstSet(X, grammer[i][p]);
                        if (haveEmpty(grammer[i][p]))//首字符First集是否为空，是则继续，否则跳出while循环
                            p++;
                        else
                            break;
                    }
                }
            }
        }
    }
}
//加入follow集，type=0表示将follow加入，type=1表示将first加入
void addfollowSet(char X, char c, int type) {
    int i = VT(c);
    int j = VN(X);
    if (i < vtnum)//c是终结符
        followSet[j][i] = 1;
    else {//c是非终结符
        int m = VN(c);
        if (type == 0) {
            for (int k = 0; k <= vtnum; k++)
                if (followSet[m][k] == 1)
                    followSet[j][k] = 1;
        }
        else {
            for (int k = 0; k < vtnum; k++)
                if (firstSet[m][k] == 1)
                    followSet[j][k] = 1;
        }
    }
}
//求非终结符的follow集
void follow(char X) {
    followSet[0][vtnum] = 1;//将#加入开始符的follow集
    for (int i = 0; i < pronum; i++) {
        int p = 3;
        while (grammer[i][p] != '\0' && grammer[i][p] != X)
            p++;
        if (grammer[i][p++] == X) {
            if (grammer[i][p] == '\0') {//X是右部的最后一个字符
                int j = VN(grammer[i][0]);
                int flag = 0;
                for (int k = 0; k <= vtnum; k++) {
                    if (followSet[j][k] == 1) {
                        flag = 1;
                        break;
                    }
                }
                if (flag == 0 && grammer[i][0] != X)//follow集未被求过且是左部字符不为X
                    follow(grammer[i][0]);
                addfollowSet(X, grammer[i][0], 0);
            }
            else {//X不是右部最后一个字符
                while (haveEmpty(grammer[i][p]) && grammer[i][p] != '\0') {
                    addfollowSet(X, grammer[i][p], 1);
                    int j = VN(grammer[i][0]);
                    int flag = 0;
                    for (int k = 0; k <= vtnum; k++) {
                        if (followSet[j][k] == 1)
                        {
                            flag = 1;
                            break;
                        }
                    }
                    if (flag == 0 && grammer[i][0] != X)//follow集未被求过且是左部字符不为X
                        follow(grammer[i][0]);
                    addfollowSet(X, grammer[i][0], 0);
                    p++;
                }
                if (!haveEmpty(grammer[i][p]))
                    addfollowSet(X, grammer[i][p], 1);
            }
        }
    }
}
//构造预测分析表
void pridict() {
    for (int x = 0; x < pronum; x++) {
        int i = VN(grammer[x][0]);
        int m = VT(grammer[x][3]);
        if (grammer[x][3] != '$') {
            for (int j = 0; j < vtnum; j++) {//对每个终结符a属于firstSet(grammer[x][0]),把文法grammer[x]加入到M[x][a]
                if (firstSet[i][j] == 1) {
                    if (m < vtnum) {//m是终结符
                        if (M[i][j] == -1 && grammer[x][3] == vt[j])
                            M[i][j] = x;
                    }
                    else {//m是非终结符
                        int n = VN(grammer[x][3]);
                        if (M[i][j] == -1 && firstSet[n][j] == 1)
                            M[i][j] = x;
                    }
                }
            }
        }
        if (haveEmpty(grammer[x][0])) {//若$属于firstSet(grammer[x][0]),对任何b属于followSet(grammer[x][0])把grammer[x]加入到M[X][b]
            for (int k = 0; k <= vtnum; k++) {
                if (followSet[i][k] == 1)
                    M[i][k] = x;
            }
        }
    }
}
//输出first集，follow集，预测分析表
void print_pridict() {
    cout<<"终结符:"<<vt<<endl;
    cout<<"非终结符:"<<vn<<endl;
    cout<<"文法产生式:"<<endl;
    for (int i = 0; i < pronum; ++i)
        cout<<grammer[i]<<endl;
    cout<<"first集:"<<endl;
    for (int i = 0; i < vnnum; i++) {
        cout<<"first("<<vn[i]<<")={";
        for (int j = 0; j < vtnum; j++) {
            if(firstSet[i][j])
                cout<<" "<<vt[j];
        }
        if(firstSet[i][vtnum])
            cout<<" $";
        cout<<" }"<<endl;
    }
    cout<<"follow集:"<<endl;
    for (int i = 0; i < vnnum; i++) {
        cout<<"follow("<<vn[i]<<")={";
        for (int j = 0; j < vtnum; j++) {
            if(followSet[i][j])
                cout<<" "<<vt[j];
        }
        if(followSet[i][vtnum])
            cout<<" #";
        cout<<" }"<<endl;
    }
    cout<<"预测分析表:"<<endl;
    cout<<"\t";
    for (int i = 0; i < vtnum; i++)
        cout<< vt[i] <<"\t";
    cout<<"#\t"<<endl;
    for (int i = 0; i < vnnum; i++) {
        cout<< vn[i] <<"\t";
        for (int j = 0; j <= vtnum; j++) {
            if (M[i][j] != -1) {
                int k = M[i][j];
                cout<< grammer[k] <<"\t";
            }
            else cout<<"\t";
        }
        cout<<endl;
    }
}
//初始化
void init() {
    memset(input, '\0', sizeof(input));
    memset(firstSet, 0, sizeof(firstSet));
    memset(followSet, 0, sizeof(followSet));
    memset(M, -1, sizeof(M));
    vtnum = strlen(vt);
    vnnum = strlen(vn);
    for (int i = 0; i < vnnum; i++)
        first(vn[i]);//对每个非终结符求first集
    for (int i = 0; i < vnnum; i++)
        follow(vn[i]);//对每个非终结符求follow集
    pridict();
    print_pridict();
}
//分析程序
void analysis() {
    cout<<"请输入要分析的句型:";
    cin >> input;
    while(turn())
    {
        char stack[100] = { '\0' };//符号栈
        int sp = 0;//栈顶指针
        int p = 0;//读头
        int step = 0;
        int flag = 0;//识别成功标志
        char c2;//已弹出的栈顶元素
        stack[sp++] = '#';
        stack[sp++] = vn[0];
        cout<<"语法分析:\nstep\tstack\t\tinput";
        for (int i = 0; i<c1_cnt; i++)
            cout<<" ";
        cout<<"action"<<endl;
        while (c1[p] != '\0') {
            cout<<step++<<"\t"<<stack<<"\t";
            if(sp<8)cout<<"\t";
            for (int i = 0; i<p; i++)
                cout<<" ";
            for (int i = p; i<c1_cnt; i++)
                cout<<c1[i];
            cout<<"     ";
            c2 = stack[--sp];
            stack[sp] = '\0';//弹出栈顶元素,并把栈顶后一位赋值为'\0',sp指向栈顶后一位
            if (VT(c2) < vtnum) {//c2是终结符
                if (c2 == c1[p]) {
                    p++;
                    cout<<"匹配"<<c2<<endl;
                }
                else break;
            }
            else if(c2 == '#') {//c2是#
                if (c1[p] == c2) {
                    flag = 1;//运行成功
                    cout<<"识别成功"<<endl;
                }
                break;
            }
            else {//c2是非终结符
                int j = M[VN(c2)][VT(c1[p])];//查预测分析表
                if (j != -1) {
                    cout<<"推导"<<grammer[j]<<endl;//输出对应的文法
                    if (grammer[j][3] != '$') {//逆序压入,忽略空字符$
                        int k = 3;
                        while (grammer[j][k] != '\0')
                            k++;
                        for (int t = k - 1; t >= 3; t--) {
                            stack[sp++] = grammer[j][t];
                            stack[sp] = '\0';
                        }
                    }
                }
                else break;
            }
        }
        if (flag == 1)
            cout<<"该字符串符合文法!\n"<<endl;
        else
            cout<<"\n该字符串不符合文法!\n"<<endl;
        memset(input, '\0', sizeof(input));
        input_cnt=0;
        memset(c1,'\0',sizeof(c1));
        c1_cnt=0;
        cout<<"请输入要分析的句型:";
        cin >> input;
    }
}
int main() {
    init();
    analysis();
    return 0;
}