
// Online IDE - Code Editor, Compiler, Interpreter

#include<iostream>

using namespace std;
typedef struct bstree* bstptr;
struct bstree{
    bstptr lc=NULL,rc=NULL;
    int val;
};
void insert(bstptr &t,int x)
{
    if(!t)
    {
        t=new bstree();
        t->val=x;
        return;
    }
    if(t->val>x)insert(t->lc,x);
    else insert(t->rc,x);
}

void preorder_print(bstptr &t)
{
    if(!t)return ;
    cout<<t->val<<" ";
    preorder_print(t->lc);
    preorder_print(t->rc);
}

void postorder_print(bstptr &t)
{
    if(!t)return ;
    postorder_print(t->lc);
    postorder_print(t->rc);
    cout<<t->val<<" ";
}

void inorder_print(bstptr &t)
{
    if(!t)return;
    inorder_print(t->lc);
    cout<<t->val<<" ";
    inorder_print(t->rc);
}

int main()
{
    bstptr t=NULL;
    insert(t,9);
    insert(t,10);
    insert(t,1);
    insert(t,100);
    insert(t,29);
    insert(t,200);
    insert(t,290);
    preorder_print(t);
    cout<<endl;
    postorder_print(t);
    cout<<endl;
    inorder_print(t);
}
