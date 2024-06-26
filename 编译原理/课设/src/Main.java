import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        System.out.print("input filename:");
        Scanner input = new Scanner(System.in);
        String fileName=input.next();
        printfile(fileName); //打印C语言源程序
        compile c = new compile(fileName); //编译
        //c.printsymbol(); //打印词法分析结果
        //c.printsigntable(); //打印变量表
        c.printquad(); //打印中间代码四元式
        System.out.println("\nAsm Code:\n"+c.getComCode()); //打印汇编代码
        System.out.println("Compile successful! Cost "+ c.getTime() +" ms.");
    }
    private static void printfile(String fileName) //打印源程序
    {
        try (BufferedReader reader = new BufferedReader(new FileReader(fileName))) {
            System.out.println("C Code:");
            String line;
            int line_cnt=1;
            while ((line = reader.readLine()) != null) //逐行读取文件内容，不读取换行符和末尾的空格
                System.out.println(line_cnt++ +":\t"+line);
        } catch (IOException e) {
            throw new IllegalArgumentException(fileName+" not found!");
        }
    }
}