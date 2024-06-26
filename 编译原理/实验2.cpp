#include<iostream>
#include<cstring>
using namespace std;
char vt[100];//�ս��
char vn[100];//���ս��
char grammer[100][100];//����ʽ
int vtnum, vnnum, pronum;//�ս�������ս���Լ�����ʽ�ĸ���
char firstSet[100][100];//first��
char followSet[100][100];//follow��
int M[100][100];//������
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
//��ʼ��
void init() {
    memset(grammer, '\0', sizeof(grammer));
    memset(firstSet, 0, sizeof(firstSet));
    memset(followSet, 0, sizeof(followSet));
    memset(M, -1, sizeof(M));
    cout<<"�������ķ��ķ��ս��:"<<endl;
    cin>>vn;
    vnnum = strlen(vn);
    cout<<"�������ķ����ս��:"<<endl;
    cin>>vt;
    vtnum = strlen(vt);
    cout<<"�������ķ��Ĳ���ʽ����:"<<endl;
    cin>>pronum;
    cout<<"�������ķ�:"<<endl;
    for (int i = 0; i < pronum; i++) {
        cin>>grammer[i];
        getchar();
    }
    for (int i = 0; i < vnnum; i++)
        first(vn[i]);//��ÿ�����ս����first��
    for (int i = 0; i < vnnum; i++)
        follow(vn[i]);//��ÿ�����ս����follow��
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
    cout<<"\nfirst��:\n"<<endl;
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
    cout<<"\nfollow��:\n"<<endl;
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
    cout<<"\nԤ�������:\n"<<endl;
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
//��������
void analysis() {
    while (true) {
        char stack[100] = { '\0' };//����ջ
        int sp = 0;//ջ��ָ��
        int p = 0;//��ͷ
        int step = 0;
        int flag = -1;//ʶ��ɹ���־
        char c1[20] = { '\0' };//���봮
        char c2;//�ѵ�����ջ��Ԫ��
        stack[sp++] = '#';
        stack[sp++] = vn[0];
        cout<<"\n������Ҫ�����ľ���(��#��β):"<<endl;
        cin>>c1;
        for(char i : c1) {
            if(i == '#')flag=0;
        }
        if(flag==-1)break;
        cout<<"\n��������:\n\nstep\tstack\t\tinput\t\taction"<<endl;
        while (c1[p] != '\0') {
            cout<<step++<<"\t"<<stack<<"\t\t";
            for (int i = 0; i<p; i++)
                cout<<" ";
            for (int i = p; c1[i] != '\0'; i++)
                cout<<c1[i];
            cout<<"\t\t";
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
            cout<<"\n���ַ��������ķ�!\n"<<endl;
        else
            cout<<"\n\n���ַ����������ķ�!\n"<<endl;
    }
}
int main() {
    init();
    pridict();
    print_pridict();
    analysis();
    return 0;
}