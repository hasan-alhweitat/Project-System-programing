#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <openssl/sha.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>

//server 2 socket

#define MAX_WORD_LENGTH 256
#define MAX_HASH_LENGTH 65
#define DICT_SIZE 1000

struct HashEntry{
    char word[MAX_WORD_LENGTH];
    char hash[MAX_HASH_LENGTH];
};

struct HashEntry hash_dict[DICT_SIZE];
int dict_size = 0;

pthread_mutex_t lock;

void loadHash() {
    FILE *file = fopen("hashResultSocket.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }
    while (fscanf(file, "%s %s", hash_dict[dict_size].word, hash_dict[dict_size].hash) == 2) {
        dict_size++;
    }
    fclose(file);
}

void saveHash(const char *word, const char *hash) {
    FILE *file = fopen("hashResultSocket.txt", "a");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }
    fprintf(file, "%s %s\n", word, hash);
    printf("Hashing completed and stored in hashResultSocket.txt\n");
    fclose(file);
}

void hashFunction(const char *word, char *output) {
    for (int i = 0; i < dict_size; ++i) {
        if (strcmp(hash_dict[i].word, word) == 0) {
            strcpy(output, hash_dict[i].hash);
            printf("The hash already hashed befor and stored in file hashResultSocket.txt");
            pthread_mutex_unlock(&lock);
            return;
        }
    }

    unsigned char sha256_digest[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *)word, strlen(word), sha256_digest);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        sprintf(output + (i * 2), "%02x", sha256_digest[i]);
    }
    output[SHA256_DIGEST_LENGTH * 2] = '\0';

    strcpy(hash_dict[dict_size].word, word);
    strcpy(hash_dict[dict_size].hash, output);
    saveHash(word, output);
    ++dict_size;

    pthread_mutex_unlock(&lock);
}

int num = 1;

int main() {
    int sockfd, newsockfd, portno = 1201, clilen;
    char buffer[MAX_WORD_LENGTH];
    struct sockaddr_in serv_addr, cli_addr;

    loadHash();

    // Step 1: initialize the server socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // the connection upon tcp/ip
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // Step 2: assign the server socket with the file descriptor
    bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    listen(sockfd, 5);
    printf("Server 2 is waiting......\n");

    // Step 3: waiting for connection
    while (1) {
        
        // Step 4: create the connection
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

        // Step 5: write what your server actually does
        printf("Server 2 accept the connection\n");

        bzero(buffer, MAX_WORD_LENGTH);
        read(newsockfd, buffer, MAX_WORD_LENGTH - 1);
        clock_t start = clock();
        for(int j = 0; j< 5; j++){
            clock_t start = clock();
            printf("Client %d\n", num);
            num = num + 1;
            char output_hash[MAX_HASH_LENGTH];
            hashFunction(buffer, output_hash);
            write(newsockfd, output_hash, strlen(output_hash));
            close(newsockfd);
            clock_t end = clock();
            printf("\nExecution time for hash function: %.10f seconds\n\n", (double)(end - start) / CLOCKS_PER_SEC);
            
        }
        clock_t end = clock();
        printf("\nExecution time server program 2: %.10f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
        close(newsockfd);
    }
    close(sockfd);
    return 0;
}
