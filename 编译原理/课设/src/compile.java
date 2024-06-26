import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.Arrays;
import java.util.Objects;
import java.util.Stack;

public class compile {
    public compile(String FileName) //构造方法
    {
        long stime = System.nanoTime();
        try (BufferedReader reader = new BufferedReader(new FileReader("keyword.txt"))) {
            String line;
            for (int i = 0; (line = reader.readLine()) != null; i++) {
                String[] tokens = line.split("\\s+");
                propertytable[i] = new PropertyTable();
                propertytable[i].symbol=tokens[0];
                propertytable[i].code=tokens[1];
                propertytable_count++;
            }
        } catch (IOException e) {
            throw new IllegalArgumentException("keyword.txt not found!");
        }
        String string;
        try (BufferedReader reader = new BufferedReader(new FileReader(FileName))) {
            StringBuilder stringBuilder= new StringBuilder();
            String line;
            while ((line = reader.readLine()) != null) //逐行读取文件内容，不读取换行符和末尾的空格
                stringBuilder.append(line).append('\n');
            string = stringBuilder.toString();
        } catch (IOException e) {
            throw new IllegalArgumentException(FileName+" not found!");
        }
        Test(string); // 对程序进行词法分析
        while (E_i < symbol_count)
            Program();
        gen("end".toCharArray(),-1,-1,"end".toCharArray());
        quadtocom();
        time=(System.nanoTime()-stime)/1000000.;
    }
    public double getTime() //返回编译花费时间
    {
        return time;
    }
    public String getComCode() //返回汇编语言源程序
    {
        return ComCode;
    }
    public void printpropertytable() //打印词法分析符号表
    {
        System.out.println("\nPropertyTable:");
        for (int i = 0; i < propertytable_count; i++) {
            System.out.println(propertytable[i].symbol+"\t"+propertytable[i].code);
        }
    }
    public void printsymbol() //打印词法分析结果
    {
        System.out.println("\nSymbol:");
        System.out.println("line:\tcode:\tsign:");
        for (int i = 0; i < symbol_count; i++) {
            System.out.println(symbol[i].line+"\t\t"+symbol[i].code+"\t\t"+symbol[i].sign);
        }
    }
    public void printsigntable() //打印变量表
    {
        System.out.println("\nSignTable:");
        for (int i = 0; i < signtable_count; i++) {
            System.out.println(signtable[i].name+"\t"+signtable[i].cat+"\t");
        }
    }
    public void printquad() //打印中间代码四元式
    {
        System.out.println("\nQuad Code:");
        for (int i = 1; i < quad_i-1; i++)
            System.out.println(i+":\t("+quad[i].op+","
                    +quad[i].arg1+","+quad[i].arg2+","+quad[i].result+")");
    }
    public void printl() //打印汇编跳转标记
    {
        System.out.println("\nL:");
        for (int i=0; i < L_i; i++) // 打印
            System.out.println(L[i].i+"\t"+L[i].li);
    }
    private static double time; //编译花费时间
    private static String ComCode; //汇编语言源程序
    private static final int[][] action1= //表达式计算LR分析表
    { {  3, -1, -1, -1, -1,  2, -1, -1,  1},
      { -1,  4,  5,  6,  7, -1, -1, -2, -1},
      {  3, -1, -1, -1, -1,  2, -1, -1,  8},
      { -1,106,106,106,106, -1,106,106, -1},
      {  3, -1, -1, -1, -1,  2, -1, -1,  9},
      {  3, -1, -1, -1, -1,  2, -1, -1, 10},
      {  3, -1, -1, -1, -1,  2, -1, -1, 11},
      {  3, -1, -1, -1, -1,  2, -1, -1, 12},
      { -1,  4,  5,  6,  7, -1, 13, -1, -1},
      { -1,101,101,  6,  7, -1,101,101, -1},
      { -1,102,102,  6,  7, -1,102,102, -1},
      { -1,103,103,103,103, -1,103,103, -1},
      { -1,104,104,104,104, -1,104,104, -1},
      { -1,105,105,105,105, -1,105,105, -1}
    };
    private static final int[][] action2= //bool表达式LR分析表
    //{'i','r',(17,)18,!34,&14,|15,'#','E','A','O'}
    { {  1, -1,  4, -1,  5, -1, -1, -1, 13,  7,  8},//s0
      { -1,  2, -1,101, -1,101,101,101, -1, -1, -1},//s1
      {  3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},//s2
      { -1, -1, -1,102, -1,102,102,102, -1, -1, -1},//s3
      {  1, -1,  4, -1,  5, -1, -1, -1, 11,  7,  8},//s4
      {  1, -1,  4, -1,  5, -1, -1, -1,  6,  7,  8},//s5
      { -1, -1, -1,104, -1,104,104,104, -1, -1, -1},//s6
      {  1, -1,  4, -1,  5, -1, -1, -1, 14,  7,  8},//s7
      {  1, -1,  4, -1,  5, -1, -1, -1, 15,  7,  8},//s8
      {105, -1,105, -1,105, -1, -1, -1, -1, -1, -1},//s9
      {107, -1,107, -1,107, -1, -1, -1, -1, -1, -1},//s10
      { -1, -1, -1, 12, -1,  9, 10, -1, -1, -1, -1},//s11
      { -1, -1, -1,103, -1,103,103,103, -1, -1, -1},//s12
      { -1, -1, -1, -1, -1,  9, 10, -2, -1, -1, -1},//s13
      { -1, -1, -1,106, -1,106,106,106, -1, -1, -1},//s14
      { -1, -1, -1,108, -1,  9,108,108, -1, -1, -1} //s15
    };
    private static int line=1;//C源程序行数标记
    private static class PropertyTable {
        private String symbol;
        private String code;
    } //储存关键字
    private static final PropertyTable[] propertytable=new PropertyTable[1000];
    private static int propertytable_count=0;
    private static class Symbol {
        int line;
        private String sign;
        private int code;
    } //储存单词
    private static final Symbol[] symbol=new Symbol[1000];
    private static int symbol_count=0;
    private static class SignTable {
        private String name;
        private String cat;
        private int value;
    } //储存字符串变量
    private static final SignTable[] signtable=new SignTable[2000];
    private static int signtable_count=0;
    private static class Quad {
        String op;
        String arg1;
        String arg2;
        String result;
    } //储存四元式
    private static final Quad[] quad=new Quad[1000];
    private static int quad_i = 1;
    private static class Com {
        String lable;
        String op;
        String arg1;
        String arg2;
    } //储存汇编程序
    private static final Com[] com=new Com[1000];
    private static int com_i;
    private static class L {
        int i;
        String li;
    } //储存汇编跳转标签
    private static final L[] L=new L[1000];
    private static int L_i;
    private static int type=0; //词法分析时字符串类型
    private static int Flag; //是否需要中间变量
    private static int Temp; //中间变量
    private static int T_i = 1; //中间变量编号
    private static int sym1; //语义分析读头
    private static int E_i = 0; //语义分析读头下标
    private static int NXQ = 1; //下一个中间代码编号
    private static boolean E_err_flag=false; //赋值语句结尾)处理标识
    private static int E_TC; //bool语句真出口
    private static int E_FC; //bool语句假出口
    public static void Test(String string) //词法分析
    {
        int p=0;
        int k;
        char[] word = new char[20];
        char[] str=string.toCharArray();
        while(p<str.length){
            while(str[p]==' ')
                p++;
            if(str[p]=='\n')
                line++;
            k=0;
            Arrays.fill(word, ' ');
            word[k++]=str[p];
            if(Character.isAlphabetic(str[p])){
                p++;
                while ((Character.isAlphabetic(str[p]))||Character.isDigit(str[p])||str[p]=='_')
                    word[k++]=str[p++];
                String wordstr=new String(word).strip();
                switch (wordstr) {
                    case "int" -> type = 1;
                    case "float" -> type = 2;
                    case "char" -> type = 3;
                }
                find(word);
            }
            else if (Character.isDigit(str[p])) {
                p++;
                while (Character.isDigit(str[p]))
                    word[k++]=str[p++];
                find(word);
            }
            else if (str[p]=='<'||str[p]=='>'||str[p]=='='|| str[p]=='!'||str[p]=='&'||str[p]=='|') {
                p++;
                if(str[p-1]=='&'){
                    if(str[p]=='&')
                        word[k]=str[p++];
                }
                else if(str[p-1]=='|'){
                    if(str[p]=='|')
                        word[k]=str[p++];
                } else if (str[p]=='=') {
                    word[k]=str[p++];
                }
                find(word);
            }
            else {
                p++;
                if(Objects.equals(new String(word).strip(), ";"))
                    type=0;
                find(word);
            }
        }
    }
    public static void find(char[] word) //词法分析:字符串查表
    {
        if(Character.isDigit(word[0])) { //整数
            symbol[symbol_count] = new Symbol();
            symbol[symbol_count].line=line;
            symbol[symbol_count].sign= new String(word).strip();
            symbol[symbol_count++].code=2;
        }
        else {
            int i;
            for(i=0; i<propertytable_count; i++) {
                if(Objects.equals(propertytable[i].symbol, new String(word).strip())) {
                    symbol[symbol_count] = new Symbol();
                    symbol[symbol_count].line=line;
                    symbol[symbol_count].sign = new String(word).strip();
                    symbol[symbol_count++].code=Integer.parseInt(propertytable[i].code);
                    break;
                }
            }
            if(i==propertytable_count) {
                if(Character.isAlphabetic(word[0])) //如果符合就是字符串常量
                    findcreate(word);
                else if(word[0]>=32) {
                    symbol[symbol_count] = new Symbol();
                    symbol[symbol_count].line=line;
                    symbol[symbol_count].sign = new String(word).strip();
                    symbol[symbol_count++].code=0;
                }
            }
        }
    }
    private static void findcreate(char[] word) //词法分析:添加字符串常量表
    {
        int i;
        for(i=0; i<signtable_count; i++)
        {
            if(Objects.equals(signtable[i].name, new String(word).strip()))
            {
                symbol[symbol_count] = new Symbol();
                symbol[symbol_count].line=line;
                symbol[symbol_count].sign=new String(word).strip();
                symbol[symbol_count++].code=1;
                return;
            }
        }
        if(i==signtable_count && type>0) // 表示字符串常量表中没有单词p,就把p存储到字符串常量表和单词表中
        {
            symbol[symbol_count] = new Symbol();
            symbol[symbol_count].line=line;
            symbol[symbol_count].sign=new String(word).strip();
            symbol[symbol_count++].code=1;
            signtable[signtable_count] = new SignTable();
            signtable[signtable_count].name=new String(word).strip();
            switch (type) {
                case 1 -> signtable[signtable_count++].cat = "int";
                case 2 -> signtable[signtable_count++].cat = "float";
                case 3 -> signtable[signtable_count++].cat = "char";
            }
        }
        if(type==0) {
            throw new IllegalArgumentException(new String(word).strip() +" not defined");
        }
    }
    private static void Program() //语法及语义分析
    {
        int temp,q,efc_tmp;
        sym1=symbol[E_i++].code;
        if(sym1 >= 27 && sym1 <= 29) {
            sym1=symbol[E_i++].code;
            while(sym1 != 24) { //;
                if(sym1 == 1) {
                    sym1=symbol[E_i++].code;
                    if(sym1 == 16) { //=
                        char[] tempstring=symbol[E_i - 2].sign.toCharArray();
                        if(E()) {
                            temp=entry(tempstring);
                            if(Flag == 1)
                                gen("=".toCharArray(),Temp,-1,signtable[temp].name.strip().toCharArray());
                            else
                                gen("=".toCharArray(),1000 + T_i - 1,-1,signtable[temp].name.strip().toCharArray());
                        }
                    }
                    if(sym1 == 25) //,
                        sym1=symbol[E_i++].code;
                    else if(sym1 == 24)//;
                        break;
                    else {
                        throw new IllegalArgumentException("Find Error In Line "+ symbol[E_i-1].line +": Data Definition error!");
                    }
                }
                else {
                    throw new IllegalArgumentException("Find Error In Line "+ symbol[E_i-1].line +": Data Definition error!");
                }
            }
        }
        else if(sym1 == 1) {
            char[] tempstring=symbol[E_i - 1].sign.toCharArray();
            sym1=symbol[E_i++].code;
            if(sym1 == 16) {
                if(E()) {
                    temp=entry(tempstring);
                    if(Flag == 1)
                        gen("=".toCharArray(),Temp,-1,signtable[temp].name.strip().toCharArray());
                    else
                        gen("=".toCharArray(),1000 + T_i - 1,-1,signtable[temp].name.strip().toCharArray());
                }
                if(sym1 != 24) {
                    throw new IllegalArgumentException("Find Error In Line "+ symbol[E_i-1].line +": Data Assignment error!");
                }
            }
            else {
                throw new IllegalArgumentException("Find Error In Line "+ symbol[E_i-1].line +": Data Assignment error!");
            }
        }
        else if(sym1 == 30) { //if
            sym1=symbol[E_i++].code;
            if(sym1 == 17) {
                EB();
                if(sym1 == 18) {
                    Backpatch(E_TC ,NXQ);
                    efc_tmp=E_FC;
                    Program();
                    sym1=symbol[E_i++].code;
                    if(sym1 == 31) {
                        q=NXQ;
                        gen("j".toCharArray(),-1,-1,"0".toCharArray());
                        Backpatch(efc_tmp,NXQ);
                        Program();
                        quad[q].result=Integer.toString(NXQ);
                    }
                    else {
                        E_i--;
                        Backpatch(efc_tmp,NXQ);
                    }
                }
                else {
                    throw new IllegalArgumentException("Find Error In Line "+ symbol[E_i-1].line +": lack of ) in if sentences!");
                }
            }
            else {
                throw new IllegalArgumentException("Find Error In Line "+ symbol[E_i-1].line +": lack of ( in if sentences!");
            }
        }
        else if(sym1 == 32) { //while
            sym1=symbol[E_i++].code;
            if(sym1 == 17) {
                String s_q=Integer.toString(NXQ);
                EB();
                if(sym1 == 18) {
                    Backpatch(E_TC,NXQ);
                    efc_tmp=E_FC;
                    Program();
                    gen("j".toCharArray(),-1,-1,s_q.toCharArray());
                    Backpatch(efc_tmp, NXQ);
                }
                else {
                    throw new IllegalArgumentException("Find Error In Line "+ symbol[E_i-1].line +": lack of ) in while sentences!");
                }
            }
            else {
                throw new IllegalArgumentException("Find Error In Line "+ symbol[E_i-1].line +": lack of ( in while sentences!");
            }
        }
        else if(sym1 == 33) { //do
            q=NXQ;
            Program();
            sym1=symbol[E_i++].code;
            if(sym1 == 32) {
                sym1=symbol[E_i++].code;
                if(sym1 == 17) {
                    EB();
                    if(sym1 == 18) {
                        Backpatch(E_TC,q);
                        Backpatch(E_FC, NXQ);
                    }
                    else {
                        throw new IllegalArgumentException("Find Error In Line "+ symbol[E_i-1].line +": lack of ) in while sentences!");
                    }
                }
                else {
                    throw new IllegalArgumentException("Find Error In Line "+ symbol[E_i-1].line +": lack of ( in while sentences!");
                }
            }
        }
        else if (sym1 == 36) { //for
            String s_q,s_q1;
            int q1;
            sym1=symbol[E_i++].code;
            if(sym1 == 17) {
                Program();
                q1=NXQ;
                EB();
                q=NXQ;
                if(sym1==24) {
                    sym1=symbol[E_i++].code;
                    if(sym1 == 1) {
                        char[] tempstring = symbol[E_i - 1].sign.toCharArray();
                        sym1 = symbol[E_i++].code;
                        if (sym1 == 16) {
                            E_err_flag=true;
                            if (E()) {
                                temp = entry(tempstring);
                                if (Flag == 1)
                                    gen("=".toCharArray(), Temp, -1, signtable[temp].name.strip().toCharArray());
                                else
                                    gen("=".toCharArray(), 1000 + T_i - 1, -1, signtable[temp].name.strip().toCharArray());
                            }
                            E_err_flag=false;
                            if (sym1 != 18) {
                                throw new IllegalArgumentException("Find Error In Line "+ symbol[E_i-1].line +": Data Assignment error!");
                            }
                        } else {
                            throw new IllegalArgumentException("Find Error In Line "+ symbol[E_i-1].line +": Data Assignment error!");
                        }
                    }
                    s_q1=Integer.toString(q1);
                    gen("j".toCharArray(),-1,-1,s_q1.toCharArray());
                    if(sym1 == 18) {
                        Backpatch(E_TC,NXQ);
                        efc_tmp=E_FC;
                        Program();
                        s_q=Integer.toString(q);
                        gen("j".toCharArray(),-1,-1,s_q.toCharArray());
                        Backpatch(efc_tmp, NXQ);
                    }
                    else {
                        throw new IllegalArgumentException("Find Error In Line "+ symbol[E_i-1].line +": lack of ) in for sentences!");
                    }
                }
            }
            else {
                throw new IllegalArgumentException("Find Error In Line "+ symbol[E_i-1].line +": lack of ( in for sentences!");
            }
        }
        else if(sym1==21){
            while (E_i < symbol_count && symbol[E_i].code!=22)
                Program();
            E_i++;
        }
        //System.out.println("\nQuad Code:");
        //for (int i = 1; i < quad_i-1; i++)
        //    System.out.println(i+":\t("+quad[i].op+","
        //            +quad[i].arg1+","+quad[i].arg2+","+quad[i].result+")");
    }
    private static boolean E() //表达式计算
    {
        int sym,E1_place,E2_place,l,j,t;
        boolean acc;
        Stack<Integer> sp1= new Stack<>();
        Stack<Integer> sp3= new Stack<>();
        sp1.push(0);
        sp3.push((int) '@');
        Flag=0;
        sym1=symbol[E_i++].code;
        if(sym1 == 1 || sym1 == 2)
            sym='i';
        else
            sym=sym1;
        acc=false;
        while(!acc) {
            l= sp1.peek();
            j=precede1(sym);
            t=action1[l][j];
            if(t==-1) {
                if(sym1==18&&E_err_flag&&sp1.size()<=2)
                    return true;
                else {
                    throw new IllegalArgumentException("Find Error In Line "+ symbol[E_i-1].line +
                            ": Expression Error Because Of "+ symbol[E_i-1].sign +" !"+
                            "\nsp1:"+sp1+"\nsp3:"+sp3);
                }
            }
            if(t==-2)
                acc=true;
            if(t>=0 && t<100) {
                sp1.push(t);
                if(sym=='i')
                    sp3.push(entry(symbol[E_i-1].sign.toCharArray()));
                else
                    sp3.push((int) '@');
                sym1=symbol[E_i++].code;
                if(sym1==1 || sym1==2)
                    sym='i';
                else
                    sym=sym1;
            }
            if(t>=100 && t<=200)
                switch (t) {
                    case 101 -> {
                        sp1.pop();
                        sp1.pop();
                        sp1.pop();
                        E2_place=sp3.pop();
                        sp3.pop();
                        E1_place=sp3.pop();
                        Temp = newtemp('+', E1_place, E2_place);
                        t = sp1.peek();
                        j = precede1('E');
                        sp1.push(action1[t][j]);
                        gen("+".toCharArray(), E1_place, E2_place, "T".toCharArray());
                        T_i++;
                        sp3.push(Temp);
                    }
                    case 102 -> {
                        sp1.pop();
                        sp1.pop();
                        sp1.pop();
                        E2_place=sp3.pop();
                        sp3.pop();
                        E1_place=sp3.pop();
                        Temp = newtemp('-', E1_place, E2_place);
                        t = sp1.peek();
                        j = precede1('E');
                        sp1.push(action1[t][j]);
                        gen("-".toCharArray(), E1_place, E2_place, "T".toCharArray());
                        T_i++;
                        sp3.push(Temp);
                    }
                    case 103 -> {
                        sp1.pop();
                        sp1.pop();
                        sp1.pop();
                        E2_place=sp3.pop();
                        sp3.pop();
                        E1_place=sp3.pop();
                        Temp = newtemp('*', E1_place, E2_place);
                        t = sp1.peek();
                        j = precede1('E');
                        sp1.push(action1[t][j]);
                        gen("*".toCharArray(), E1_place, E2_place, "T".toCharArray());
                        T_i++;
                        sp3.push(Temp);
                    }
                    case 104 -> {
                        sp1.pop();
                        sp1.pop();
                        sp1.pop();
                        E2_place=sp3.pop();
                        sp3.pop();
                        E1_place=sp3.pop();
                        Temp = newtemp('/', E1_place, E2_place);
                        t = sp1.peek();
                        j = precede1('E');
                        sp1.push(action1[t][j]);
                        gen("/".toCharArray(), E1_place, E2_place, "T".toCharArray());
                        T_i++;
                        sp3.push(Temp);
                    }
                    case 105 -> {
                        sp1.pop();
                        sp1.pop();
                        sp1.pop();
                        sp3.pop();
                        Temp=sp3.pop();
                        sp3.pop();
                        t = sp1.peek();
                        j = precede1('E');
                        sp1.push(action1[t][j]);
                        sp3.push(Temp);
                    }
                    case 106 -> {
                        sp1.pop();
                        Temp=sp3.peek();
                        t = sp1.peek();
                        j = precede1('E');
                        sp1.push(action1[t][j]);
                        Flag++;
                    }
                }
        }
        return true;
    }
    private static void EB() //bool表达式
    {
        int sym,E1_place,E2_place,l,j,t,op,temp;
        int E_temp,EA_FC = 0,EO_TC = 0;
        boolean acc=false,bracket_flag=false;
        char[] tempstring = "".toCharArray();
        Stack<Integer> sp1=new Stack<>();
        Stack<Integer> sp2=new Stack<>();
        Stack<Integer> sp3=new Stack<>();
        sp1.push(0);
        sp2.push(24);
        sp3.push((int) '@');
        if(sym1==17) {
            bracket_flag=true;
            sym=sym1;
        }
        else {
            sym1=symbol[E_i++].code;
            if(sym1 == 1 || sym1 == 2) {
                tempstring=symbol[E_i - 1].sign.toCharArray();
                sym1=symbol[E_i].code;
                if(sym1>=3 && sym1<=6 || sym1 == 16) {
                    if(sym1==16) {
                        E_i++;
                        if(E()) {
                            temp=entry(tempstring);
                            gen("=".toCharArray(),1000+T_i-1,-1,signtable[temp].name.toCharArray());
                            E_i--;
                        }
                    }
                    else {
                        tempstring="T".toCharArray();
                        E_i--;
                        E();
                        T_i--;
                        E_i--;
                    }
                }
                sym='i';
            }
            else sym=sym1;
        }
        while(!acc) {
            l=sp1.peek();
            j=precede2(sym);
            t=action2[l][j];
            if(t==-1)  {
                throw new IllegalArgumentException("Find Error In Line "+ symbol[E_i-1].line +
                        ": Bool Expression Error Because Of "+ symbol[E_i-1].sign +" !"+
                        "\nsp1(state):"+sp1+"\nsp2(sign):"+sp2+"\nsp3(op):"+sp3);
            }
            if(t==-2) acc=true;
            if(t>=0 && t<100) {
                sp1.push(t);
                sp2.push(sym);
                if(sym=='i')
                    sp3.push(entry(tempstring));
                else
                    sp3.push((int) '@');
                sym1=symbol[E_i++].code;
                if(sym1==1 || sym1==2) {
                    tempstring=symbol[E_i - 1].sign.toCharArray();
                    sym1=symbol[E_i].code;
                    if(sym1 >= 3 && sym1 <= 6 || sym1 == 16) {
                        //NXQ_temp=NXQ;
                        if(sym1 == 16) {
                            E_i++;
                            if(E()) {
                                temp=entry(tempstring);
                                gen("=".toCharArray(),1000 + T_i - 1,-1,signtable[temp].name.toCharArray());
                                E_i--;
                            }
                        }
                        else {
                            tempstring="T".toCharArray();
                            E_i--;
                            E();
                            T_i--;
                            E_i--;
                        }
                    }
                    sym='i';
                }
                else sym=sym1;
            }
            if(t >= 100 && t <= 200)
                switch (t) {
                    case 101 -> {
                        sp1.pop();
                        sp2.pop();
                        E1_place=sp3.peek();
                        t = sp1.peek();
                        sp2.push((int) 'E');
                        j = precede2('E');
                        sp1.push(action2[t][j]);
                        E_TC = NXQ;
                        E_FC = NXQ + 1;
                        gen("jnz".toCharArray(), E1_place, -1, "0".toCharArray());
                        gen("j".toCharArray(), -1, -1, "0".toCharArray());
                    }
                    case 102 -> {
                        sp1.pop();
                        sp1.pop();
                        sp1.pop();
                        sp2.pop();
                        op=sp2.pop();
                        sp2.pop();
                        E2_place=sp3.pop();
                        sp3.pop();
                        E1_place=sp3.pop();
                        t = sp1.peek();
                        sp2.push((int) 'E');
                        j = precede2('E');
                        sp1.push(action2[t][j]);
                        E_TC = NXQ;
                        E_FC = NXQ + 1;
                        switch (op){
                            case 7 -> gen("j<".toCharArray(), E1_place, E2_place, "0".toCharArray());
                            case 8 -> gen("j<=".toCharArray(), E1_place, E2_place, "0".toCharArray());
                            case 9 -> gen("j==".toCharArray(), E1_place, E2_place, "0".toCharArray());
                            case 10 -> gen("j!=".toCharArray(), E1_place, E2_place, "0".toCharArray());
                            case 11 -> gen("j>".toCharArray(), E1_place, E2_place, "0".toCharArray());
                            case 12 -> gen("j>=".toCharArray(), E1_place, E2_place, "0".toCharArray());
                        }
                        gen("j".toCharArray(), -1, -1, "0".toCharArray());
                        sp3.push((int) '@');
                    }
                    case 103 -> {
                        sp1.pop();
                        sp1.pop();
                        sp1.pop();
                        sp2.pop();
                        sp2.pop();
                        sp2.pop();
                        sp3.pop();
                        temp=sp3.pop();
                        sp3.pop();
                        t = sp1.peek();
                        sp2.push((int) 'E');
                        j = precede2('E');
                        sp1.push(action2[t][j]);
                        sp3.push(temp);
                    }
                    case 104 -> { // !
                        sp1.pop();
                        sp1.pop();
                        sp2.pop();
                        sp2.pop();
                        sp3.pop();
                        sp3.pop();
                        t = sp1.peek();
                        sp2.push((int) 'E');
                        j = precede2('E');
                        sp1.push(action2[t][j]);
                        E_temp = E_FC;
                        E_FC = E_TC;
                        E_TC = E_temp;
                        sp3.push((int) '@');
                    }
                    case 105 -> { // &&
                        sp1.pop();
                        sp1.pop();
                        sp2.pop();
                        sp2.pop();
                        sp3.pop();
                        sp3.pop();
                        t = sp1.peek();
                        sp2.push((int) 'A');
                        j = precede2('A');
                        sp1.push(action2[t][j]);
                        Backpatch(E_TC, NXQ);
                        EA_FC = E_FC;
                        sp3.push((int) '@');
                    }
                    case 106 -> { // &&
                        sp1.pop();
                        sp1.pop();
                        sp2.pop();
                        sp2.pop();
                        sp3.pop();
                        sp3.pop();
                        t = sp1.peek();
                        sp2.push((int) 'E');
                        j = precede2('E');
                        sp1.push(action2[t][j]);
                        E_FC = Merg(EA_FC, E_FC);
                        sp3.push((int) '@');
                    }
                    case 107 -> { // ||
                        sp1.pop();
                        sp1.pop();
                        sp2.pop();
                        sp2.pop();
                        sp3.pop();
                        sp3.pop();
                        t = sp1.peek();
                        sp2.push((int) 'O');
                        j = precede2('O');
                        sp1.push(action2[t][j]);
                        Backpatch(E_FC, NXQ);
                        EO_TC = E_TC;
                        sp3.push((int) '@');
                    }
                    case 108 -> { // ||
                        sp1.pop();
                        sp1.pop();
                        sp2.pop();
                        sp2.pop();
                        sp3.pop();
                        sp3.pop();
                        t = sp1.peek();
                        sp2.push((int) 'E');
                        j = precede2('E');
                        sp1.push(action2[t][j]);
                        E_TC = Merg(EO_TC, E_TC);
                        sp3.push((int) '@');
                    }
                }
            //System.out.println("sp1(state):"+sp1);
            //System.out.println("sp2(sign):"+sp2);
            //System.out.println("sp3(op):"+sp3);
            //System.out.println("sym="+sym+",l="+l+",j="+j+",t="+t);
            //System.out.println();
            if(bracket_flag&&sp1.size()==3&&sp1.peek()==11&&sym1==18)
                return;
        }
    }
    private static int newtemp(char op,int E1_place,int E2_place) //新增中间变量
    {
        int temp = 0,temp1,temp2;
        String T="T"+T_i;
        if(E1_place>=10000)
            temp1=E1_place-10000;
        else
            temp1=signtable[E1_place].value;
        if(E2_place>=10000)
            temp2=E2_place-10000;
        else
            temp2=signtable[E2_place].value;
        switch (op) {
            case '+' -> temp=temp1+temp2;
            case '-' -> temp=temp1-temp2;
            case '*' -> temp=temp1*temp2;
            case '/' -> {
                if(temp2!=0)
                    temp=temp1/temp2;
            }
        }
        signtable[1000 + T_i]=new SignTable();
        signtable[1000 + T_i].name = T;
        signtable[1000 + T_i].value=temp;
        return 1000 + T_i;
    }
    private static void gen(char[] op,int arg1,int arg2,char[] result) //产生四元式
    {
        quad[quad_i]=new Quad();
        quad[quad_i].op = new String(op).strip();
        if(arg1 >= 10000)
            quad[quad_i].arg1 = symbol[findstring(arg1 - 10000)].sign;
        else if(arg1==-1)
            quad[quad_i].arg1 = "-";
        else
            quad[quad_i].arg1 = signtable[arg1].name;
        if(arg2>=10000)
            quad[quad_i].arg2 = symbol[findstring(arg2 - 10000)].sign;
        else if(arg2==-1)
            quad[quad_i].arg2 = "-";
        else
            quad[quad_i].arg2 = signtable[arg2].name;
        if(Arrays.equals(result, "T".toCharArray()))
            quad[quad_i].result = "T"+T_i;
        else
            quad[quad_i].result = new String(result).strip();
        quad_i++;
        NXQ=quad_i;
    }
    private static int findstring(int x) //查symbol表
    {
        for (int i = 0; i < symbol_count; i++)
            if(Character.isDigit(symbol[i].sign.toCharArray()[0]))
                if(Integer.parseInt(symbol[i].sign)==x)
                    return i;
        return -1;
    }
    private static void Backpatch(int p,int t) //回填
    {
        while(p != 0) {
            String q = quad[p].result;
            quad[p].result=Integer.toString(t);
            p=Integer.parseInt(q);
        }
    }
    private static int entry(char[] word) //查变量表
    {
        int i;
        for(i=0; i < signtable_count; i++) {
            if(Objects.equals(signtable[i].name, new String(word).strip()))
                return i;
        }
        if(Objects.equals(new String(word).strip(),"T")) {
            i=1000 + T_i;
            T_i++;
        }
        else
            i=Integer.parseInt(new String(word).strip())+10000;
        return i;
    }
    private static int Merg(int p1, int p2) //合并回填链
    {
        int p;
        if(p1 > p2 || p2 == 0)
            return p1;
        else {
            p=p2;
            while(!Objects.equals(quad[p].result, "0"))
                p=Integer.parseInt(quad[p].result);
            quad[p].result=Integer.toString(p1);
            return p2;
        }
    }
    private static int precede1(int a) //查action1表
    {
        int i=0;
        int[] PAT ={'i',3,4,5,6,17,18,'#','E'};
        while(i<=8 && PAT[i]!=a)
            i++;
        if(i==9)
            i=7;
        return i;
    }
    private static int precede2(int a) //查action2表
    {
        int[] PAT ={'i','r',17,18,34,14,15,'#','E','A','O'};
        if(a>=7 && a<=12) return 1;
        int i=0;
        while(i<=10 && PAT[i]!=a)
            i++;
        if(i==11)
            i = 7;
        return i;
    }
    private static void quadtocom() //中间代码四元式产生汇编代码
    {
        int j=0,k;
        int[] arr = new int[100];
        Arrays.fill(arr,10000);
        for (int i=1; i < quad_i; i++) { //把每个四元式要跳转的序号存入数组arr
            if (Character.isDigit(quad[i].result.toCharArray()[0])) { //判断四元式的result是否为整数
                int arrnum=Integer.parseInt(quad[i].result);//把数字字符串转化为整数
                int n;
                for (n=0; n < j; n++)//判断整数是否已在arr数组中,目的是消除重复项
                    if (arr[n] == arrnum)
                        break;
                if (n == j)
                    arr[j++]=arrnum;
            }
        }
        Arrays.sort(arr); // 排序
        for (int i=0; i < j; i++) {
            L[L_i]=new L();
            L[L_i].i=arr[i];
            L[L_i++].li="L"+ (i+1);
        }
        for (int i=1; i < quad_i; i++) { // 从第一个四元式翻译成汇编
            if (Objects.equals(quad[i].op, "jnz")) { //判断
                com[com_i]=new Com();
                for (k=0; k < j; k++) {
                    if (L[k].i == i) {
                        com[com_i].lable=L[k].li;
                        break;
                    }
                }
                if (k == j)
                    com[com_i].lable="";
                com[com_i].op="CMP";
                com[com_i].arg1=quad[i].arg1;
                com[com_i++].arg2="0";
                com[com_i]=new Com();
                com[com_i].lable="";
                com[com_i].op="JNZ";
                for (k=0; k < j; k++)
                    if (L[k].i == Integer.parseInt(quad[i].result))
                        com[com_i].arg1=L[k].li;
                com[com_i++].arg2="";
            }
            if (Objects.equals(quad[i].op, "j")) {
                com[com_i]=new Com();
                for (k=0; k < j; k++) {
                    if (L[k].i == i) {
                        com[com_i].lable=L[k].li;
                        break;
                    }
                }
                if (k == j)
                    com[com_i].lable="";
                com[com_i].op="JMP";
                for (k=0; k < j; k++)
                    if (L[k].i == Integer.parseInt(quad[i].result))
                        com[com_i].arg1=L[k].li;
                com[com_i++].arg2="";
            }
            if (Objects.equals(quad[i].op,"j>")) {
                com[com_i]=new Com();
                for (k=0; k < j; k++) {
                    if (L[k].i == i) {
                        com[com_i].lable=L[k].li;
                        break;
                    }
                }
                if (k == j)
                    com[com_i].lable="";
                com[com_i].op="CMP";
                com[com_i].arg1=quad[i].arg1;
                com[com_i++].arg2=quad[i].arg2;
                com[com_i]=new Com();
                com[com_i].lable="";
                com[com_i].op="JA";
                for (k=0; k < j; k++)
                    if (L[k].i == Integer.parseInt(quad[i].result))
                        com[com_i].arg1=L[k].li;
                com[com_i++].arg2="";
            }
            if (Objects.equals(quad[i].op,"j>=")) {
                com[com_i]=new Com();
                for (k=0; k < j; k++) {
                    if (L[k].i == i) {
                        com[com_i].lable=L[k].li;
                        break;
                    }
                }
                if (k == j)
                    com[com_i].lable="";
                com[com_i].op="CMP";
                com[com_i].arg1=quad[i].arg1;
                com[com_i++].arg2=quad[i].arg2;
                com[com_i]=new Com();
                com[com_i].lable="";
                com[com_i].op="JAE";
                for (k=0; k < j; k++)
                    if (L[k].i == Integer.parseInt(quad[i].result))
                        com[com_i].arg1=L[k].li;
                com[com_i++].arg2="";
            }
            if (Objects.equals(quad[i].op,"j<")) {
                com[com_i]=new Com();
                for (k=0; k < j; k++) {
                    if (L[k].i == i) {
                        com[com_i].lable=L[k].li;
                        break;
                    }
                }
                if (k == j)
                    com[com_i].lable="";
                com[com_i].op="CMP";
                com[com_i].arg1=quad[i].arg1;
                com[com_i++].arg2=quad[i].arg2;
                com[com_i]=new Com();
                com[com_i].lable="";
                com[com_i].op="JB";
                for (k=0; k < j; k++)
                    if (L[k].i == Integer.parseInt(quad[i].result))
                        com[com_i].arg1=L[k].li;
                com[com_i++].arg2="";
            }
            if (Objects.equals(quad[i].op,"j<=")) {
                com[com_i]=new Com();
                for (k=0; k < j; k++) {
                    if (L[k].i == i) {
                        com[com_i].lable=L[k].li;
                        break;
                    }
                }
                if (k == j)
                    com[com_i].lable="";
                com[com_i].op="CMP";
                com[com_i].arg1=quad[i].arg1;
                com[com_i++].arg2=quad[i].arg2;
                com[com_i]=new Com();
                com[com_i].lable="";
                com[com_i].op="JBE";
                for (k=0; k < j; k++)
                    if (L[k].i == Integer.parseInt(quad[i].result))
                        com[com_i].arg1=L[k].li;
                com[com_i++].arg2="";
            }
            if (Objects.equals(quad[i].op,"j==")) {
                com[com_i]=new Com();
                for (k=0; k < j; k++) {
                    if (L[k].i == i) {
                        com[com_i].lable=L[k].li;
                        break;
                    }
                }
                if (k == j)
                    com[com_i].lable="";
                com[com_i].op="CMP";
                com[com_i].arg1=quad[i].arg1;
                com[com_i++].arg2=quad[i].arg2;
                com[com_i]=new Com();
                com[com_i].lable="";
                com[com_i].op="JE";
                for (k=0; k < j; k++)
                    if (L[k].i == Integer.parseInt(quad[i].result))
                        com[com_i].arg1=L[k].li;
                com[com_i++].arg2="";
            }
            if (Objects.equals(quad[i].op,"j!=")) {
                com[com_i]=new Com();
                for (k=0; k < j; k++) {
                    if (L[k].i == i) {
                        com[com_i].lable=L[k].li;
                        break;
                    }
                }
                if (k == j)
                    com[com_i].lable="";
                com[com_i].op="CMP";
                com[com_i].arg1=quad[i].arg1;
                com[com_i++].arg2=quad[i].arg2;
                com[com_i]=new Com();
                com[com_i].lable="";
                com[com_i].op="JNE";
                for (k=0; k < j; k++)
                    if (L[k].i == Integer.parseInt(quad[i].result))
                        com[com_i].arg1=L[k].li;
                com[com_i++].arg2="";
            }
            if (Objects.equals(quad[i].op,"=")) {
                com[com_i]=new Com();
                for (k=0; k < j; k++)
                    if (L[k].i == i) {
                        com[com_i].lable=L[k].li;
                        break;
                    }
                if (k == j)
                    com[com_i].lable="";
                com[com_i].op="MOV";
                com[com_i].arg1="R0";
                com[com_i++].arg2=quad[i].arg1;
                com[com_i]=new Com();
                com[com_i].lable="";
                com[com_i].op="MOV";
                com[com_i].arg1=quad[i].result;
                com[com_i++].arg2="R0";
            }
            if (Objects.equals(quad[i].op,"+")) {
                com[com_i]=new Com();
                for (k=0; k < j; k++)
                    if (L[k].i == i) {
                        com[com_i].lable=L[k].li;
                        break;
                    }
                if (k == j)
                    com[com_i].lable="";
                com[com_i].op="MOV";
                com[com_i].arg1="R0";
                com[com_i++].arg2=quad[i].arg1;
                com[com_i]=new Com();
                com[com_i].lable="";
                com[com_i].op="MOV";
                com[com_i].arg1="R1";
                com[com_i++].arg2=quad[i].arg2;
                com[com_i]=new Com();
                com[com_i].lable="";
                com[com_i].op="ADD";
                com[com_i].arg1="R1";
                com[com_i++].arg2="R0";
                com[com_i]=new Com();
                com[com_i].lable="";
                com[com_i].op="MOV";
                com[com_i].arg1=quad[i].result;
                com[com_i++].arg2="R1";
            }
            if (Objects.equals(quad[i].op,"-")) {
                com[com_i]=new Com();
                for (k=0; k < j; k++)
                    if (L[k].i == i) {
                        com[com_i].lable=L[k].li;
                        break;
                    }
                if (k == j)
                    com[com_i].lable="";
                com[com_i].op="MOV";
                com[com_i].arg1="R0";
                com[com_i++].arg2=quad[i].arg1;
                com[com_i]=new Com();
                com[com_i].lable="";
                com[com_i].op="MOV";
                com[com_i].arg1="R1";
                com[com_i++].arg2=quad[i].arg2;
                com[com_i]=new Com();
                com[com_i].lable="";
                com[com_i].op="SUB";
                com[com_i].arg1="R1";
                com[com_i++].arg2="R0";
                com[com_i]=new Com();
                com[com_i].lable="";
                com[com_i].op="MOV";
                com[com_i].arg1=quad[i].result;
                com[com_i++].arg2="R1";
            }
            if (Objects.equals(quad[i].op,"*")) { //乘法
                com[com_i]=new Com();
                for (k=0; k < j; k++)
                    if (L[k].i == i) {
                        com[com_i].lable=L[k].li;
                        break;
                    }
                if (k == j)
                    com[com_i].lable="";
                com[com_i].op="MOV";
                com[com_i].arg1="AL";
                com[com_i++].arg2=quad[i].arg1;
                com[com_i]=new Com();
                com[com_i].lable="";
                com[com_i].op="MUL";
                com[com_i].arg1=quad[i].arg2;
                com[com_i++].arg2="";
                com[com_i]=new Com();
                com[com_i].lable="";
                com[com_i].op="MOV";
                com[com_i].arg1=quad[i].result;
                com[com_i++].arg2="AX";
            }
            if (Objects.equals(quad[i].op,"/")) { //除法
                com[com_i]=new Com();
                for (k=0; k < j; k++)
                    if (L[k].i == i) {
                        com[com_i].lable=L[k].li;
                        break;
                    }
                if (k == j)
                    com[com_i].lable="";
                com[com_i].op="MOV";
                com[com_i].arg1="AX";
                com[com_i++].arg2=quad[i].arg1;
                com[com_i]=new Com();
                com[com_i].lable="";
                com[com_i].op="DIV";
                com[com_i].arg1=quad[i].arg2;
                com[com_i++].arg2="";
                com[com_i]=new Com();
                com[com_i].lable="";
                com[com_i].op="MOV";
                com[com_i].arg1=quad[i].result;
                com[com_i++].arg2="AL";
            }
            if (Objects.equals(quad[i].op,"end")) { //结束
                com[com_i]=new Com();
                for (k=0; k < j; k++)
                    if (L[k].i == i) {
                        com[com_i].lable=L[k].li;
                        break;
                    }
                if (k == j)
                    com[com_i].lable="";
                com[com_i].op="MOV";
                com[com_i].arg1="AH";
                com[com_i++].arg2="4CH";
                com[com_i]=new Com();
                com[com_i].lable="";
                com[com_i].op="INT";
                com[com_i].arg1="21H";
                com[com_i++].arg2="";
            }
        }
        StringBuilder str = new StringBuilder();
        str.append("""
                ASSUME CS:CODE,DS:DATA
                DATA SEGMENT
                """);
        for (int i = 0; i < signtable_count; i++)
            str.append("    ").append(signtable[i].name).append("   DB   ?\n");
        for (int i = 1; i < T_i; i++)
            str.append("    ").append(signtable[1000+i].name).append("   DB   ?\n");
        str.append("""
                DATA ENDS
                CODE SEGMENT
                START:MOV AX,DATA
                      MOV DS,AX
                """);
        for (int i=0; i < com_i; i++) {
            if (!Objects.equals(com[i].lable, "")){
                str.append(com[i].lable).append(":");
                str.append(" ".repeat(Math.max(0, 5 - com[i].lable.length())));
                str.append(com[i].op).append(" ").append(com[i].arg1);
            }
            else
                str.append("      ").append(com[i].op).append(" ").append(com[i].arg1);
            if (!Objects.equals(com[i].arg2, "")) {
                str.append(",").append(com[i].arg2);
            }
            str.append("\n");
        }
        str.append("""
                CODE ENDS
                END START
                """);
        ComCode=str.toString();
    }
}