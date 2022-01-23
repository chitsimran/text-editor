#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Commands:
// B - Go 1 position backward
// F - Go 1 position forward
// S - Jump to beginning
// E - Jump to end
// I - Insert character at current position
// D - Delete character at current position
// Z - Undo
// X - Exit


#define clear() printf("\033[H\033[J") 

struct Node {
    char *word;
    struct Node* next;
    struct Node* prev;
    int length;
};

struct Stack {
    int ind;
    char operation;
    char c;
    struct Node* curNode;
    struct Stack* next;
};

int currentIndex;
struct Node* currentNode;
struct Node* head;
struct Node* tail;
struct Stack* undo;

void addNode(char *word) {
    struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
    temp->word = word;
    temp->length = strlen(word);
    if (!head) {
        head = tail = temp;
    } else {
        temp->prev = tail;
        tail->next = temp;
        tail = temp;
    }
}

void readFile() {
    FILE *fp = fopen("text.txt", "r");
    char c = 'x';
    char *word = NULL;
    int ind = 0, size = 0;

    while ((c = fgetc(fp)) != EOF) {
        if (ind <= size) {
            size += 8;
            word = realloc(word, size);
        }
        if (c == ' ' || c == '\n') {
            word[ind] = c;
            word[++ind] = '\0';
            addNode(word);
            ind = size = 0;
            word = NULL;
        } else {
            word[ind++] = c;
        }
    }

    if (word) {
        word[ind] = '\0';
        addNode(word);
    }

    currentNode = tail;
    currentIndex = tail->length - 1;

    fclose(fp);
}

void moveBackward() {
    if (currentIndex) {
        currentIndex--;
    } else {
        if (currentNode == head) return;
        currentNode = currentNode->prev;
        currentIndex = currentNode->length - 1;
    }
}

void moveForward() {
    if (currentIndex == currentNode->length-1) {
        if (currentNode == tail) return;
        currentIndex = 0;
        currentNode = currentNode->next;
    } else {
        currentIndex++;
    }
}

void jumpToBeginning() {
    currentIndex = -1;
    currentNode = head;
}

void jumpToEnd() {
    currentIndex = tail->length - 1;
    currentNode = tail;
}

void insertChar(char c) {
    currentNode->word = (char *)realloc(currentNode->word, (currentNode->length)+1);
    (currentNode->length)++;
    for (int i = (currentNode->length)-1; i > currentIndex; i--) {
        currentNode->word[i] = currentNode->word[i-1];
    }
    currentNode->word[++currentIndex] = c;
    
}

char deleteChar() {
    char c = currentNode->word[currentIndex];

    for (int i = currentIndex; i < currentNode->length-1; i++) {
        currentNode->word[i] = currentNode->word[i+1];
    }
    currentNode->word = (char *)realloc(currentNode->word, currentNode->length-1);
    currentNode->word[currentNode->length-1] = '\0';
    (currentNode->length)--;
    currentIndex--;
    if (currentIndex < 0 && currentNode->length <= 1) {
        if (currentNode == head) {
            head = currentNode->next;
            head->prev = NULL;
            free(currentNode);
            currentNode = head;
            currentIndex = 0;
        } else {
            currentNode->prev->next = currentNode->next;
            if (currentNode->next) currentNode->next->prev = currentNode->prev;
            struct Node* temp = currentNode;
            currentNode = currentNode->prev;
            free(temp);
            currentIndex = currentNode->length - 1;
        }
    } else if (currentIndex < 0 && currentNode == head) {
        currentIndex = -1;
    }

    return c;
}

void print() {
    struct Node* temp = head;
    while (temp) {
        if (temp == currentNode) {
            for (int i = 0; i <= currentIndex; i++) {
                printf("%c", currentNode->word[i]);
            }
            printf("|");
            for (int i = currentIndex+1; i < currentNode->length; i++) {
                printf("%c", currentNode->word[i]);
            }
        } else {
            printf("%s", temp->word);
        }
        temp = temp->next;
    }
}

void undoOperation() {
    if (!undo) return;
    struct Stack* temp = undo;
    undo = undo->next;
    char op = temp->operation;
    if (op == 'B') {
        moveForward();
    } else if (op == 'F') {
        moveBackward();
    } else if (op == 'I') {
        deleteChar();
    } else if (op == 'D') {
        insertChar(temp->c);
    } else if (op == 'S' || op == 'E') {
        currentNode = temp->curNode;
        currentIndex = temp->ind;
    }

    free(temp);
}

void operation(char c) {
    struct Stack* temp = (struct Stack*)malloc(sizeof(struct Stack));
    temp->operation = c;
    int k = 0;
    switch(c) {
        case 'B':
            moveBackward();
            k = 1;
            break;
        case 'F':
            moveForward();
            k = 1;
            break;
        case 'S':
            temp->ind = currentIndex;
            temp->curNode = currentNode;
            jumpToBeginning();
            k = 1;
            break;
        case 'E':
            temp->ind = currentIndex;
            temp->curNode = currentNode;
            jumpToEnd();
            k = 1;
            break;
        case 'I':
            printf("Enter a character: ");
            char c;
            scanf(" %c", &c);
            insertChar(c);
            temp->ind = currentIndex;
            k = 1;
            break;
        case 'D':
            if (currentIndex == -1) return;
            temp->c = deleteChar();
            k = 1;
            break;
        case 'Z':
            undoOperation();
            return;
        default:
            free(temp);
            break;  
    }

    if (k) {
        temp->next = undo;
        undo = temp;
    }
}

int main()
{
    currentIndex = 0;
    currentNode = head = tail = NULL;
    undo = NULL;
    readFile();
    char c = 'x';
    while (c != 'X') {
        print();
        printf("\n");
        scanf("%c", &c);
        operation(c);
        clear();
    }
    return 0;
}