#include "pch.h"

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

int * value_;
int lag = 0;

void sorttimes(int *variabletimes_, int *truevaluefirst_) //�Դ��������Ԫ���ִ�����������
{
	int i, j, max = -1, c = 0;
	for (j = 1; j < variablenumbers + 1; j++)
	{
		for (i = 1; i < variablenumbers + 1; i++)
		{
			if (variabletimes_[i] > max)
			{
				max = variabletimes_[i];
				c = i;
			}
		}
		truevaluefirst_[j] = c;
		variabletimes_[c] = 0;
		max = -1;
	}
}

void copyarray(int *variabletime, int *newtime)
{
	int i;
	for (i = 1; i < variablenumbers + 1; i++)
		newtime[i] = variabletime[i];
	//return newtime;
}

int isunitClause(Line *S)
{
	//�ж��Ƿ�Ϊ���Ӿ�,���ҷ��ر�Ԫ���,û�оͷ���0
	Line * L = S;
	while (L != NULL)
	{
		if (L->firstvariable == NULL)    //���Ӿ�
		{
			continue;
		}
		if (L->firstvariable->next == NULL)
		{
			return L->firstvariable->name;
		}
		L = L->nextline;
	}
	return 0;
}

int  unittransmit(Line *S, int *newbacktrack)   //���Ӿ䴫������
{
	int unitliteral = isunitClause(S);
	if (unitliteral == 0) //û�е��Ӿ�
		return 0;
	//printf("���Ӿ䣺%d \n", unitliteral);
	newbacktrack[abs(unitliteral)] = 1;
	value_[abs(unitliteral)] = unitliteral > 0 ? 1 : 0;//Ϊ�����Ԫ��ֵʹ��Ϊ��
//������ͨ��ѭ������cnf������ɾ�����Ӿ��Լ�ɾ���为����
	Line *L = S, *q = NULL;
	while (L != NULL)
	{
		Clause *p1 = L->firstvariable;
		Clause *p2 = NULL;
		while (p1 != NULL)
		{
			if (p1->name == unitliteral)//�ҵ����Ӿ����ֲ��ҵ���ָ��
			{
				if (L == S)//����ǵ�һ��Ҫ�ı��һ�еĽṹ
				{
					if (S->nextline == NULL)
					{
						//*S = *((Line *)NULL);
						removevariable(L->firstvariable);//�ͷŵ��Ӿ�
						free(L);
						S = NULL;
						L = NULL;
					}
					else
					{
						removevariable(L->firstvariable);//�ͷŵ��Ӿ�
						Line * q = L->nextline;
						*S = *(S->nextline);
						free(q);
						// S=S->nextline;
						//free(L);
						L = NULL;
					}
				}
				else
				{
					q->nextline = L->nextline;
					removevariable(L->firstvariable);//�ͷŵ��Ӿ�
					free(L);
					L = q;
				}
				break;
			}
			else if (p1->name == -unitliteral)   //�ҵ������֣�������Ӿ����Ƴ����������ֱ���
			{
				if (p1 == L->firstvariable)
				{
					L->firstvariable = p1->next;
					free(p1); //�ͷŵ��Ӿ��ڴ�
				}
				else
				{
					p2->next = p1->next;
					free(p1); //�ͷŵ��Ӿ��ڴ�
				}
				p1 = p2;
				continue;
			}
			p2 = p1;
			p1 = p1->next;
		}
		q = L;
		L = L == NULL ? S : L->nextline;
	}
	return 1;
}

int boole(int variable)  //�����жϱ�Ԫ����
{
	if (variable > 0)
		return 1;
	else return -1;
}

int find(Line  *S)//�ҵ�������
{
	//int *literal = (int*)alloca((variablenumbers + 1)* sizeof(int));//���������־�����ֲ��ҳ�ʼ��Ϊ0
	int * literal = (int*)calloc(variablenumbers + 1, sizeof(int));
	/*	int i = 0;
	for (i = 0; i < variablenumbers + 1; i++)
		literal[i] = 0;*/
	if (literal == NULL)
	{
		return  -2;
	}
	Line *L = S;
	while (L != NULL)
	{
		Clause * c = L->firstvariable;
		while (c != NULL)
		{
			int f = literal[abs(c->name)];//������¼�ҵ��Ĵ�����
			if (f == 0) 
				literal[abs(c->name)] = boole(c->name);
			else if (f == -1 && (c->name) > 0)//��ʱ�ж��������������һ��
			{
				literal[abs(c->name)] = 2;
				//	printf("%d ", literal[abs(c->name)]);
			}
			else if (f == 1 && (c->name) < 0)
			{
				literal[abs(c->name)] = 2;
				//	printf("%d ", literal[abs(c->name)]);
			}
			c = c->next;
		}
		L = L->nextline;
	}
	int i;
	for (i = 1; i < variablenumbers + 1; i++) //�����ҵ���һ��������
	{
		if (literal[i] == -1 || literal[i] == 1)
		{
			int j = literal[i];
				free(literal);
			return i * j;
		}
	}
	return 0;
}

int deleteclause(Line  *S, int *newbacktrack) //������ɾ��
{
	int pliteral = find(S);
	if (pliteral == 0)
		return 0;
	//printf("�����֣�%d \n", pliteral);
	newbacktrack[abs(pliteral)] = 1;
	value_[abs(pliteral)] = pliteral > 0 ? 1 : 0;
	Line * L = S;
	Line * q=NULL;
	while (L != NULL)  //ɾ�����������ֵ��Ӿ�
	{
		Clause * c = L->firstvariable;
		while (c != NULL)
		{
			if (c->name == pliteral)
			{
				if (L == S)
				{
					if (S->nextline == NULL)  //��ʱ��ֻ��һ��
					{
						//*S =*((Line *)NULL);
					    removevariable(L->firstvariable); //�ͷŴ������ڴ�
						free(L);
						S = NULL;
						L = NULL;
					}
					else
					{
                        removevariable(L->firstvariable);//�ͷŴ������ڴ�
						Line *q1 = L->nextline;
						*S = *(S->nextline);
						// S->firstline=L->nextline;
						free(q1);
						L = NULL;
					}
				}
				else
				{
					q->nextline = L->nextline;
					removevariable(L->firstvariable);//�ͷŴ������ڴ�
					free(L);
					L = q;
				}
				break;
			}
			c = c->next;
		}
		q = L;
		L = L == NULL ? S : L->nextline;
	}
	return 1;
}

int Allunit(Line  *S) //����Ƿ����ж��ǵ��Ӿ�
{
	int * literal = (int*)calloc(variablenumbers + 1, sizeof(int));
	//int * literal = (int *)alloca((variablenumbers + 1)* sizeof(int));
		/*int i = 0;
		for (i = 0; i < variablenumbers + 1; i++)
			literal[i] = 0;*/
	if (literal == NULL)
		return -2;
	Line * L = S;
	while (L != NULL)
	{
		Clause * c = L->firstvariable;
		while (c != NULL)
		{
			int j = literal[abs(c->name)];
			if (j == 0) 
				literal[abs(c->name)] = boole(c->name);
			//���֮ǰ�ҵ��������Ԫ
			else if (j == -1 && (c->name) > 0)
			{
					free(literal);
				return 0;
			}
			else if (j == 1 && (c->name) < 0)
			{
							free(literal);
				return 0;
			}
			c = c->next;
		}
		L = L->nextline;
	}
	//��ʱû�г�ͻ��Ԫ�����ǿ��Ծ���������Ԫ��ֵ
	L = S;
	while (L != NULL)
	{
		Clause *c = L->firstvariable;
		while (c != NULL)
		{
			value_[abs(c->name)] = c->name > 0 ? 1 : 0;
			c = c->next;
		}
		L = L->nextline;
	}
	return 1;
}

int isEmptyClause(Line  *S)  //�Ƿ��п��Ӿ�
{
	Line * L = S;
	while (L != NULL)
	{
		if (L->firstvariable == NULL)
			return 1;
		L = L->nextline;
	}
	return 0;
}

int check(Line *S)  //����Ƿ�����㣬�������㷵��-1��������0��
{
	if (isEmptyClause(S))//�п��Ӿ�
		return -1;
	if (Allunit(S) && checkchangevariable(value_) != 0)
		return 1;
	return 0;
}

int checkchangevariable(int *value_)  //ȷ�����б�Ԫ��������ֵ
{
	int i = 1;
	for (i = 1; i < variablenumbers + 1; i++)
	{
		if (value_[i] == -1)
			return  0;
	}
	return 1;
}

int checksoulution(int *value, Line * S)
{
	int flag = 0;
	//int i = 0;
	Line  *L = S;
	while (L != NULL)
	{
		flag = 0;  //��ʼ���鵱ǰ��
		Clause *c = L->firstvariable;
		while (c != NULL)
		{
			if (c->name == 0)  //�ڸ��Ƶ�ʱ�������˵�һ��Ϊ0�������Ҫ����
			{
				c=c->next;
				flag = 1;
				continue;
			}
			if (value[abs(c->name)] + boole(c->name) == 2 || value[abs(c->name)] + boole(c->name) == -1) //һ��ֻҪ��һ�����Ϊ�漴��
			{
              flag = 1;
			  //i++;
			 // printf("%d %d \n", c->name,i);
              break;
			}
			/*if (value[abs(c->name)] + boole(c->name) == 1 || value[abs(c->name)] + boole(c->name) == 0)  //�������
				flag = 0;*/
			c = c->next;
		}
		if (flag == 0)
			break;
		L = L->nextline;
	}
	if (flag == 1)
		return 1;
	else return 0;
}

int choose(Line * S)
{
	return S->firstvariable->name;
}

int occurtimesmax(Line *S, int * variabletimes_, int *backtrack_)
{
	int i = 1;
	for (i = 1; i < variablenumbers + 1; i++)
	{
		//printf("%d true[%d]=%d back[%d]=%d \n", i, i,variabletimes_[i], variabletimes_[i], backtrack_[variabletimes_[i]]);
		if (backtrack_[variabletimes_[i]] != 1) //�ҵ����ִ������Ĳ���ûѡ����ı�Ԫ
		{
			backtrack_[variabletimes_[i]] = 1;
			if (negativetimes[variabletimes_[i]] > positivetimes[variabletimes_[i]]) //�����ָ������϶�
				return 0 - variabletimes_[i];
			else
				return variabletimes_[i];
		}
	}
	return S->firstvariable->name;
}

Line * split(Line * S, int lname)  //ִ�з��Ѳ���

{
	value_[abs(lname)] = lname > 0 ? 1 : 0;
	//printf("�˴�ѡȡ�ı�Ԫ�ǣ�%d\n", lname);
	Line * newLine = NULL;
	Line * p1 = S;
	Line  * p2 = NULL;
	while (p1 != NULL)
	{
		Line *copyCL = copyClause(p1);
		if (copyCL == NULL)
		{
			lag = -2;
			return NULL;
		}
		if (newLine == NULL)
		{
			newLine = copyCL;
		}
		if (p2 != NULL)
		{
			p2->nextline = copyCL;
		}
		p2 = copyCL;
		p1 = p1->nextline;
	}
	Line *q = (Line *)malloc(sizeof(Line));
	if (q == NULL)
	{
		lag = -2;
		return NULL;
	}
	q->firstvariable = NULL;
	q->nextline = NULL;
	Clause *c = (Clause *)malloc(sizeof(Clause));
	if (c == NULL)
	{
		lag = -2;
		return NULL;
	}
	c->name = lname;
	c->next = NULL;
	q->firstvariable = c;
	q->nextline = newLine;
	//S->firstline=q;
		// *(S)=*(q);
	return q;
}

void removevariable(Clause * c)  //ɾ����ǰ��
{
	while (c != NULL)
	{
		Clause * next = c->next;
		free(c);
		c = next;
	}
}

void removeClause(Line * S)  //ɾ������cnf��ʽ
{
	//    Line *L=S;
	while (S != NULL)
	{
		Line * nextline = S->nextline;
		if (S->firstvariable != NULL) 
			removevariable(S->firstvariable);
		free(S);
		S = nextline;
	}
}

int DPLL(Line *S, int *back)
{
	if (S == NULL)
		return -2;
	if (lag == -2)
		return -2;
	int k = check(S);
	if (k != 0)//����Ƿ��Ѿ����ڿ�����״̬
	{
		removeClause(S);
		return k;
	}
	while (1)//���Ӿ䴫��
	{
		k = check(S);
		if (k != 0)
		{
			removeClause(S);
			return k;
		}
		k = unittransmit(S, back);
		if (k == 0) //û�е��Ӿ�
			break;
		if (k == -2)  //��ʱ�ڴ����ʧ��
			return -2;
	}
	while (1) //����������
	{
		int k = check(S);
		if (k != 0)
		{
			removeClause(S);
			return k;
		}
		k = deleteclause(S, back);
		if (k == 0)
			break;
		if (k == -2)
			return -2;
	}
	// int key = choose(S);  //ѡȡ��Ԫ���Դ�ʱΪ�˷�������ѡȡ��һ����Ԫ

	 //��Ϊ���Զ�

	int key = occurtimesmax(S, maxtimes, back);  //ѡȡ���ִ������ı�Ԫ
	//printf("ѡȡ�ı�ԪΪ%d\n",key);
	back[abs(key)] = 1;
	int * newbacktrack = (int *)calloc((variablenumbers + 1), sizeof(int));
	if (newbacktrack == NULL)
		return -2;
	copyarray(back, newbacktrack);
	//printfarray(back);
	//printfarray(newbacktrack);
	int status = DPLL(split(S, key), newbacktrack);
	if (status == 1)
	{
		free(newbacktrack);
		return 1;
	}
	else if (status == -2)
		return -2;
	else
		/*{
			printfarray(newbacktrack);
			return DPLL(split(S, 0 - key), newbacktrack);//=ʧ�ܻ���֮��û�жԵ�ǰѡȡ�ı�Ԫ��0�ʶ����ݲ������
		}*/
	{
		int k = DPLL(split(S, 0 - key), newbacktrack);
		if (k != 1)
		{
			back[abs(key)] = 0;
			free(newbacktrack);
			return k;
		}
		else
		{
			free(newbacktrack);
            return 1;
		}
			
	}

}

void printValuation(Line *S)
{
	int i;
	for (i = 1; i < variablenumbers + 1; i++)
	{
		printf("%d=%d ", i, value_[i]);
		if (i % 10 == 0)
			printf("\n");
	}
	printf("\n");
}
