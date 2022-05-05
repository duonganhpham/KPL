#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ctype.h> 
#include <stdbool.h>


typedef struct DATA{
    char *word;
    int number;
    char *index;
} data;

data words[10000];
char stopWords[10000][100];
int wordNumber = 0, stopWordNumber = 0;

// Hàm check stop word 
bool checkStopWord(char *str) {
    for (int i=0; i<stopWordNumber; i++) {
        if (strcmp(str, stopWords[i]) == 0) {
            return false;
        }
    }
    return true;
}

// Hàm check xem có phải là tên riêng không 
bool checkWord(char *str, bool beforeIsDot){
    if (beforeIsDot == true) {
        return true;
    } else if ('A' <= str[0] && str[0] <= 'Z') {
        return false;
    }
}

// chuyển int sang str
char str[10];
char *intToString(int a) {

    sprintf(str, "%d ", a); 
    return str;
}

// Hàm chuyển lowercase
void toLowerString(char *str) {
    for(int i = 0; str[i]; i++){
        str[i] = tolower(str[i]);
    }
}

//Hàm thêm từ cũng như chỉ số dòng vào mảng 
void addWord(char *str, int currentLine) {
    for (int i=0; i<wordNumber; i++) 
    {
        if (strcmp(str, words[i].word) == 0) {
            words[i].number++;
            char *temp = (char *) malloc( ((int) strlen(words[i].index)  +  strlen(str)) * sizeof(char) );
            strcat(temp, words[i].index);
            free(words[i].index);
            words[i].index = temp;
            strcat(words[i].index, intToString(currentLine));

            // printf("--%s %d %s\n", words[i].word, words[i].number, words[i].index);

            return;
        }
    }

    words[wordNumber].word = (char *) malloc( ((int) strlen(str)) * sizeof(char) );
    strcpy(words[wordNumber].word, str);

    words[wordNumber].number = 1;

    char *temp = (char *) malloc( ((int) strlen(intToString(currentLine))) * sizeof(char) );
    free(words[wordNumber].index);
    words[wordNumber].index = temp;
    strcat(words[wordNumber].index, intToString(currentLine));

    // printf("--%s %d %s\n", words[wordNumber].word, words[wordNumber].number, words[wordNumber].index);

    wordNumber++;
}

// Hàm sắp xếp theo thứ tự ABC
void sortABC(data *word , int number){
   for (int i=0 ;i<number; i++){
       for(int j=i+1;j<number;j++){
           if(strcmp(word[i].word,word[j].word)>0){
               data temp = word[i];
               word[i]=word[j];
               word[j]=temp;
           }
       }
   }

}

// Hàm đọc file stopword 
void readStopWord() 
{
    FILE *f2;
    f2 = fopen("stopw.txt","r");
    if (f2 == NULL) {
        printf("Khong mo duoc file stop_word.txt\n");
        exit(1);
    } else {
        printf("Mo thanh cong file stopw.txt\n");
    }

    char c[100];

    do{
        fscanf(f2, "%s", c);
        if (strlen(c) > 0) {
            strcpy(stopWords[stopWordNumber++], c);
        }
    } while(feof(f2)==0);
}

int main(){

    FILE *f1;
    f1 = fopen("vanban.txt","r");


    if(f1 == NULL) {
        printf("Khong mo duoc file vanban.txt");
        exit(1);
    } else{
       printf("Mo thanh cong file vanban.txt \n");
    }

    readStopWord();

    char str[50] = "";
    char c;
    bool beforeIsDot = true;
    int currentLine = 1;
    do{
        fscanf(f1, "%c", &c);
        if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')) {
            str[strlen(str) + 1] = '\0';
            str[strlen(str)] = c;
        } else 
        {
            if (strlen(str) > 0) {
                if (checkWord(str, beforeIsDot) ) {
                    toLowerString(str);
                    if (checkStopWord(str)) {
                        addWord(str, currentLine);
                    }
                } 
                //printf("%s\n", str);
            }
            
            if (c == '.' || c == '?' || c == '!') {
                beforeIsDot = true;
            } else if (c != ' ' || strlen(str) > 0) {
                beforeIsDot = false;
            }

            if (c == '\n' || c == '\r') {
                currentLine++;
            }

            str[0] = '\0';
        }

    } while(feof(f1)==0);
    sortABC(words,wordNumber);
    for (int i=0; i<wordNumber; i++) {
        printf("%s  %s\n", words[i].word, words[i].index);
    }

    fclose(f1);

}