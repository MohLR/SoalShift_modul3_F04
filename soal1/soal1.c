
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

unsigned long long hasil[50];
    int n;

    pthread_t tid[50];

unsigned long long fact(int n){

if(n==0 || n==1)
return 1;
else 
return n*fact(n-1);
}
void *factorial(void *p){
int i;
int *x = (int*) p;
pthread_t id = pthread_self();
for (int i=0;i<n;i++) {
	if (pthread_equal(id, tid[i]))
		hasil[*x]=fact(*x);
}

pthread_exit(0);
 
}

int banding (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

int main(int argc, char *argv[])
{ 
    n = argc-1;
    int i;
    int ar[n];
    for(i=0;i<n;i++){
       ar[i] = atoi(argv[i+1]);
    }

    for(i=0;i<n;i++){
        pthread_create(&tid[i],NULL,&factorial,&ar[i]);
    }
    for(i=0;i<n;i++){
        pthread_join(tid[i],NULL);
    }
    qsort(ar, n, sizeof(int), banding);
    for(i=0;i<n;i++){
        printf("%d! = %llu\n",ar[i],hasil[ar[i]]);
    }

}
