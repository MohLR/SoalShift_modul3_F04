#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <termios.h>

pthread_t tid[3];
int *stok;
int over;

void* display(void* arg){
  while (!over) {
    system("clear");
    printf("Shop\nFood stock : %d\nChoices\n1. Restock\n2. Exit\n",*stok);
    sleep(1);
  }
}

void* input(void* arg){
  while (!over) {
    int c=getchar();
    if(c=='1'){
      *stok+=1;
      system("clear");
      printf("Restocked.\n");
    }else if(c=='2'){
      over=1;
    }
  }
}

void* term(void* arg){
	/*Mulai code copas dari https://stackoverflow.com/questions/1798511/how-to-avoid-pressing-enter-with-getchar*/
	static struct termios oldt, newt;

  /*tcgetattr gets the parameters of the current terminal
  STDIN_FILENO will tell tcgetattr that it should write the settings
  of stdin to oldt*/
  tcgetattr( STDIN_FILENO, &oldt);
  /*now the settings will be copied*/
  newt = oldt;

  /*ICANON normally takes care that one line at a time will be processed
  that means it will return if it sees a "\n" or an EOF or an EOL*/
  newt.c_lflag &= ~(ICANON | ECHO);

  /*Those new settings will be set to STDIN
  TCSANOW tells tcsetattr to change attributes immediately. */
  tcsetattr( STDIN_FILENO, TCSANOW, &newt);
	/*Akhir code copas*/
	while(!over){
	}
  system("clear");
  printf("You left the shop\n");
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

int main(){
  key_t key = 38435;

	int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
	stok = shmat(shmid, NULL, 0);
  int err;
	err=pthread_create(&tid[0],NULL,&display,NULL);
	if(err!=0){
		printf("\ncan't create thread: [%s]", strerror(err));
		exit(EXIT_FAILURE);
	}
	err=pthread_create(&tid[1],NULL,&input,NULL);
	if(err!=0){
		printf("\ncan't create thread: [%s]", strerror(err));
		exit(EXIT_FAILURE);
	}
  err=pthread_create(&tid[2],NULL,&term,NULL);
	if(err!=0){
		printf("\ncan't create thread: [%s]", strerror(err));
		exit(EXIT_FAILURE);
	}
  pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);
	pthread_join(tid[2],NULL);
  shmdt(stok);
	shmctl(shmid, IPC_RMID, NULL);
	return 0;
}
