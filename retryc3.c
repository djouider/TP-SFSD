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
        open_tof(&T,"journal_magazine",'e');
        open_index(&I2,"index_test2_journal",'e');
        //bulk_load_lof(F,I,10000,&cout);
        //printf("cout for bulk_load is %d\n",cout);
        affich_entet_lnof(F);
        affich_entet_index(I);
        printf("1\n");
        Read_Block_lnof(F,&buffer,0);
        printf("1\n");
        for (i=0;i<10;i++){
            r = rand() % (buffer.nb);
            printf("i=%d | %d %s, %s, %s, %s, %d %d \n\n",r,buffer.Tab[r].Document_id, buffer.Tab[r].Title, buffer.Tab[r].Author, buffer.Tab[r].Type, buffer.Tab[r].Domaine, buffer.Tab[r].Pub_year, buffer.Tab[r].Available_qty);
        }

        list_index = resize_list_index(list_index,get_Header_index(I,"num_ins")); 
        list_index_journal = resize_list_index(list_index_journal,(get_Header_tof(T,"num_block")/b)+1);
        if (list_index == NULL || list_index_journal == NULL){
            printf("ERROR: Memory allocation failed for list_indexes in main\n");
            return 0;
        }
        load_index(I,list_index);
        printf("before update\n");
        affich_entet_tof(T);
        affich_entet_index(I2);
        load_index(I2,list_index_journal);

        //bulk_laod_revue_periodique(F,T,I2,list_index_journal,&cout);
        printf("1\n");
        Read_Block_lnof(F,&buffer,0);
        printf("1\n");
        for (i=0;i<10;i++){
            r = rand() % (buffer.nb);
            printf("i=%d | %d %s, %s, %s, %s, %d %d \n\n",r,buffer.Tab[r].Document_id, buffer.Tab[r].Title, buffer.Tab[r].Author, buffer.Tab[r].Type, buffer.Tab[r].Domaine, buffer.Tab[r].Pub_year, buffer.Tab[r].Available_qty);
        }
        modify_document(F,I,&cout,list_index);
        update_journal(F,&T,I2,list_index_journal,&cout);
        printf("afetr update\n");
        affich_entet_tof(T);
        affich_entet_index(I2);
        load_index(I2,list_index_journal);

        Read_block_tof(T,&buffer_tof,1);
        for (i=0;i<10;i++){
            printf("1i=%d | %d %s, %s, %s, %d %d \n\n",i,buffer_tof.Tab[i].Document_id, buffer_tof.Tab[i].Title, buffer_tof.Tab[i].Author, buffer_tof.Tab[i].Domaine, buffer_tof.Tab[i].Pub_year, buffer_tof.Tab[i].Available_qty);
        }

        for (i=buffer_tof.nb - 10;i<buffer_tof.nb;i++){
            printf("2i=%d | %d %s, %s, %s, %d %d \n\n",i,buffer_tof.Tab[i].Document_id, buffer_tof.Tab[i].Title, buffer_tof.Tab[i].Author, buffer_tof.Tab[i].Domaine, buffer_tof.Tab[i].Pub_year, buffer_tof.Tab[i].Available_qty);
        }

        printf("looking through the list_index %d\n",get_Header_index(I2,"num_ins"));
        for (i=0;i<get_Header_index(I2,"num_ins");i++){
            printf("3i=%d ,id= %d blk=%d pos=%d\n",i,list_index_journal[i].key,list_index_journal[i].adr_block,list_index_journal[i].position);
        }
        /*Read_Block_lnof(F,&buffer,0);
        for (i=0;i<10;i++){
            //printf("1\n");
            printf("i=%d | %d %s, %s, %s, %s, %d %d \n\n",i,buffer.Tab[i].Document_id, buffer.Tab[i].Title, buffer.Tab[i].Author, buffer.Tab[i].Type, buffer.Tab[i].Domaine, buffer.Tab[i].Pub_year, buffer.Tab[i].Available_qty);
        }*/

        /*printf("before in main fucntion:\n");
        for (i=get_Header_index(I,"num_ins")-10;i<get_Header_index(I,"num_ins");i++){
            printf("i=%d ,id= %d blk=%d pos=%d\n",i,list_index[i].key,list_index[i].adr_block,list_index[i].position);
        }
        printf("\n");

        delete_document(F,I,list_index,&cout);
        printf("cout for add is %d\n",cout.read + cout.write);
        printf("\nnrec=%d num_block=%d\n",get_Header_lnof(F,"nrec"),get_Header_lnof(F,"Lastblk"));

        printf("after in main fucntion:\n");
        for (i=get_Header_index(I,"num_ins")-10;i<get_Header_index(I,"num_ins");i++){
            printf("i=%d ,id= %d blk=%d pos=%d\n",i,list_index[i].key,list_index[i].adr_block,list_index[i].position);
        }
        printf("\n");

        printf("\n");
        load_index(I,list_index);
        printf("after the load function:\n");
        for (i=get_Header_index(I,"num_ins")-10;i<get_Header_index(I,"num_ins");i++){
            printf("i=%d ,id= %d blk=%d pos=%d\n",i,list_index[i].key,list_index[i].adr_block,list_index[i].position);
        }
        affich_entet_lnof(F);
        affich_entet_index(I);
        //modify_document(F,I,&cout,list_index);
        //printf("cout for modify is %d\n",cout.read + cout.write);

       //delete_document(F,I,list_index,&cout);
        //load_index(I,list_index);
        //printf("cout for delete is %d\n",cout.read + cout.write);*/

        close_index(I2);
        close_index(I); 
        close_tof(T);  
        close_lnof(F);
         
    }
}