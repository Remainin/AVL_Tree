#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#define LH +1 //���
#define EH 0  //�ȸ�
#define RH -1 //�Ҹ�

using namespace std;

typedef struct AVLNode
{
	int data;
	int bf;
	struct AVLNode* lchild;
	struct AVLNode* rchild;
}AVLNode, *AVL;
void L_Rotate(AVLNode* &p)
{
	AVLNode * rc = NULL;
	rc = p->rchild;			//rcָ��p�������������
	p->rchild = rc->lchild;//rc���������ҽ�Ϊp��������
	rc->lchild = p;
	p = rc;					//pָ���µĸ����
}

void R_Rotate(AVLNode* &p)
{
	AVLNode * lc = NULL;
	lc  = p->lchild;		//lcָ��p�������������
	p->lchild = lc->rchild;	//lc���������ҽ�Ϊp��������
	lc->rchild = p;
	p = lc;					//pָ���µĸ����
}

void leftBalance(AVLNode* &t)
{
	AVLNode* lc = NULL;
	AVLNode* rd = NULL;
	lc = t->lchild;
	switch(lc->bf)
	{
		case LH:					//LL��ת
			t->bf = EH;
			lc->bf = EH;
			R_Rotate(t);
			break;

		case EH:					//deleteAVL��Ҫ��insertAVL�ò���
			t->bf = LH;
			lc->bf = RH;
			R_Rotate(t);
			break;

		case RH:					//LR��ת
			rd = lc->rchild;
			switch(rd->bf)
			{
				case LH:
					t->bf = RH;
					lc->bf = EH;
					break;
				case EH:
					t->bf = EH;
					lc->bf = EH;
					break;
				case RH:
					t->bf = EH;
					lc->bf = LH;
					break;
			}
			rd->bf = EH;
			L_Rotate(t->lchild);    //����дL_Rotate(lc);���õ������ò���
			R_Rotate(t);
			break;
	}
}

void rightBalance(AVLNode* &t)
{
	AVLNode* rc = NULL;
	AVLNode *ld = NULL;

	rc = t->rchild;
	switch(rc->bf)
	{
		case LH:				//RL��ת
			ld = rc->lchild;
			switch(ld->bf)
			{
				case LH:
					t->bf = EH;
					rc->bf = RH;
					break;
				case EH:
					t->bf = EH;
					rc->bf = EH;
					break;
				case RH:
					t->bf = LH;
					rc->bf = EH;
					break;
			}
			ld->bf = EH;
			R_Rotate(t->rchild);//����дR_Rotate(rc);���õ������ò���
			L_Rotate(t);
			break;

		case EH:				//deleteAVL��Ҫ��insertAVL�ò���
			t->bf = RH;
			rc->bf = LH;
			L_Rotate(t);
			break;

		case RH:				//RR��ת
			t->bf = EH;
			rc->bf = EH;
			L_Rotate(t);
			break;
	}
}

AVLNode* searchAVL(AVL& t, int key)
{
	if((t == NULL)||((key) ==(t->data)))
		return t;
   	else if((key)<(t->data)) /* ���������м������� */
     	return searchAVL(t->lchild,key);
   	else
     	return searchAVL(t->rchild,key); /* ���������м������� */
}

bool insertAVL(AVL& t, int & e, bool& taller)
{
	if(t == NULL)
	{
		t = (AVLNode*)malloc(sizeof(AVLNode));
		t->data = e;
		t->lchild = t->rchild = NULL;
		t->bf = EH;
		taller = true;
	}
	else
	{
		if((e)==(t->data))		//�����Ѻ��ùؼ��֣�������
		{
			taller = false;
			return false;
		}
		else if((e)<(t->data))//���������в��Ҳ����
		{
			if(!insertAVL(t->lchild, e, taller))//����������ʧ��
			{
				return false;
			}
			if(taller)					//����������ɹ���������������
			{
				switch(t->bf)
				{
					case LH:			//ԭ��t������������������
						leftBalance(t); //����ƽ�⴦��
						taller = false;
						break;
					case EH:			//ԭ��t�����������������ȸ�
						t->bf = LH;		//��������������������
						taller = true;	//������������
						break;
					case RH:			//ԭ��t������������������
						t->bf = EH;		//�������������ȸ�
						taller = false;
						break;
				}
			}
		}
		else							//���������в��Ҳ����
		{
			if(!insertAVL(t->rchild, e, taller))//����������ʧ��
			{
				return false;
			}
			if(taller)					//����������ɹ���������������
			{
				switch(t->bf)
				{
					case LH:			//ԭ��t������������������
						t->bf = EH;
						taller = false;
						break;
					case EH:			//ԭ��t�����������������ȸ�
						t->bf = RH;
						taller = true;
						break;
					case RH:			//ԭ��t������������������
						rightBalance(t);//����ƽ�⴦��
						taller = false;
						break;
				}
			}
		}
	}
	return true;						//����ɹ�
}


bool deleteAVL(AVL& t, int key, bool& shorter)
{
	if(t == NULL)						//�����ڸ�Ԫ��
	{
		return false;					//ɾ��ʧ��
	}
	else if((key)==(t->data))		//�ҵ�Ԫ�ؽ��
	{
		AVLNode* q = NULL;
		if(t->lchild == NULL)			//������Ϊ��
		{
			q = t;
			t = t->rchild;
			delete q;
			shorter = true;
		}
		else if(t->rchild == NULL)		//������Ϊ��
		{
			q = t;
			t = t->lchild;
			delete q;
			shorter = true;
		}
		else							//��������������,
		{
			q = t->lchild;
			while(q->rchild)
			{
				q = q->rchild;
			}
			t->data = q->data;
			deleteAVL(t->lchild, q->data, shorter);	//���������еݹ�ɾ��ǰ�����
		}
	}
	else if((key)<(t->data))		//�������м�������
	{
		if(!deleteAVL(t->lchild, key, shorter))
		{
			return false;
		}
		if(shorter)
		{
			switch(t->bf)
			{
				case LH:
					t->bf = EH;
					shorter = true;
					break;
				case EH:
					t->bf = RH;
					shorter = false;
					break;
				case RH:
					rightBalance(t);	//��ƽ�⴦��
					if(t->rchild->bf == EH)//ע�������ͼ˼��һ��
						shorter = false;
					else
						shorter = true;
					break;
			}
		}
	}
	else								//�������м�������
	{
		if(!deleteAVL(t->rchild, key, shorter))
		{
			return false;
		}
		if(shorter)
		{
			switch(t->bf)
			{
				case LH:
					leftBalance(t);		//��ƽ�⴦��
					if(t->lchild->bf == EH)//ע�������ͼ˼��һ��
						shorter = false;
					else
						shorter = true;
					break;
				case EH:
					t->bf = LH;
					shorter = false;
					break;
				case RH:
					t->bf = EH;
					shorter = true;
					break;
			}
		}
	}
	return true;
}

void inOrderTraverse(AVL t)
{
    if(t)
    {
        inOrderTraverse(t->lchild);
        cout<<t->data<<" ";
        inOrderTraverse(t->rchild);
    }
}

void printAVL(AVL t)
{
    cout<<"������� "<<endl;
    inOrderTraverse(t);
}


int main()
{
	AVL t ;
	t = NULL;
	bool taller = false;
	bool shorter = false;
	int key;
	int choice = -1;
	bool flag = true;

	while(flag)
	{
		cout<<"0 ��ӡ"<<endl
			<<"1 ����"<<endl
			<<"2 ɾ��"<<endl
			<<"3 ����"<<endl
			<<"4 ����"<<endl
			<<"         "<<endl
			<<"��������Ҫ���еĲ����� ";
		cin>>choice;
		switch(choice)
		{
			case 0:
				printAVL(t);
				cout<<endl;
				break;
			case 1:
				cout<<endl<<"������Ҫ�������ֵ������0��ʾ����"<<endl;
				while(cin>>key && key)
				{
					if(insertAVL(t,key,taller))

					{
						cout<<"��ֵ "<<key<<" ����ɹ���"<<endl;
					}
					else
					{
						cout<<"����ʧ�ܣ������к��д���ֵ "<< endl;
					}
				}
				cout<<endl;
				break;

			case 2:
				cout<<endl<<"������Ҫɾ������ֵ������0��ʾ������"<<endl;
				while(cin>>key && key)
				{
					if(deleteAVL(t,key,shorter))
					{
						cout<<"��ֵ "<<key<<" ɾ���ɹ���"<<endl;
					}
					else
					{
						cout<<"������ûҪ�ҵ����Ҫɾ������ֵ��"<<endl;
					}
				}
				cout<<endl;
				break;

			case 3:
				cout<<endl<<"������Ҫ���ҵ���ֵ������0��ʾ����:"<<endl;
				while(cin>>key && key)
				{
					if(searchAVL(t,key))
						cout<<key<<" ���ҳɹ���"<<endl;
					else
						cout<<" ���ֲ����ڴ���ֵ������ʧ��"<<endl;
				}
				cout<<endl;
				break;

			case 4:
				flag = false;
				break;

			default:
				cout<<"��������Ĳ��������������������룡"<<endl;

		}
	}

    return 0;
}
