#include "head.h"

//lists
Node* delete(Node *head, char v[]) {
    if (head==NULL) return NULL;
    Node *headcopy = head;

    if (strcmp(headcopy->val.teamName, v)==0) {
        head=head->next;
        //free(headcopy);
        return head;
    }
    Node *prev=head;
    headcopy=head->next;
    while (headcopy != NULL) {
        if (strcmp(headcopy->val.teamName, v)==0) {
            prev->next = headcopy->next;
            //free(headcopy);
            break;
        }
        prev=headcopy;
        headcopy=headcopy->next;
    }
    return head;
}

List* copyStackToLinkedList(Stack* stack) {
    List* newList = NULL;
    List* current = NULL;
    Stack* stackPtr = stack;

    while (stackPtr != NULL) {
        // Crează un nou nod pentru lista
        List* newNode = (List*)malloc(sizeof(List));
        newNode->teamName = (char*)malloc(strlen(stackPtr->data->val.teamName) + 1);
        strcpy(newNode->teamName, stackPtr->data->val.teamName);
        newNode->point = stackPtr->point;
        newNode->next = NULL;

        if (newList == NULL) {
            newList = newNode;
            current = newNode;
        } else {
            current->next = newNode;
            current = current->next;
        }

        stackPtr = stackPtr->next;
    }

    return newList;
}

List* copyNodeFromStackToList(Stack* stackNode, List* listNode) {
    /*if (stackNode == NULL || listNode == NULL)
        return NULL; */

    if (stackNode->data->val.teamName != NULL) {
        int teamNameLength = strlen(stackNode->data->val.teamName) + 1;
        listNode->teamName = (char*)malloc(teamNameLength * sizeof(char));
        strcpy(listNode->teamName, stackNode->data->val.teamName);
    } else {
        listNode->teamName = NULL;
    }

    listNode->point = stackNode->point;
    return listNode;
}

void freeList(Node* head) {
    Node* curr=head;
    while (curr!=NULL) {
        Node* next=curr->next;
        free(curr->val.teamName);
        for (int i=0; i<curr->val.nPlayers; i++) {
            free(curr->val.p[i].firstName);
            free(curr->val.p[i].secondName);
        }
        free(curr->val.p);
        free(curr);
        curr=next;
    }
}

void swapNodes(List** headRef, List* node1, List* node2) {
    if (node1 == node2)
        return;

    List* prevNode1 = NULL;
    List* prevNode2 = NULL;
    List* currentNode = *headRef;

    while (currentNode != NULL) {
        if (currentNode->next == node1)
            prevNode1 = currentNode;
        else if (currentNode->next == node2)
            prevNode2 = currentNode;

        currentNode = currentNode->next;
    }

    if (prevNode1 != NULL)
        prevNode1->next = node2;
    else
        *headRef = node2;

    if (prevNode2 != NULL)
        prevNode2->next = node1;
    else
        *headRef = node1;

    List* temp = node1->next;
    node1->next = node2->next;
    node2->next = temp;
}

//stacks
Stack* initializeStack() {
    Stack* stack = NULL;
    return stack;
}
void emptyStack(Stack** stack) {
    while (*stack != NULL) {
        Stack* temp = *stack;
        *stack = (*stack)->next;
        free(temp);
    }
}

void push(Stack** s,Node* node, float points) {
    Stack* new_node = (Stack*)malloc(sizeof(Stack));
    new_node->data = node;
    new_node->point = points;
    new_node->next = *s;
    *s = new_node;
}

void addStackToStack(Stack* stack1, Stack** stack2) {
    if (stack1 == NULL || *stack2 == NULL) {
        return;
    }

    Stack* curr = stack1;

    while (curr != NULL) {
        push(stack2, curr->data, curr->point);
        curr = curr->next;
    }
}


void mergeStacks(const Stack* stack1, Stack** stack2) {
    if (*stack2==NULL) {
        const Stack* curr = stack1;
        while (curr != NULL) {
        push(stack2, curr->data, curr->point);
        curr = curr->next;
        }
    }
    else {
    const Stack* curr1 = stack1;

    while (curr1 != NULL) {
        // Verificăm dacă nodul curent din stack1 are același teamName cu un nod din stack2
        int teamNameExists = 0;
        Stack* temp = *stack2;
        while (temp != NULL) {
            if (temp->data != NULL && curr1->data != NULL && strcmp(temp->data->val.teamName, curr1->data->val.teamName) == 0) {
                teamNameExists = 1;
                temp->point = curr1->point;  // Actualizăm valoarea point
                break;
            }
            temp = temp->next;
        }

        if (!teamNameExists) {
            push(stack2, curr1->data, curr1->point);
        }

        curr1 = curr1->next;
    }
    }
}

void reverse_stack(Stack** s) {
    Stack* new_stack = NULL;
    while (*s != NULL) {
        Stack* next_node = (*s)->next;
        (*s)->next = new_stack;
        new_stack = *s;
        *s = next_node;
    }
    *s = new_stack;
}

int countElements(Stack* stack) {
    int count=0;
    Stack* current=stack;
    while (current!=NULL) {
        count++;
        current=current->next;
    }
    return count;
}

//queues
Queue *createQueue(){
    Queue *q;
    q=(Queue*)malloc(sizeof(Queue));
    if (q==NULL) 
        return NULL ;
    q->front=q->rear=NULL ;
    return q;
}

void enqueue(Queue* q, Node* head) {
    while (head != NULL) {
        Node* node = (Node*) malloc(sizeof(Node));
        node->val.nPlayers = head->val.nPlayers;
        node->val.teamName = (char*)malloc(30 * sizeof(char));
        strcpy(node->val.teamName, head->val.teamName);
        node->val.p = (Player*) malloc(node->val.nPlayers * sizeof(Player));
        for(int i = 0; i < node->val.nPlayers; i++) {
            node->val.p[i].firstName = (char*) malloc(30 * sizeof(char));
            node->val.p[i].secondName = (char*) malloc(30 * sizeof(char));
            strcpy(node->val.p[i].firstName, head->val.p[i].firstName);
            strcpy(node->val.p[i].secondName, head->val.p[i].secondName);
            node->val.p[i].points = head->val.p[i].points;
        }
        node->next = NULL;

        if (q->rear == NULL) {
            q->rear = node;
            q->front = node;
        } else {
            q->rear->next = node;
            q->rear = node;
        }
        head = head->next;
    }
}

int isEmpty (Queue *q) {
    return (q->front==NULL);
}

void emptyQueue(Queue* queue) {
    Node* current = queue->front;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    queue->front = NULL;
    queue->rear = NULL;
}

void copyinqfroms(Queue* q, Stack* stack) {
    if (stack == NULL || stack->data == NULL) {
        printf("Stiva este vida.\n");
        return;
    }

    Stack* current = stack;
    while (current != NULL) {
        Node* newNode = (Node*)malloc(sizeof(Node));
        newNode->val = current->data->val;
        newNode->next = NULL;

        if (q->rear == NULL) {
            q->front = q->rear = newNode;
        }
        else {
            q->rear->next = newNode;
            q->rear = newNode;
        }

        current = current->next;
    }
}


//BTS
Tree* createNodeTree(char* teamName, float point) {
    Tree* newNode = (Tree*)malloc(sizeof(Tree));
    newNode->teamName = strdup(teamName);
    newNode->point = point;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}


//AVL
int nodeHeight(NodeAVL* root) {
    if(root == NULL) return -1;
    else return root->height;
}

NodeAVL* RightRotation(NodeAVL* z) {
    NodeAVL* y=z->left;
    NodeAVL* T3=y->right;
    y->right=z;
    z->left=T3;
    z->height=max(nodeHeight(z->left),nodeHeight(z->right))+1;
    y->height=max(nodeHeight(y->left),nodeHeight(y->right))+1;
    return y;
}

NodeAVL* LeftRotation(NodeAVL* z) {
    NodeAVL* y=z->right;
    NodeAVL* T2=y->left;
    y->left=z;
    z->right=T2;
    z->height=max(nodeHeight(z->left),nodeHeight(z->right))+1;
    y->height=max(nodeHeight(y->left),nodeHeight(y->right))+1;
    return y;
}

NodeAVL* LRRotation (NodeAVL* z) {
    z->left=LeftRotation(z->left);
    return RightRotation(z);
}

NodeAVL* RLRotation (NodeAVL* z) {
    z->right=RightRotation(z->right);
    return LeftRotation(z);
}

NodeAVL* createNodeAVL(List* val) {
    NodeAVL* newNode = (NodeAVL*)malloc(sizeof(NodeAVL));
    newNode->height = 0;
    newNode->val = val;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

int getHeight(NodeAVL* root) {
    if (root == NULL)
        return 0;
    
    int leftHeight = getHeight(root->left);
    int rightHeight = getHeight(root->right);
    
    return 1 + max(leftHeight, rightHeight);
}

//write results in file
void writeResults (FILE* out, Node* head) //for teamElimination function
{
    fprintf(out,"%s %s \n\n","Results", "teamElimination:");
    while(head!=NULL)
    {
        fprintf(out,"%s\n",head->val.teamName);
        head=head->next;
    }
}

void writeQueueToFile(Queue* q, int i, FILE* fp) {
    if (fp == NULL) {
        printf("Eroare la deschiderea fisierului.\n");
        return;
    }
    if (isEmpty(q)) {
        fprintf(fp, "Coada este goala.\n"); 
    } else {
        fprintf(fp, "\n\nRound %d:\n",i); //Header coloane
        Node* temp=q->front;
        while (temp!=NULL) {
            fprintf(fp, "%-40s\t%-40s\n", temp->val.teamName, temp->next->val.teamName);
            temp=temp->next->next;
        }
    }
}

void display_stack(Stack* s, int i, FILE* fp) {
    if (fp == NULL) {
        printf("Eroare la deschiderea fisierului.\n");
        return;
    }
    fprintf(fp, "\nWinners of Round %d:\n",i); //Header
    while(s!=NULL) {
        fprintf(fp, "%-40s\t%-40f\n",s->data->val.teamName,s->point);
        s=s->next;
    }
}

void printTreeToFileRecursive(Tree* root, FILE* file) {
    if (root != NULL) {
        printTreeToFileRecursive(root->right, file);
        fprintf(file, "%-40s\t%-40f\n", root->teamName, root->point);
        printTreeToFileRecursive(root->left, file);
    }
}

void printTreeToFile(Tree* root) {
    FILE* temp = fopen("temp.out", "w"); 
    if (temp == NULL) {
        printf("Eroare la deschiderea fisierului!\n");
        return;
    }
    printTreeToFileRecursive(root, temp);
    fclose(temp);
    FILE* out=fopen("r.out","a");
    temp =fopen("temp.out","r");
    fprintf(out, "\n\nContent of BTS:\n");
    if (temp != NULL) {
        char ch;
        while ((ch = fgetc(temp)) != EOF) {
            fputc(ch, out);
        }
        fclose(temp); 
    }
    fclose(out); 
    remove("temp.out");
}

void printLevelAVL(NodeAVL* root, int level, FILE* outFile) {
    if (root == NULL)
        return;

    if (level == 0) {
        fprintf(outFile, "%s - %.6f\n", root->val->teamName, root->val->point);
    } else if (level > 0) {
        printLevelAVL(root->left, level - 1, outFile);
        printLevelAVL(root->right, level - 1, outFile);
    }
}

void printSecondLevel(NodeAVL* root) {
    FILE* outFile = fopen("r.out", "a");
    if (outFile == NULL) {
        printf("Nu se poate deschide fisierul r.out\n");
        return;
    }

    printLevelAVL(root, 1, outFile);

    fclose(outFile);
}

//print results
void printQueue(Queue* q) {
    if(isEmpty(q)) {
        printf("Coada este goala.\n");
        return;
    }
    printf("Coada contine:\n");
    Node* temp=q->front;
    while (temp!=NULL) {
        printf("%s\n", temp->val.teamName);
        temp=temp->next;
    }
    printf("\n");
}

void displayStack(Stack* stack) {
    if (stack == NULL) {
        printf("Stiva este vida.\n");
        return;
    }

    printf("Stiva:\n");

    Stack* current = stack;
    while (current != NULL) {
        printf("Echipa: %s\n", current->data->val.teamName);
        printf("Jucatori:\n");

        for (int i = 0; i < current->data->val.nPlayers; i++) {
            Player player = current->data->val.p[i];
            printf("- %s %s, Puncte: %d\n", player.firstName, player.secondName, player.points);
        }

        printf("Punctaj echipa: %.2f\n", current->point);
        printf("\n");

        current = current->next;
    }
}


void display(Node* head) {
    while (head != NULL) {
        printf("%d %s\n", head->val.nPlayers, head->val.teamName);
        for (int i = 0; i < head->val.nPlayers; i++) {
            printf("%s %s %d\n", head->val.p[i].firstName, head->val.p[i].secondName, head->val.p[i].points);
        } printf("\n");
        head=head->next;
    }
    printf("\n");
}

void printLevel(NodeAVL* root, FILE* file) {
    if (root == NULL)
        return;
    fprintf(file, "Team Name: %s Point: %.2f\n", root->val->teamName, root->val->point);
    printLevel(root->left, file);
    printLevel(root->right, file);
}

int max(int a, int b) {
    return ((a>b)?a:b);
}

void printList(List* node) {
    while (node != NULL) {
        printf("Team: %s, Points: %.2f\n", node->teamName, node->point);
        node = node->next;
    }
} 
