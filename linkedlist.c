#include<stdlib.h>
struct llist{
	int val;
	struct llist* next;
};
void insert(struct llist* t,int x)
{
	if(t==NULL){
		t=(struct llist*) malloc(sizeof(struct llist));
		t->val=x;
		return;
        }
	while(t->next!=NULL)t=t->next;
	t->next=(struct llist*) malloc(sizeof(struct llist));
	t->val=x;
}
void insert_front(struct llist**  t,int x)
{
	new_head=(struct llist*) malloc(sizeof(struct llist);
	new_head->val=x;
	new_head->next=(*t)->next;
	*t=new_head;
}
void print (struct llist* t)
{
	while(t!=NULL){
	printf("%i ",t->val);
	t=t->next;
        }
	printf("\n");
}
int main(){
	head=(struct llist*) malloc(sizeof(struct llist));

}
	

