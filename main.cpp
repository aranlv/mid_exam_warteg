#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>

struct Dish{
  char name[10];
  int price;
  int qty;
  Dish *next, *prev;
} *head, *tail;

Dish *createNode(char name[], int price, int qty){
  Dish *newNode = (Dish *)malloc(sizeof(Dish));
  strcpy(newNode->name, name);
  newNode->price = price;
  newNode->qty = qty;
  newNode->next = NULL;
  return newNode;
}

const int MAX_BUCKETS = 5; // banyak index yang mau dibuat

struct Ctm{
  char name[255];
  Ctm *next;
} *top[MAX_BUCKETS], *bottom[MAX_BUCKETS];

Ctm *createCtm(char name[]){
  Ctm *newNode = (Ctm *)malloc(sizeof(Ctm));
  strcpy(newNode->name, name);
  newNode->next = NULL;
  return newNode;
}

unsigned long djb2(char str[]){ 
  unsigned long key = 5381;

  for(int i = 0; str[i] != '\0'; i++){
    int ascii = int(str[i]);
    key = (key << 5) + key  + ascii;
  }

  return key % MAX_BUCKETS;
}
void pushCtm(char name[]){
  Ctm *newNode = createCtm(name);
  int idx = djb2(name);

  if(!top[idx]){ // linked list kosong
    top[idx] = bottom[idx] = newNode;
  } else{
    top[idx]->next = newNode;
    top[idx] = newNode;
  }
}


void pushTail(char name[], int price, int qty) //KEPAKE
{
  Dish *temp = createNode(name, price, qty); // create the new node
  if (!head){
    head = tail = temp; // tail will be the new head and the new tail
  } else{
    tail->next = temp; // current tail will point to the newly created node
    temp->prev = tail; // supposedly-new-tail's prev is the previous-tail's address
    tail = temp;       // the new node will be the last node in the LL (new tail)
  }
}
void pushHead(char name[], int price, int qty){ 
  Dish *temp = createNode(name, price, qty);
  if (!head){
    head = tail = temp;
  } else{
    temp->next = head;
    head->prev = temp;
    head = temp;
  }
}
void popTail(){ //MASUK POPMID
  if (!head){
    return; 
  } else if (head == tail){
    free(head);         
    head = tail = NULL; 
  } else{
    Dish *newTail = tail->prev;
    newTail->next = tail->prev = NULL; 
    free(tail);        
    tail = newTail; 
  }
}
void popHead(){ //MASUK POPMID
  if (!head){
    return;
  } else if (head==tail){
    free(head);
    head=tail=NULL;
  } else{ //second node becomes head
    Dish *newHead = head->next;
    head->next = newHead->prev = NULL;
    free(head);
    head = newHead;
  }
}
void popMid(char query[]){ //KEPAKE
  if (!head){ //no nodes
    return;
  } else if (strcmp(head->name, query)==0){//if the query is the first element
    popHead();
  } else if (strcmp(tail->name, query)==0){//if the query is the last element
    popTail();
  } else{
    Dish *current = head;
    while(current){
      //current before && is current!=NULL, make sure you don't try to access NULL->value and result in segmentation fault.
        if(strcmp(current->name, query)==0){
          current->prev->next = current->next;
          current->next->prev = current->prev;
          current->prev = current->next = NULL;
          free(current);
          current = NULL;
        }
        else current = current->next;
    } 
  }
}
void pushMid(char name[], int price, int qty){ //GAPERLU
  if(!head){
    pushHead(name, price, qty);
  } else{
    Dish *newNode = createNode(name, price, qty);
  }
}

void printMainMenu(){
  // OS
  printf("System: ");
  #ifdef _WIN32
	  printf("Windows\n");
  #elif __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_OS_IPHONE && TARGET_IPHONE_SIMULATOR
      printf("iPhone stimulator\n");
    #elif TARGET_OS_IPHONE
      printf("iPhone\n");
    #elif TARGET_OS_MAC
      printf("MacOS\n");
    #endif
  #elif __linux__
    printf("Linux OS\n");
  #endif

  // TIME
  time_t t; time(&t);
  printf("%s\n", ctime(&t));

  // LIST
  puts("1. Add Dish");
  puts("2. Remove Dish");
  puts("3. Add Customer");
  puts("4. Search Customer");
  puts("5. View Warteg");
  puts("6. Order");
  puts("7. Payment");
  puts("8. Exit Warteg");
  printf(">> ");

}
void addDish(){
  char dish[10]; int valid = 1; do{
    printf("Insert the name of the dish [Lowercase letters]: ");
    scanf("%[^\n]s", dish); getchar(); valid = 1;

    for(int i=0; i<strlen(dish); i++){ // validate lowercase
      if(dish[i]!=' ' && (dish[i] < 'a' || dish[i] > 'z')) {
        valid = 0; break;
      }
    }
    
    if(head){ // validate unique value when there LL not empty
      Dish *current = head; 
      while(current){
        if(strcmp(current->name, dish)==0){
          valid = 0; break;
        }
        current = current->next;
      } 
    }
    
  } while(valid==0);
  
  int price; do{
    printf("Insert the price of the dish [1000..50000]: ");
    scanf("%d", &price); getchar();
  } while(price < 1000 || price > 50000);

  int qty; do{
    printf("Insert the quantity of the dish [1..999]: ");
    scanf("%d", &qty); getchar();
  } while(qty < 1 || qty > 999);

  // PUSH
  pushTail(dish, price, qty);
  puts("The dish has been added!");
  puts("Press enter to continue...");
  getchar();
}
void removeDish(){
  puts("          Bude's Menu");
  puts("================================");
  printf("No.     Name    Quantity  Price \n");
  //      !234!23456789012!2345678!2345678
  int i=1; for (Dish *temp = head; temp != NULL; i++, temp = temp->next){
    //NO
    printf("%d.  ", i);

    //NAME
    int s=(12-strlen(temp->name));
    for(int j=0; j<s/2; j++) printf(" ");
    printf("%s", temp->name);
    for(int j=0; j<(s%2==0 ? s/2 : s/2+1); j++) printf(" ");

    //QUANTITY
    for(int j=0; j<2; j++) printf(" ");
    printf("%.3d", temp->qty);
    for(int j=0; j<3; j++) printf(" ");

    //PRICE
    printf(" Rp%d\n", temp->price);
  }
  puts("================================");
  
  int valid; char dish[10]; do{
    printf("Insert dish's name to be deleted: ");
    scanf("%[^\n]s", dish); valid = 1;
    
    for(int i=0; i<strlen(dish); i++){ // validate lowercase
      if(dish[i]!=' ' && (dish[i] < 'a' || dish[i] > 'z')) {
        valid = 0; break;
      }
    }
  } while(valid==0);
  
  popMid(dish);
  puts("The dish has been removed!");
  puts("Press enter to continue...");
  getchar();
}
void addCustomer(){
  int valid; char name[10]; do{
    printf("Insert the customer's name [Without space]: ");
    scanf("%[^\n]s", name); getchar(); valid = 1;

    for(int i=0; i<strlen(name); i++){ // validate
      if(name[i]==' ') {
        valid = 0; break;
      }
      if(!((name[i] >= 'a' && name[i] <= 'z')||(name[i] >= 'A' && name[i] <= 'Z'))){
        valid = 0; break;
      }
    }
  } while(valid==0);
  
  pushCtm(name);
  puts("Customer has been added!");
  puts("Press enter to continue...");
  getchar();
}
void searchCustomer(){
  int valid; char name[10]; do{
    printf("Insert the customer's name to be searched: ");
    scanf("%[^\n]s", name); getchar(); valid = 1;

    for(int i=0; i<strlen(name); i++){ // validate
      if(name[i]==' ') {
        valid = 0; break;
      }
      if(!((name[i] >= 'a' && name[i] <= 'z')||(name[i] >= 'A' && name[i] <= 'Z'))){
        valid = 0; break;
      }
    }
  } while(valid==0);


  int p=0; for (int i = 0; i < MAX_BUCKETS; i++){
    if(top[i]){
      Ctm *curr = top[i];
      while(curr){
        if(strcmp(curr->name, name)==0){
          p = 1; break;
        }
        curr = curr->next;
      }
    } 
  }

  p==0?printf("%s is not present.\n", name):printf("%s is present.\n", name);
  puts("Press enter to continue...");
  getchar();
}
void printCustomers(){
  int j=1; for (int i = 0; i < MAX_BUCKETS; i++){
    if(top[i]){
      Ctm *curr = top[i];
      while(curr){
        printf("%d. %s\n", j, curr->name); 
        j++;
        curr = curr->next;
      }
    } 
  }
  puts("Press enter to continue...");
  getchar();
}
void order(){
  printf("Insert the customer's name:")
}


int main(){
  int i; while(i!=8){
    do{
      //puts("===");printLinkedList();puts("===");
      printMainMenu();
      scanf("%d", &i); getchar();
      
      if(i==1) addDish();
      else if(i==2) removeDish();
      else if(i==3) addCustomer();
      else if(i==4) searchCustomer();
      else if(i==5) printCustomers();
      else if(i==8) return 0;
      
    } while(i<1 || i>8);  
  }
}