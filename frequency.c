#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define NUM_LETTERS ((int)26)

typedef struct Node {
char letter;
long unsigned int count;
struct Node* children[NUM_LETTERS];
} Node;

Node* new_node(char c);
void split_string(char *str, char *temp_arr, int w_parameter);
int search_word(Node *node, char *word);
void insert(Node *node, char *word);
void print_all(Node *root, char hold[] ,int s, int w_parameter);
void free_all(Node *root);
int check_char(Node *node, char c);

//Creates new node (using malloc - in the end of the program we free all memory we allocated)
Node* new_node(char c){
    Node *node = (struct Node*)calloc(1, sizeof(struct Node));
    if(node == NULL){
        printf("Failed allocating memory");
        exit(1);
    }
    node->count = 0;
    node->letter = c;
    for (int i = 0; i < NUM_LETTERS; i++){
        node->children[i] = NULL;
    }
    return node;
}

//This function uses all other functions, when at the end - all words in the given string are printed by order, and the memory is freed.
void split_string(char *str, char *temp_arr, int w_parameter){
    Node *root = (struct Node*)calloc(1, sizeof(struct Node));
    if(root == NULL){
        printf("Failed allocating memory");
        exit(1);
    }
     for (int i = 0; i < NUM_LETTERS; i++){
        root->children[i] = NULL;
    }
    root->count = 0;
    char delim[] = {" \t\n\r"};
    char *p = strtok(str, delim);
    while(p != NULL){
        int ans = search_word(root, p); 
        if(!ans){
            insert(root, p);
        }
        p = strtok(NULL, delim);
    }
   print_all(root, temp_arr, 0, w_parameter);
   free_all(root);
}

//Returns 1 if a word exists in the tree, 0 otherwise
int search_word(Node *node, char *word){
    if(node){
        for(int i = 0; i < NUM_LETTERS; i++){
            char c = *(word);
            word ++;
            if(node->children[i] != NULL && node->children[i]->letter == c){
                    if (*(word) == '\0'){
                        node->children[i]->count = node->children[i]->count + 1;
                        return 1;
                    }
                    return search_word(node->children[i], word);
                }
            }
        }
    return 0;
}

//If word does not exist in the tree, this function inserts it
void insert(Node *node, char *word){
    int index;
    if(node){
        index = word[0]-'a';
        if (node->children[index]){ // Letter exists
            if (strlen(word) == 1){
                node->children[index]->count++;
                return;
            }
            memmove(word, word + 1, strlen(word));
            insert(node->children[index], word);
        }
        else{
            char ch = word[0];
            Node *new = new_node(ch);
            node->children[index] = new;
            if (strlen(word) == 1){
                node->children[index]->count++;
                return;
            }
            memmove(word, word + 1, strlen(word));
            insert(node->children[index], word);
        }
    }
}

// Prints all words (and how many times they exist) - lexicographic order:
// if w_parameter is 1 - then prints in ascending order, if 0 - prints Descending.
void print_all(Node* node, char temp[], int s, int w_parameter){
    int i = 0;
    if(node == NULL){
        return;
    }
    if (w_parameter){
        for(i = NUM_LETTERS-1 ; i >= 0 ; i--){
            temp[s] = 'a'+i;
            print_all(node->children[i], temp, s+1, w_parameter);
        }
        if (node->count > 0) {
            temp[s] = '\0';
            printf("%s %ld\n", temp, node->count);
        }
    }
    else{
        if (node->count > 0) {
            temp[s] = '\0';
            printf("%s %ld\n", temp, node->count);
        }
        for(i = 0; i < NUM_LETTERS; i++){
            temp[s] = 'a'+i;
            print_all(node->children[i], temp, s+1, w_parameter);
        }
    }
    return;
  }

//This function frees deallocates the memory previously allocated by call to malloc (in our case - when we created new node)
void free_all(Node* node){
    if(node){
        for(int i = 0; i < NUM_LETTERS; i++){
            if(node->children[i]){
                free_all(node->children[i]);
            }
        }
        free(node);
    }
}


int main(int argc, char *argv[]){
    int capacity = 256;
    char *str = NULL;
    str = (char*)calloc(capacity, sizeof(char));
     if(str == NULL){
        printf("Failed allocating memory");
        exit(1);
    }
    char ch;
    int flag = 1;
    int i = 0;
    int w_parameter = 0;

    //Checks if r exists after ./frequency
    if (argc == 2){
        if (*argv[1] == 'r')
            w_parameter = 1;
    }
    while (flag){
        if(scanf("%c", &ch) == EOF){
            flag = 0;
        }
        else{
            if(i == capacity){
                capacity *= 2;
                str = (char*)realloc(str, capacity*sizeof(char));
                if(str == NULL){
                    printf("Failed allocating memory");
                    exit(1);
                }
            }
            if (ch==' ' || ch == '\n' || ch == '\t' || isalpha(ch)){
            str[i] = tolower(ch);
            i++;
            }
        }
    }
    // char temp_arr[i];
    char *temp_arr = NULL;
    temp_arr = (char*)calloc(i, sizeof(char));
    if (!temp_arr){
        printf("Failed allocating memory");
        exit(1);
    }
    split_string(str, temp_arr, w_parameter);
    free(str);
    free(temp_arr);
    return 0;
}
