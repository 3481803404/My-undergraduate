#include<iostream>
#include<cstring>
using namespace std;

char input[100];//���봮
int input_cnt=0;
char c1[100] ="";//���������봮
int c1_cnt=0;
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
        {
            c1[c1_cnt++]='#';
            return 1;
        }
        if (temp>=1&&temp<=3)
        {
            if(temp==1)
                cout << token << "\t" << temp << "\t��ʶ��" << endl;
            else if(temp==2)
                cout << token << "\t" << temp << "\t����" << endl;
            else if(temp==3)
                cout << token << "\t" << temp << "\t������" << endl;
            c1[c1_cnt++]='i';
        }
        else if(temp>=5&&temp<=8||temp==19||temp==20)
        {
            cout << token << "\t" << temp << "\t�����" << endl;
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
            cout << "�ʷ�����!" << endl;
            return 0;
        }
    }
}

char vt[100]="i+-*/()";//�ս��
char vn[100]="EeTtF";//���ս��
char grammer[100][100]={"E->Te","e->+Te","e->-Te","e->$","T->Ft",
                        "t->*Ft","t->/Ft","t->$","F->i","F->(E)"};//����ʽ
int vtnum, vnnum, pronum=10;//�ս�������ս���Լ�����ʽ�ĸ���
char firstSet[100][100];//first��
char followSet[100][100];//follow��
int M[100][100];//������

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
//�ж�first�����Ƿ�������ַ�
bool haveEmpty(char c) {
    int i = VT(c);
    if (i < vtnum)
        return false;
    else
        return firstSet[VN(c)][vtnum] == 1;
}
//����first��
void addfirstSet(char X, char c) {
    int i = VT(c);
    int j = VN(X);
    if (i < vtnum)//c���ս����c����first(X)
        firstSet[j][i] = 1;
    else {//c�Ƿ��ս����first(c)����first(X)
        i = VN(c);
        for (int k = 0; k <= vtnum; k++)
            if (firstSet[i][k] == 1)
                firstSet[j][k] = 1;
    }
}
//����ս����first��
void first(char X) {
    for (int i = 0; i < pronum; i++) {
        if (grammer[i][0] == X) {//��X����Ӧ�Ĳ���ʽ
            if (grammer[i][3] == '$')//����ʽ�Ҳ�Ϊ��
                firstSet[VN(X)][vtnum] = 1;
            else {//����ʽ�Ҳ��ǿ�
                int p = 3;
                while (grammer[i][p] != '\0') {
                    int q = VT(grammer[i][p]);
                    if (q < vtnum) {//����ʽ�Ҳ���һ���ַ�Ϊ�ս��
                        addfirstSet(X, grammer[i][p]);
                        break;
                    }
                    else {//����ʽ�Ҳ���һ���ַ�Ϊ���ս��
                        q = VN(grammer[i][p]);
                        int flag = 0;
                        for (int m = 0; m <= vtnum; m++)
                            if (firstSet[q][m] == 1)
                            {
                                flag = 1;
                                break;
                            }
                        if (flag == 0)//First��δ�����
                            first(grammer[i][p]);
                        addfirstSet(X, grammer[i][p]);
                        if (haveEmpty(grammer[i][p]))//���ַ�First���Ƿ�Ϊ�գ������������������whileѭ��
                            p++;
                        else
                            break;
                    }
                }
            }
        }
    }
}
//����follow����type=0��ʾ��follow���룬type=1��ʾ��first����
void addfollowSet(char X, char c, int type) {
    int i = VT(c);
    int j = VN(X);
    if (i < vtnum)//c���ս��
        followSet[j][i] = 1;
    else {//c�Ƿ��ս��
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
//����ս����follow��
void follow(char X) {
    followSet[0][vtnum] = 1;//��#���뿪ʼ����follow��
    for (int i = 0; i < pronum; i++) {
        int p = 3;
        while (grammer[i][p] != '\0' && grammer[i][p] != X)
            p++;
        if (grammer[i][p++] == X) {
            if (grammer[i][p] == '\0') {//X���Ҳ������һ���ַ�
                int j = VN(grammer[i][0]);
                int flag = 0;
                for (int k = 0; k <= vtnum; k++) {
                    if (followSet[j][k] == 1) {
                        flag = 1;
                        break;
                    }
                }
                if (flag == 0 && grammer[i][0] != X)//follow��δ������������ַ���ΪX
                    follow(grammer[i][0]);
                addfollowSet(X, grammer[i][0], 0);
            }
            else {//X�����Ҳ����һ���ַ�
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
                    if (flag == 0 && grammer[i][0] != X)//follow��δ������������ַ���ΪX
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
//����Ԥ�������
void pridict() {
    for (int x = 0; x < pronum; x++) {
        int i = VN(grammer[x][0]);
        int m = VT(grammer[x][3]);
        if (grammer[x][3] != '$') {
            for (int j = 0; j < vtnum; j++) {//��ÿ���ս��a����firstSet(grammer[x][0]),���ķ�grammer[x]���뵽M[x][a]
                if (firstSet[i][j] == 1) {
                    if (m < vtnum) {//m���ս��
                        if (M[i][j] == -1 && grammer[x][3] == vt[j])
                            M[i][j] = x;
                    }
                    else {//m�Ƿ��ս��
                        int n = VN(grammer[x][3]);
                        if (M[i][j] == -1 && firstSet[n][j] == 1)
                            M[i][j] = x;
                    }
                }
            }
        }
        if (haveEmpty(grammer[x][0])) {//��$����firstSet(grammer[x][0]),���κ�b����followSet(grammer[x][0])��grammer[x]���뵽M[X][b]
            for (int k = 0; k <= vtnum; k++) {
                if (followSet[i][k] == 1)
                    M[i][k] = x;
            }
        }
    }
}
//���first����follow����Ԥ�������
void print_pridict() {
    cout<<"�ս��:"<<vt<<endl;
    cout<<"���ս��:"<<vn<<endl;
    cout<<"�ķ�����ʽ:"<<endl;
    for (int i = 0; i < pronum; ++i)
        cout<<grammer[i]<<endl;
    cout<<"first��:"<<endl;
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
    cout<<"follow��:"<<endl;
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
    cout<<"Ԥ�������:"<<endl;
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
//��ʼ��
void init() {
    memset(input, '\0', sizeof(input));
    memset(firstSet, 0, sizeof(firstSet));
    memset(followSet, 0, sizeof(followSet));
    memset(M, -1, sizeof(M));
    vtnum = strlen(vt);
    vnnum = strlen(vn);
    for (int i = 0; i < vnnum; i++)
        first(vn[i]);//��ÿ�����ս����first��
    for (int i = 0; i < vnnum; i++)
        follow(vn[i]);//��ÿ�����ս����follow��
    pridict();
    print_pridict();
}
//��������
void analysis() {
    cout<<"������Ҫ�����ľ���:";
    cin >> input;
    while(turn())
    {
        char stack[100] = { '\0' };//����ջ
        int sp = 0;//ջ��ָ��
        int p = 0;//��ͷ
        int step = 0;
        int flag = 0;//ʶ��ɹ���־
        char c2;//�ѵ�����ջ��Ԫ��
        stack[sp++] = '#';
        stack[sp++] = vn[0];
        cout<<"�﷨����:\nstep\tstack\t\tinput";
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
            stack[sp] = '\0';//����ջ��Ԫ��,����ջ����һλ��ֵΪ'\0',spָ��ջ����һλ
            if (VT(c2) < vtnum) {//c2���ս��
                if (c2 == c1[p]) {
                    p++;
                    cout<<"ƥ��"<<c2<<endl;
                }
                else break;
            }
            else if(c2 == '#') {//c2��#
                if (c1[p] == c2) {
                    flag = 1;//���гɹ�
                    cout<<"ʶ��ɹ�"<<endl;
                }
                break;
            }
            else {//c2�Ƿ��ս��
                int j = M[VN(c2)][VT(c1[p])];//��Ԥ�������
                if (j != -1) {
                    cout<<"�Ƶ�"<<grammer[j]<<endl;//�����Ӧ���ķ�
                    if (grammer[j][3] != '$') {//����ѹ��,���Կ��ַ�$
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
            cout<<"���ַ��������ķ�!\n"<<endl;
        else
            cout<<"\n���ַ����������ķ�!\n"<<endl;
        memset(input, '\0', sizeof(input));
        input_cnt=0;
        memset(c1,'\0',sizeof(c1));
        c1_cnt=0;
        cout<<"������Ҫ�����ľ���:";
        cin >> input;
    }
}
int main() {
    init();
    analysis();
    return 0;
}