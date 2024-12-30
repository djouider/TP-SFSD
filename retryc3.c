#include "modules.h"

int main(){
    srand(time(NULL));
    fichier_lnof *F;
    block_lof buffer;
    bool found;
    buffer.link = -1;
    buffer.nb = 0;
    int i,j=0,N,r,*list,cout;
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
        open_tof(&T,"journal_magazine",'e');
        open_index(&I2,"index_test2_journal",'e');
        //bulk_load_lof(F,I,10000,&cout);
        //printf("cout for bulk_load is %d\n",cout);
        
        list_index = resize_list_index(list_index,get_Header_index(I,"num_ins")); 
        list_index_journal = resize_list_index(list_index_journal,get_Header_tof(T,"num_block")+1);
        if (list_index == NULL || list_index_journal == NULL){
            printf("ERROR: Memory allocation failed for list_indexes in main\n");
            return 0;
        }
        load_index(I,list_index);

        for (i=0;i<10;i++){
            r = rand() % (get_Header_lnof(F,"nrec") + 1);
            printf("r=%d id= %d block=%d position=%d\n",r,list_index[r].key,list_index[r].adr_block,list_index[r].position);
        }
        for (i=0;i<10;i++){
            r = rand() % (get_Header_lnof(F,"nrec") + 1);
            printf("r=%d id= %d block=%d position=%d\n",r,list_index[r].key,list_index[r].adr_block,list_index[r].position);
        }
        printf("\nnrec=%d num_block=%d\n",get_Header_lnof(F,"nrec"),get_Header_lnof(F,"Lastblk"));
        Add_document(F,I,list_index,&cout);
        printf("cout for add is %d\n",cout);
        load_index(I,list_index);
        modify_document(F,I,&cout);
        printf("cout for modify is %d\n",cout);

        delete_document(F,I,list_index,&cout);
        load_index(I,list_index);
        printf("cout for delete is %d\n",cout);

        close_index(I2);
        close_index(I); 
        close_lnof(F);
        close_tof(T);   
    }
}