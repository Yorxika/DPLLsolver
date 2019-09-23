#include "pch.h"

#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include "time.h"
#include <cstring>
#include<algorithm>
using namespace std;

int finalsudoku[10][10];

int LOC(int x, int y, int z)  //��ά����Ѱַ��ʽ��Ӧ��9*9*9��CNF��Ԫ
{
	int k = x * 81 + y * 9 + z;
	return k;
}

Line * createCNF(Line *L, int variablenumber, int Clausenumber)//�ȴ���һ��ʲô��û�е�CNF����
{
	int *v = &variablenumbers;//�޸ı�Ԫ��Ŀ�Լ��Ӿ���Ŀ
	*v = variablenumber;
	v = &Clausenumbers;
	*v = Clausenumber;
	L = (Line *)malloc(sizeof(Line));
	L->firstvariable = NULL;
	L->nextline = NULL;
	return L;
}

Line * changeintoCNF(Sudoku *s)//ת����CNF��ʽ
{
	int i, j, k, l, c, m;
	int *v = &Clausenumbers;
	*v = 0;
	Line *L = (Line *)malloc(sizeof(Line));
	L->firstvariable = NULL;
	L->nextline = NULL;
	int b[9], b2[9], b3[9], b4[9];
	if ((L = createCNF(L, 9 * 9 * 9, 0)) == NULL)
	{
		return NULL;
	}
	for (i = 0; i < 9; i++)//���ÿ�����ӵ����
	{
		for (j = 0; j < 9; j++)
		{
			if (s->s[i][j] != 0)//����������������
			{
				for (k = 1; k < 10; k++) //����һ�����Ӿ�
				{
					if (k == s->s[i][j])
					{
						b[0] = LOC(i, j, k);
					}
					else
					{
						b[0] = 0-LOC(i, j, k);
					}
					L = addClause(L, 1, b);
					(*v)++;
				}
				for (k = j + 1; k < 9; k++)//ͬһ��������ֲ������
				{
					if (s->s[i][k] == 0)
					{
						b[0] = 0-LOC(i, j, s->s[i][j]);
						b[1] = 0-LOC(i, k, s->s[i][j]);
						L = addClause(L, 2, b);
						(*v)++;
						//*L = *(addClause(L, 2, b));
					}
				}
				for (k = i + 1; k < 9; k++)//ͬһ�е�����Ҳ�������
				{
					if (s->s[k][j] == 0) {
						b[0] = 0-LOC(i, j, s->s[i][j]);
						b[1] = 0-LOC(k, j, s->s[i][j]);
						L = addClause(L, 2, b);
						(*v)++;
						//L=addClause(L, 2, b);
					}
				}
				for (k = i + 1; k < i / 3 * 3 + 3; k++) //3*3�Ź���Ҳ�������
				{
					for (l = j / 3 * 3; l < j / 3 * 3 + 3; l++)
					{
						if (l == j) //ͬһ�����Ѿ��ȽϹ���
						{
							continue;
						}
						if (s->s[k][l] == 0)
						{
							b[0] = 0-LOC(i, j, s->s[i][j]);
							b[1] = 0-LOC(k, l, s->s[i][j]);
							L = addClause(L, 2, b);
							(*v)++;
							//L=addClause(L, 2, b);
						}
					}
				}
			}
			else //��ʱ������Ӵ���������
			{
				for (k = 1, c = 0; k <= 9; k++)
				{
					s->s[i][j] = k;
					if (checkkeyword(s, i, j))//�ҵ����ܵ�ȡֵ�Ϳ��ܵ�λ��
					{
						b[c] = k;
						b2[c] = LOC(i, j, k);
						c++;
					}
				}
				s->s[i][j] = 0;
				L = addClause(L, c, b2);//���������Ӿ䣬����ֻ����1����Ԫȡ��
				for (k = 0; k < c - 1; k++)//��������Ԫֻ��ȡ��
				{
					for (l = k + 1; l < c; l++)
					{
						b2[0] = 0-LOC(i, j, b[k]);
						b2[1] = 0-LOC(i, j, b[l]);
						L = (addClause(L, 2, b2));
						(*v)++;
					}
				}
				for (k = j + 1; k < 9; k++)
				{
					if (s->s[i][k] != 0)//ͬһ������λ�������֣��������
					{
						b2[0] =0-LOC(i, j, s->s[i][k]);
						b2[1] =0-LOC(i, k, s->s[i][k]);
						L = (addClause(L, 2, b2));
						(*v)++;
					}
					else//Ϊ��������һ��
					{
						for (l = 0; l < c; l++)
						{
							b2[0] = 0 - LOC(i, j, b[l]);
							b2[1] = 0 - LOC(i, k, b[l]);
							L = (addClause(L, 2, b2));
							(*v)++;
						}
					}
				}
				for (k = i + 1; k < 9; k++)//���м��
				{
					if (s->s[k][j] != 0)//�ո�������д�����ֲ������
					{
						b2[0] = 0 - LOC(i, j, s->s[k][j]);
						b2[1] = 0 - LOC(k, j, s->s[k][j]);
						L = (addClause(L, 2, b2));
						(*v)++;
					}

					else //�����ǿո����ֲ�����ͬ
					{
						for (l = 0; l < c; l++)
						{
							b2[0] = 0 - LOC(i, j, b[l]);
							b2[1] = 0 - LOC(k, j, b[l]);
							L = (addClause(L, 2, b2));
							(*v)++;
						}
					}
				}

				for (k = i + 1; k < i / 3 * 3 + 3; k++)//3*3�Ź�����,���ڵ�ǰ���ж������ˣ�����������ǰ�е�ǰ��
				{
					for (l = j / 3 * 3; l < j / 3 * 3 + 3; l++)
					{
						if (l == j)
						{
							continue;
						}
						if (s->s[k][l] != 0)//�ո�������д�����ֲ������
						{
							b2[0] = 0 - LOC(i, j, s->s[k][l]);
							b2[1] = 0 - LOC(k, j, s->s[k][l]);
							L = (addClause(L, 2, b2));
							(*v)++;
						}

						else//�����ǿո����ֲ�����ͬ
						{
							for (m = 0; m < c; m++) {
								b2[0] = 0 - LOC(i, j, b[m]);
								b2[1] = 0 - LOC(k, l, b[m]);
								L = (addClause(L, 2, b2));
								(*v)++;
							}
						}
					}
				}
			}
		}
	}
	for (i = 0; i < 9; i++)//ÿ�б�����1-9
	{
		memset(b, 0, sizeof(int) * 9);
		for (j = 0, l = 0; j < 9; j++)//�ҵ�δ��д�������Լ�λ��
		{
			if (s->s[i][j] != 0)
			{
				b[s->s[i][j] - 1] = 1;
			}
			else
			{
				b2[l] = j;
				l++;
			}
		}
		//printf("l=%d\n", l);
		for (j = 0, k = 0; j < 9; j++)
		{
			if (b[j] == 0)  //δ��д������
			{
				b[k] = j + 1;
				k++;
			}
		}
		for (j = 0; j < k; j++)  //��δ��д��������䵽��λ
		{
			for (c = 0; c < l; c++)
			{
				b3[c] = LOC(i, b2[c], b[j]);
			}
			L = addClause(L, l, b3);
			(*v)++;
		}
	}
	for (i = 0; i < 9; i++)//ÿ�б�����1-9
	{
		memset(b, 0, sizeof(int) * 9);
		for (j = 0, l = 0; j < 9; j++)
		{
			if (s->s[j][i] != 0)//�ҵ�δ��д�������Լ�λ��
			{
				b[s->s[j][i] - 1] = 1;
			}
			else
			{
				b2[l] = i;
				l++;
			}
		}
		for (j = 0, k = 0; j < 9; j++)
		{
			if (b[j] == 0)
			{
				b[k] = j + 1;
				//k++;
			}
		}
		for (j = 0; j < k; j++)
		{
			for (c = 0; c < l; c++)
			{
				b3[c] = LOC(i, b2[c], b[j]);
			}
			L = addClause(L, l, b3);
			(*v)++;
		}
	}
	for (i = 0; i < 9; i += 3) //�Ź��������1~9
	{
		for (m = 0; m < 9; m += 3)
		{
			memset(b, 0, sizeof(int) * 9);
			for (j = 0, l = 0; j < 9; j++)
			{
				if (s->s[i + j / 3][m + j % 3] != 0) //���г���ȡ��
				{
					b[s->s[i + j / 3][m + j % 3] - 1] = 1;//���δ�������
				}
				else
				{
					b2[l] = (m + j % 3);//���δ���λ��
					b4[l] = (i + j / 3);
					l++;
				}
			}
			for (j = 0, k = 0; j < 9; j++)
			{
				if (b[j] == 0)
				{
					b[k] = j + 1;
					k++;
				}
			}
			for (j = 0; j < k; j++)
			{
				for (c = 0; c < l; c++)
				{
					b3[c] = LOC(b4[c], b2[c], b[j]);
				}
				L = addClause(L, l, b3);
				(*v)++;
			}
		}
	}
	//printCNF(L);
	return L;
}

void cnfparser(Line *L)
{
	Line *S = L;
	int *v = &variablenumbers;
	*v = 729;
	value_ = (int*)calloc((variablenumbers)+1, sizeof(int));
	occurtimes = (int *)calloc(variablenumbers + 1, sizeof(int));
	positivetimes = (int *)calloc(variablenumbers + 1, sizeof(int));
	negativetimes = (int *)calloc(variablenumbers + 1, sizeof(int));
	backtrack = (int *)calloc(variablenumbers + 1, sizeof(int));
	//inittimes = (int *)calloc(variablenumbers + 1, sizeof(int));
	maxtimes = (int *)calloc(variablenumbers + 1, sizeof(int));
	multiplytimes = (int *)calloc(variablenumbers + 1, sizeof(int));
	while (S != NULL)
	{
		Clause *c = S->firstvariable;
		while (c != NULL)
		{
			if (c->name > 0)
			{
				positivetimes[abs(c->name)]++;
				//inittimes[abs(c->name)] ++;
				occurtimes[abs(c->name)]++;
			}
			else
			{
				//inittimes[abs(c->name) ]++;
				occurtimes[abs(c->name)]++;
				negativetimes[abs(c->name)]++;
			}
			//printf("%d||", c->name);
			c = c->next;
		}
		//printf("\n");
		S = S->nextline;
	}
	int i;
	for (i = 1; i < variablenumbers + 1; i++)
		multiplytimes[i] = positivetimes[i] * negativetimes[i];
	/*for (i = 1; i < variablenumbers + 1; i++)
   {
	   printf("vtimes[%d]=%d  true[%d]=%d \n", i, variabletimes[i], i, truevaluefirst[i]);
   }*/
	sorttimes(multiplytimes, maxtimes);
	//sorttimes(occurtimes, maxtimes);
	/*for (i = 1; i < variablenumbers + 1; i++)
	{
		printf("vtimes[%d]=%d  true[%d]=%d \n", i, variabletimes[i], i, truevaluefirst[i]);
	}*/
	free(occurtimes);// ֮ǰfree��������������ڲ�����
	free(multiplytimes);
	free(positivetimes);
	free(negativetimes);
}

void starts()
{
	srand(unsigned(time(NULL)));  /* ����random_shuffle����������� */
	for (int i = 1; i <= 9; ++i)
		for (int j = 1; j <= 9; ++j)
			finalsudoku[i][j] = 0;

	for (int i = 1; i <= 9; ++i)
		finalsudoku[1][i] = i;

	random_shuffle(&(finalsudoku[1][1]), &(finalsudoku[1][10]));  /* ��һ��������в��� */

	createfinalsudoku(2, 1);  /* �ӵڶ��п�ʼ���� */
}

int createfinalsudoku(int i, int j)  //�����ڣ� i , j ��λ�ô����Դ洢������,�ҵ����򷵻�true�����򷵻�false
{
	if (i > 9 || j > 9)
		return TRUE;

	for (int k = 1; k <= 9; ++k)
	{
		int can = TRUE;                // can �������ڼ�¼����k�ܷ���� ( i , j ) ��
		for (int m = 1; m < i; ++m)
			if (finalsudoku[m][j] == k)  // ���ͬһ���Ƿ���ֹ�����k
			{
				can = FALSE;
				break;
			}
		if (can)
			for (int n = 1; n < j; ++n)
				if (finalsudoku[i][n] == k)  // ���ͬһ���Ƿ���ֹ�����k
				{
					can = FALSE;
					break;
				}
		if (can)
		{
			int up1 = (i / 3) * 3 + 3; // (i,j)�������ڵ�3��3С����i���������
			int up2 = (j / 3) * 3 + 3;   // (i,j)�������ڵ�3��3С������j���������

			if (i % 3 == 0)    //���������������Ĵ���
				up1 = i;
			if (j % 3 == 0)
				up2 = j;

			for (int p = up1 - 2; p <= up1; ++p)  //�����3��3��С�������Ƿ������ͬһ������ 
			{
				if (can == FALSE)   
					break;
				for (int q = up2 - 2; q <= up2; ++q)
					if (finalsudoku[p][q] == k)
					{
						can = FALSE;
						break;
					}
			}
		}
		if (can)
		{
			finalsudoku[i][j] = k;
			if (j < 9)
			{
				if (createfinalsudoku(i, j + 1)) //��ͬһ�е���һλ�ÿ�ʼ���� 
					return TRUE;
			}
			else
			{
				if (i < 9)
				{
					if (createfinalsudoku(i + 1, 1))    //����һ�еĵ�һ���ո�ʼ���� 
						return TRUE;
				}
				else
					return TRUE;  //i >= 9  && j >= 9  , ��������

			}
			finalsudoku[i][j] = 0;   //�Ҳ������Ҫ�ظ�ԭ״��������������������Ӱ�� 
		}
	}
	return FALSE;  //1��9�����Թ������У��򷵻صݹ����һ�� 
}

Sudoku* createSudoku(Sudoku *s)//�ڶ���������
{
	Sudoku *ss = NULL;
	s->next = NULL;
	int i, j, k, c, flag = 0, x, y;
	x = rand() % 9;
	y = rand() % 9;
	s->s[x][y] = 0;//���ڵ�1����
	while (1)  //�ڶ�����
	{
		startsolveSudoku(s, &c);//�����������õ���ĸ���c
		if (c > 1)//��ʱ�ⲻΨһ
		{
			s->s[x][y] = k;
			flag++; //�����������������ζ���ڶ�����ĿԶ����17����ʾ��������17��������Ψһ��
			if (flag >= 17)
			{
				break;
			}
			for (ss = s->next; ss != NULL; ss = s->next)// �������ɵĶ�ûʲô����
			{
				s->next = ss->next;
				free(ss);
			}
		}
		else
		{
			do {
				x = rand() % 9;
				y = rand() % 9;
			} while (s->s[x][y] == 0);
			k = s->s[x][y]; //��ʱ(x,y)��Ϊ0����¼�����ֵ
			s->s[x][y] = 0;
			/*flag++;
			if (flag >=50)  //��ʱ����60�����������˳���
			{
				break;
			}*/
		}
	}
	return s;
}

int startsolveSudoku(Sudoku *s, int *count)  //��ʼ�������
{
	*count = 0;
	solveSudoku(s, 0, 0, count);
	if (count > 0)
		return TRUE;
	else
		return FALSE;
}

int solveSudoku(Sudoku *s, int x, int y, int *count) //�������
{
	int i, j, k;
	Sudoku *ss;
	while (s->s[x][y] != 0 && x < 9)//�����������ҵ���һ��Ϊ0�Ŀ�
	{
		y++;
		if (y == 9) //������һ��
		{
			x++;
			y = 0;
		}
	}
	if (x > 8)  //��ʱ��������û�пյ�
	{
		ss = (Sudoku*)malloc(sizeof(Sudoku));
		for (j = 0; j < 9; j++) {
			for (k = 0; k < 9; k++) {
				ss->s[j][k] = s->s[j][k];
			}
		}
		ss->next = s->next;
		s->next = ss;
		(*count)++;
		return TRUE;
	}
	for (i = 1; i <= 9; i++)
	{
		s->s[x][y] = i;
		if ((k = checkkeyword(s, x, y)) == 1)
		{
			if (x == 8 && y == 8)//�ﵽ���һ������
			{
				ss = (Sudoku*)malloc(sizeof(Sudoku));
				for (j = 0; j < 9; j++)
				{
					for (k = 0; k < 9; k++)
					{
						ss->s[j][k] = s->s[j][k];
					}
				}
				ss->next = s->next;
				s->next = ss;
				s->s[x][y] = 0;
				(*count)++;
				return TRUE;
			}
			else
			{
				if (y == 8)  //�����һ��
					solveSudoku(s, x + 1, 0, count);
				else  //�����һ��
					solveSudoku(s, x, y + 1, count);
			}
		}
	}
	s->s[x][y] = 0;
	return FALSE;
}

void printSudoku(Sudoku *s) //��ӡ����
{
	int i, j;
	//printf("��Ϊ���ɵ�����:\n");
	printf("-------------------\n");
	for (i = 0; i < 9; i++) {
		printf("|");
		for (j = 0; j < 9; j++) {
			printf("%d%c", s->s[i][j], ((j + 1) % 3 == 0) ? '|' : ' ');
		}
		printf("\n");
		if ((i + 1) % 3 == 0) {
			printf("|");
			printf("------------------\n");
		}
	}
}

int  checkSudoku(Sudoku *s)  //������ɵ������Ƿ�Ϸ�
{
	int i, j, k, l;
	int flag = 0;
	char b[9];
	for (i = 0; i < 9; i++)  //���м��
	{
		memset(b, 0, sizeof(int) * 9);
		for (j = 0; j < 10; j++)
		{
			if (s->s[i][j] == 0)  //�����Ϊ0�����
			{
				flag = 1;
			}
			else if (b[s->s[i][j] - 1] == 0)  //�����ǰ�ո��������ֻ������һ���޳�ͻ
			{
				b[s->s[i][j] - 1] = 1;
			}
			else
			{
				return FALSE;
			}
		}
	}
	for (i = 0; i < 9; i++)// �м��
	{
		memset(b, 0, sizeof(int) * 9);
		for (j = 0; j < 9; j++)
		{
			if (s->s[j][i] == 0) {
				flag = 1;
			}
			else if (b[s->s[j][i] - 1] == 0)
			{
				b[s->s[j][i] - 1] = 1;
			}
			else {
				return FALSE;
			}
		}
	}
	for (i = 0; i < 9; i += 3) //3*3���,�ӵ�һ���Ź���ʼ���Ҽ��
	{
		for (j = 0; j < 9; j += 3)
		{
			memset(b, 0, sizeof(int) * 9);
			for (k = i; k < i + 3; k++)
			{
				for (l = j; l < j + 3; l++)
				{
					if (s->s[k][l] == 0)
					{
						flag = 1;
					}
					else if (b[s->s[k][l]] == 0)
					{
						b[s->s[k][l]] = 1;
					}
					else
					{
						return FALSE;
					}
				}
			}
		}
	}
	if (flag == 1)
		return 0;
	else return 1;
}

Sudoku *inputsolution() //���������Ľ�
{
	int i, j;
	char ch;
	Sudoku *s = (Sudoku*)malloc(sizeof(Sudoku));
	if (s == NULL)
		return NULL;
	s->next = NULL;
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			scanf("%d", &s->s[i][j]);
		}
	}
	return s;
}

Sudoku * changeintoSudoku()
{
	int i,j,k;
	Sudoku *s =(Sudoku *) malloc(sizeof(Sudoku));
	s->next = NULL;
	for(i=0;i<9;i++)
	for (j=0;j<9;j++)
	{
		for (k = 1; k < 10; k++)
		{
			/*printf("[%d][%d][%d]=%d ", i, j, k, LOC(i, j, k));
			if (k == 9)
				printf("\n");*/
			if (value_[LOC(i, j, k)] == 1)
				s->s[i][j] = k;
		}
	}
	return s;
}

int checkkeyword(Sudoku *s, int x, int y) //��������������Ƿ���ȷ
{
	int i, j;
	for (i = 0; i < 9; i++)  //�������Ƿ��ͻ
	{
		if (s->s[x][y] == s->s[i][y] && i != x)
		{
			return FALSE;
		}

	}
	for (i = 0; i < 9; i++)  //��������Ƿ��ͻ
	{
		if (s->s[x][y] == s->s[x][i] && i != y)
		{
			return FALSE;
		}

	}
	for (i = x / 3 * 3; i < x / 3 * 3 + 3; i++)//���3*3�Ź����Ƿ��ͻ
	{
		for (j = y / 3 * 3; j < y / 3 * 3 + 3; j++)
		{
			if (s->s[x][y] == s->s[i][j] && (i != x || j != y))
			{
				return FALSE;
			}

		}
	}
	return TRUE;
}

void writeresult(Line *L)
{
	FILE *fp;
	fp = fopen("Sudoku.cnf", "w+");
	if (fp == NULL)
	{
		printf("���ļ�ʧ��\n");
		exit(1);
	}
	fprintf(fp, "p cnf %d %d\n", variablenumbers, Clausenumbers);
	Line *S = L;
	while(S!=NULL)
	{ 
		Clause *c = S->firstvariable;
		while (c != NULL)
		{
			fprintf(fp,"%d ", c->name);
			c = c->next;
		}
		fprintf(fp,"0\n");
		S = S->nextline;
	}
	fclose(fp);
}

int  checkinput(Sudoku *s, Sudoku *ss)
{
	int flag = 1,i,j;
	for(i=0;i<9;i++)
		for (j = 0; j < 9; j++)
		{
			if (s->s[i][j] == ss->s[i][j])
				continue;
			if (s->s[i][j] != ss->s[i][j])
			{
				flag = 0;
				break;
			}
		}
	if (flag == 0)
		return 0;
	else
		return 1;
}








