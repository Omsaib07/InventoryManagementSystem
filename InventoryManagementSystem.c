#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct InventoryItem {
    int id;
    char name[50];
    int quantity;
    float price;
    int demand;
    struct InventoryItem* next;
}II;

typedef struct trie{
    struct trie* t[26];
    int EOW;    
}TRIE;

typedef struct hashtable{
    int key;
    int visit;
}HT;

int t_s=100;
int n=0;
HT* create_hash_table(){
    HT* temp=(HT*)malloc(t_s*sizeof(HT));
    for (int i = 0; i < t_s; i++)
    {
        temp[i].visit=0;
    }
    
    return temp;
}

void insertHT(HT* st,int k){
    if(n==t_s){
        printf("hash table is full\n");
        return;
    }
    else{
        int index;
        index=k%t_s;
        while(st[index].visit!=0){
            index=(index+1)%t_s;
        }
        st[index].key=k;
        st[index].visit=1;
        n++;
    }
}
void deleteHT(HT* st,int k){
    int i=0;
    int index;
    if(n==0){
        return;
    }
    index=k%t_s;
    while(i<n){
        if(st[index].visit){
            if(st[index].key==k){
                st[index].visit=0;
                n--;
                return;
            }
            i++;
        }
        index=(index+1)%t_s;
    }
}

int searchHT(HT* st,int k){
    int i=0;
    int index;
    if(n==0){
        return 0;
    }
    index=k%t_s;
    while(i<n){
        if(st[index].visit){
            if(st[index].key==k){
                return 1;
            }
            i++;
        }
        index=(index+1)%t_s;
    }
    return 0;
}

TRIE* getTrieNode(){
    TRIE* temp=(TRIE*)malloc(sizeof(TRIE));
    for (int i = 0; i < 26; i++)
    {
        temp->t[i]=NULL;
    }
    temp->EOW=0;
}

void insertTrie(TRIE* root,char *a){
    TRIE* curr=root;
    for (int i = 0; a[i] !='\0'; i++)
    {
        int index=a[i]-'a';
        if(curr->t[index]==NULL){
            curr->t[index]=getTrieNode();
        }
        curr=curr->t[index]; 
    }
    curr->EOW=1;
    

}
II* createItem(int id,const char* name, int quantity, float price,int demand) {
    II* newItem = (II*)malloc(sizeof(II));
    newItem->id = id;
    strcpy(newItem->name, name);
    newItem->quantity = quantity;
    newItem->price = price;
    newItem->demand=demand;
    newItem->next = NULL;

    return newItem;
}

II* addNewItem(II* inventory,int id,char* name,int quantity,float price,int demand, TRIE* start,HT* checker) {
    
                

    II* newItem=createItem(id,name, quantity,price,demand);
    newItem->id=id;
    if (inventory == NULL) {
        inventory = newItem;
    } 
    else {
        II* current = inventory;
        
        while (current->next != NULL) {
            current = current->next;
        }
        
        
        
        current->next = newItem;
    
    }
    


    printf("Item added to inventory.\n");
    insertHT(checker,id);
    insertTrie(start,name);
    return inventory;
}

II* addExistingItem(II* inventory,int id,int n,HT* checker){
    if(searchHT(checker,id)){
        II* current=inventory;
        while (current->next != NULL && current->id!=id) {
            current = current->next;
        }
        if(current->id==id){
            current->quantity=current->quantity+n;
            return inventory;
        }
    }
   else{
    printf("Item of that id does not exist\n");
    return inventory;
   }
}

II* sell(II* inventory,int n,int id,HT* checker){
    II* current=inventory;
    II* acurrent=NULL;
    II* bcurrent=NULL;
    if(inventory==NULL){
        printf("Shop is empty\n");
        return inventory;
    }
    if(current->id==id){
        if(current->quantity>n){
            current->quantity=current->quantity-n;
            return inventory;
        }
        if(current->quantity<n){
            printf("NOT ENOUGH STOCK AVAILABLE\n");
            return inventory;
        }
        if(current->quantity==n){
            inventory=inventory->next;
            free(current);
            deleteHT(checker,id);
            return inventory;
        }
    }
    while(current->id!=id && current->next!=NULL){
        current=current->next;
    }
    if(current->id==id && current->next!=NULL){
        bcurrent=current;
        acurrent=inventory;
        while(acurrent->next!=bcurrent){
            acurrent=acurrent->next;
        }
        if(bcurrent->quantity>n){
            bcurrent->quantity=bcurrent->quantity-n;
            return inventory;
        }
        if(bcurrent->quantity<n){
            printf("NOT ENOUGH STOCK AVAILABLE\n");
            return inventory;
        }
        if(bcurrent->quantity==n){
            acurrent->next=bcurrent->next;
            free(bcurrent);
            deleteHT(checker,id);
            return inventory;
        }

    }
    
    if(current->id==id && current->next==NULL){
        acurrent= inventory;
        current=inventory;
        while(current->next!=NULL){
            acurrent=current;
            current=current->next;

        }
        if(current->quantity>n){
            current->quantity=current->quantity-n;
            return inventory;
        }
        if(current->quantity<n){
            printf("NOT ENOUGH STOCK AVAILABLE\n");
            return inventory;
        }
        if(current->quantity==n){
            acurrent->next=current->next;
            free(current);
            deleteHT(checker,id);
            return inventory;
        }
        

    }
    if(current->next==NULL && current->id!=id){
            printf("No such item available\n");
            return inventory;
        }

}
void displayInventory(II* inventory) {
    if(inventory==NULL){
        printf("The shop is empty\n");
        return;
    }
    printf("\nInventory:\n");
    printf("ID\tName\tQuantity\tPrice\n");
    

    const II* current = inventory;
    while (current != NULL) {
        printf("%d\t%s\t%d\t\t%.2f\n", current->id, current->name, current->quantity, current->price);
        current = current->next;
    }
    printf("\n");
}

II* searchItemById( II* inventory, int id) {
    const struct InventoryItem* current = inventory;
    while (current != NULL) {
        if (current->id == id) {
            return (II*)current; 
        }
        current = current->next;
    }

    return NULL;
}


void displayAllWords(TRIE* root, char buffer[], int depth) {
    if (root == NULL) {
        return;
    }

    // Check if the current node is the end of a word
    if (root->EOW) {
        printf("%s\n", buffer);
    }

    // Display words for each child node
    for (int i = 0; i <26; i++) {
        if (root->t[i] != NULL) {
            buffer[depth] = 'a' + i;
            buffer[depth + 1] = '\0';

            // Recursively display words for child nodes
            displayAllWords(root->t[i], buffer, depth + 1);
        }
    }
}

//priority queue

int main() {
    II* inventory = NULL;
    TRIE* start=getTrieNode();
    HT* s=create_hash_table();
    int choice;
    int searchId;
    char buffer[100];
   
   

    
    do {
        printf("Menu:\n");
        printf("1. Add Item\n");
        printf("2. Display Inventory Details\n");
        printf("3. Search Item by ID\n");
        printf("4. Display All Items By Name\n");
        printf("5.Sell Item\n");
        printf("6. Add Existing Item\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                int id, quantity;
                float price;
                char name[50];
                int demand;
                

                printf("Enter item details:\n");
                printf("ID: ");
                
                scanf("%d", &id);
                
                printf("Name: ");
                scanf("%s", name);
                printf("Quantity: ");
                scanf("%d", &quantity);
                printf("Price: ");
                scanf("%f", &price);
                printf("Demand: ");
                scanf("%d", &demand);
                


                inventory=addNewItem(inventory,id,name,quantity,price,demand,start,s);
                break;
            }
            case 2:
                displayInventory(inventory);
                break;
            case 3:
                printf("Enter the ID to search: ");
                scanf("%d", &searchId);
                struct InventoryItem* foundItem = searchItemById(inventory, searchId);

                if (foundItem != NULL) {
                    printf("Item with ID %d found:\n", searchId);
                    printf("ID\tName\tQuantity\tPrice\n");
                    printf("%d\t%s\t%d\t\t%.2f\n", foundItem->id, foundItem->name, foundItem->quantity, foundItem->price);
                } else {
                    printf("Item with ID %d not found in inventory.\n", searchId);
                }
                break;

            case 4:
                displayAllWords(start, buffer, 0);
                break;
            case 5:
                 printf("enter the id of the item that you want to sell\n");
                 int a,b;
                 scanf("%d",&a);
                 printf("enter the quantity of it that you want to sell\n");
                 scanf("%d",&b);
                inventory=sell(inventory,b,a,s);
                break;
            case 6:
                 printf("enter the id:\n");
                 int nide,nqe;
                 scanf("%d",&nide);
                 printf("enter the quantity:\n");
                 scanf("%d",&nqe);
                 inventory=addExistingItem(inventory,nide,nqe,s);
                 break;
                 

            case 7:
                printf("Exiting program. Freeing memory...\n");
                break;
            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (choice != 7);

    // Free allocated memory
    while (inventory != NULL) {
        struct InventoryItem* temp = inventory;
        inventory = inventory->next;
        free(temp);
    }

    return 0;
}
