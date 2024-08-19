#include <stdio.h>
#include <string.h>

int main() {
    char word1[100];
    char word2[100];
    
    printf("Enter one word: ");
    scanf("%s", word1);

    printf("Enter two word: ");
    scanf("%s", word2);

    if (strcmp(word1, word2) == 0) {
        printf("The words are the same.\n");
    } else {
        printf("The words are different.\n");
    }
    return 0;
}
