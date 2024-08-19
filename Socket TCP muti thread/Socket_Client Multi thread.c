#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <time.h>
#include <pthread.h>

#define SIZE_ARRAY 1000
#define MAX_WORD_LENGTH 256
#define MAX_HASH_LENGTH 65
int arr[SIZE_ARRAY];
pthread_mutex_t lock;  

// client socket

void *generateArry(void *arg){
  clock_t start, end;
  start = clock();
  srand(time(NULL));
  for (int i = 0; i < SIZE_ARRAY; i++) {
    arr[i] = rand() % 100;
  }
  end = clock();
  printf("\nExecution time for Generate array random: %.10f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

  pthread_exit(NULL);
}

int main()
{
  int sockfd, portno1 = 1200, portno2 = 1201; 
  struct sockaddr_in serv_addr1, serv_addr2;
  struct hostent *server;
  
  //step 1: Create a socket
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  server = gethostbyname("localhost");
  
  //step 2 build the connection soket to the server
  serv_addr1.sin_family = AF_INET;
  bcopy((char *)server->h_addr,(char *)&serv_addr1.sin_addr.s_addr,server->h_length);
  serv_addr1.sin_port = htons(portno1);
  
  //step 3 to connect with server upon the data inserted serv_addr
  if (connect(sockfd, (struct sockaddr *) &serv_addr1, sizeof(serv_addr1)) < 0) {
    perror("ERROR connecting to server 1");
    exit(1);
  }
  
  //step 4 write your senario
  clock_t start = clock();
  int number = 5;
  pthread_t threads[number];
  int thread_args[number];
  
  pthread_mutex_init(&lock, NULL);

  for (int i=0; i < number; i++){
    thread_args[i] = i + 1;
    pthread_create(&threads[i], NULL, generateArry, (void *) &thread_args[i]);
  }
    
  for (int i = 0; i < number; i++) {
    pthread_join(threads[i], NULL);
  }
  

  pthread_mutex_destroy(&lock);

  if (write(sockfd, arr, sizeof(arr)) < 0) {
    perror("ERROR writing to socket");
    exit(1);
  }
    
  close(sockfd);

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  serv_addr2.sin_family = AF_INET;
  bcopy((char *)server->h_addr,(char *)&serv_addr2.sin_addr.s_addr, server->h_length);
  serv_addr2.sin_port = htons(portno2);

  if (connect(sockfd, (struct sockaddr *) &serv_addr2, sizeof(serv_addr2)) < 0) {
    perror("ERROR connecting to server 2");
    exit(1);
  }

  for(int i=0; i<5; i++){
    char word[] = "Hasan";
    if (write(sockfd, word, strlen(word)) < 0) {
      perror("ERROR writing to socket for Server 2");
      exit(1);
    }

    char buffer[MAX_HASH_LENGTH];
    bzero(buffer, MAX_HASH_LENGTH);
    if (read(sockfd, buffer, MAX_HASH_LENGTH - 1) < 0) {
      perror("ERROR reading from socket for Server 2");
      exit(1);
    }
  }

  close(sockfd);

  clock_t end = clock();
  printf("\nExecution time client programm: %.10f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
  
  return 0;
}