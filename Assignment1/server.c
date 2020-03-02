// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include<pthread.h>
#define PORT 8888
int arr_end=0;

struct client_node{
  int sock;
  int valread;
};


struct client_node client_node_arr[100];
char* buffer_server[1024];

void sendtoall(char* msg){
  for(int j=0;j<arr_end;j++){
    send(client_node_arr[j].sock , msg , strlen(msg) , 0 );
  }
}

void* client_handler(void* temp){
  while(1){
    struct client_node temp2 = *((struct client_node *)temp);
    //int a = *((int *) temp);
    //free(temp);
    //printf("%d\n",temp2.sock );
    int valread = read( temp2.sock, buffer_server, 1024);
    printf("%s\n",buffer_server);
    if(buffer_server!=NULL){
      if(buffer_server != "bye"){
        sendtoall(buffer_server);
      }
      else{
        //remove from array;
        int start = 0;
        while(start<=arr_end){
          if(client_node_arr[start].sock == temp2.sock){
            int t1 = start;
            while(t1<arr_end-1){
              client_node_arr[t1] = client_node_arr[t1+1];
            }
            break;
          }
          start++;
        }
      }
    }
  }
  free(temp);
  return NULL;
}

int main(int argc, char const *argv[]) {
  int server_fd, new_socket, valread;
	struct sockaddr_in address;
  int opt = 1;
	int addrlen = sizeof(address);
  // Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
  // Forcefully attaching socket to the port 8888
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt))){
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
  address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );
  // Forcefully attaching socket to the port 8888
  if (bind(server_fd, (struct sockaddr *)&address,sizeof(address))<0){
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  //listen
  if (listen(server_fd, 3) < 0){
    perror("listen");
    exit(EXIT_FAILURE);
  }
  while(1){
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0){
  		perror("accept");
  		exit(EXIT_FAILURE);
  	}
    //printf("connected\n");
    //printf("%d\n",new_socket);
    struct client_node* temp = (struct client_node*)malloc(sizeof(struct client_node));
    temp->sock = new_socket;

    client_node_arr[arr_end] = *temp;
    //printf("%d\n",arr_end );
    pthread_t client_listen_always;
    pthread_create(&client_listen_always,NULL,client_handler,temp);
    arr_end++;
  }

  return 0;
}
