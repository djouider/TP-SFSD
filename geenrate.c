#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>

bool find_list(int *list, int len,int N){
    int i=0 ;
    
    bool found=false;
    while ((!found) && (i<len))
    {
        if (list[i] == N){
            found = true;
        }
        i++;
    }
    return found;
}

int generate_random_id(int *list){
    int n,i;bool f;
    do {
        n = 110000 +  (rand() * rand()) % (880001);
        i = n - 110000;
        //n = 1 +  rand() % (30);
    }while(list[i] == 1);
    //printf("\n");
    list[i] = 1;
    return n;
}

int generate_random_id2(int *list, int *len){
    int n;bool f;
    do {
        n = 110000 +  (rand() * rand()) % (880001);
        f = find_list(list,*len,n);
    }while(f);
    //printf("\n");
    list[(*len)] = n;
    (*len)++;
    return n;
}

void check_double(int *list,int length){
    int i,j;
    bool found;
    for (i=0;i<length;i++){
        for (j=i+1;j<length;j++){
            if (list[i] == list[j]){
                printf("i=%d j=%d value=%ddjnhgfbdvc\n",i,j,list[i]);
            }
        }
    }
}

int main() {
    int len =0,i,N,l=0;
    int *list2 = (int*)malloc(880000*sizeof(int));
    double time1,time2;
    bool ok;
    clock_t start , end;
    srand(time(NULL));
    printf("pick up a number\n");
    scanf("%d",&N);
    int list[N];
    for (i=0;i<N;i++){
        list2[i] = 0;
    }
    
    //* comparing between the two methods;

    //*method 1: (using hashing)
    start = clock();
    for (i=0;i<N;i++){
        len = generate_random_id(list2);
        printf("i=%d i/b=%d random=%d\n",i+1,i/1024,len);
        //list[i] = len;
    }
    end = clock();
    time1 = ((double) (end - start)) / CLOCKS_PER_SEC;


    //*method 2:linear search
    start = clock();
    for (i=0;i<N;i++){
        len = generate_random_id2(list,&l);
        printf("i=%d i/b=%d length=%d random=%d\n",i+1,i/1024,l,len);
        //list[i] = len;
    }
    end = clock();
    time2 = ((double) (end - start)) / CLOCKS_PER_SEC;


    printf("time took for hashing:%f\n",time1);
    printf("time took for linear %f\n",time2);
   /*for (i=0;i<32767;i++){
        generate_random_id(list,&len,ok);
    }*/
    //printf("finding :\nn=%d f=%x\nn=%d f=%x\nn=%d f=%x\n",115347,find_list2(list,len,115347),114257,find_list2(list,len,114257),142719,find_list2(list,len,142719));
    //* testing if the problem is in find list
    /*int list2[32770],len2 = 32768;
    for (i=0;i<32768;i++){
        list[i] =  110000 +  rand() % (880001); 
    }
    printf("double ?= %x\n",find_list(list2,len2,110000 +  rand() % (880001)));*/
} 
