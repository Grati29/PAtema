# include "head.h"
void process_objectives(int objectives[]) {
    // Implementeaza logica pentru fiecare obiectiv în func?ie de valorile din objectives
    for (int i = 0; i < 5; i++) {
        if (objectives[i] == 1) {
            // Implementeaza cerin?a corespunzatoare obiectivului i
            printf("Cerin?a %d realizata!\n", i + 1);
        }
    }
}
//task1
int makelist(Node** head, FILE* in) {
    int n, i, j; // n-number of teams
    if (in == NULL) {
        printf("Can't open the file.");
        return -1;
    }
    fscanf(in, "%d", &n);
    for (i=0; i<n; i++) 
    {
        Node* newNode = (Node*)malloc(sizeof(Node));
        if (newNode != NULL) {
            fscanf(in, "%d", &(newNode->val.nPlayers));
            fgetc(in); // Skip newline
            newNode->val.teamName = (char*)malloc(30 * sizeof(char));
            fgets(newNode->val.teamName, 30, in);
            newNode->val.teamName[strcspn(newNode->val.teamName, "\n")] = '\0'; // Remove newline
            newNode->val.p = (Player*)malloc(sizeof(Player) * newNode->val.nPlayers);
            for (j = 0; j < newNode->val.nPlayers; j++) {
                newNode->val.p[j].firstName = (char*)malloc(30 * sizeof(char));
                newNode->val.p[j].secondName = (char*)malloc(30 * sizeof(char));
                fscanf(in, "%s", newNode->val.p[j].firstName);
                fscanf(in, "%s", newNode->val.p[j].secondName);
                fscanf(in, "%d", &(newNode->val.p[j].points));
            }
            newNode->next = NULL;
            if (*head == NULL) {
                *head = newNode;
            } else {
                Node* aux=*head;
                while (aux->next!=NULL) {
                    aux=aux->next;
                }
                aux->next=newNode;
            }
        }
    }
    return n;
}

//task2
float calculatePoints(Node*head)
{
    int points=0;
    float totalPoints=0.00;
    if (head!=NULL) {
        for (int j=0; j<head->val.nPlayers; j++) {
            points=points+head->val.p[j].points;
        }
        totalPoints=(float)points/head->val.nPlayers;
    }
    return totalPoints;
}

Node* teamElimination(Node* head, int n, int *newn) {
    int nelim = 0; // nelim - number of eliminated teams
    int j, p=1;
    float totalPoints, minim; // totalPoints - for each team
    char tName[30]; // tName - the name of the team which has the minimum points
    Node* copyHead;

    while ((p*2)<n) {
        p=p*2;
    }
    for (int k=0; k<n-p; k++) {
        copyHead = head;
        minim = 999999.9999;
        while(copyHead!=NULL) {
        totalPoints=calculatePoints(copyHead);
            if(totalPoints<minim) {
                minim=totalPoints;
                strcpy(tName, copyHead->val.teamName);
            }
        copyHead=copyHead->next;
        }
        head=delete(head, tName);
        nelim++;
    }
    *newn=n-nelim;
    return head;
}

//task3
Stack* stacks(Queue* q, Stack* sWin, Stack* sLose,  FILE* out, int i) {
    float pointst1,pointst2;
    FILE* tempFile = fopen("temp.out", "w");
    writeQueueToFile(q,i,tempFile);
    while(q->front!=NULL)
    {
        pointst1=calculatePoints(q->front);  
        pointst2=calculatePoints(q->front->next);
        if(pointst1 >= pointst2) {
            for(int j = 0; j < q->front->val.nPlayers; j++) {
            q->front->val.p[j].points++;
        }
            push(&sWin,q->front,calculatePoints(q->front)); 
            push(&sLose,q->front->next,calculatePoints(q->front->next)); 
        }
        else {
            for(int j = 0; j < q->front->next->val.nPlayers; j++) {
            q->front->next->val.p[j].points++;
        }
            push(&sLose,q->front,calculatePoints(q->front)); 
            push(&sWin,q->front->next,calculatePoints(q->front->next));
        }
        q->front=q->front->next;
        q->front=q->front->next;
    }
    reverse_stack(&sWin);
    reverse_stack(&sLose);
    display_stack(sWin, i, tempFile);
    fclose(tempFile);
    out=fopen("r.out","a");
    tempFile=fopen("temp.out","r");
    if (tempFile != NULL) {
        char ch;
        while ((ch = fgetc(tempFile)) != EOF) {
            fputc(ch, out);
        }
        fclose(tempFile); 
    }
    fclose(out); 
    remove("temp.out");
    return sWin;
}

List* task3(Queue* q, FILE* out, char winner[]) {
    int nr, i=1;
    List* newlist=NULL;
    Stack *sWin,*sLose, *copysWin;
    sWin=initializeStack();
    sLose=initializeStack();
    copysWin=initializeStack();
    sWin=stacks(q,sWin,sLose,out,i);
    nr=countElements(sWin);
    while(nr>=1) {
        mergeStacks(sWin, &copysWin);
        i++;
        if(nr==1) {
            strcpy(winner,sWin->data->val.teamName);
            mergeStacks(sWin, &copysWin);
            break;
        }
    else {
        mergeStacks(sWin, &copysWin);
        emptyQueue(q);
        copyinqfroms(q,sWin);
        emptyStack(&sWin);
        emptyStack(&sLose);
        sWin=stacks(q,sWin,sLose,out,i);
        }
        nr=countElements(sWin);
    } 
    newlist=copyStackToLinkedList(copysWin);
    return newlist;
}

//task4
Tree* insertNode(Tree* root, char* teamName, float point) {
    if (root == NULL) {
        return createNodeTree(teamName, point);
    }
    
    if (point>root->point) {
        root->left=insertNode(root->left, teamName, point);
    } else if (point<root->point) {
        root->right=insertNode(root->right, teamName, point);
    } else {
        if (strcmp(teamName, root->teamName) > 0) {
            root->right=insertNode(root->right, teamName, point);
        } else {
            root->left=insertNode(root->left, teamName, point);
        }
    }
    return root;
}

//task5
void sortList(List** headRef) {
    if (*headRef == NULL || (*headRef)->next == NULL)
        return;

    int swapped;
    List** ptr1;
    List* lptr = NULL;

    do {
        swapped = 0;
        ptr1 = headRef;

        while ((*ptr1)->next != lptr) {
            List* current = *ptr1;
            List* next = current->next;

            if (current->point < next->point || (current->point == next->point && strcmp(current->teamName, next->teamName) < 0)) {
                current->next = next->next;
                next->next = current;
                *ptr1 = next;
                swapped = 1;
            }

            ptr1 = &(*ptr1)->next;
        }

        lptr = *ptr1;
    } while (swapped);
}

NodeAVL* insertNodeAVL(NodeAVL* root, List* val) {
    if (root == NULL)
        return createNodeAVL(val);

    int cmp = strcmp(val->teamName, root->val->teamName);

    if (cmp < 0) {
        root->left = insertNodeAVL(root->left, val);
        if (nodeHeight(root->left) - nodeHeight(root->right) == 2) {
            if (strcmp(val->teamName, root->left->val->teamName) < 0)
                root = RightRotation(root);
            else
                root = LRRotation(root);
        }
    } else if (cmp > 0) {
        root->right = insertNodeAVL(root->right, val);
        if (nodeHeight(root->right) - nodeHeight(root->left) == 2) {
            if (strcmp(val->teamName, root->right->val->teamName) > 0)
                root = LeftRotation(root);
            else
                root = RLRotation(root);
        }
    }

    root->height = (nodeHeight(root->left) > nodeHeight(root->right)) ? nodeHeight(root->left) : nodeHeight(root->right) + 1;

    return root;
}

NodeAVL* copyListToAVL(List* head) {
    NodeAVL* root = NULL;
    List* current = head;

    while (current != NULL) {
        root = insertNodeAVL(root, current);
        current = current->next;
    }
    return root;
}

int main() {
	    FILE *file;
    char buffer[256];
    int objectives[5];

    // Numele fi?ierului de intrare c.in
    char *input_file = "c.in";

    // Deschide fi?ierul de intrare
    file = fopen(input_file, "r");
    if (file == NULL) {
        printf("Nu s-a putut deschide fisierul de intrare.\n");
        return 1;
    }

    // Citirea obiectivelor din fi?ierul de intrare
    fgets(buffer, sizeof(buffer), file);
    sscanf(buffer, "%d %d %d %d %d", &objectives[0], &objectives[1], &objectives[2], &objectives[3], &objectives[4]);

    // Procesarea obiectivelor
    process_objectives(objectives);

    // Închide fi?ierul de intrare
    fclose(file)
	
	
    //task1
    Node* head = NULL;
    int nTeams;
    FILE* in = fopen("d.in", "r");
    FILE* out= fopen("r.out","w+a");
    nTeams=makelist(&head, in);
    fclose(in);
    
    //task2
    int nT; //nT- number of teams after teamElimination function
    head=teamElimination(head, nTeams, &nT);
    writeResults(out,head);
    fclose(out);
    
    //task3
    Queue *q;
    Stack *sWin,*sLose;
    List* newlist;
    char winner[50];
    q=createQueue();
    enqueue(q, head);
    newlist=task3(q,out,winner);
    
    //task4
    Tree* root=NULL;
    List* copynewlist=newlist;
    while(copynewlist!=NULL) {
        root=insertNode(root, copynewlist->teamName, copynewlist->point);
        copynewlist=copynewlist->next;
    }
    printTreeToFile(root);
    //printList(newlist);
    
    //task5
    NodeAVL* avl=NULL;
    sortList(&newlist);
    avl=copyListToAVL(newlist);
    printSecondLevel(avl);
    
    //freememory
    freeList(head);
    
    return 0;
}
