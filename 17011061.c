//  Tahir Can Özer
//  17011061
//  Algoritma Analizi 4. Ödev

#include <stdio.h>
#include <stdlib.h>
#include<string.h>

typedef struct Queue {
    int front, rear, size;
    unsigned capacity;
    int* array;
}QUEUE;

QUEUE* createQueue(unsigned capacity){
    QUEUE* queue = (QUEUE*)malloc(sizeof(QUEUE));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    
    queue->rear = capacity - 1;
    queue->array = (int*)malloc(queue->capacity * sizeof(int));
    return queue;
}

int isFull(QUEUE* queue)
{
    return (queue->size == queue->capacity);
}

int isEmpty(QUEUE* queue)
{
    return (queue->size == 0);
}

void enqueue(QUEUE* queue, int item)
{
    if (isFull(queue))
        return;
    queue->rear = ((queue->rear + 1) % (queue->capacity));
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
    //   printf("%d enqueued to queue\n", item);
}

int dequeue(QUEUE* queue)
{
    if (isEmpty(queue))
        return 0;
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1)
    % queue->capacity;
    queue->size = queue->size - 1;
    //    printf("%d dequeued from queue\n", item);
    return item;
}

int front(QUEUE* queue)
{
    if (isEmpty(queue))
        return 0;
    return queue->array[queue->front];
}

int rear(QUEUE* queue)
{
    if (isEmpty(queue))
        return 0;
    return queue->array[queue->rear];
}

typedef struct node{
    int isFollow;
    int isBot;
    char fName[15];
    char lName[15];
}NODE;

//dosyadaki toplam kisi sayisini hesaplayip donduren
//fonksiyon
int countNodes(FILE *fp){
    char buffer[200];
    int count=0;
    while(fgets(buffer,201,fp)){
        count++;
    }
    return count;
}

void readFile(FILE *fp,NODE** socialNet,int numberOfNodes){
    
    char buffer[200]; //line okumak icin buffer
    char fName[15]; //
    char lName[15];
    int i=0,j=0,k;
    
    char* ptr1; // token islemi icin kullandigim pointerlar
    char* ptr2;
    
    
    while(fgets(buffer,201,fp)){
        
        ptr1 = strtok(buffer,"\r");
        ptr2 = strtok(ptr1,",");
        
        i = atoi(ptr2);
        ptr2 = strtok(NULL,",");    //sonraki token a gec
        strcpy(fName,ptr2);
        ptr2 = strtok(NULL,",");
        strcpy(lName,ptr2);
        
        fgets(buffer,201,fp);
        ptr1 = strtok(buffer,"\r");
        ptr2 = strtok(ptr1,",");
        
        //graph bir matrise aktarilmistir.
        //satir index leri i. kisinin takip ettigi kisiler(j degerleri) icin 1 degerini almistir
        //dolayisiyla sutunlardaki degerler araciligiyla j. kisiyi takip eden her bir i kisisine ulasilabilir
        
        while(ptr2 != NULL ){
            j = atoi(ptr2);
            socialNet[i][j].isFollow = 1;       //i. kisinin takip ettigi kisiler
            for(k=0;k<numberOfNodes;k++){
                strcpy(socialNet[i][k].fName,fName);
                strcpy(socialNet[i][k].lName,lName);
                
            }
            ptr2 = strtok(NULL,",");    //sonraki token a gec
        }
    }
}
//baslangic durumundaki in-degree degerlerinin hesaplanmasini
//saglayan fonksiyon
void calculateInitialDegrees(NODE **socialNet,int *initialDegrees, int numberOfNodes){
    
    int i,j;
    int sum;
    
    for(j=0;j<numberOfNodes;j++){
        sum = 0;
        for(i=0;i<numberOfNodes;i++){
            if((socialNet[i][j].isFollow == 1) && (socialNet[i][j].isBot == 0)){
                sum++;
            }
        }
        initialDegrees[j] = sum;
    }
}


//verilen M degerinden kucuk takipci sayisina sahip kisilerin elenmesini
//saglayan fonksiyon
void isBot(NODE** socialNet, int M, int *greaterThanM,int numberOfNodes){
    int i,j;
    int sum=0;
    int changeFlag=0;
    int botFlag;
    
    while(changeFlag == 0 ){
        changeFlag = 1;
        for(j=0;j<numberOfNodes;j++){
            sum = 0;
            botFlag = 1;
            
            for(i=0;i<numberOfNodes;i++){
                if((socialNet[i][j].isFollow == 1) && (socialNet[i][j].isBot == 0)){
                    sum++;
                    botFlag=0;
                }
                
            }
            if((sum < M) ){
                for(i=0;i<numberOfNodes;i++){
                    socialNet[i][j].isBot = 1;
                    socialNet[j][i].isBot = 1;
                }
                changeFlag = 0;
            }
            if(botFlag == 1){
                changeFlag = 1;
            }
            greaterThanM[j] = sum;
        }
    }
}


// ilgili kisinin influencer olup olmadigina sum =>x ve totalConnections[kisi] => y
//karsilastirmalari yaparak karar veren fonksiyon
void isInfluencer(NODE** socialNet, int X, int Y,int *influencer,int *totalConnections,int numberOfNodes){
    
    int i,j;
    int sum;
    
    for(j=0;j<numberOfNodes;j++){
        sum = 0;
        
        for(i=0;i<numberOfNodes;i++){
            if((socialNet[i][j].isFollow == 1) && (socialNet[i][j].isBot == 0)){
                sum++;
            }
        }
        if((sum >= X) && (totalConnections[j] >= Y)){
            influencer[j] = 1;
        }
    }
}

//kendisine gonderilen kaynak dugumden yola cikarak bu dugumu
//takip eden dugumleri ve bunlari takip eden diger dugumleri bulan bfs algoritmasi
int BFS(QUEUE* queue,NODE** socialNet,int visited[],int index,int numberOfNodes){
    
    int i;
    int v;
    int count=0;
    
    if(visited[index] == 0){
        enqueue(queue,index);
        visited[index] = 1;
        while(!isEmpty(queue)){
            v = dequeue(queue);
            for(i=0;i<numberOfNodes;i++){
                if((socialNet[i][v].isBot == 0) && (socialNet[i][v].isFollow == 1) && (visited[i]==0)){
                    enqueue(queue, i);
                    count++;
                    visited[i] = 1;
                }
            }
        }
    }
    return count;
}

// Dugumlerin toplam baglanti sayisinin hesaplandigi fonksiyon
// Bu fonksiyon icerisinde BFS fonskyinonunu cagirarak her bir kisi icin
// toplam baglanti sayisini hesapliyor
void calculateTotalConnections(QUEUE* queue,NODE** socialNet,int *totalConnections,int numberOfNodes){
    
    int i,j;
    int flag=0;
    int totalConnectionNumber;
    
    int *visited;
    int *visitedCopy;
    visited = calloc(numberOfNodes, sizeof(int));
    visitedCopy = calloc(numberOfNodes, sizeof(int));
    
    for(i=0;i<numberOfNodes;i++){
        flag = 0;
        for(j=0;j<numberOfNodes;j++){
            if(socialNet[i][j].isBot == 0){
                flag=1;
            }
        }if(flag == 0){
            visited[i] = 1;  // botlari ayikla
            visitedCopy[i] = 1;  // botlari ayikla
        }
    }
    
    for(i=0;i<numberOfNodes;i++){
        
        totalConnectionNumber = BFS(queue,socialNet,visited,i,numberOfNodes);
        totalConnections[i] = totalConnectionNumber;
        
        for(j=0;j<numberOfNodes;j++){
            visited[j] = visitedCopy[j];  //array in value larini eski duruma getir
        }
    }
    free(visited);
    free(visitedCopy);
}


int main(int argc, const char * argv[]) {
    
    int i;
    int mode;
    int numberOfNodes;
    int m,x,y;
    FILE* fp;
    
    fp = fopen("socialNET.txt","r");
    if(fp != NULL){
        numberOfNodes = (countNodes(fp)/2)+1;
    }else{
        return 0;
    }
    fclose(fp);
    
    int *influencer;
    int *initialDegrees;
    int *totalConnections;
    int *greaterThanM;
    influencer = calloc(numberOfNodes, sizeof(int));
    initialDegrees = calloc(numberOfNodes, sizeof(int));
    totalConnections = calloc(numberOfNodes, sizeof(int));
    greaterThanM = calloc(numberOfNodes, sizeof(int));
    
    NODE **graph = (NODE **)calloc(numberOfNodes,sizeof(NODE *));
    QUEUE* queue = createQueue(numberOfNodes);
    
    for (i = 0; i < numberOfNodes; ++i){
        graph[i] = (NODE *)calloc(numberOfNodes,sizeof(NODE));
    }
    
    fp = fopen("socialNET.txt","r");
    
    if(fp != NULL){
        readFile(fp,graph,numberOfNodes);
    }else{
        return 0;
    }
    fclose(fp);
    
    printf("Normal Mode: 1, Detailed Mode: 2\n");
    printf("-------------------------------------------------------------------------------------\n\n");
    printf("Calistirmak istediginiz modu giriniz : ");
    scanf("%d",&mode);
    
    if(mode == 1){
        printf("M Degerini giriniz : ");
        scanf("%d",&m);
        printf("X Degerini giriniz : ");
        scanf("%d",&x);
        printf("Y Degerini giriniz : ");
        scanf("%d",&y);
        printf("\n");
        
        isBot(graph, m, greaterThanM,numberOfNodes);
        calculateTotalConnections(queue,graph,totalConnections,numberOfNodes);
        isInfluencer(graph,x,y,influencer,totalConnections,numberOfNodes);
        
        printf("Influencer kişilerin bilgileri :\n\n");
        for(i=1;i<numberOfNodes;i++){
            if(influencer[i] == 1){
                printf("%2d - %s %s",i,graph[i][0].fName,graph[i][0].lName);
                printf("\n");
            }
        }
        
    }else if(mode == 2){
        
        printf("M Degerini giriniz : ");
        scanf("%d",&m);
        printf("X Degerini giriniz : ");
        scanf("%d",&x);
        printf("Y Degerini giriniz : ");
        scanf("%d",&y);
        printf("\n");
        
        calculateInitialDegrees(graph, initialDegrees,numberOfNodes);
        isBot(graph, m,greaterThanM,numberOfNodes);
        calculateTotalConnections(queue,graph,totalConnections,numberOfNodes);
        isInfluencer(graph,x,y,influencer,totalConnections,numberOfNodes);
        
        printf("Graftaki bütün düğümlerin başlangıç durumunda in-degree değerleri \n\n");
        
        for(i=1;i<numberOfNodes;i++){
            printf("%2d. Dugumun baslangic in-degree degeri : %d",i,initialDegrees[i]);
            printf("\n");
        }
        
        printf("-------------------------------------------------------------------------------------\n\n");
        
        
        printf("Verilen M değeri için elenmeyen düğümlerin in-degree değerleri ve bu düğümlerdeki kişilerin ad-soyad bilgileri \n\n");
        
        for(i=1;i<numberOfNodes;i++){
            if(greaterThanM[i] != 0){
                printf("Dugum No: %2d In-Degreee:%2d  Ad-Soyad: %s %s",i,greaterThanM[i],graph[i][0].fName,graph[i][0].lName);
                printf("\n");
            }
        }
        
        printf("-------------------------------------------------------------------------------------\n\n");
        printf("Influencer kişilerin ad-soyad bilgileri,in-degree değerleri ve toplam bağlantı sayısı\n\n");
        
        for(i=1;i<numberOfNodes;i++){
            if(influencer[i] == 1){
                printf("Dugum No: %2d  Toplam Baglanti: %2d Ad-Soyad: %s %s",i,totalConnections[i],graph[i][0].fName,graph[i][0].lName);
                printf("\n");
            }
        }
        
    }else{
        printf("\n Gecersiz Deger Girildi \n\n");
    }
    
    free(influencer);
    free(initialDegrees);
    free(totalConnections);
    free(greaterThanM);
    
    return 0;
}
