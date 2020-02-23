// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<pthread.h>
#include <signal.h>
#include<stdlib.h>
#define PORT 8888

char* client_buffer[1024];

int sockfd = 0;

void* read_handlerfunc(void* args){
  while (1) {
    int valread = read( sockfd ,client_buffer, 1024);
  	printf("%s\n",client_buffer);
  }
  return NULL;
}
 /*void catch_ctrl_c_and_exit(int sig){
   char* endmsg="byeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee";
   send(sockfd,endmsg,strlen(endmsg),0);
 }*/
/*void* write_handlerfunc(void* args){
  while (1) {
    printf("asdf\n");
    scanf("%s",client_buffer);
    send(sockfd , client_buffer , strlen(client_buffer) , 0 );
  }
  return NULL;
}*/

int main(int argc, char const *argv[]){

  //signal(SIGINT, catch_ctrl_c_and_exit);

	struct sockaddr_in serv_addr;
  //socket creation
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("\n Socket creation error \n");
		return -1;
	}
  //socket address and attach it to port of that address
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0){
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}
  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
		printf("\nConnection Failed \n");
		return -1;
	}
  printf("connected\n" );

  pthread_t read_handler;

  pthread_create(&read_handler,NULL,read_handlerfunc,NULL);
  while(1){
    scanf("%s",client_buffer);
    send(sockfd , client_buffer , strlen(client_buffer) , 0 );
    //printf("sent\n" );
  }

  return 0;
}
