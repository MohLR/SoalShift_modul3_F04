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

## Soal 4

## Soal 5
