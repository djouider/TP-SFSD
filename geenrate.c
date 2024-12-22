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

bool find_list2(int *list, int len,int N){
    int i=0 ;
    bool found=false;
    while ((!found) && (i<len))
    {
        if (list[i] == N){
            found = true;
            printf("index=%d\n",i);
        }
        i++;
    }
    return found;
}

int generate_random_id(int *list){
    int n,i;bool f;
    do {
        n = 110000 +  (rand() * 100) % (880001);
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
        n = 1 +  rand() % (880001);
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
    int len =0,i,N;
    int *list2 = (int*)malloc(880000*sizeof(int));
    bool ok;
    clock_t start , end;
    printf("pick up a number\n");
    scanf("%d",&N);
    int list[N];
    for (i=0;i<N;i++){
        list2[i] = 0;
    }
    start = clock();
    //* comparing between the two methods;
    //method 1:
    N=32766;
    for (i=0;i<N;i++){
        len = generate_random_id(list2);
        printf("i=%d i/b=%d random=%d\n",i+1,i/1024,len);
        list[i] = len;
    }
    end = clock();
    printf("time took %f\n",((double) (end - start)) / CLOCKS_PER_SEC);

    for(i=32768;i<100000;i++){
        if (list2[i] == 1){
        printf("list[%d]=%d\n",i,list2[i]);
        }
    }

    start = clock();
    check_double(list,N);
    end = clock();
    printf("time took %f\n",((double) (end - start)) / CLOCKS_PER_SEC);
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
