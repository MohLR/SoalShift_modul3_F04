# SoalShift_modul3_F04
Jawaban Soal Shift Sisop Modul 3

## Soal 1
Pada soal 1, kita diminta untuk membuat program yang dapat ngeprint hasil dari faktorial yang kita input secara berurutan dari terkecil hingga terbesar. Sekarang kami akan menjelaskan potongan kodenya.

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
Disini kami membuat array yang menyimpan hasil, banyak angka, dan id thread. Lalu membuat fungsi untung menghitung faktorialnya secara rekursif.

	int banding (const void * a, const void * b){ 
	return ( *(int*)a - *(int*)b );
	}
Disini kami membuat fungsi untuk membandingkan inputnya.

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
Pada main kami merubah inputan dikurang 1 karena variabel argc menyimpan banyaknya argumen beserta nama file. Lalu kami membuat agar string dirubah ke integer. Lalu membuat sebanyak input yang kita masuki lalu dijoinkan. Setelah itu disorting yang sudah dibandingkan pada fungsi banding. Lalu diprint.

## Soal 2
Pada soal 2, kita diminta untuk membuat suatu server penjual dan pembeli yang hanya bisa diakses oleh satu client pada masing-masing server. server penjual dan pembeli saling berbagi resource yaitu stok. 
Kami membuat 4 program untuk memenuhi kebutuhan yaitu program server penjual, program server pembeli, program client penjual, dan program client pembeli. pada setiap program server, terdapat thread yang akan siap menerima sesuatu yang dikirim oleh client setiap saat, potongan kodenya dari server pembeli adalah
```
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
							if(!strcmp(buffer,"beli")){
								if(*stok==0) send(new_socket,gagal,strlen(gagal),0);
								else{
									*stok-=1;
									send(new_socket,berhasil,strlen(berhasil),0);
								}
							}else send(new_socket,pesan,strlen(pesan),0);
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
```
terdapat sedikit perbedaan dengan server penjual yaitu pada
```
if (bytes > 0) {
  // got data from the client.
  if(!strcmp(buffer,"beli")){
    if(*stok==0) send(new_socket,gagal,strlen(gagal),0);
    else{
      *stok-=1;
      send(new_socket,berhasil,strlen(berhasil),0);
    }
  }else send(new_socket,pesan,strlen(pesan),0);
}
```
pada server penjual, hanya dilihat apakah yang dikirim adalah tambah, sedangkan pada server pembeli dilihat ketika yang dikirm adalah beli, maka dicek dulu stoknya, jika ada maka berhasil jika tidak maka gagal.
Untuk server penjual, ada thread lain yang selalu men-print stok sekarang setiap 5 detik. Potongan kodenya adalah
```
void* printStok(void* arg){
	while(1){
		printf("Stok Sekarang: %d\n", *stok);
		sleep(5);
	}
}
```
## Soal 3
Saya masih belum bisa dan belum mengerti.

## Soal 4
Tidak tahu cara mengambil 10 list dari list process yang berjalan bersamaan

## Soal 5
Pada soal 5, kita diminta membuat suatu game. game ini cukup sederhana yaitu terdapat 3 mode yaitu standby, battle, dan shop.
pada standby mode; hunger,hygiene akan berkurang sedangkan health akan bertambah kami membuat tiga buat thread berbeda yang bekerja sendiri-sendiri.
Berikut potongan codenya:
```
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
```
untuk membuat terminal menerima input pada ketikan keyboard, kami mengubah mode dari terminal, hal ini ada pada suatu thread.
Berikut potongan kodenya:
```
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
```
Lalu ada thread yang menunggu input tersendiri. kodenya adalah
```
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
```
setelah itu ada thread tersendiri lagi untuk display. kodenya adalah:
```
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
```

Terdapat program penjual juga untuk menyetok makanan pada toko. Pada programnya, kami membuat tiga thread berbeda yaitu:

Untuk mengganti terminal:
```
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
```
Untuk menerima input:
```
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
```
Untuk mendisplay:
```
void* display(void* arg){
  while (!over) {
    system("clear");
    printf("Shop\nFood stock : %d\nChoices\n1. Restock\n2. Exit\n",*stok);
    sleep(1);
  }
}
```
