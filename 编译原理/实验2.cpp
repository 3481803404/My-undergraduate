#include<iostream>
#include<cstring>
using namespace std;
char vt[100];//终结符
char vn[100];//非终结符
char grammer[100][100];//产生式
int vtnum, vnnum, pronum;//终结符，非终结符以及产生式的个数
char firstSet[100][100];//first集
char followSet[100][100];//follow集
int M[100][100];//分析表
int VT(char ch) {
    int i;
    for (i = 0; i < vtnum; i++)
        if (vt[i] == ch)
            break;
    return i;
}
int VN(char ch) {
    int i;
    for (i = 0; i < vnnum; i++)
        if (vn[i] == ch)
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
//初始化
void init() {
    memset(grammer, '\0', sizeof(grammer));
    memset(firstSet, 0, sizeof(firstSet));
    memset(followSet, 0, sizeof(followSet));
    memset(M, -1, sizeof(M));
    cout<<"请输入文法的非终结符:"<<endl;
    cin>>vn;
    vnnum = strlen(vn);
    cout<<"请输入文法的终结符:"<<endl;
    cin>>vt;
    vtnum = strlen(vt);
    cout<<"请输入文法的产生式个数:"<<endl;
    cin>>pronum;
    cout<<"请输入文法:"<<endl;
    for (int i = 0; i < pronum; i++) {
        cin>>grammer[i];
        getchar();
    }
    for (int i = 0; i < vnnum; i++)
        first(vn[i]);//对每个非终结符求first集
    for (int i = 0; i < vnnum; i++)
        follow(vn[i]);//对每个非终结符求follow集
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
    cout<<"\nfirst集:\n"<<endl;
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
    cout<<"\nfollow集:\n"<<endl;
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
    cout<<"\n预测分析表:\n"<<endl;
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
//分析程序
void analysis() {
    while (true) {
        char stack[100] = { '\0' };//符号栈
        int sp = 0;//栈顶指针
        int p = 0;//读头
        int step = 0;
        int flag = -1;//识别成功标志
        char c1[20] = { '\0' };//输入串
        char c2;//已弹出的栈顶元素
        stack[sp++] = '#';
        stack[sp++] = vn[0];
        cout<<"\n请输入要分析的句型(以#结尾):"<<endl;
        cin>>c1;
        for(char i : c1) {
            if(i == '#')flag=0;
        }
        if(flag==-1)break;
        cout<<"\n分析过程:\n\nstep\tstack\t\tinput\t\taction"<<endl;
        while (c1[p] != '\0') {
            cout<<step++<<"\t"<<stack<<"\t\t";
            for (int i = 0; i<p; i++)
                cout<<" ";
            for (int i = p; c1[i] != '\0'; i++)
                cout<<c1[i];
            cout<<"\t\t";
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
            cout<<"\n该字符串符合文法!\n"<<endl;
        else
            cout<<"\n\n该字符串不符合文法!\n"<<endl;
    }
}
int main() {
    init();
    pridict();
    print_pridict();
    analysis();
    return 0;
}