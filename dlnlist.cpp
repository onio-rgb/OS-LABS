
// Online IDE - Code Editor, Compiler, Interpreter

#include<iostream>

using namespace std;
typedef struct dlnlist* dlnptr;
struct dlnlist{
    int val;
    dlnptr left=NULL,right=NULL;
    dlnlist(int x)
    {
        val=x;
    }
};
void append_front(dlnptr &head,int x)
{
    if(!head){
        head=new dlnlist(x);
        return;
    }
    dlnptr new_head=new dlnlist(x);
    new_head->right=head;
    head->left=new_head;
    head=new_head;
}
void append_end(dlnptr &head,int x){
    if(!head)
    {
        head=new dlnlist(x);
        return;
    }
    dlnptr t=head;
    while(t->right)t=t->right;
    
    dlnptr temp=new dlnlist(x);
    t->right=temp;
    temp->left=t;
}
void print(dlnptr t)
{
    while(t)
    {
        cout<<t->val<<" ";
        t=t->right;
    }
    cout<<endl;
}
void del_node(dlnptr &root,int x)
{
    dlnptr t=root;
    dlnptr prev=t;
    while(t)
    {
        if(t->val==x)
        {
            if(prev!=t)
            {
                prev->right=t->right;
                dlnptr temp=t;
                t=t->right;
                temp->right=NULL;
                if(t)
                t->left=NULL;
                temp->left=NULL;
                free(temp);
            }
            else {
                root=root->right;
                root->left=NULL;
                t=t->right;
                prev=t;
            }
        }
        else 
        {
            prev=t;
            t=t->right;
        }
    }
}
int main()
{
    dlnptr t=NULL;
    append_end(t,10);
    append_end(t,20);
    append_front(t,8);
    append_front(t,8);
    append_end(t,8);
    print(t);
    del_node(t,8);
    print(t);
}