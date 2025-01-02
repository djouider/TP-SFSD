#include "modules.h"
#ifndef files_modules

void bulk_load_lof(fichier_lnof *F,fichier_tof_index *I,int N,cost *cout){
    block_lof buffer;
    (*cout).read = 0;
    (*cout).write = 0;
    buffer.nb = 0;
    int i,j=0,id,k=0,*list;
    enreg_index *list_index = (enreg_index *)malloc(N * sizeof(enreg_index)),temp;
    block_index buffer_index;

    //* initiallizing the list
    list = (int *)malloc(880000*sizeof(int));
    for (i=0;i<880000;i++){
        list[i] = 0;
    }
    if (list == NULL || list_index == NULL){
        printf("ERROR: Memory allocation problem in bulk_load");
    } else {

        //* starting to fill   
        for (i=0;i<N;i++){
            if ( j < b){
                fill_buffer(&buffer,j,list,&id);
                list_index[k].key = id;
                list_index[k].adr_block = i/b;
                list_index[k].position = j;
                list_index[k].del = 0;
                //printf("%d %s, %s, %s, %s, %d %d \n\n",buffer.Tab[0].Document_id, buffer.Tab[0].Title, buffer.Tab[0].Author, buffer.Tab[0].Type, buffer.Tab[0].Domaine, buffer.Tab[0].Pub_year, buffer.Tab[0].Available_qty);
                //printf("i=%d i/b=%d, j=%d\n",i,i/b,j);
                buffer.nb++;
                j++;
                k++;
            }else {
                Write_Block_lnof(F,&buffer,get_Header_lnof(F,"Lastblk"));(*cout).write++; 
                Alloc_block_lnof(F);(*cout).read++; (*cout).write++;(*cout).write++; 
                Read_Block_lnof(F,&buffer,get_Header_lnof(F,"Lastblk"));(*cout).read++; 
                buffer.nb = 1;
                j=0;
                fill_buffer(&buffer,j,list,&id);
                list_index[k].key = id;
                list_index[k].adr_block = i/b;
                list_index[k].position = j;
                list_index[k].del = 0;
                k++;
                //printf("%d %s, %s, %s, %s, %d %d \n\n",buffer.Tab[0].Document_id, buffer.Tab[0].Title, buffer.Tab[0].Author, buffer.Tab[0].Type, buffer.Tab[0].Domaine, buffer.Tab[0].Pub_year, buffer.Tab[0].Available_qty);
                j++;
                //printf("lastblk=%d\n",get_Header_lnof(F,"Lastblk"));
            }
        }
        // writing the last block
        Write_Block_lnof(F,&buffer,get_Header_lnof(F,"Lastblk"));(*cout).write++; 
    set_Header_lnof(F,"nrec",N);
    printf("terminated with success\n\n");

    //* sorting the list_index
    sort_list(list_index,N);

    //* bulkloading the index file
    j=0;
    buffer_index.nb =0;
        for (i=0;i<N;i++){
            if (j<b){
                buffer_index.Tab[j] = list_index[i];
                buffer_index.nb++;
                j++;
            }else{
                Write_Block_index(I,&buffer_index,get_Header_index(I,"num_block"));(*cout).write++; 
                Alloc_block_index(I);(*cout).read++; (*cout).write++;(*cout).write++;
                Read_Block_index(I,&buffer_index,get_Header_index(I,"num_block"));(*cout).read++; 
                buffer_index.nb =1;
                buffer_index.Tab[0] = list_index[i];
                j=1;
            }
        }
    Write_Block_index(I,&buffer_index,get_Header_index(I,"num_block"));(*cout).write++; 
    set_Header_index(I,"num_ins",N);
    printf("index terminated with success\n\n");
    }
}

void Search_by_id (fichier_tof_index *I,int key,bool *found,int *block,int *position,cost *cout){
    *found = false;
    (*cout).read=0;
    (*cout).write = 0;
    if (key < 110000 || key > 990000){
        printf("\n\nkey out of range\n\n");

    } else {
        int m,inf_b = 0,sup_b = get_Header_index(I,"num_block");
        block_index buff;
        bool stop=false;

        while(!stop && (inf_b <= sup_b)){
            m = (inf_b + sup_b)/2;

            Read_Block_index(I,&buff,m);(*cout).read++;
            //printf("nb in block index= %d is %d 1=%d and l=%d\n",m,buff.nb,buff.Tab[0].key,buff.Tab[buff.nb - 1].key);
            if (key >= buff.Tab[0].key && key <= buff.Tab[buff.nb - 1].key){
                Recherche_dicho_bufer(buff,key,found,block,position);
                //printf(" bloc in index_file=%d\n",m);
                stop = true;
            } else {
                if (key < buff.Tab[1].key){
                    sup_b = m-1;
                } else {
                    inf_b = m +1 ;
                }
            }
        }
    }
}

void modify_document(fichier_lnof *F,fichier_tof_index *I,cost *cout){
    block_index buffer_index;
    block_lof buffer;
    enreg e;
    (*cout).read =0;
    (*cout).write=0;
    bool found;
    int key,block,position;

    printf("\ngive the id of the record you want to modify\n");
    scanf("%d",&key);

    Search_by_id(I,key,&found,&block,&position,cout);  //!
    if (found){
        //the prints are just to see the changes
        Read_Block_lnof(F,&buffer,block);(*cout).read++;
        printf("the %d is in block %d position %d\n",buffer.Tab[position].Document_id,block,position);
        e = buffer.Tab[position];
        printf("before | %d %s, %s, %s, %s, %d %d \n\n",buffer.Tab[position].Document_id, buffer.Tab[position].Title, buffer.Tab[position].Author, buffer.Tab[position].Type, buffer.Tab[position].Domaine, buffer.Tab[position].Pub_year, buffer.Tab[position].Available_qty);
        read_changes(&e);
        buffer.Tab[position] = e;
        printf("after | %d %s, %s, %s, %s, %d %d \n\n",buffer.Tab[position].Document_id, buffer.Tab[position].Title, buffer.Tab[position].Author, buffer.Tab[position].Type, buffer.Tab[position].Domaine, buffer.Tab[position].Pub_year, buffer.Tab[position].Available_qty);
        Write_Block_lnof(F,&buffer,block);(*cout).write++;
    } else{
        printf("\nThe record was not found\n");
    }
} 

void Add_document(fichier_lnof *F,fichier_tof_index *I,enreg_index *list_index,cost *cout) {
    int num,choice,i,cpt=0,nrec=get_Header_lnof(F,"nrec"),adr = get_Header_lnof(F,"Lastblk"),min=990001,index,j,cnd,q,r;
    char respond[15];
    enreg document;
    block_lof buffer;
    block_index buffer_index;
    (*cout).read =0;
    (*cout).write=0;

    printf("how many documents you want to add ?\n");
        scanf("%d",&num);

    if (num > 0 ){
        printf("how do you want to generate them ?\n1. manually\n2. automaticlly\n");
            scanf("%d",&choice);

            while (choice == 1 && num > 5) {
                printf("the number to insert is a little bit long do you want to enter the document manually (yes or no)?\n");
                scanf("%s",respond);
            }
            
            if (strcmp(respond,"no") == 0){
                choice = 2;
            }

            //printf("before list_index\n");
            list_index = resize_list_index(list_index,nrec+num);
            Read_Block_lnof(F,&buffer,get_Header_lnof(F,"Lastblk")); (*cout).read++;

            switch (choice){
                case 1: //manually
                    //* filling the lnof file
                    for (i=0;i<num;i++){
                        printf("\nfilling the infos of document number %d\n----------------------------------\n",i+1);
                        fill_document_manually(&document,list_index,nrec);
                        if ( buffer.nb < b){
                            buffer.Tab[buffer.nb] = document;
                            buffer.nb++;
                        } else {
                            Write_Block_lnof(F,&buffer,get_Header_lnof(F,"Lastblk")); (*cout).write++;
                            Alloc_block_lnof(F);(*cout).write++;(*cout).write++;(*cout).read++;
                            adr++;
                            Read_Block_lnof(F,&buffer,get_Header_lnof(F,"Lastblk"));(*cout).read++;
                            buffer.nb = 1;
                            buffer.Tab[0] = document;
                        }
                        Write_Block_lnof(F,&buffer,get_Header_lnof(F,"Lastblk"));(*cout).write++;
                        //* filling the list
                        list_index[nrec].key = document.Document_id;
                        //printf("document_id= %d\n",document.Document_id);
                        if (document.Document_id < min) {
                            min = document.Document_id;
                        }
                        list_index[nrec].adr_block =  adr;
                        list_index[nrec].position = buffer.nb-1;
                        nrec++;
                    }
                    

                    //* sorting the list
                    sort_list(list_index,nrec);
            
                    //* filling the index file 
                    if (! search_list(list_index,nrec,min,&index)){
                        printf("%d was not found in the list\n",min); //* to check if the search didnt have problems
                    } 
                    adr = index / b; 
                    cnd = index; //b * adr
                    j = index % b;

                    Read_Block_index(I,&buffer_index,adr); (*cout).read++;
                    for (i=cnd;i<get_Header_index(I,"num_ins");i++){ 
                        if (j < b) {
                            buffer_index.Tab[j] = list_index[index];
                            //printf("nb = %d\n",buffer_index.nb);
                            j++;
                            index++;
                        } else {
                            Write_Block_index(I,&buffer_index,adr);(*cout).write++;
                            adr++;
                            //printf("first adr = %d num_block=%d i=%d index= %d j= %d\n",adr,get_Header_index(I,"num_block"),i,index,j);
                            Read_Block_index(I,&buffer_index,adr);(*cout).read++;
                            buffer_index.Tab[0] = list_index[index];
                            j=1;
                            index++;
                        }
                    }
                    Write_Block_index(I,&buffer_index,adr);(*cout).write++; //! we can get rid of this and the other one

                    cnd = i;
                    for (i=cnd;i<nrec;i++){ //adding the new values
                        if (j < b) {
                            buffer_index.Tab[j] = list_index[index];
                            //printf("nb = %d\n",buffer_index.nb);
                            index++;
                            j++;
                            buffer_index.nb++;
                        } else {
                            Write_Block_index(I,&buffer_index,adr); (*cout).write++;
                            adr++;
                            Alloc_block_index(I); (*cout).write++;(*cout).write++; (*cout).read++;
                            Read_Block_index(I,&buffer_index,adr);(*cout).read++;
                            buffer_index.Tab[0] = list_index[index];
                            j=1;
                            index++;
                            buffer_index.nb = 1;
                        }
                    }
                    Write_Block_index(I,&buffer_index,get_Header_index(I,"num_block"));(*cout).write++;

                    set_Header_index(I,"num_ins",nrec);
                    set_Header_lnof(F,"nrec",nrec);
                    break;

                case 2: //autimatticlly

                    //* filling the lnof file
                    for (i=0;i<num;i++){
                        fill_document_random(&document,list_index,nrec); 
                        if ( buffer.nb < b){
                            buffer.Tab[buffer.nb] = document;
                            buffer.nb++;
                        } else {
                            Write_Block_lnof(F,&buffer,get_Header_lnof(F,"Lastblk"));(*cout).write++;
                            Alloc_block_lnof(F);(*cout).write++;(*cout).write++;(*cout).read++;
                            adr++;
                            Read_Block_lnof(F,&buffer,get_Header_lnof(F,"Lastblk"));(*cout).read++;
                            buffer.nb = 1;
                            buffer.Tab[0] = document;
                        }
                        //* filling the list
                        list_index[nrec].key = document.Document_id;
                        //printf("document_id= %d\n",document.Document_id);
                        if (document.Document_id < min) {
                            min = document.Document_id;
                        }
                        list_index[nrec].adr_block =  adr;
                        list_index[nrec].position = buffer.nb-1;
                        nrec++;
                    }
                    Write_Block_lnof(F,&buffer,get_Header_lnof(F,"Lastblk"));(*cout).write++;

                    //* sorting the list
                    sort_list(list_index,nrec);
            
                    //* if the search had a problem
                    if (! search_list(list_index,nrec,min,&index)){
                        printf("%d was not found in the list\n",min); //* to check if the search didnt have problems
                    } 
                    adr = index / b; 
                    cnd = index; //b * adr
                    j = index % b;

                    Read_Block_index(I,&buffer_index,adr);(*cout).read++;
                    for (i=cnd;i<get_Header_index(I,"num_ins");i++){ //? i = index
                        if (j < b) {
                            buffer_index.Tab[j] = list_index[index];
                            //printf("nb = %d\n",buffer_index.nb);
                            j++;
                            index++;
                        } else {
                            Write_Block_index(I,&buffer_index,adr); (*cout).write++;
                            adr++;
                            //printf("first adr = %d num_block=%d i=%d index= %d j= %d\n",adr,get_Header_index(I,"num_block"),i,index,j);
                            Read_Block_index(I,&buffer_index,adr);(*cout).read++;
                            buffer_index.Tab[0] = list_index[index];
                            j=1;
                            index++;
                        }
                    }
                    Write_Block_index(I,&buffer_index,adr);(*cout).write++; //! this also 

                    cnd = i;
                    for (i=cnd;i<nrec;i++){ //adding the new values
                        if (j < b) {
                            buffer_index.Tab[j] = list_index[index];
                            //printf("nb = %d\n",buffer_index.nb);
                            index++;
                            j++;
                            buffer_index.nb++;
                        } else {
                            Write_Block_index(I,&buffer_index,adr); (*cout).write++;
                            adr++;
                            Alloc_block_index(I);(*cout).write++;(*cout).write++;(*cout).read++;
                            Read_Block_index(I,&buffer_index,adr);(*cout).read++;
                            buffer_index.Tab[0] = list_index[index];
                            j=1;
                            index++;
                            buffer_index.nb = 1;
                        }
                    }
                    Write_Block_index(I,&buffer_index,get_Header_index(I,"num_block"));(*cout).write++;

                    set_Header_index(I,"num_ins",nrec);
                    set_Header_lnof(F,"nrec",nrec);
                    break;
            }  
    }
}

void delete_document(fichier_lnof *F,fichier_tof_index *I,enreg_index *list_index,cost *cout){ //? in this i am not using ndel in the meantime (propably i will take rid of it) 
    int id,i,j,pos,nrec = get_Header_index(I,"num_ins"),adr,cnd;            //? it maybe needs some optimisation
    bool same,last;
    char choice[20];
    block_lof buffer;
    block_index buffer_index;
    enreg e;
    enreg_index temp;
    (*cout).write = 0;
    (*cout).read=0;

    do {
        printf("enter the id of the document you want to delete\n");
        scanf("%d",&id);

        if (search_list(list_index,nrec,id,&pos)){
            //?printf("id %d was found in position %d\n",id,pos);
            temp = list_index[pos];

            //* replace its value in the lnof file
            Read_Block_lnof(F,&buffer,get_Header_lnof(F,"Lastblk")); (*cout).read++;
            Read_Block_index(I,&buffer_index,get_Header_index(I,"num_block")); (*cout).read++;
            e = buffer.Tab[buffer.nb-1]; 
            buffer.nb--;
            buffer_index.nb--;
            same = (list_index[pos].key == e.Document_id); // if we are deleting the last elememt = maybe tsleh fl 2eme cas
            last = (same && buffer.nb ==0);

            Write_Block_index(I,&buffer_index,get_Header_index(I,"num_block")); (*cout).write++;
            Write_Block_lnof(F,&buffer,get_Header_lnof(F,"Lastblk"));(*cout).write++;
            if (buffer.nb == 0) { // the block will be deleted
                //update in both the lnof and the index file
                set_Header_index(I,"num_block",get_Header_index(I,"num_block")-1);
                set_Header_lnof(F,"Lastblk",get_Header_lnof(F,"Lastblk")-1);

                //? take in mind that it may delete the last block 
            }
            if (!same ){
            Read_Block_lnof(F,&buffer,list_index[pos].adr_block);(*cout).read++;
            //?printf("berfor %d \n",buffer.Tab[list_index[pos].position].Document_id);
            buffer.Tab[list_index[pos].position] = e;
            Write_Block_lnof(F,&buffer,list_index[pos].adr_block);(*cout).write++;
            //printf("after %d \n",buffer.Tab[list_index[pos].position].Document_id);
            //printf("\n1\n");
            }

            //? nrec--
            for (i=pos;i<nrec-1;i++){
                list_index[i] = list_index[i+1];
            }
            // update the list_index for the new value it will always find it
            if (!last && !same){
                if (search_list(list_index,nrec-1,e.Document_id,&j)){
                    //?printf("id %d was found in position %d\n",e.Document_id,j);
                } else { printf("ther was a problem");}

                //printf("before j=%d id=%d adr=%d pos=%d\n",j,list_index[j].key,list_index[j].adr_block,list_index[j].position);
                list_index[j].adr_block = temp.adr_block; //? temp => list_index[pos]
                list_index[j].position = temp.position;
                id=j;
                //?printf("after j=%d id=%d adr=%d pos=%d\n",j,list_index[j].key,list_index[j].adr_block,list_index[j].position);
            }
        
            set_Header_index(I,"num_ins",nrec-1);
            set_Header_lnof(F,"nrec",nrec-1);

            //update the index file
            adr = pos / b; 
            cnd = pos; //b * adr
            j = pos % b;

            //printf("j=%d id=%d adr=%d\n",j,id,adr);
            Read_Block_index(I,&buffer_index,adr);(*cout).read++;
                for (i=cnd;i<nrec-1;i++){ //? i = index
                            if (j < b) {
                                buffer_index.Tab[j] = list_index[pos];
                                if (pos == id) {
                                    //?printf("found j=%d id=%d adr=%d pos=%d\n",j,buffer_index.Tab[j].key,buffer_index.Tab[j].adr_block,buffer_index.Tab[j].position);
                                }
                                //printf("nb = %d\n",buffer_index.nb);
                                j++;
                                pos++;
                            } else {
                                Write_Block_index(I,&buffer_index,adr);(*cout).write++; 
                                adr++;
                                //printf("first adr = %d num_block=%d i=%d index= %d j= %d\n",adr,get_Header_index(I,"num_block"),i,pos,j);
                                Read_Block_index(I,&buffer_index,adr);(*cout).read++;
                                buffer_index.Tab[0] = list_index[pos];
                                j=1;
                                pos++;
                            }
                        }
                        Write_Block_index(I,&buffer_index,adr);(*cout).write++;
        } else {
            printf("the id was not found in the list_index\n");
        }

        printf("do you want to delete another document ?\n(yes or no)\n");
        scanf("%s",choice);
    } while (strcmp(choice,"yes") == 0);
}

void bulk_laod_revue_periodique(fichier_lnof *F,fichier_tof *T,fichier_tof_index *I,cost *cout){ //I is the index file of journal_magazine
    block_lof buff_lof;
    block_tof buff_tof;
    int i,j,k=0,link = get_Header_lnof(F,"Firstblk"),num=0,len=1024,i2;
    enreg_tof *list = (enreg_tof*)malloc(len * sizeof(enreg_tof));
    enreg_index *list_index ;
    block_index buffer_index;
    (*cout).write = 0;
    (*cout).read=0;

    Read_Block_lnof(F,&buff_lof,link);(*cout).read++;
    for ( i = 0; i <= get_Header_lnof(F,"Lastblk"); i++){ //Read_Block_lnof(F,&buff_lof,i); //if it doesnt work
        Read_Block_lnof(F,&buff_lof,i); (*cout).read++;
        for (j=0; j<buff_lof.nb; j++){
            if (strcmp(buff_lof.Tab[j].Type,"Revues et periodiques") == 0) {
                if (k >= len){
                    len++;
                    list = resize_list_tof(list,len);
                }
                list[k].Document_id = buff_lof.Tab[j].Document_id ; list[k].Pub_year = buff_lof.Tab[j].Pub_year; list[k].Available_qty = buff_lof.Tab[j].Available_qty;
                strcpy(list[k].Title, buff_lof.Tab[j].Title); strcpy(list[k].Author, buff_lof.Tab[j].Author); strcpy(list[k].Domaine, buff_lof.Tab[j].Domaine);
                k++;
                num++;
            }
        }
    }
     
    //*sorting the list
    sort_list_tof(list,num);
    list_index = (enreg_index*)malloc((num / b) * sizeof(enreg_index));
    i2=0;
    //* loading the file
    j=0;
    k=0;
    Read_block_tof(T,&buff_tof,0);(*cout).read++;
    buff_tof.nb=0;
    for(i=0;i<num;i++){
        if (k < b){
            buff_tof.Tab[k].Document_id = list[j].Document_id ; buff_tof.Tab[k].Pub_year = list[j].Pub_year; buff_tof.Tab[k].Available_qty = list[j].Available_qty;
            strcpy(buff_tof.Tab[k].Title, list[j].Title); strcpy(buff_tof.Tab[k].Author, list[j].Author); strcpy(buff_tof.Tab[k].Domaine, list[j].Domaine);
            buff_tof.nb++;
            k++;
            j++;     
        }else{
            list_index[i2].key = buff_tof.Tab[k].Document_id;
            list_index[i2].adr_block = get_Header_tof(T,"num_block");
            list_index[i2].position = buff_tof.nb-1;
            i2++;
            Write_block_tof(T,&buff_tof,get_Header_tof(T,"num_block")); (*cout).write++;
            Alloc_block_tof(T);(*cout).read++;(*cout).write++;(*cout).write++;
            Read_block_tof(T,&buff_tof,get_Header_tof(T,"num_block"));(*cout).read++;
            k=0;
            buff_tof.Tab[k].Document_id = list[j].Document_id ; buff_tof.Tab[k].Pub_year = list[j].Pub_year; buff_tof.Tab[k].Available_qty = list[j].Available_qty;
            strcpy(buff_tof.Tab[k].Title, list[j].Title); strcpy(buff_tof.Tab[k].Author, list[j].Author); strcpy(buff_tof.Tab[k].Domaine, list[j].Domaine);
            buff_tof.nb=1;
            k=1;
            j++;
        }
    }    
    list_index[i2].key = buff_tof.Tab[k].Document_id;
    list_index[i2].adr_block = get_Header_tof(T,"num_block");
    list_index[i2].position = buff_tof.nb-1;
    Write_block_tof(T,&buff_tof,get_Header_tof(T,"num_block"));(*cout).write++;
    set_Header_tof(T,"nrec",num);
    
    //* loading the index file from list_index
    j=0;
    buffer_index.nb =0;
        for (i=0;i<i2+1;i++){
            if (j<b){
                buffer_index.Tab[j] = list_index[i];
                buffer_index.nb++;
                j++;
            }else{
                Write_Block_index(I,&buffer_index,get_Header_index(I,"num_block"));(*cout).write++;
                Alloc_block_index(I);(*cout).read++; (*cout).write++;(*cout).write++;
                Read_Block_index(I,&buffer_index,get_Header_index(I,"num_block"));(*cout).read++;
                buffer_index.nb =1;
                buffer_index.Tab[0] = list_index[i];
                j=1;
            }
        }
    Write_Block_index(I,&buffer_index,get_Header_index(I,"num_block"));(*cout).write++;
    set_Header_index(I,"num_ins",i2+1);
    printf("index-journal terminated with success\n\n");

}

void Search_by_id_tof (fichier_tof *T,int key,bool *found,int *block,int *position,cost *cout){
    *found = false;
    (*cout).read=0;
    (*cout).write=0;
    if (key < 110000 || key > 990000){
        printf("\n\nkey out of range\n\n");

    } else {
        int m,inf_b = 0,sup_b = get_Header_tof(T,"num_block");
        block_tof buff;
        bool stop=false;

        while(!stop && (inf_b <= sup_b)){
            m = (inf_b + sup_b)/2;

            Read_block_tof(T,&buff,m);(*cout).read++;
            //printf("nb in block = %d is %d 1=%d and l=%d\n",m,buff.nb,buff.Tab[0].Document_id,buff.Tab[buff.nb - 1].Document_id);
            if (key >= buff.Tab[0].Document_id && key <= buff.Tab[buff.nb - 1].Document_id){
                Recherche_dicho_bufer_tof(buff,key,found,position);
                //printf(" bloc in tof_file=%d\n",m);
                *block = m;
                stop = true;
            } else {
                if (key < buff.Tab[0].Document_id){
                    sup_b = m-1;
                } else {
                    inf_b = m +1 ;
                }
            }
        }
    }
}

#endif