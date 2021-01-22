#include "includes.h" // echo_server_IPv4.c
#define BUFSIZE 1024 // client-server 사이 message 의 최대 길이
#define IDSIZE 1024
#define PORT_NUM 50000 // server port number

struct order {
	char product[20];
	int quantity;
	int bill;
};

void* mouth_sv(void* argument); // 실행 될 thread 함수
void* ear_sv(void* argument);
void menual(char **user);
int main(int argc, char **argv)
{

	int sock;
	char message[BUFSIZE];
	int str_len, thread_args, result_code, result_code2, result_code3;
	struct sockaddr_in serv_addr, clnt_addr; //IPv4 address structure
	int clnt_addr_size;
	pthread_t threads;
	pthread_t threads2;
	pthread_t threads3;
	if (argc != 2) { printf("Usage : %s <IP> \n", argv[0]); exit(1); }
	sock = socket(PF_INET, SOCK_STREAM, 0); /* socket type
	을 stream socket으로 setting */
	if (sock == -1) { perror("socket() error"); exit(1); }
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(PORT_NUM);
	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) { perror("connect() error"); }
	menual(argv);

	thread_args = sock;
	result_code = pthread_create(&threads, NULL, mouth_sv, (void*)&thread_args); // create client thread
	result_code2 = pthread_create(&threads2, NULL, ear_sv, (void*)&thread_args);
	//result_code3 = pthread_create(&threads3, NULL, recv_struct, (void*)&thread_args);
	assert(!result_code);
	assert(!result_code2);
	//assert(!result_code3);
	result_code = pthread_detach(threads); // detach echo thread
	result_code2 = pthread_detach(threads2);
	//result_code3 = pthread_detach(threads3);
	assert(!result_code);
	assert(!result_code2);
	//assert(!result_code3);


	while (1);


	return 0;
}
// client message를 전달받아 다시 client에 전송하는 thread 함수
void* mouth_sv(void* arg) /* Send to Server */
{
	int clnt_sock;
	clnt_sock = *(int*)arg;
	char message[BUFSIZE] = "0", msg_before[BUFSIZE] = "0";
	int cnt = 0, num;
	int otp, otp_cnt = 0;
	int strLen;
	int res_strcmp;
	time_t current_time;
	int month = 0, day = 0;
	while (1) {
		fgets(message, BUFSIZE, stdin); /* get message from standard input stream */
		message[strlen(message) - 1] = '\0';
		if (strcmp(message, "q") == 0) {
			fprintf(stderr, "Bye!\n");
			break;
		}
		else if (strcmp(message, "y") == 0) {
			printf("Rewrite password\n");
			cnt = 0;
		}
		else if (cnt == 0) {
			printf("Hi %s\n", message);
			write(clnt_sock, message, strlen(message)); //Send Id 
		}
		else if (cnt == 1) {
			printf("Your PW: %s\n", message);
			write(clnt_sock, message, strlen(message)); //Send PW

			write(clnt_sock, "Product", BUFSIZE); //Ask the menu to server

		}
		else if (cnt == 2) {
			write(clnt_sock, message, strlen(message)); //Send the flower number
		}
		else if (cnt == 3) {
			write(clnt_sock, message, strlen(message)); // send quantity
			write(clnt_sock, "detail", BUFSIZE);
			//printf("When will you pick up?(month) ex.12 \n");//send the day(month)
		}
		else if (cnt == 4) {
			//month=atoi(message);
			write(clnt_sock, message, strlen(message)); //send the day(month)

			//printf("When will you pick up?(day) ex.28 \n");//send the day(day) 
		}

		else if (cnt == 5) {
			//day=atoi(message); 
			write(clnt_sock, message, strlen(message));


		}
		cnt++;
		//printf("cnt : %d\n",cnt);
		//if(cnt>=2){

		//}
		memset(message, 0, sizeof(message));
	}
	close(clnt_sock);

}

void* ear_sv(void* arg2) /* Recieve from server */

{
	int clnt_sock;
	clnt_sock = *(int*)arg2;
	char message[BUFSIZE];//,product[BUFSIZE],price[BUFSIZE];
	char *product, *price;
	int strLen;
	struct order buf;
	int nread, end = 1;
	int show = 0;
	int i = 1; // start from '1', not '0'

	while ((strLen = read(clnt_sock, message, BUFSIZE)) != 0) {
		if (strcmp(message, "Retry") == 0) {
			printf("\nWorng password! If you enter 'y', you can retry.\n");
			printf("Or 'q', you can logout.\n\n");
		}
		else if (strcmp(message, "Product") == 0) {
			//printf("~~~server~~~\n");
			printf("\n--------------Product--------------\n");
			while (((strLen = read(clnt_sock, message, BUFSIZE)) > 0)) {
				if (strcmp(message, "Product_end") == 0) {
					printf("-----------------------------------\n");
					break;
				}

				if (i % 2 == 1)
					printf("(%d)%s: ", (i / 2) + 1, message);
				else if (i % 2 == 0) //
				{
					printf("%s won\n", message);
				}
				i++;

			}

		}
		else if (strcmp(message, "detail") == 0) {
			i = 0;
			printf("\n--------------Details--------------\n");
			while (((strLen = read(clnt_sock, &buf, sizeof(struct order))) > 0)) {

				printf("Order: %s\nQuantity: %d\nBill: %d\n\n", buf.product, buf.quantity, buf.bill);
				printf("--------------------------------------\n");
				i++;

				if (i == 1)	break;

			}
		}
		else
			printf("<Notice> %s\n", message);

		//if (strcmp(message,"Okay, BYE!!")==0)
					//break;
		memset(message, 0, sizeof(message));
	}
	close(clnt_sock);
	exit(0);

}

void menual(char **user)
{
	system("clear");
	printf("\n-----------Flower Reservation----------\n");
	printf("server port : %s \n", user[1]);
	printf("--------------- Menual ----------------\n");
	printf("1) Write your ID\n");
	printf("2) Write your PW\n");
	printf("3) Order\n");
	printf("4) Detail\n");
	printf("---------------------------------------\n");
	printf("If you write 'q',you can logout\n");
	printf("-------------------------------\n");
	printf("Write your ID : ");
}
