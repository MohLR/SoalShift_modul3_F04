#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<time.h>
#define PORT 8000

int server_fd, new_socket;
struct sockaddr_in address;
int opt = 1;
int addrlen = sizeof(address);
char buffer[1024] = {0};
char *pesan="Maaf permintaan tidak dapat diproses", *berhasil="Permintaan telah diproses";
int *stok=0;
pthread_t tid[2];

void* printStok(void* arg){
	while(1){
		printf("Stok Sekarang: %d\n", *stok);
		sleep(5);
	}
}

/* Dari https://www.linuxquestions.org/questions/programming-9/how-could-server-detect-closed-client-socket-using-tcp-and-c-824615/ */
void* handler(void* arg){
	fd_set read_sd;
	FD_ZERO(&read_sd);
   	FD_SET(new_socket, &read_sd);
   	while (1) {
      		fd_set rsd = read_sd;

      		int sel = select(new_socket + 1, &rsd, 0, 0, 0);

      		if (sel > 0) {
         		// client has performed some activity (sent data or disconnected?)
         		int bytes = recv(new_socket, buffer, sizeof(buffer), 0);

         		if (bytes > 0) {
            			// got data from the client.
				if(!strcmp(buffer,"tambah")){
					*stok+=1;
					send(new_socket,berhasil,strlen(berhasil),0);
				}
				else send(new_socket,pesan,strlen(pesan),0);
         		}else if (bytes == 0) {
            			// client disconnected.
            			//break;
				printf("The Client has disconnected.\nTrying to create new connection.\n");
				if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        				perror("accept");
        				exit(EXIT_FAILURE);
    				}else printf("Connection Established\n");
         		}else{
            			// error receiving data from client. You may want to break from
            			// while-loop here as well.
				printf("There's an Error when receiving data from the Cilent.\n");
         		}
      		}else if (sel < 0){
         		// grave error occurred.
			printf("A grave Error occured.\nTrying to create new connection.\n");
			//break;
			if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        			perror("accept");
        			exit(EXIT_FAILURE);
    			}else printf("Connection Established\n");
      		}
   	}
	close(new_socket);
}

int main(){
	key_t key = 184567;

        int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
        stok = shmat(shmid, NULL, 0);
	int err;
	err=pthread_create(&(tid[0]),NULL,&printStok,NULL);
	if(err!=0){
		printf("\ncan't create thread: [%s]", strerror(err));
		exit(EXIT_FAILURE);
	}
      
    	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        	perror("socket failed");
        	exit(EXIT_FAILURE);
    	}
      
   	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        	perror("setsockopt");
        	exit(EXIT_FAILURE);
    	}

    	address.sin_family = AF_INET;
    	address.sin_addr.s_addr = INADDR_ANY;
    	address.sin_port = htons( PORT );
      
    	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        	perror("bind failed");
        	exit(EXIT_FAILURE);
    	}

    	if (listen(server_fd, 1) < 0) {
        	perror("listen");
        	exit(EXIT_FAILURE);
    	}

    	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        	perror("accept");
        	exit(EXIT_FAILURE);
    	}else printf("Connection Established\n");
	err=pthread_create(&(tid[1]),NULL,&handler,NULL);
	if(err!=0){
		printf("\ncan't create thread: [%s]", strerror(err));
		exit(EXIT_FAILURE);
	}
	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);
	shmdt(stok);
        shmctl(shmid, IPC_RMID, NULL);	
	exit(EXIT_SUCCESS);
}
