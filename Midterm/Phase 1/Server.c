#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/time.h>

int clientCount = 0;
int groupCount = 0;

#define BUFSIZE 48000
#define RAND_MAX 1000




static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

struct client{
	int index;
	char username[1024];
	int sockID;
	struct sockaddr_in clientAddr;
	int len;

};

struct group{

	int index;
	int no_people;
	char username_g[1024];
	int people[1024];

};


struct client Client[1024];
struct group Group[1024];


pthread_t thread[1024];


int universalBuff[BUFSIZE];
void * reception(void * attrClient){

//	SETTING CLIENT ATTRIBUTES
	struct client* attrClient1 = (struct client*) attrClient;
	int index = attrClient1 -> index;
	int clientSocket = attrClient1 -> sockID;
	char username[1024];
		
//	RECEIVE FIRST MESSAGE FROM CLIENT WHICH IS HIS USERNAME
	recv(clientSocket,username,1024,0);
	strcpy(Client[index].username,username);
	printf("USER %s JOINED AND ID : %d IS ALLOTED.\n",username,index + 1);



	int mine=0;
	int data[BUFSIZE];
	while(1)
	{
		if(memcmp(data,universalBuff,sizeof(data))){
			memcpy(data,universalBuff,sizeof(data));
			printf("BUFFER ALTERED.\n");
			 if(send(clientSocket,data,BUFSIZE,NULL)==-1){printf("ERROR SENDING NEW ONE\n");}
			 else{printf("SENT NEW ONE");}
			}

		int read = recv(clientSocket,universalBuff,BUFSIZE,0);
		if(read==-1){printf("ERROR RECEIVEING ");}
		printf(" Mine is %d port\n",htons(clientSocket));
/*
		for(int i=0;i<clientCount;i++)
		{
			printf("sending to %d port\n",htons(Client[i].clientAddr.sin_port));

			//send(Client[i].sockID,data,BUFSIZE,NULL);
			if(htons(Client[i].clientAddr.sin_port )!= htons(clientSocket)){
				if(send(Client[i].sockID,data,BUFSIZE,NULL)){printf("ERROR SENDING TO PORT %d\n",htons(Client[i].clientAddr.sin_port));}
				else{printf("SENT TO PORT %d\n",htons(Client[i].clientAddr.sin_port));}
				
				}
		}
*/
	}
	return NULL;



}



	

int main(int argc,char *argv[]){

	int port;
	port = atoi(argv[1]);
	int serverSocket = socket(PF_INET, SOCK_STREAM, 0);

//	SERVER ATTRIBUTES
	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = htons(INADDR_ANY);
//	BIND TO PORT
	if(bind(serverSocket,(struct sockaddr *) &serverAddr , sizeof(serverAddr)) == -1) return 0;

//	LISTEN TO INCOMING MESSAGES
	if(listen(serverSocket,1024) == -1) return 0;

	printf("INITIALIZED AT PORT %d\n",port);

//	IF ANY CONNECTION COMES IT CREATES A NEW THREAD FOR IT WITH FUNCTION "reception" ASSIGNED

	while(1){
		Client[clientCount].sockID = accept(serverSocket, (struct sockaddr*) &Client[clientCount].clientAddr, &Client[clientCount].len);
		Client[clientCount].index = clientCount;
		pthread_create(&thread[clientCount+groupCount], NULL, reception, (void *) &Client[clientCount]);
		clientCount ++;
	}

	for(int i = 0 ; i < clientCount ; i ++)
		pthread_join(thread[i],NULL);

}
