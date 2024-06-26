#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>

using namespace std;

string V[100][2]; //�洢��ֺ���ķ�
int vi = 0; //�洢��ֺ��ж�����
int t = 0;
int s = 0;
int l;
int r;

string FIRSTVT[20][2]; //�洢firstvt��
string LASTVT[20][2]; //�洢lastvt��
string str; //�洢�����ķ�
string str_a = "#"; // �¶�ջ
string str_b; // ʣ�����봮
string analysis_table[100][5]; // �ķ���������

char table[100][100]; // ������ȹ�ϵ��

void init_string(string &a) // ɾ���ַ����ĵ�һ��Ԫ��
{
    for (int i = 1; i <= a.length(); ++i)
    {
        a[i - 1] = a[i];
    }
}

bool is_CHAR(char c) // �ж��Ƿ�Ϊ��д��ĸ
{
    if (c >= 'A' && c <= 'Z')
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool is_in(int i, string x) // �жϴ��ַ���x�����һ����ʼ����������i���ַ��Ƿ��зǴ�д��ĸ
{
    bool flag = false;
    for (int j = 0; j < i; j++)
    {
        if (!is_CHAR(x[x.length() - j - 1]))
        {
            flag = true;
        }
    }
    return flag;
}

void split(string a) // ����ķ���ʹ�䲻���У�
{
    for (int i = 3; i < a.length(); ++i)
    {
        V[vi][0] = a[0];
        while (a[i] != '|' && i < a.length())
        {
            V[vi][1] += a[i];
            i++;
        }
        vi++;
    }
}

void read_file(string file_path) //���ж�ȡ�ļ�
{
    fstream f(file_path);
    vector<string> words;
    string line;
    while (getline(f, line))
    {
        words.push_back(line);
    }
    cout << "�����ķ�:" << endl;
    for (int i = 0; i < words.size(); i++)
    {
        cout << words[i] << endl;
        split(words[i]);
    }
}

int find_index(char a) //Ѱ���ַ�a��firstvt����lastvt�е�λ��
{
    for (int i = 0; i < t; ++i)
    {
        if (FIRSTVT[i][0][0] == a)
        {
            return i;
        }
    }
    return -1;
}

int find_table_index(char a) //Ѱ���ַ�a��������ȹ�ϵ���е�λ��
{
    for (int i = 0; i <= s; ++i)
    {
        if (table[i][0] == a)
        {
            return i;
        }
    }
    return -1;
}

void get_start() //��ȡ���ս��
{
    for (int i = 0; i < vi; ++i)
    {
        bool flag = true;
        for (int j = 0; j < t; ++j)
        {
            if (FIRSTVT[j][0] == V[i][0])
            {
                flag = false;
            }
        }
        if (flag)
        {
            FIRSTVT[t][0] = V[i][0];
            LASTVT[t][0] = V[i][0];
            t++;
        }
    }
}

void add_firstvt(string b, int a) //�ж��ַ���b�Ƿ������Ϊa��firstvt�У�û�������
{
    for (int s = 0; s < b.length(); ++s)
    {
        bool flag = true;
        char c = b[s];
        if (c <= 'Z' && c >= 'A')
        {
            continue;
        }
        for (int i = 0; i < FIRSTVT[a][1].length(); ++i)
        {
            if (c == FIRSTVT[a][1][i])
            {
                flag = false;
            }
        }
        if (flag)
        {
            FIRSTVT[a][1] += c;
        }
    }
}

void add_firstvt(char c, int a) //�ж��ַ�c�Ƿ������Ϊa��firstvt�У�û�������
{
    bool flag = true;
    for (int i = 0; i < FIRSTVT[a][1].length(); ++i)
    {
        if (c <= 'Z' && c >= 'A')
        {
            continue;
        }
        if (c == FIRSTVT[a][1][i])
        {
            flag = false;
        }
    }
    if (flag)
    {
        FIRSTVT[a][1] += c;
    }
}

void add_lastvt(string b, int a) //�ж��ַ���b�Ƿ������Ϊa��lastvt�У�û�������
{
    for (int s = 0; s < b.length(); ++s)
    {
        bool flag = true;
        char c = b[s];
        if (c <= 'Z' && c >= 'A')
        {
            continue;
        }
        for (int i = 0; i < LASTVT[a][1].length(); ++i)
        {
            if (c == LASTVT[a][1][i])
            {
                flag = false;
            }
        }
        if (flag)
        {
            LASTVT[a][1] += c;
        }
    }
}

void add_lastvt(char c, int a) //�ж��ַ���c�Ƿ������Ϊa��lastvt�У�û�������
{
    bool flag = true;
    for (int i = 0; i < LASTVT[a][1].length(); ++i)
    {
        if (c <= 'Z' && c >= 'A')
        {
            continue;
        }
        if (c == LASTVT[a][1][i])
        {
            flag = false;
        }
    }
    if (flag)
    {
        LASTVT[a][1] += c;
    }
}

string get_cur_firstvt(char c, int a) //��ȡ��ǰ�ַ���firstvt�����������Ϊa��firstvt��
{
    string temp;
    for (int i = 0; i < vi; ++i)
    {
        if (c == V[i][0][0])
        {
            if (!(V[i][1][0] <= 'Z' && V[i][1][0] >= 'A'))
            {
                add_firstvt(V[i][1][0], a);
            }
            else
            {
                if (c != V[i][1][0])
                {
                    temp = get_cur_firstvt(V[i][1][0], find_index(V[i][1][0]));
                    add_firstvt(temp, a);
                }
                if (V[i][1].length() > 2)
                {
                    if (!(V[i][1][1] <= 'Z' && V[i][1][1] >= 'A'))
                    {
                        add_firstvt(V[i][1][1], a);
                    }
                }
            }
        }
    }
    return FIRSTVT[a][1];
}

string get_cur_lastvt(char c, int a) //��ȡ��ǰ�ַ���lastvt�����������Ϊa��lastvt��
{
    string temp;
    for (int i = 0; i < vi; ++i)
    {
        int s = V[i][1].length();
        if (c == V[i][0][0])
        {
            if (!(V[i][1][s - 1] <= 'Z' && V[i][1][s - 1] >= 'A'))
            {
                add_lastvt(V[i][1][s - 1], a);
            }
            else
            {
                if (c != V[i][1][s - 1])
                {
                    temp = get_cur_lastvt(V[i][1][s - 1], find_index(V[i][1][0]));
                    add_lastvt(temp, a);
                }
                if (V[i][1].length() > 2)
                {
                    if (!(V[i][1][s - 2] <= 'Z' && V[i][1][s - 2] >= 'A'))
                    {
                        add_lastvt(V[i][1][s - 2], a);
                    }
                }
            }
        }
    }
    return LASTVT[a][1];
}


void get_firstvt() //��ȡ�����ķ���firstvt
{
    for (int i = 0; i < t; i++)
    {
        get_cur_firstvt(FIRSTVT[i][0][0], i);
    }
}

void get_lastvt() //��ȡ�����ķ���lastvt
{
    for (int i = 0; i < t; i++)
    {
        get_cur_lastvt(LASTVT[i][0][0], i);
    }
}

void print_firstvt(string t, string a) //��ӡ���ռ���Ϊt��firstvt
{
    cout << "FIRSTVT(" << t << ") = {";
    for (int i = 0; i < a.length(); ++i)
    {
        if (i == a.length() - 1)
        {
            cout << "\"" << a[i] << "\"";
        }
        else
        {
            cout << "\"" << a[i] << "\"" << ", ";
        }
    }
    cout << "}" << endl;
}

void print_lastvt(string t, string a) //��ӡ���ս��Ϊt��lastvt
{
    cout << "LASTVT(" << t << ") = {";
    for (int i = 0; i < a.length(); ++i)
    {
        if (i == a.length() - 1)
        {
            cout << "\"" << a[i] << "\"";
        }
        else
        {
            cout << "\"" << a[i] << "\"" << ", ";
        }
    }
    cout << "}" << endl;
}

void init_table() //��ʼ��������ȹ�ϵ��
{
    table[0][0] = '\\';
    for (int i = 0; i < vi; ++i)
    {
        for (int j = 0; j < V[i][1].length(); ++j)
        {
            bool flag = true;
            for (int k = 0; k < s + 1; ++k)
            {
                if (table[k + 1][0] == V[i][1][j] || (V[i][1][j] <= 'Z' && V[i][1][j] >= 'A'))
                {
                    flag = false;
                }
            }
            if (flag)
            {
                table[s + 1][0] = V[i][1][j];
                table[0][s + 1] = V[i][1][j];
                s++;
            }
        }
    }
    for (int l = 1; l < s + 1; ++l)
    {
        for (int i = 1; i < s + 1; ++i)
        {
            table[l][i] = ' ';
        }
    }
}

void get_table() //����������ȹ�ϵ��
{
    for (int i = 0; i < vi; ++i)
    {
        for (int j = 0; j < V[i][1].length(); ++j)
        {
            //ab
            if (!(V[i][1][j] <= 'Z' && V[i][1][j] >= 'A') && !(V[i][1][j + 1] <= 'Z' && V[i][1][j + 1] >= 'A') &&
                j + 1 < V[i][1].length())
            {
                table[find_table_index(V[i][1][j])][find_table_index(V[i][1][j + 1])] = '=';
            }
            //aQb
            if ((!(V[i][1][j] <= 'Z' && V[i][1][j] >= 'A')) && (V[i][1][j + 1] <= 'Z' && V[i][1][j + 1] >= 'A') &&
                (!(V[i][1][j + 2] <= 'Z' && V[i][1][j + 2] >= 'A')) && j + 2 < V[i][1].length())
            {
                table[find_table_index(V[i][1][j])][find_table_index(V[i][1][j + 2])] = '=';
            }
            //aQ
            if ((!(V[i][1][j] <= 'Z' && V[i][1][j] >= 'A')) && (V[i][1][j + 1] <= 'Z' && V[i][1][j + 1] >= 'A') &&
                j + 1 < V[i][1].length())
            {
                for (int k = 0; k < FIRSTVT[find_index(V[i][1][j + 1])][1].length(); ++k)
                {
                    table[find_table_index(V[i][1][j])][find_table_index(
                            FIRSTVT[find_index(V[i][1][j + 1])][1][k])] = '<';
                }
            }
            //Qa
            if ((V[i][1][j] <= 'Z' && V[i][1][j] >= 'A') && !(V[i][1][j + 1] <= 'Z' && V[i][1][j + 1] >= 'A') &&
                j + 1 < V[i][1].length())
            {
                for (int k = 0; k < LASTVT[find_index(V[i][1][j])][1].length(); ++k)
                {
                    table[find_table_index(LASTVT[find_index(V[i][1][j])][1][k])][find_table_index(
                            V[i][1][j + 1])] = '>';
                }
            }
        }
    }
}


void print_table() //��ӡ������ȹ�ϵ��
{
    for (int i = 0; i < s + 1; ++i)
    {
        for (int j = 0; j < s + 1; ++j)
        {
            cout << table[i][j] << "\t";
        }
        cout << endl;
    }
}

char get_relationship(char a, char b) //��ȡ�ս��a��b�����ȹ�ϵ
{
    return table[find_table_index(a)][find_table_index(b)];
}

bool is_reduce() //�ж��Ƿ���Թ�Լ
{
    for (int i = 0; i < vi; ++i)
    {
        int count = 0;
        int f = str_a.length() - 1;
        for (int j = V[i][1].length() - 1; j >= 0 && f >= 0; j--, f--)
        {
            if (is_in(V[i][1].length(), str_a))
            {
                if (is_CHAR(str_a[f]) && is_CHAR(V[i][1][j]))
                {
                    count++;
                }
                else if (str_a[f] == V[i][1][j])
                {
                    count++;
                }
            }
            else
            {
                continue;
            }
        }
        if (count == V[i][1].length())
        {
            r = i;
            return true;
        }
    }
    return false;
}


void analyze_input_string() // ������������ķ��ķ�������
{
    analysis_table[0][0] = "����";
    analysis_table[0][1] = "�¶�ջ";
    analysis_table[0][2] = "���ȹ�ϵ";
    analysis_table[0][3] = "ʣ�����봮";
    analysis_table[0][4] = "�ƽ����Լ";
    str_b = str;
    char relationship;
    l = 1;
    int x;
    stringstream ss;
    while (true)
    {
        ss << l;
        int index = str_a.length() - 1;
        analysis_table[l][0] = ss.str();
        analysis_table[l][3] = str_b;
        analysis_table[l][1] = str_a;
        ss.clear();
        ss.str("");
        if (is_CHAR(str_a[index]))
        {
            for (int i = str_a.length() - 1; i >= 0; i--)
            {
                if (!is_CHAR(str_a[i]))
                {
                    index = i;
                    break;
                }
            }
        }
        relationship = get_relationship(str_a[index], str_b[0]);
        analysis_table[l][2] = relationship;
        if (relationship == '=')
        {
            if (str_a[index] == '#' && str_b[0] == '#')
            {
                analysis_table[l][4] = "���";
                break;
            }
            else
            {
                analysis_table[l][4] = "�ƽ�";
                str_a += str_b[0];
                analysis_table[l + 1][1] = str_a;
                init_string(str_b);
            }
        }
        else if (relationship == '<')
        {
            analysis_table[l][4] = "�ƽ�";
            str_a += str_b[0];
            analysis_table[l + 1][1] = str_a;
            init_string(str_b);
        }
        else if (relationship == '>')
        {
            if (is_reduce())
            {
                analysis_table[l][4] = "��Լ";
                str_a[str_a.length() - V[r][1].length()] = V[r][0][0];
                str_a.erase(str_a.length() - V[r][1].length() + 1, V[r][1].length() - 1);
            }
            else
            {
                cout << "���봮�Ƿ�" << endl;
                exit(-1);
            }
        }
        l++;
    }
}

void print_analyze_process() //��ӡ��������ķ��ķ�������
{
    cout << "������ȷ�������" << endl;
    //cout << setw(12) << analysis_table[0][0] << setw(16) << analysis_table[0][1] << setw(16) << analysis_table[0][2]
    //     << setw(24)
    //     << analysis_table[0][3] << setw(20)
    //     << analysis_table[0][4] << endl;
    for (int i = 1; i <= l; ++i)
    {
        cout.width(10);
        cout << analysis_table[i][0];
        cout.width(12);
        cout << analysis_table[i][1];
        cout.width(10);
        cout << analysis_table[i][2];
        cout.width(20);
        cout << analysis_table[i][3];
        cout << analysis_table[i][4];
        cout << endl;
    }
}

int main(int argv, char *arg[])
{
    cout.setf(std::ios::left);
    cout<<"input filename:"<<endl;
    string filename;
    cin>>filename;
    read_file(filename);
    cout << "��ֺ��ķ�:" << endl;
    for (int i = 0; i < vi; ++i)
    {
        cout << V[i][0] << "->" << V[i][1] << endl;
    }
    cout << "���ս��:" << endl;
    get_start();
    for (int j = 0; j < t; ++j)
    {
        cout << FIRSTVT[j][0] << endl;
    }
    cout << "FIRSTVT:" << endl;
    get_firstvt();
    for (int k = 0; k < t; ++k)
    {
        print_firstvt(FIRSTVT[k][0], FIRSTVT[k][1]);
    }
    cout << "LASTVT:" << endl;
    get_lastvt();
    for (int k = 0; k < t; ++k)
    {
        print_lastvt(LASTVT[k][0], LASTVT[k][1]);
    }
    cout << "������ȹ�ϵ��" << endl;
    init_table();
    get_table();
    print_table();
    cout << "�������ķ�����#����:" << endl;
    cin >> str;
    analyze_input_string();
    print_analyze_process();
    return 0;
}