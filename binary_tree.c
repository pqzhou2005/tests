#include <stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct node
{
	int data;
	struct node *left;
	struct node *right;
	struct node *parent;
}node,*Proot;

void create(node **root);

int insert(node **root,int data);

int delete(node **root,int data);

node *search(node *root,int data);

int traversal(node *root);
/**
int serachMin();

int serachMax();
*/
int main(int args,char **argv)
{
	node *root=NULL,*pnode=NULL;
	create(&root);
	
	traversal_2(root);
	/*pnode = search(root,2);
	
	if(pnode!=NULL){
		printf("%d\n",pnode->data);
	}else{
		printf("%s\n","not found!");
	}*/

}


void create(node **root)
{
        insert(root,4);
        insert(root,7);
        insert(root,1);
        insert(root,2);
        insert(root,43);	
        insert(root,5);
}

int traversal(node *root)
{
	if(root!=NULL)
	{
		if(root->left!=NULL) printf("left:%d\n",root->left->data);
		traversal(root->left);
		printf("%d\n",root->data);
		if(root->right!=NULL) printf("right:%d\n",root->right->data);
		traversal(root->right);
	}
}

int traversal_2(node *root)
{
	node *stack,*p;
	stack = (node *)malloc(sizeof(node)*30);
	memset(stack,0,sizeof(node)*30);
	int i = 0;
	
	p = root;
	while(p!=NULL || i>0)
	{
		while(p!=NULL)
		{
			stack[i]=*p;
			printf("push stack i:%d data:%d\n",i,p->data);
			p = p->left;
			i++;
		}
		
		i--;
		p = &stack[i];
		printf("pop stack i:%d data:%d\n",i,p->data);

		printf("%d\n",p->data);

		p = p->right;		
	}
}

int insert(node **root,int data)
{
	if((*root)==NULL)
	{
		node *pnode = (node *)malloc(sizeof(node));
		if(pnode==NULL){
			perror("malloc is error");
			return ;
		}
		memset(pnode,0,sizeof(node));
		pnode->left = pnode->right = pnode->parent = NULL;
		pnode->data = data;
		(*root) = pnode;
		return ;
	}

	if((*root)->right==NULL && (*root)->data<data)
	{
		node *pnode = (node *)malloc(sizeof(node));
                if(pnode==NULL){
			perror("malloc is error");
			return ;
		}
		memset(pnode,0,sizeof(node));
		pnode->left = pnode->right = NULL;
                pnode->data = data;
		pnode->parent = (*root);
                (*root)->right = pnode;
		return ;
	}

	if((*root)->left==NULL && (*root)->data>data)
	{
		node *pnode = (node *)malloc(sizeof(node));
		if(pnode==NULL){
			perror("malloc is error");
			return ;
		}
		memset(pnode,0,sizeof(node));
                pnode->left = pnode->right = pnode->parent = NULL;
                pnode->data = data;
                pnode->parent = (*root);
		(*root)->left = pnode;
		return ;
	}

	if((*root)->data<data)
	{
		insert(&(*root)->right,data);
	}
	else
	{
		insert(&(*root)->left,data);
	}
	
}

int delete(node **root,int data)
{
	node *p,*q = NULL;
	p = search(*root,data);
	if(p==NULL) return -1;	

	if(p->left==NULL && p->right==NULL)
	{
		if(p->parent==NULL)
		{
			free(p);
			*root = NULL;
		}
		else
		{
			if(p->parent->left==p)
			{
				p->parent->left = NULL;
			}
			else
			{
				p->parent->right = NULL;
			}
			free(p);
		}
	}
	else if(p->left==NULL && p->right!=NULL)
	{
		if(p->parent==NULL)
		{
			p->right->parent = NULL;
			*root = p->right;
			free(p);
		}
		else
		{
			p->right->parent = p->parent;
			if(p->parent->left==p)
			{
				p->parent->left = p->right;	
			}
			else
			{
				p->parent->right = p->right;
			}
			free(p);
		}
	}
	else if(p->left!=NULL && p->right==NULL)
	{
		if(p->parent==NULL)
		{
			p->left->parent = NULL;
			*root = p->left;
			free(p);
		}
		else
		{
			p->left->parent = p->parent;
			if(p->parent->left==p)
			{
				p->parent->left = p->left;
			}
			else
			{
				p->parent->right = p->left;
			}
		}
	}
	else
	{	
			q = p->left;
			while(q->right!=NULL)
			{
				q = q->right;
			}
			
			p->data = q->data;
			
			if(q->left!=NULL)
			{
				q->left->parent = q->parent;
				q->parent->right = q->left;		
			}else
			{
				q->parent->right = q->left;
			}
			
			free(q);				
	}
	
	return 0;
}

node *search(node *root,int data)
{
	if(root==NULL)
	{
		return NULL;
	}
	
	if(root->data<data)
	{
		search(root->right,data);	
	}
	else if(root->data>data)
	{
		search(root->left,data);
	}
	else
	{
		return root;
	}
}
