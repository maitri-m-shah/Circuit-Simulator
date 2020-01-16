#include <stdio.h>
#include <string.h>
//Create a Linked List
typedef struct LinkedList{
        char* key; 
        int value;
        struct LinkedList* next;
}Node;

Node* root;

//Create Linked list for instructions
typedef struct LinkedListInst {
    char* op;
    char* var1;
    char* var2;
    char* output;
    struct LinkedListInst* next;
}InsNode;

InsNode* Insroot;

//create a InsNode
InsNode* createInsNode(char* op, char* var1, char* var2, char* output){
    InsNode* headins = (InsNode*)malloc(sizeof(InsNode));
    headins->op = op;
    headins->var1 = var1;
    headins->var2 = var2;
    headins->output = output;
    return headins;
}

//create a Node
Node* createNode(char* key,int value, Node* next){
    Node* head = (Node*)malloc(sizeof(Node));
    head->key = key;
    head->value = value;
    head->next = next;
    return head;
}

//Setting Method
void set(char* key, int value){
    Node* prev = root;
    if (prev == NULL) {
        root = createNode(key, value, NULL);
        return;
    }
    Node* head = NULL;
    while (prev != NULL) {
        if (strcmp(key, prev->key) == 0) {
            prev->value = value;
            return;
        }
        head = prev;
        prev = prev->next;
    }
    head->next = createNode(key, value, NULL);
}

//Getting method
int get(char* key) {
    Node* ptr = root;
    while (ptr != NULL) {
        if (strcmp(key, ptr->key) == 0) {
            return ptr->value;
        }
        ptr = ptr->next;
    }
    return -1;

}

void logic(int maxIndex, char** inKeys, char** outKeys, int nutOut) {
    int i = 0;
    for (i = 0; i < maxIndex; i++) {
        printf("%d ", get(inKeys[i]));
    }
    InsNode* ptr = Insroot;
    while (ptr != NULL) {
        if (strcmp(ptr->op, "NOT") == 0) {
            set(ptr->output, NOT(get(ptr->var1)));
        } else if (strcmp(ptr->op, "AND") == 0) {
            set(ptr->output, AND(get(ptr->var1), get(ptr->var2)));
        }
          else if (strcmp(ptr->op, "NAND" ) == 0){
            set(ptr->output, NAND(get(ptr->var1), get(ptr->var2)));
          }
          else if(strcmp(ptr->op, "OR") == 0) {
            set(ptr->output, OR(get(ptr->var1), get(ptr -> var2)));
          }
          else if(strcmp(ptr -> op, "NOR") == 0){
            set(ptr->output, NOR(get(ptr->var1), get(ptr->var2)));
          }
          else if(strcmp(ptr -> op, "XOR") == 0){
            set(ptr ->output, XOR( get(ptr->var1), get(ptr->var2)));
          }

        ptr = ptr->next;
    }
    for (i = 0; i < nutOut; i++) {
        printf("%d ", get(outKeys[i]));
    }
    printf("\n");
}

void execute(int index, int maxIndex, char** inKeys, char** outKeys, int nutOut) {
    if (index < maxIndex) {
        set(inKeys[index], 0);
        execute(index + 1, maxIndex, inKeys, outKeys, nutOut);
        set(inKeys[index], 1);
        execute(index + 1, maxIndex, inKeys, outKeys, nutOut);
    } else {
        logic(maxIndex, inKeys, outKeys, nutOut);
    }
}

int NOT(int num1){
    if(num1 == 1) {
        return 0;
    }
    return 1;

}

int AND(int num1, int num2){
    if( (num1 == 1) && (num2==1) ){
        return 1;
    }
     return 0;
}

int NAND (int num1, int num2){
    if( (num1 == 1) && (num2 == 1)){
        return 0;
    }
    return 1;
}

int OR (int num1, int num2){
    if( (num1 == 1) || (num2 == 1) ){
        return 1;
    }
    return 0;
}

int NOR (int num1, int num2){
    if( (num1 == 1) || (num2 == 1) ){
        return 0;
    }
    return 1;
}

int XOR (int num1, int num2){
    return num1 ^ num2;
}

int main(int argc, char** argv) {
    FILE* fle;
    fle = fopen(argv[1], "r");
    if (fle == NULL) {
        printf("INPUT FILE BOI\n");
        return 0;
    }
    int NUM_INPUTS;
    int NUM_OUTPUTS;
    fscanf(fle, "INPUTVAR %d", &NUM_INPUTS);
    char** keys = malloc(sizeof(char*) * NUM_INPUTS);
    int i = 0;
    for (i = 0; i < NUM_INPUTS; i++) {
        char* key = malloc(sizeof(char) * 256);
        fscanf(fle, "%s", key);
        keys[i] = key;
    }
    fscanf(fle, "\nOUTPUTVAR %d", &NUM_OUTPUTS);
    //if there are invalid number of outputs, return 0
    if (NUM_OUTPUTS == 0) {
        return 0;
    }
    // Read the output instructions
    char** outputKeys = malloc(sizeof(char*) * NUM_OUTPUTS);
    for (i = 0; i < NUM_OUTPUTS; i++) {
        char* value = malloc(sizeof(char) * 256);
        fscanf(fle, "%s", value);
        outputKeys[i] = value;
    }
    fscanf(fle, "\n");
    Insroot = createInsNode("DELME", "A", "B", "O");
    InsNode* ptr = Insroot;
    while (!feof(fle)) {
        char* OP = malloc(sizeof(char) * 256);
        fscanf(fle, "%s", OP);
        if (strcmp(OP, "NOT") == 0) {
            char* var1 = malloc(sizeof(char) * 256);
            char* output = malloc(sizeof(char) * 256);
            fscanf(fle, "%s %s\n", var1, output);
            ptr->next = createInsNode(OP, var1, NULL, output);
        } else {
            char* var1 = malloc(sizeof(char) * 256);
            char* var2 = malloc(sizeof(char) * 256);
            char* output = malloc(sizeof(char) * 256);
            fscanf(fle, "%s %s %s\n", var1, var2, output);
            ptr->next = createInsNode(OP, var1, var2, output);
        }
        ptr = ptr->next;
    }
    Insroot = Insroot->next;
    execute(0, NUM_INPUTS, keys, outputKeys, NUM_OUTPUTS);
    return 0;
}
