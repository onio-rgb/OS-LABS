
// Online IDE - Code Editor, Compiler, Interpreter

#include<iostream>

using namespace std;
typedef struct lnlist* lnptr;
struct lnlist{
    int val;
    lnptr next=NULL;
    lnlist(int x)
    {
        val=x;
    }
};
void append_end(lnptr &root,int x)
{
    if(!root)
    {
        root=new lnlist(x);
        return;
    }
    lnptr t=root;
    while(t->next)
    t=t->next;
    t->next=new lnlist(x);
}
void del_node(lnptr &root,int x)
{
    lnptr t=root;
    lnptr prev=t;
    while(t)
    {
        if(t->val==x)
        {
            if(prev!=t)
            {
                prev->next=t->next;
            }
            else root=root->next;
            lnptr temp=t;
            t=t->next;
            free(temp);
        }
        else 
        {
            prev=t;
            t=t->next;
        }
    }
}
lnptr find(lnptr t,int x)
{
    while(t)
    {
        if(t->val==x)return t;
        t=t->next;
    }
    return NULL;
}
void append_front(lnptr &root,int x)
{
    if(!root)
    {
        root=new lnlist(x);
        return;
    }
    lnptr new_root=new lnlist(x);
    new_root->next=root;
    root=new_root;
}
void print(lnptr t)
{
    while(t)
    {
        cout<<t->val<<" ";
        t=t->next;
    }
    cout<<endl;
}
int main()
{
    lnptr t=NULL;
    append_end(t,10);
    append_end(t,20);
    append_front(t,5);
    append_end(t,5);
    append_end(t,90);
    append_front(t,80);
    print(t);
    del_node(t,5);
    print(t);
}