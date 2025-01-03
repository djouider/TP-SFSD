#include "files_modules.h"

int main(){
    srand(time(NULL));
    fichier_lnof *F;
    block_lof buffer;
    bool found;
    buffer.link = -1;
    buffer.nb = 0;
    int i,j=0,N,r,*list;
    cost cout;
    fichier_tof_index *I,*I2;
    fichier_tof *T;
    enreg_index *list_index = (enreg_index*)malloc(sizeof(enreg_index)),*list_index_journal = (enreg_index*)malloc(sizeof(enreg_index));
    enreg e;
    block_index buffer_index,buffer_index2;
    block_tof buffer_tof;

    //* writing the blocks

    open_lnof(&F,"test2",'e');
    
    if (F->f != NULL ){
        //* creating the index 
        open_index(&I,"index_test2",'e');
        list_index = resize_list_index(list_index,get_Header_index(I,"num_ins"));
        open_tof(&T,"journal_magazine",'n');
        open_index(&I2,"index_test2_journal",'n');

        load_index(I,list_index);
        //bulk_laod_revue_periodique(F,T,I2,list_index_journal,&cout);
        load_index(I2,list_index_journal);

        Read_block_tof(T,&buffer_tof,0);
        for (i=0;i<10;i++){
            r = rand() % (buffer_tof.nb);
            printf("i=%d | %d %s, %s, %s, %d %d \n\n",r,buffer_tof.Tab[r].Document_id, buffer_tof.Tab[r].Title, buffer_tof.Tab[r].Author, buffer.Tab[r].Domaine, buffer_tof.Tab[r].Pub_year, buffer_tof.Tab[r].Available_qty);
        }

            printf("the index\n\n");
        //Read_Block_index(I2,&buffer_index,0);
        for (i=0;i<get_Header_index(I2,"num_ins");i++){
            printf("i=%d id=%d blk=%d pos=%d\n",i,buffer_index.Tab[i].key,buffer_index.Tab[i].adr_block,buffer_index.Tab[i].position);
        }

        printf("\nlist_index\n");
        for (i=0;i<get_Header_index(I2,"num_ins");i++){
            printf("i=%d id=%d blk=%d pos=%d\n",i,list_index_journal[i].key,list_index_journal[i].adr_block,list_index_journal[i].position);
        }
        list_index = resize_list_index(list_index,get_Header_index(I,"num_ins")); 
        list_index_journal = resize_list_index(list_index_journal,(get_Header_tof(T,"num_block")/b)+1);
        if (list_index == NULL || list_index_journal == NULL){
            printf("ERROR: Memory allocation failed for list_indexes in main\n");
            return 0;
        }
        

       
        close_index(I2);
        close_index(I); 
        close_tof(T);  
        close_lnof(F);
         
    }
}