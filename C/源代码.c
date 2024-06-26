#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct {
	int num;
	char name[16];
	char department[8];
	int salary1;
	int insurance;
	int tax;
	int salary2;
}worker;
typedef struct node {
	worker data;
	struct node* next;
}node;
node* listinit() {//从文件中读出数据创建链表
	node* L;
	L = (node*)malloc(sizeof(node));
	L->next = NULL;
	node* r = L;
	printf("*************************\n正在从文件中读入数据......\n");
	FILE* fp = fopen("information.txt", "rb");
	if(fp!=NULL)printf("读入数据成功\n");
	else {
		printf("读入数据失败\n");
		exit(0);
	}
	while (!feof(fp)) {
		node* p;
		p = (node*)malloc(sizeof(node));
		worker x;
		if (fread(&x, sizeof(worker), 1, fp)) {
			p->data = x;
			r->next = p;
			r = p;
		}
	}
	fclose(fp);
	r->next = NULL;
	return L;
}
int lengthoflist(node* H) {//求表长
	node* p = H->next;
	int i = 0;
	while (p != NULL) {
		p = p->next;
		i++;
	}
	return i;
}
void printlist(node* H) {//遍历输出
	if (H->next == NULL)printf("系统中无员工记录\n");
	else {
		printf("工号\t姓名\t部门\t应发\t保险\t个税\t实发\n");
		for (node* p = H->next; p != NULL; p = p->next)
			printf("%d\t%s\t%s\t%d\t%d\t%d\t%d\n", p->data.num, p->data.name, p->data.department,
				p->data.salary1, p->data.insurance, p->data.tax, p->data.salary2);
	}
}
node* locatepre1(node* H, int num) {//查找给定值的结点的前驱（工号）
	node* pre = H, * p;
	for (p = pre->next; p != NULL && p->data.num != num; p = p->next)
		pre = p;
	if (p != NULL)return pre;
	else return NULL;
}
node* locatepre2(node* H, char* x) {//查找给定值的结点的前驱（姓名）
	node* pre = H, * p;
	for (p = H->next; p != NULL && *p->data.name != *x; p = p->next)
		pre = p;
	if (p != NULL)return pre;
	else return NULL;
}
void insert(node* p, int x) {//在给定结点后插入结点
	node* new;
	new = (node*)malloc(sizeof(node));
	new->data.num = x;
	scanf("%s%s%d%d%d", &new->data.name, &new->data.department,
		&new->data.salary1, &new->data.insurance, &new->data.tax);
	new->data.salary2 = new->data.salary1 - new->data.insurance - new->data.tax;
	new->next = p->next;
	p->next = new;
}
void sort1(node* H) {//排序（按工号）
	int i = lengthoflist(H);
	if (i >= 2) {
		node* p, * q;
		worker t;
		int j, k;
		for (j = i - 1; j >= 1; j--) {
			p = H->next;
			q = p->next;
			for (k = 1; k <= j; k++) {
				if (p->data.num > q->data.num) {
					t = p->data;
					p->data = q->data;
					q->data = t;
				}
				p = p->next;
				q = q->next;
			}
		}
	}
}
void sort2(node* H) {//排序（按实发工资）
	int i = lengthoflist(H);
	if (i >= 2) {
		node* p, * q;
		worker t;
		int j, k;
		for (j = i - 1; j >= 1; j--) {
			p = H->next;
			q = p->next;
			for (k = 1; k <= j; k++) {
				if (p->data.salary2 > q->data.salary2) {
					t = p->data;
					p->data = q->data;
					q->data = t;
				}
				p = p->next;
				q = q->next;
			}
		}
	}
}
int main() {
	printf("*****职工工资管理系统*****\n");
	printf("该程序可以实现以下功能：\n");
	printf("1.职工信息录入\n");
	printf("2.职工信息查询\n");
	printf("3.职工信息排序\n");
	printf("4.职工信息修改\n");
	printf("5.职工信息删除\n");
	printf("6.职工信息浏览\n");
	printf("7.职工信息存盘并退出系统\n");
	node* L = listinit();
	while (1) {
		printf("*************************\n请输入功能选项(1--7)：");
		int i;
		scanf("%d", &i);
		switch (i) {
		case 1: {
			printf("请输入工号,姓名,部门,应发,保险,个税（空格隔开）：\n");
			int x;
			scanf("%d", &x);
			if (locatepre1(L, x) != NULL)printf("已有工号为%d的员工的信息\n", x);
			else {
				insert(L, x);
				printf("员工信息录入成功\n");
			}
			break;
		}
		case 2: {
			printf("输入1按工号查找,输入2按姓名查找，输入其他退出查找：");
			int x;
			scanf("%d", &x);
			if (x == 1) {
				printf("请输入要查找的工号：");
				int t;
				scanf("%d", &t);
				node* p = locatepre1(L, t);
				if (p != NULL) {
					p = p->next;
					printf("工号\t姓名\t部门\t应发\t保险\t个税\t实发\n");
					printf("%d\t%s\t%s\t%d\t%d\t%d\t%d\n", p->data.num, p->data.name,
						p->data.department, p->data.salary1, p->data.insurance,
						p->data.tax, p->data.salary2);
				}
				else printf("未查询到有关员工的信息\n");
			}
			else if (x == 2) {
				printf("请输入要查找的姓名：");
				char t[16];
				scanf("%s", t);
				node* p = locatepre2(L, t);
				if (p != NULL) {
					p = p->next;
					printf("工号\t姓名\t部门\t应发\t保险\t个税\t实发\n");
					printf("%d\t%s\t%s\t%d\t%d\t%d\t%d\n", p->data.num, p->data.name,
						p->data.department, p->data.salary1, p->data.insurance,
						p->data.tax, p->data.salary2);
				}
				else printf("未查询到有关员工的信息\n");
			}
			break;
		}
		case 3: {
			printf("输入1按工号排序,输入2按实发工资排序，输入其他退出排序：");
			int x;
			scanf("%d", &x);
			if (x == 1) {
				sort1(L);
				printf("按工号排序完成\n");
			}
			else if (x == 2) {
				sort2(L);
				printf("按实发工资排序完成\n");
			}
			break;
		}
		case 4: {
			printf("请输入要修改信息的员工工号：");
			int x;
			scanf("%d", &x);
			node* p = locatepre1(L, x);
			if (p != NULL) {
				p = p->next;
				printf("输入1修改姓名，2修改部门，3修改应发工资，4修改保险，5修改个税,输入其他放弃修改：");
				int t;
				scanf("%d", &t);
				if (t >= 1 && t <= 5) {
					printf("输入修改信息：");
					switch (t) {
					case 1:scanf("%s", &p->data.name); break;
					case 2:scanf("%s", &p->data.department); break;
					case 3:scanf("%d", &p->data.salary1); break;
					case 4:scanf("%d", &p->data.insurance); break;
					case 5:scanf("%d", &p->data.tax); break;
					}
					p->data.salary2 = p->data.salary1 - p->data.insurance - p->data.tax;
					printf("已完成信息修改\n");
				}
				else printf("已放弃本次信息修改\n");
			}
			else printf("不存在工号为%d的员工\n", x);
			break;
		}
		case 5: {
			printf("请输入要删除的员工的工号：");
			int x;
			scanf("%d", &x);
			node* p = locatepre1(L, x);
			if (p != NULL) {
				node* q = p->next;
				p->next = q->next;
				free(q);
				printf("删除完成\n");
			}
			else printf("不存在工号为%d的员工\n", x);
			break;
		}
		case 6:
			printlist(L);
			break;
		case 7: {
			FILE* fp = fopen("information.txt", "wb");
			for (node* p = L->next; p != NULL; p = p->next)
				fwrite(&p->data, sizeof(worker), 1, fp);
			fclose(fp);
			printf("数据已存盘\n");
			return 0;
		}
		default:printf("*************************\n输入有误，请重新输入\n");
		}
	}
}