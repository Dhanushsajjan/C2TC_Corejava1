#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#define MAX 100

void *even(void *);
void *odd(void *);
static int count=0;
int main()
{
	pthread_t t1,t2;

	pthread_create(&t1,NULL,&even,NULL);
	pthread_create(&t2,NULL,&odd,NULL);

	pthread_join(t1,NULL);
	pthread_join(t2,NULL);

	pthread_exit(0);
	return 0;

}
void *even(void *)
{
	while(count<MAX)
	{
	    while(count%2==0)
		{
		  printf("%d\t",count);
	          count++;
		}
	}
}
void *odd(void *)
{
	while(count<MAX)
	{
		printf("%d\t",count);
		count++;
	}

}
