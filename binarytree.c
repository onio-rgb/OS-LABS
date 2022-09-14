
#include<stdlib.h>

int printf(const char * format , ...);
struct btree{
    int i; 
    struct btree* left;
    struct btree* right;
};    

void insert(struct btree** t,int x){
     if(*t==NULL){
          *t=(struct btree*)malloc(sizeof(struct btree));
          (*t)->i=x;
     }
     else if((*t)->i>x)
     insert(&((*t)->left),x);

     else insert(&((*t)->right),x);
}
void inorder(struct btree** t){
     if((*t)==NULL)return;
     inorder(&((*t)->left));

     printf("%i \n",(*t)->i);
     inorder(&((*t)->right));
}
int main()
{
    struct btree* root=NULL;
    insert(&(root),10);
    insert(&(root),20);
    insert(&(root),5);
    inorder(&(root));
}
