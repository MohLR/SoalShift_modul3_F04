#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <termios.h>

int status,hunger,hygiene,health,makanan,monster,over;
time_t bath;
int *stok;
pthread_t tid[6];

void* display(void* arg){
	while (!over) {
		time_t current=time(&current);
		if (status==1) {
			/* standby */
			system("clear");
			printf("Standby Mode\nHealth : %d\nHunger : %d\nHygiene : %d\nFood left : %d\n", health,hunger,hygiene,makanan);
			if(bath==0||current-bath>=20){
				printf("Bath is ready\n");
			} else{
				printf("Bath will be ready in %lds\n",20-(current-bath));
			}
			printf("Choices\n1. Eat\n2. Bath\n3. Battle\n4. Shop\n5. Exit\n");
		} else if (status==0) {
			/*battle*/
			system("clear");
			printf("Battle Mode\nMonster's Health :%d\nEnemy's Health :%d\nChoices\n1. Attack\n2. Run\n", health,monster);
		} else {
			/* shop */
			system("clear");
			printf("Shop Mode\nShop food stock : %d\nYour food stock : %d\nChoices\n1. Buy\n2. Back\n", *stok,makanan);
		}
		sleep(1);
	}
}

void* input(void* arg){
	while(!over){
		if(status==1){
			monster=100;
			time_t current=time(&current);
			int c=getchar();
			switch (c) {
				case '1':
					if (makanan>0) {
						makanan-=1;
						if(hunger+15>200) hunger=200;
						else hunger+=15;
					}
					break;
				case '2':
					if (bath==0||current-bath>=20) {
						bath=current;
						if (hygiene+30>100) hygiene=100;
						else hygiene+=30;
					}
					break;
				case '3':
					status-=1;
					break;
				case '4':
					status+=1;
					break;
				case '5':
					over=1;
					break;
				default:
					break;
			}
		}else if (status==0) {
			int c=getchar();
			if(c=='1'){
				system("clear");
				printf("You attacked!\nThe enemy took 20 damage.\n");
				monster-=20;
				if(monster<=0) {
					printf("The enemy died.\n");
					status+=1;
				}
				else {
					printf("The enemy attacked!\nYou took 20 damage.\n");
					health-=20;
					if(health<=0){
						printf("You died.\n");
						over=1;
					}
				}
			} else if(c=='2'){
				system("clear");
				printf("Succesfully fled.\n");
				status+=1;
			}
		}else{
			int c=getchar();
			if(c=='1'&&(*stok>0)){
				makanan+=1;
				*stok-=1;
			} else if(c=='1'&&(*stok<=0)){
				system("clear");
				printf("Shop's stock is empty\n");
			}else if(c=='2'){
				system("clear");
				printf("You left the shop\n");
				status-=1;
			}
		}
	}
}

void* hungry(void* arg){
	while(!over){
		if(status==1){
			sleep(10);
			hunger-=5;
			if(hunger<=0) over=1;
		}
	}
}

void* dirty(void* arg){
	while(!over){
		if(status==1){
			sleep(30);
			hygiene-=10;
		}
	}
}

void* regen(void* arg){
	while(!over){
		if(status==1){
			sleep(10);
			if(health+5>300) health=300;
			else health+=5;
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
	printf("Permainan berakhir.\n");
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

int main(){
	key_t key = 38435;

	int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
	stok = shmat(shmid, NULL, 0);
	*stok=0;
	printf("Masukkan nama monstermu: ");
	char nama[1000];
	scanf("%s",nama);
	printf("Nama monstermu adalah: ");
	int i;
	for(i=0;i<=strlen(nama);i++){
		putchar(nama[i]);
	}
	printf("\n");
	sleep(1);
	over=0;
	bath=0;
	status=1;
	hunger=200;
	hygiene=100;
	health=300;
	makanan=0;
	int err;
	err=pthread_create(&tid[0],NULL,&term,NULL);
	if(err!=0){
		printf("\ncan't create thread: [%s]", strerror(err));
		exit(EXIT_FAILURE);
	}
	err=pthread_create(&tid[1],NULL,&display,NULL);
	if(err!=0){
		printf("\ncan't create thread: [%s]", strerror(err));
		exit(EXIT_FAILURE);
	}
	err=pthread_create(&tid[2],NULL,&input,NULL);
	if(err!=0){
		printf("\ncan't create thread: [%s]", strerror(err));
		exit(EXIT_FAILURE);
	}
	err=pthread_create(&tid[3],NULL,&hungry,NULL);
	if(err!=0){
		printf("\ncan't create thread: [%s]", strerror(err));
		exit(EXIT_FAILURE);
	}
	err=pthread_create(&tid[4],NULL,&dirty,NULL);
	if(err!=0){
		printf("\ncan't create thread: [%s]", strerror(err));
		exit(EXIT_FAILURE);
	}
	err=pthread_create(&tid[5],NULL,&regen,NULL);
	if(err!=0){
		printf("\ncan't create thread: [%s]", strerror(err));
		exit(EXIT_FAILURE);
	}
	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);
	pthread_join(tid[2],NULL);
	pthread_join(tid[3],NULL);
	pthread_join(tid[4],NULL);
	pthread_join(tid[5],NULL);
	shmdt(stok);
	shmctl(shmid, IPC_RMID, NULL);
	return 0;
}
