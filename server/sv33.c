#include "includes.h" // echo_server_IPv4.c
#define BUFSIZE 1024 // client-server 사이 message 의 최대 길이
#define IDSIZE 20
#define PORT_NUM 50000 // server port number
#define MAXCHAR 20
#define arraySize 20
#define PWSIZE 4

int timer[BUFSIZE] = { 0 };

struct order {
	char product[20];
	int quantity;
	int bill;
};

void* auto_logout(void *arg);
void deal_save(char user[], char order[], int quantity, int bill);
//void show_product(char (*products)[MAXCHAR]);
void* ear_sv(void* argument);
//void* deal(void* argument,char message);
int mkfunc(char *dname);

int main(int argc, char **argv)
{
	int serv_sock, clnt_sock;
	int str_len, thread_args, result_code, result_code2, result_code3;
	struct sockaddr_in serv_addr, clnt_addr; //IPv4 address structure
	int clnt_addr_size;
	pthread_t threads;
	pthread_t threads2;
	//pthread_t threads3;
	int fd_serialnum;


	if (argc != 1) { printf("Usage : &s\n", argv[0]); exit(1); }

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1) { perror("socket() error"); exit(1); }
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET; /* Allows IPv4 socket */
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); /* 모든 주소의 client 접속 가능 */
	serv_addr.sin_port = htons(PORT_NUM);
	// server socket binding
	if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) { perror("bind() error"); exit(1); }
	if (listen(serv_sock, 5) == -1) { perror("listen() error"); exit(1); }
	for (;;) { /* 접속된 client 별 thread 생성하여 detach */
		clnt_addr_size = sizeof(clnt_addr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
		if (clnt_sock == -1) { perror("accept() error"); exit(1); }
		printf("Connection from: %s:%d\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));


		thread_args = clnt_sock;

		result_code = pthread_create(&threads, NULL, auto_logout, (void*)&thread_args); // create client thread
		result_code2 = pthread_create(&threads2, NULL, ear_sv, (void*)&thread_args);
		//result_code3 = pthread_create(&threads3, NULL, show_menu, (void*)&thread_args);

		assert(!result_code);
		assert(!result_code2);
		//assert(!result_code3);

		result_code = pthread_detach(threads); // detach echo thread
		result_code2 = pthread_detach(threads2);
		//result_code3 = pthread_detach(threads3);

		assert(!result_code);
		assert(!result_code2);
		//assert(!result_code3);

	}

	close(serv_sock);
	return 0;


}

void* ear_sv(void* arg2) /* argument로 client socket을 전달받음 */
{
	int i = 0, n = 0, nb = 0, bill;
	int fd;
	int clnt_sock;
	int quantity;
	int nread;
	int member_flag;
	struct order buf, buf1;
	int cnt = 0;
	pthread_t thread;
	char message[BUFSIZE] = "0", cwd[4096];
	char user[BUFSIZE];
	char deal_msg[BUFSIZE] = "0";
	char buffer[BUFSIZE] = "0";
	int strLen, size;
	char home[BUFSIZE] = "/home/hong/System_programming/Flower", now[BUFSIZE];
	clnt_sock = *(int*)arg2;

	char products[arraySize][MAXCHAR] = { "Rose Bouquet","Tulip Bouquet","Lily Bouquet","White Boupuet",
										"Rose","Tulip","" }; //20 chars * 20 products
	char price[arraySize][MAXCHAR] = { "34000","38000","44000","100000","3000","2700","" };
	char order[MAXCHAR];
	int month = 0, day = 0;


	while (((strLen = read(clnt_sock, message, BUFSIZE)) > 0) && timer[clnt_sock] < 300) {
		message[strlen(message)] = '\0';
		timer[clnt_sock] = 0;

		//printf("cnt : %d\n", cnt);
		//printf("strlen : %d\n",strLen);

		/*Make member directory*/
		if (cnt == 0) {
			chdir(home);
			printf("User ID: %s\n", message);
			strcpy(user, message);

			member_flag = mkfunc(user); // make user directory, profile
			if (member_flag == 0)
				write(clnt_sock, "Welcome! New member!", BUFSIZE);
			//strcpy(account,message);

			write(clnt_sock, "Write your PW (****) ", BUFSIZE);
		}

		/*Save password for a new member or
		Check if password is correct and retry*/
		else if (cnt == 1) {
			printf("PW : %s\n", message); //recive pw, save to Profile

			chdir(home);
			chdir(user);
			getcwd(now, 4096);
			fprintf(stderr, "now lotation 3 : %s\n", now);

			if ((fd = open("Profile", O_RDWR)) < 0)
				return -1;
			printf("Open %s's profile\n", user);

			if ((nread = read(fd, &buffer, PWSIZE)) == 0)
			{
				printf("File is empty. Save password!\n");
				write(fd, message, PWSIZE);
				close(fd);    // 파일 포인터 닫기
				write(clnt_sock, "Login Successful!", BUFSIZE);
				write(clnt_sock, "Product", BUFSIZE);

			}

			else {
				printf("Check password\n");

				if (strcmp(buffer, message) == 0) {
					write(clnt_sock, "Login Successful!", BUFSIZE);
					write(clnt_sock, "Product", BUFSIZE);
					printf("Pass to log in\n ");
				}
				else {
					write(clnt_sock, "Failed Login", BUFSIZE);
					write(clnt_sock, "Retry", BUFSIZE);
					printf("Fail to log in\n");
					cnt--;
				}
			}

			close(fd);

		}

		/*Show products*/
		else if (cnt == 2) {
			printf("%s\n", message);

			if (strcmp(message, "Product") == 0) //Show the products
			//res_cmp2 = strcmp(deal_msg,"save"); //Save the day

			//write(clnt_sock,"Product",BUFSIZE);
			{
				for (n = 0;n < arraySize;n++) {
					//printf("%d\n",strlen(products[n]));
					if (strlen(products[n]) > 1)
					{
						write(clnt_sock, products[n], BUFSIZE);
						write(clnt_sock, price[n], BUFSIZE);
						printf("(%d)%s: %swon\n", n + 1, products[n], price[n]);

					}

				}
				write(clnt_sock, "Product_end", BUFSIZE);
				write(clnt_sock, "Which number do you want?", BUFSIZE);

			}
		}
		/*Order*/
		else if (cnt == 3) {
			nb = atoi(message) - 1;
			strcpy(order, products[nb]);
			write(clnt_sock, "How many do you want to order?", BUFSIZE);
		}
		/*Order quantity*/
		else if (cnt == 4) {
			quantity = atoi(message);
			bill = (atoi(price[nb]))*(quantity);
			write(clnt_sock, "detail", BUFSIZE);
			printf("User: %s, Order: %s, Quantity: %d,Bill: %d\n", user, order, quantity, bill);
			/*
			//deal_save(user,order,order_quantity,bill);
			*/
			int fd3, nread, i = 0, nwrtie;
			struct order buf;
			char buffer[BUFSIZE];
			char d_now[4096];

			//chdir("..");
			getcwd(d_now, 4096); //"/home/hong/System_programming/Flower"
			printf("now lotation: %s\n", d_now);

			if ((fd3 = open("orderbook", O_RDWR)) < 0) {
				perror("open");
				exit(EXIT_FAILURE);
			}
			/*
							printf("fd3 %d\n",fd3);
							nwrtie=write(fd3,order,MAXCHAR);
							printf("%d\n",nwrtie);
							nwrtie=write(fd3,quantity,MAXCHAR);
							printf("%d\n",nwrtie);
							nwrtie=write(fd3,bill,MAXCHAR);
							printf("%d\n",nwrtie);
							/*
							 while((nread=pread(fd,&buf,sizeof(struct order),sizeof(struct order)*i))>0){
								printf("%d\n",i);
								if(strcmp(buf.product,"NO")==0);
									break;
								i++;

							}
						*/
						//printf("%d\n",i);
			strcpy(buf.product, order);
			//printf("%s %s\n",buf.product,order);

			//strcpy(buf.quantity,char(quantity));
			buf.quantity = quantity;
			//strcpy(buf.bill,char(bill));
			buf.bill = bill;


			nwrtie = write(fd, &buf, sizeof(struct order));
			printf("%d\n", nwrtie);

			printf("User: %s\nOrder: %s\nQuantity: %d\nBill: %d\n", user, buf.product, buf.quantity, buf.bill);

			write(clnt_sock, &buf, sizeof(struct order));
			close(fd3);
			//write(clnt_sock,"Detail_end",BUFSIZE);
			//write(clnt_sock,"Complete to order! If you want details, write 'detail' ",BUFSIZE); //pick one
			//write(clnt_sock,"Or if you want to logout, write 'q' ",BUFSIZE);
		}

		else if (cnt >= 5) {
			write(clnt_sock, "Thank you! Come and get your itme!", BUFSIZE);
			close(clnt_sock);
		}
		cnt++;
		printf("%d\n", cnt);
		memset(message, 0, sizeof(message));
	}
	close(clnt_sock);
}
void deal_save(char user[], char order[], int quantity, int bill) {
	int fd, nread, i = 0, nwrtie;
	struct order buf;
	char buffer[BUFSIZE];
	char d_now[4096];

	getcwd(d_now, 4096); //"/home/hong/System_programming/Flower"
	printf("now lotation: %s\n", d_now);

	if ((fd = open("orderbook", O_RDWR)) < 0) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	printf("Open orderbook\n");
	/*
	 while((nread=pread(fd,&buf,sizeof(struct order),sizeof(struct order)*i))>0){
		printf("%d\n",i);
		if(strcmp(buf.product,"NO")==0);
			break;
		i++;

	}
*/
	printf("%d\n", i);
	strcpy(buf.product, order);
	printf("%s %s\n", buf.product, order);
	strcpy(buf.quantity, quantity);
	strcpy(buf.bill, bill);

	nwrtie = write(fd, &buf, sizeof(struct order));
	printf("%d\n", nwrtie);

	printf("User: %s\n, Order: %s\n, Quantity: %d\n, Bill: %d\n", user, buf.product, buf.quantity, buf.bill);
	close(fd);

}
int mkfunc(char *dname) //make directory(=account) and file(=bankbook)
{
	int fd, fd2, i = 0;
	int mkdir_res, chdir_res;
	char d_home[4096] = "/home/hong/System_programming/Flower"; //my working directory
	char d_now[4096];
	int res_cmp;
	char serial_buf[20];

	getcwd(d_now, 4096);
	fprintf(stderr, "now lotation 1 : %s\n", d_now);

	if (strcmp(d_now, d_home) != 0) //when differnt
	{
		chdir(d_home);
		getcwd(d_now, 4096);
		fprintf(stderr, "now lotation 2 : %s\n", d_now);
	}
	i = chdir(dname);
	if (i < 0) { //if -1, new member
		mkdir_res = mkdir(dname, 0777);
		// old member
		if (mkdir_res == -1) {
			printf("Already the member of this service.\n");
			perror("mkdir");
			return mkdir_res;
		}
		chdir(dname);
		if ((fd = creat("Profile", 0777)) < 0) {
			perror("creat");
			exit(EXIT_FAILURE);
		}
		if ((fd2 = creat("orderbook", 0777)) < 0) {
			perror("creat");
			exit(EXIT_FAILURE);
		}
	}

	//if 0, old member
	else if (i == 0) {
		return -1;
	}
	close(fd);
	close(fd2);
	return 0;
}

void show_product(char(*products)[MAXCHAR]) {
	int nread, fd;
	char buffer[MAXCHAR];
	int i = 0;
	char d_now[4096], d_home[4096] = "/home/hong/System_programming/Flower";
	FILE *fp;

	int n = 0, l = 0;

	//	clnt_sock = *(int*)arg;
	if (chdir("..")) {
		perror("chdir");
		exit(EXIT_FAILURE);
	}

	getcwd(d_now, 4096);
	fprintf(stderr, "now lotation : %s\n", d_now);


	fp = fopen("stock", "r");
	if (fp == NULL) {
		printf("Could not open file\n");
		return 1;
	}

	//product maxium 20

	while (fgets(buffer, MAXCHAR, fp) != NULL) {
		buffer[strlen(buffer)] = '\0';
		printf("%d: %s\n", i, buffer);
		for (n = 0;n < arraySize;n++)
			(*products)[n] = buffer;
		i++;

	}
	printf("\n");

	fclose(fp);

}
void* auto_logout(void *arg)
{
	int clnt_sock;

	clnt_sock = *(int*)arg;
	while (1) {
		timer[clnt_sock] ++;
		sleep(1);
		printf("time : %d\n", timer[clnt_sock]);
		if (timer[clnt_sock] > 300)
			break;
	}
	timer[clnt_sock] = 0;
	write(clnt_sock, "BYE!!", BUFSIZE);
	close(clnt_sock);
}