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
node* listinit() {//���ļ��ж������ݴ�������
	node* L;
	L = (node*)malloc(sizeof(node));
	L->next = NULL;
	node* r = L;
	printf("*************************\n���ڴ��ļ��ж�������......\n");
	FILE* fp = fopen("information.txt", "rb");
	if(fp!=NULL)printf("�������ݳɹ�\n");
	else {
		printf("��������ʧ��\n");
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
int lengthoflist(node* H) {//���
	node* p = H->next;
	int i = 0;
	while (p != NULL) {
		p = p->next;
		i++;
	}
	return i;
}
void printlist(node* H) {//�������
	if (H->next == NULL)printf("ϵͳ����Ա����¼\n");
	else {
		printf("����\t����\t����\tӦ��\t����\t��˰\tʵ��\n");
		for (node* p = H->next; p != NULL; p = p->next)
			printf("%d\t%s\t%s\t%d\t%d\t%d\t%d\n", p->data.num, p->data.name, p->data.department,
				p->data.salary1, p->data.insurance, p->data.tax, p->data.salary2);
	}
}
node* locatepre1(node* H, int num) {//���Ҹ���ֵ�Ľ���ǰ�������ţ�
	node* pre = H, * p;
	for (p = pre->next; p != NULL && p->data.num != num; p = p->next)
		pre = p;
	if (p != NULL)return pre;
	else return NULL;
}
node* locatepre2(node* H, char* x) {//���Ҹ���ֵ�Ľ���ǰ����������
	node* pre = H, * p;
	for (p = H->next; p != NULL && *p->data.name != *x; p = p->next)
		pre = p;
	if (p != NULL)return pre;
	else return NULL;
}
void insert(node* p, int x) {//�ڸ������������
	node* new;
	new = (node*)malloc(sizeof(node));
	new->data.num = x;
	scanf("%s%s%d%d%d", &new->data.name, &new->data.department,
		&new->data.salary1, &new->data.insurance, &new->data.tax);
	new->data.salary2 = new->data.salary1 - new->data.insurance - new->data.tax;
	new->next = p->next;
	p->next = new;
}
void sort1(node* H) {//���򣨰����ţ�
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
void sort2(node* H) {//���򣨰�ʵ�����ʣ�
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
	printf("*****ְ�����ʹ���ϵͳ*****\n");
	printf("�ó������ʵ�����¹��ܣ�\n");
	printf("1.ְ����Ϣ¼��\n");
	printf("2.ְ����Ϣ��ѯ\n");
	printf("3.ְ����Ϣ����\n");
	printf("4.ְ����Ϣ�޸�\n");
	printf("5.ְ����Ϣɾ��\n");
	printf("6.ְ����Ϣ���\n");
	printf("7.ְ����Ϣ���̲��˳�ϵͳ\n");
	node* L = listinit();
	while (1) {
		printf("*************************\n�����빦��ѡ��(1--7)��");
		int i;
		scanf("%d", &i);
		switch (i) {
		case 1: {
			printf("�����빤��,����,����,Ӧ��,����,��˰���ո��������\n");
			int x;
			scanf("%d", &x);
			if (locatepre1(L, x) != NULL)printf("���й���Ϊ%d��Ա������Ϣ\n", x);
			else {
				insert(L, x);
				printf("Ա����Ϣ¼��ɹ�\n");
			}
			break;
		}
		case 2: {
			printf("����1�����Ų���,����2���������ң����������˳����ң�");
			int x;
			scanf("%d", &x);
			if (x == 1) {
				printf("������Ҫ���ҵĹ��ţ�");
				int t;
				scanf("%d", &t);
				node* p = locatepre1(L, t);
				if (p != NULL) {
					p = p->next;
					printf("����\t����\t����\tӦ��\t����\t��˰\tʵ��\n");
					printf("%d\t%s\t%s\t%d\t%d\t%d\t%d\n", p->data.num, p->data.name,
						p->data.department, p->data.salary1, p->data.insurance,
						p->data.tax, p->data.salary2);
				}
				else printf("δ��ѯ���й�Ա������Ϣ\n");
			}
			else if (x == 2) {
				printf("������Ҫ���ҵ�������");
				char t[16];
				scanf("%s", t);
				node* p = locatepre2(L, t);
				if (p != NULL) {
					p = p->next;
					printf("����\t����\t����\tӦ��\t����\t��˰\tʵ��\n");
					printf("%d\t%s\t%s\t%d\t%d\t%d\t%d\n", p->data.num, p->data.name,
						p->data.department, p->data.salary1, p->data.insurance,
						p->data.tax, p->data.salary2);
				}
				else printf("δ��ѯ���й�Ա������Ϣ\n");
			}
			break;
		}
		case 3: {
			printf("����1����������,����2��ʵ�������������������˳�����");
			int x;
			scanf("%d", &x);
			if (x == 1) {
				sort1(L);
				printf("�������������\n");
			}
			else if (x == 2) {
				sort2(L);
				printf("��ʵ�������������\n");
			}
			break;
		}
		case 4: {
			printf("������Ҫ�޸���Ϣ��Ա�����ţ�");
			int x;
			scanf("%d", &x);
			node* p = locatepre1(L, x);
			if (p != NULL) {
				p = p->next;
				printf("����1�޸�������2�޸Ĳ��ţ�3�޸�Ӧ�����ʣ�4�޸ı��գ�5�޸ĸ�˰,�������������޸ģ�");
				int t;
				scanf("%d", &t);
				if (t >= 1 && t <= 5) {
					printf("�����޸���Ϣ��");
					switch (t) {
					case 1:scanf("%s", &p->data.name); break;
					case 2:scanf("%s", &p->data.department); break;
					case 3:scanf("%d", &p->data.salary1); break;
					case 4:scanf("%d", &p->data.insurance); break;
					case 5:scanf("%d", &p->data.tax); break;
					}
					p->data.salary2 = p->data.salary1 - p->data.insurance - p->data.tax;
					printf("�������Ϣ�޸�\n");
				}
				else printf("�ѷ���������Ϣ�޸�\n");
			}
			else printf("�����ڹ���Ϊ%d��Ա��\n", x);
			break;
		}
		case 5: {
			printf("������Ҫɾ����Ա���Ĺ��ţ�");
			int x;
			scanf("%d", &x);
			node* p = locatepre1(L, x);
			if (p != NULL) {
				node* q = p->next;
				p->next = q->next;
				free(q);
				printf("ɾ�����\n");
			}
			else printf("�����ڹ���Ϊ%d��Ա��\n", x);
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
			printf("�����Ѵ���\n");
			return 0;
		}
		default:printf("*************************\n������������������\n");
		}
	}
}