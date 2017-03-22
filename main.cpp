#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#define LH +1 //左高
#define EH 0  //等高
#define RH -1 //右高

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
	rc = p->rchild;			//rc指向p的右子树根结点
	p->rchild = rc->lchild;//rc的左子树挂接为p的右子树
	rc->lchild = p;
	p = rc;					//p指向新的根结点
}

void R_Rotate(AVLNode* &p)
{
	AVLNode * lc = NULL;
	lc  = p->lchild;		//lc指向p的左子树根结点
	p->lchild = lc->rchild;	//lc的右子树挂接为p的左子树
	lc->rchild = p;
	p = lc;					//p指向新的根结点
}

void leftBalance(AVLNode* &t)
{
	AVLNode* lc = NULL;
	AVLNode* rd = NULL;
	lc = t->lchild;
	switch(lc->bf)
	{
		case LH:					//LL旋转
			t->bf = EH;
			lc->bf = EH;
			R_Rotate(t);
			break;

		case EH:					//deleteAVL需要，insertAVL用不着
			t->bf = LH;
			lc->bf = RH;
			R_Rotate(t);
			break;

		case RH:					//LR旋转
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
			L_Rotate(t->lchild);    //不能写L_Rotate(lc);采用的是引用参数
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
		case LH:				//RL旋转
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
			R_Rotate(t->rchild);//不能写R_Rotate(rc);采用的是引用参数
			L_Rotate(t);
			break;

		case EH:				//deleteAVL需要，insertAVL用不着
			t->bf = RH;
			rc->bf = LH;
			L_Rotate(t);
			break;

		case RH:				//RR旋转
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
   	else if((key)<(t->data)) /* 在左子树中继续查找 */
     	return searchAVL(t->lchild,key);
   	else
     	return searchAVL(t->rchild,key); /* 在右子树中继续查找 */
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
		if((e)==(t->data))		//树中已含该关键字，不插入
		{
			taller = false;
			return false;
		}
		else if((e)<(t->data))//在左子树中查找插入点
		{
			if(!insertAVL(t->lchild, e, taller))//左子树插入失败
			{
				return false;
			}
			if(taller)					//左子树插入成功，且左子树增高
			{
				switch(t->bf)
				{
					case LH:			//原来t的左子树高于右子树
						leftBalance(t); //做左平衡处理
						taller = false;
						break;
					case EH:			//原来t的左子树和右子树等高
						t->bf = LH;		//现在左子树比右子树高
						taller = true;	//整棵树增高了
						break;
					case RH:			//原来t的右子树高于左子树
						t->bf = EH;		//现在左右子树等高
						taller = false;
						break;
				}
			}
		}
		else							//在右子树中查找插入点
		{
			if(!insertAVL(t->rchild, e, taller))//右子树插入失败
			{
				return false;
			}
			if(taller)					//右子树插入成功，且右子树增高
			{
				switch(t->bf)
				{
					case LH:			//原来t的左子树高于右子树
						t->bf = EH;
						taller = false;
						break;
					case EH:			//原来t的左子树和右子树等高
						t->bf = RH;
						taller = true;
						break;
					case RH:			//原来t的右子树高于左子树
						rightBalance(t);//做右平衡处理
						taller = false;
						break;
				}
			}
		}
	}
	return true;						//插入成功
}


bool deleteAVL(AVL& t, int key, bool& shorter)
{
	if(t == NULL)						//不存在该元素
	{
		return false;					//删除失败
	}
	else if((key)==(t->data))		//找到元素结点
	{
		AVLNode* q = NULL;
		if(t->lchild == NULL)			//左子树为空
		{
			q = t;
			t = t->rchild;
			delete q;
			shorter = true;
		}
		else if(t->rchild == NULL)		//右子树为空
		{
			q = t;
			t = t->lchild;
			delete q;
			shorter = true;
		}
		else							//左右子树都存在,
		{
			q = t->lchild;
			while(q->rchild)
			{
				q = q->rchild;
			}
			t->data = q->data;
			deleteAVL(t->lchild, q->data, shorter);	//在左子树中递归删除前驱结点
		}
	}
	else if((key)<(t->data))		//左子树中继续查找
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
					rightBalance(t);	//右平衡处理
					if(t->rchild->bf == EH)//注意这里，画图思考一下
						shorter = false;
					else
						shorter = true;
					break;
			}
		}
	}
	else								//右子树中继续查找
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
					leftBalance(t);		//左平衡处理
					if(t->lchild->bf == EH)//注意这里，画图思考一下
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
    cout<<"中序遍历 "<<endl;
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
		cout<<"0 打印"<<endl
			<<"1 插入"<<endl
			<<"2 删除"<<endl
			<<"3 查找"<<endl
			<<"4 结束"<<endl
			<<"         "<<endl
			<<"请输入您要进行的操作： ";
		cin>>choice;
		switch(choice)
		{
			case 0:
				printAVL(t);
				cout<<endl;
				break;
			case 1:
				cout<<endl<<"请输入要插入的数值，输入0表示结束"<<endl;
				while(cin>>key && key)
				{
					if(insertAVL(t,key,taller))

					{
						cout<<"数值 "<<key<<" 插入成功！"<<endl;
					}
					else
					{
						cout<<"插入失败！此树中含有此数值 "<< endl;
					}
				}
				cout<<endl;
				break;

			case 2:
				cout<<endl<<"请输入要删除的数值，输入0表示结束："<<endl;
				while(cin>>key && key)
				{
					if(deleteAVL(t,key,shorter))
					{
						cout<<"数值 "<<key<<" 删除成功！"<<endl;
					}
					else
					{
						cout<<"此树中没要找到这个要删除的数值！"<<endl;
					}
				}
				cout<<endl;
				break;

			case 3:
				cout<<endl<<"请输入要查找的数值，输入0表示结束:"<<endl;
				while(cin>>key && key)
				{
					if(searchAVL(t,key))
						cout<<key<<" 查找成功！"<<endl;
					else
						cout<<" 树种不存在此数值！查找失败"<<endl;
				}
				cout<<endl;
				break;

			case 4:
				flag = false;
				break;

			default:
				cout<<"您所输入的操作数字有误！请重新输入！"<<endl;

		}
	}

    return 0;
}
