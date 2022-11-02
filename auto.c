#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 256

typedef struct entry
{
	char *word;
	int freq;
} Entry;

typedef struct arrayList 
{
	int size;
	Entry *data;
} List;

List *initializeList(int lineCount) {
	List *myList=malloc(sizeof(List));
	myList->size = 0;
	myList->data = malloc(sizeof(Entry)*(lineCount+5));
	return myList;
}

void *bubbleSort_word(List *myList) {
    Entry *temp = malloc(sizeof(Entry));
    for (int i=0; i<myList->size;i++) {
        for (int j=0; j<myList->size-1-i;j++) {
            if (strcmp(myList->data[j].word, myList->data[j+1].word) > 0) {
                *temp = myList->data[j];
                myList->data[j] = myList->data[j+1];
                myList->data[j+1] = *temp;
            }
        }
    }
}

void *bubbleSort_freq(List *myList, int start, int stop) {
    Entry *temp = malloc(sizeof(Entry));
    for (int i=start; i<stop;i++) {
        for (int j=start; j<stop-i+start;j++) {
            if (myList->data[j].freq > myList->data[j+1].freq) {
                *temp = myList->data[j];
                myList->data[j] = myList->data[j+1];
                myList->data[j+1] = *temp;
            }
        }
    }
}

int search_start(List *myList, char *queryword, int start, int stop) {

    if (start <= stop) {
        int mid = start + (stop - start)/2;
        if (strncmp(queryword, myList->data[mid].word, strlen(queryword)) == 0) {
            if (mid-1 < 0 || strncmp(myList->data[mid-1].word, queryword, strlen(queryword)) < 0) {
                return mid;
            }
            else
            {
                return search_start(myList, queryword, start, mid-1);
            }
        }

        else if (strncmp(queryword, myList->data[mid].word, strlen(queryword)) < 0) {
            return search_start(myList, queryword, start, mid-1);
        }

        else {
            return search_start(myList, queryword, mid+1, stop);
        }
    }

    return -1;
}

int search_stop(List *myList, char *queryword, int start, int stop) {
    
    if (start <= stop) {
        int mid = start + (stop - start)/2;
        if (strncmp(queryword, myList->data[mid].word, strlen(queryword)) == 0) {
            if (mid+1 >= myList->size || strncmp(myList->data[mid+1].word, queryword, strlen(queryword)) > 0) {
                return mid;
            }
            else
            {
                return search_stop(myList, queryword, mid+1, stop);
            }
        }

        else if (strncmp(queryword, myList->data[mid].word, strlen(queryword)) < 0) {
            return search_stop(myList, queryword, start, mid-1);
        }

        else {
            return search_stop(myList, queryword, mid+1, stop);
        }
    }
    return -1;
}

void deleteList(List *myList) {
	for (int i =0; i<myList->size;i++){
		free(myList->data[i].word);
	}
	free(myList->data);
	free(myList);
}

void insertToTail(List *myList, char *word, int freq){
	Entry *new = malloc(sizeof(Entry));
	new->word = malloc(sizeof(char)*(strlen(word)+1));
	strcpy(new->word,word);
	new->freq = freq;

	myList->data[myList->size] = *new;
	myList->size++;
}

void auto_output(List*myList, int start, int stop) {
    if (start == -1 || stop == -1) {
        printf("No suggestion!\n");
    }
    else if (stop-start < 10) {
        for (int i=stop;i>=start;i--) {
          printf("%s %d\n", myList->data[i].word, myList->data[i].freq);
        }
    }
    else 
    {
        for (int i=stop;i>=stop-9;i--) {
          printf("%s %d\n", myList->data[i].word, myList->data[i].freq);
        }
    }
}

int main(int argc, char **argv) {
    char *inputFilePath = argv[1]; //this keeps the path to input file
    char *queryWord = argv[2]; // this keeps the query word
    int lineCount=0; 
    
    //read input file
    FILE *fp = fopen(inputFilePath, "r");
    char *line = NULL; 
    size_t lineBuffSize = 0; 
    ssize_t lineSize; 
    

    if(fp == NULL){
        fprintf(stderr, "Error opening file\n");
        return -1;
    }

    
    while((lineSize = getline(&line,&lineBuffSize,fp)) !=-1)
    {
        lineCount++;
    }
 
    List *myList = initializeList(lineCount);
    

    fseek(fp, 0, SEEK_SET);
    char word[BUFSIZE]; 
    int weight;
    for(int i = 0; i < lineCount; i++)
    {
        fscanf(fp, "%s %d\n",word,&weight);
        insertToTail(myList, word, weight);
    }

    fclose(fp);
    
    bubbleSort_word(myList);
    int start, stop = -1;
    if (strcmp(queryWord, myList->data[0].word) >= 0 || strcmp(queryWord, myList->data[myList->size-1].word) <= 0) {
        start = search_start(myList, queryWord, 0, myList->size-1);
        stop = search_stop(myList, queryWord, 0, myList->size-1);
    }
    bubbleSort_freq(myList, start, stop);
   
    auto_output(myList, start, stop);
    deleteList(myList);
    
    return 0;
}
