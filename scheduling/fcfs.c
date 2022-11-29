#include<stdio.h> 
#include<sys/time.h>

void sort(int at[],int d[])
{
    int n=4;
    for(int i=0;i<n-1;i++)
    {
        for(int j=0;j<n-i;j++)
        {
            if(at[j]>at[j+1])
            {
                int c=at[j];
                at[j]=at[j+1];
                at[j+1]=c;
                c=d[j];
                d[j]=d[j+1];
                d[j+1]=c;
            }
        }
    }
}
void waiting_time(int at[],int d[])
{
    int s=at[0];
    int n=4;
    int wt[n];
    wt[0]=0;
    int wtsum=0;
    for(int i=1;i<n;i++)
    {
        s=s+d[i-1];
        wt[i]=s-at[i];
        if(wt[i]<0)wt[i]=0;
        wtsum+=wt[i];
    }
    for(int i=0;i<n;i++)printf("%d ",wt[i]);
    int avg=wtsum/n;
    printf("\n average wt : %d\n",avg);
}
void turnaround_time(int at[],int d[])
{
    int s=at[0];
    int n=4;
    int tt[n];
    tt[0]=d[0];
    int ttsum=0;
    for(int i=1;i<n;i++)
    {
        s=s+d[i-1];
        tt[i]=s-at[i]+d[i];
        if(tt[i]<0)tt[i]=0;
        ttsum+=tt[i];
    }
    for(int i=0;i<n;i++)printf("%d ",tt[i]);
    int avg=ttsum/n;
    printf("\n average tt : %d\n",avg);
}
int main()
{
    int arrival_time[]={8,5,3,13};
    int duration[]={8,15,10,4};
    sort(arrival_time,duration);
    waiting_time(arrival_time,duration);
    turnaround_time(arrival_time,duration);
}