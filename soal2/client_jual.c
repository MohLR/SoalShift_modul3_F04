#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8000
  
int main(int argc, char const *argv[]) {
    	struct sockaddr_in address;
    	int sock = 0, valread;
    	struct sockaddr_in serv_addr;
    	char *hello;
    	char buffer[1024] = {0};
    	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        	printf("\n Socket creation error \n");
        	return -1;
    	}
  
    	memset(&serv_addr, '0', sizeof(serv_addr));
  
    	serv_addr.sin_family = AF_INET;
    	serv_addr.sin_port = htons(PORT);
      
    	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        	printf("\nInvalid address/ Address not supported \n");
        	return -1;
    	}
  
    	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        	printf("\nConnection Failed \n");
        	return -1;
    	}else printf("Connection Established\n");
	while(1){
		scanf("%s",hello);
		if(!strcmp(hello,"exit")) break;
    		send(sock,hello,strlen(hello),0);
    		printf("Pesan Terkirim\n");
		memset(buffer,0,sizeof(buffer));
    		valread = read(sock,buffer,1024);
    		printf("%s\n",buffer);
	}
    	return 0;
}