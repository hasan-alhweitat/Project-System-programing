#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <sys/wait.h>
#include <fcntl.h>

#define SIZE_ARRAY 1000

int arr[SIZE_ARRAY], key = 55;
pthread_mutex_t lock; 

//server 1 socket

void linearSearch(int arr[], int size, int key) {
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (arr[i] == key) {
            count++;
        }
    }
    printf("Element frequency in linearSearch: %d\n", count);
}

void binarySearch(int arr[], int size, int key) {
    int low = 0, high = size - 1, mid, count = 0;
    while (low <= high) {
        mid = (low + high) / 2;
        if (arr[mid] == key) {
            count++;
            int left = mid - 1;
            while (left >= 0 && arr[left] == key) {
                count++;
                left--;
            }
            int right = mid + 1;
            while (right < size && arr[right] == key) {
                count++;
                right++;
            }
            break;
        } else if (arr[mid] < key) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    printf("Element frequency in Binary Search: %d\n", count);
}

void interpolationSearch(int arr[], int size, int key) {
    int low = 0, high = size - 1, count = 0;
    while (low <= high && key >= arr[low] && key <= arr[high]) {
        int pos = low + ((double)(high - low) / (arr[high] - arr[low])) * (key - arr[low]);
        if (arr[pos] == key) {
            count++;
            int left = pos - 1;
            while (left >= 0 && arr[left] == key) {
                count++;
                left--;
            }
            int right = pos + 1;
            while (right < size && arr[right] == key) {
                count++;
                right++;
            }
            break;
        } else if (arr[pos] < key) {
            low = pos + 1;
        } else {
            high = pos - 1;
        }
    }
    printf("Element frequency in interpolation Search: %d\n", count);
}

void exponentialSearch(int arr[], int size, int key) {
    int count = 0;
    if (arr[0] == key) {
        count++;
    }
    int i = 1;
    while (i < size && arr[i] <= key) {
        if (arr[i] == key) {
            count++;
        }
        i = i * 2;
    }
    int low = i / 2;
    int high = (i < size) ? i : size - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (arr[mid] == key) {
            count++;
            int left = mid - 1;
            while (left >= 0 && arr[left] == key) {
                count++;
                left--;
            }
            int right = mid + 1;
            while (right < size && arr[right] == key) {
                count++;
                right++;
            }
            break;
        } else if (arr[mid] < key) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    printf("Element frequency in exponentialSearch: %d\n", count);
}

void bubbleSort(int arr[], int size, const char *fileBubbleSort) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    
    int fb = open(fileBubbleSort, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fb == -1) {
        perror("Unable to open file");
        exit(1);
    }
    for (int i = 0; i < SIZE_ARRAY; i++) {
        char buffer[SIZE_ARRAY]; 
        int length = snprintf(buffer, sizeof(buffer), "%d ", arr[i]);
        write(fb, buffer, length);
    }
    close(fb);
}

void selectionSort(int arr[], int size, const char *fileSelectionSort) {
    for (int i = 0; i < size - 1; i++) {
        int min_index = i;
        for (int j = i + 1; j < size; j++) {
            if (arr[j] < arr[min_index]) {
                min_index = j;
            }
        }
        int temp = arr[i];
        arr[i] = arr[min_index];
        arr[min_index] = temp;
    }

    int fb = open(fileSelectionSort, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fb == -1) {
        perror("Unable to open file");
        exit(1);
    }
    for (int i = 0; i < SIZE_ARRAY; i++) {
        char buffer[SIZE_ARRAY]; 
        int length = snprintf(buffer, sizeof(buffer), "%d ", arr[i]);
        write(fb, buffer, length);
    }
    close(fb);
}

void insertionSort(int arr[], int size, const char *fileInsertionSort) {
    int i, key, j;
    for (i = 1; i < size; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
    int fb = open(fileInsertionSort, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fb == -1) {
        perror("Unable to open file");
        exit(1);
    }
    for (int i = 0; i < SIZE_ARRAY; i++) {
        char buffer[SIZE_ARRAY]; 
        int length = snprintf(buffer, sizeof(buffer), "%d ", arr[i]);
        write(fb, buffer, length);
    }
    close(fb);
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    return (i + 1);
}

void quickSort(int arr[], int low, int high, const char *fileQuickSort) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1, fileQuickSort);
        quickSort(arr, pi + 1, high, fileQuickSort);
    }
    if (low == 0 && high == SIZE_ARRAY - 1) {
        int fb = open(fileQuickSort, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fb == -1) {
            perror("Unable to open file");
            exit(1);
        }
        for (int i = 0; i < SIZE_ARRAY; i++) {
            char buffer[SIZE_ARRAY]; 
            int length = snprintf(buffer, sizeof(buffer), "%d ", arr[i]);
            write(fb, buffer, length);
        }
        close(fb);
    }
}

int num = 1;
void *multi_Threading(void *arg){
    int number = *((int *)arg);
    clock_t start, end;
    switch(number){
        case 1:
            printf("Client %d", num);
            num = num + 1;
            start = clock();
            bubbleSort(arr, SIZE_ARRAY, "bubble_sort.txt");
            printf("\nSorted array using Bubble Sort and stored in bubble_sort.txt\n");
            end = clock();
            printf("Execution time for Bubble Sort: %.10f seconds\n\n", (double)(end - start) / CLOCKS_PER_SEC);
            break;
        case 2:
            start = clock(); 
            selectionSort(arr, SIZE_ARRAY, "selection_sort.txt");
            printf("\nSorted array using Selection Sort and stored in selection_sort.txt\n");
            end = clock();
            printf("Execution time for Selection Sort: %.10f seconds\n\n", (double)(end - start) / CLOCKS_PER_SEC);
            break;
        case 3:
            start = clock(); 
            insertionSort(arr, SIZE_ARRAY, "insertion_sort.txt");
            printf("\nSorted array using Insertion Sort and stored in insertion_sort.txt\n");
            end = clock();
            printf("Execution time for Insertion Sort: %.10f seconds\n\n", (double)(end - start) / CLOCKS_PER_SEC); 
            break;
        case 4:
            start = clock(); 
            quickSort(arr, 0, SIZE_ARRAY - 1, "quick_sort.txt");
            printf("\nSorted array using Quick Sort and stored in quick_sort.txt\n");
            end = clock();
            printf("Execution time for Quick Sort: %.10f seconds\n\n", (double)(end - start) / CLOCKS_PER_SEC);
            break;
        case 5:
            start = clock(); 
            printf("\nSearching for key %d using Linear Search...\n", key);
            linearSearch(arr, SIZE_ARRAY, key);
            end = clock();
            printf("Execution time for Linear Search: %.10f seconds\n\n", (double)(end - start) / CLOCKS_PER_SEC);
            pthread_exit(NULL);
            break;
        case 6:
            start = clock(); 
            printf("\nSearching for key %d using Binary Search...\n", key);
            binarySearch(arr, SIZE_ARRAY, key);
            end = clock();
            printf("Execution time for Binary Search: %.10f seconds\n\n", (double)(end - start) / CLOCKS_PER_SEC);
            break;
        case 7:
            start = clock(); 
            printf("Searching for key %d using Interpolation Search...\n", key);
            interpolationSearch(arr, SIZE_ARRAY, key);
            end = clock();
            printf("Execution time for Interpolation Search: %.10f seconds\n\n", (double)(end - start) / CLOCKS_PER_SEC);
            break;
        case 8:
            start = clock(); 
            printf("Searching for key %d using Exponential Search...\n", key);
            exponentialSearch(arr, SIZE_ARRAY, key);
            end = clock();
            printf("Execution time for Exponential Search: %.10f seconds\n\n", (double)(end - start) / CLOCKS_PER_SEC);
            break;
        default: 
            printf("Invalid choice! Please try again.\n");
    }
    pthread_exit(NULL);
}

int main()
{
   int sockfd, newsockfd, portno = 1200, clilen;
   char buffer[256];
   struct sockaddr_in serv_addr, cli_addr;
   
   //step 1: initilize the server socket
   sockfd = socket(AF_INET, SOCK_STREAM, 0);// the commection upon tcp/ip
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_port = htons(portno);
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   
   //step 2: assign tghe server soket with the file descriptor
   bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
   
   listen(sockfd,5);
   printf("Server 1 is waiting......\n");

   //step 3: waiting for  connection
   while(1)
   {

    //step 4:create the connection
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    
    //step 5:write what you server actually do
    printf("Server 1 accpet the connection\n");

    ssize_t bytes_received = read(newsockfd, arr, sizeof(arr));
    
    clock_t start = clock();
    int number = 8;
    pthread_t threads[number];
    int thread_args[number];

    pthread_mutex_init(&lock, NULL);

    for(int j = 0; j< 5; j++){
        for (int i=0; i < 8; i++){
            thread_args[i] = i + 1;
            pthread_create(&threads[i], NULL, multi_Threading, (void *) &thread_args[i]);
        }
        for (int i = 0; i < 8; i++) {
            pthread_join(threads[i], NULL);
        }
    }   

    pthread_mutex_destroy(&lock);
    
    clock_t end = clock();
    printf("\nExecution time server 1 programm: %.10f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
    close(newsockfd);
   }
   close(sockfd);
return 0;
}