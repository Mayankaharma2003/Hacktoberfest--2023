#include<stdio.h>
# define maxsize 10
int q[maxsize],rear=-1,front=0; 
void insert();
void Delet();
void display();
int main()
{
	int ch;
	do
	{
	    printf("\n---------------queue----------------");
	    printf("\nenter 1 to insert");
	    printf("\nenter 2 to Delet");
	    printf("\nenter 3 to display");
	    printf("\nenter 4 to exit");
	    printf("\nenter your choice");
	    scanf("%d",&ch);
	    switch(ch)
	    {
		    case 1:insert();break;
		    case 2:Delet();break;
		    case 3:display();break;
		    case 4:break;
		    daufalt:printf("\nyou have enter wrong choice");
		}
    }while(ch!=4);
}
void insert()
{
    if(rear==maxsize-1)
    {
	printf("\nqueue overflow");
	}
	else
	{
		int n;
	    printf("\nenter element in queue");
		scanf("%d",&n);
		rear++;
		q[rear]=n;    
    }
}	
void Delet()
{
	if(rear<front)
    {
	printf("\nqueue underflow");
	}
	else
	{
		printf("\nthe deleted element is\t %d",q[front]);
		front++;
	}
}
void display()
{
	if(rear<front)
    {
	printf("\nqueue underflow");
	}
	else
	{
		int i;
		for(i=front;i<=rear;i++)
		{
			printf(" %d\t",q[i]);
		}
	}
}
