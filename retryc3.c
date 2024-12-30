#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define b 1024

typedef struct type_enreg {
    long int Document_id;
    char Title[71];
    char Author[31];
    char Type[30];
    char Domaine[82];
    int Pub_year;
    int Available_qty;
    bool Del;
} enreg;

typedef struct type_block_lnof {
    enreg Tab[b];
    int nb;
    int link;
} block_lof;

typedef struct type_hdr {
    int Firstblk;
    int Lastblk;
    int nrec;
    int ndel;
} header_lof ;

typedef struct lnof_file{
    FILE *f;
    header_lof h;
} fichier_lnof ;

void open_lnof(fichier_lnof **F,char filename[], char mode){ //F->f will be pointing after the header
    *F = (fichier_lnof *)malloc(sizeof (fichier_lnof));
    if (mode == 'E' || mode == 'e'){ // create a nwe lof file
        (*F)->f = fopen(filename,"rb+");
        if ((*F)->f == NULL) {
            printf("there was a problem opening the file");
            exit( EXIT_FAILURE );
        }
        fread(&((*F)->h),sizeof(header_lof),1,(*F)->f);
    }
    else { //mode New 
        (*F)->f = fopen(filename,"wb+");
        if ((*F)->f == NULL) {
            printf("there was a problem opening the file");
            exit( EXIT_FAILURE );
        }
        //initialize the header
        (*F)->h.Firstblk = 0;
        (*F)->h.Lastblk = 0;
        (*F)->h.nrec = 0;
        (*F)->h.ndel = 0;
        fwrite(&((*F)->h),sizeof(header_lof),1,(*F)->f);
        block_lof buffer;
        buffer.nb =0;
        buffer.link =-1;
        fwrite(&buffer,sizeof(block_lof),1,(*F)->f);
    }
    printf("--LnOF file opened succesfuly\n\n");
}

void close_lnof(fichier_lnof *F){
    rewind(F->f);
    fwrite(&F->h, sizeof(header_lof),1,F->f);
    fclose(F->f);
    free(F);
    printf("--LnOF file closed succesfuly\n");
}

int get_Header_lnof(fichier_lnof *F,char *field){
    if (strcmp(field, "Firstblk") == 0) return F->h.Firstblk;
    if (strcmp(field, "Lastblk") == 0) return F->h.Lastblk;
    if (strcmp(field, "nrec") == 0) return F->h.nrec;
    if (strcmp(field, "ndel") == 0) return F->h.ndel;
    fprintf(stderr, "getHeader : Unknown headerName: \"%s\"\n", field);
}

void set_Header_lnof(fichier_lnof *F,char *field,int val){
    if (strcmp(field, "Firstblk") == 0) {F->h.Firstblk = val ;return;}
    if (strcmp(field, "Lastblk") == 0) {F->h.Lastblk = val ; return;}
    if (strcmp(field, "nrec") == 0) {F->h.nrec = val ; return;}
    if (strcmp(field, "ndel") == 0) {F->h.ndel = val ; return;}
    fprintf(stderr, "setHeader : Unknown headerName: \"%s\"\n", field);
}

void Read_Block_lnof(fichier_lnof *F,block_lof *buffer,int i){
    if (i<0 || i>get_Header_lnof(F,"Lastblk") ){fprintf(stderr, "Read_block_lnof : number of block does not exist: \"%d\"\n", i);; return;}
    fseek(F->f,sizeof(header_lof) + i*sizeof(block_lof),SEEK_SET);
    fread(buffer,sizeof(block_lof),1,F->f);
}

void Write_Block_lnof(fichier_lnof *F,block_lof *buffer,int i){
    if (i<0 || i>get_Header_lnof(F,"Lastblk") ){fprintf(stderr, "Write_block_lnof : number of block does not exist: \"%d\"\n", i);; return;}
    fseek(F->f,sizeof(header_lof) + i*sizeof(block_lof),SEEK_SET);
    fwrite(buffer,sizeof(block_lof),1,F->f);
    //fflush(F->f);
}

void Alloc_block_lnof (fichier_lnof *F){
    block_lof buf;
    Read_Block_lnof(F,&buf,get_Header_lnof(F,"Lastblk"));
    buf.link = get_Header_lnof(F,"Lastblk") +1;
    Write_Block_lnof(F,&buf,get_Header_lnof(F,"Lastblk"));
    buf.nb =0;
    buf.link =-1;
    set_Header_lnof(F,"Lastblk",get_Header_lnof(F,"Lastblk")+1);
    Write_Block_lnof(F,&buf,get_Header_lnof(F,"Lastblk"));
} //? 1 Read   2 Writes

// la structure TOF (pour l'index)

typedef struct type_enreg_index{
    int key;
    int adr_block;
    int position;
    bool del;
}enreg_index;

typedef struct type_bloc_tof_index
{
    enreg_index Tab[b];
    int nb;
}block_index ;

typedef struct type_hdr_index{
    int num_block; //index of blocks
    int num_ins;
    int num_del;
} header_index;

typedef struct index_file {
    FILE *f;
    header_index h;
    char name[60];
} fichier_tof_index;

void open_index(fichier_tof_index **F,char *filename,char mode){
    *F = (fichier_tof_index *)malloc(sizeof (fichier_tof_index));
    if (mode == 'E' || mode == 'e'){ // create a nwe lof file
        (*F)->f = fopen(filename,"rb+");
        if ((*F)->f == NULL) {
            printf("there was a problem opening the file");
            exit( EXIT_FAILURE );
        }
        fread(&((*F)->h),sizeof(header_index),1,(*F)->f);
    }
    else { //mode New 
        (*F)->f = fopen(filename,"wb+");
        if ((*F)->f == NULL) {
            printf("there was a problem opening the file");
            exit( EXIT_FAILURE );
        }
        //initialize the header
        (*F)->h.num_block = 0;
        (*F)->h.num_ins = 0;
        (*F)->h.num_del = 0;
        fwrite(&((*F)->h),sizeof(header_index),1,(*F)->f);
        block_index buffer;
        buffer.nb =0;
        fwrite(&buffer,sizeof(block_index),1,(*F)->f); 
    }

    strcpy((*F)->name,filename);
    printf("\n--index file opened succesuly\n\n");
}

void close_index( fichier_tof_index *F )
{
   // saving header part in secondary memory (at the begining of the stream F->f)
   rewind(F->f);
   fwrite( &F->h, sizeof(header_index), 1, F->f );
   //fflush(F->f);
   fclose(F->f);
   printf("\n--index file closed succesfuly\n\n");  
   free( F );
}


int get_Header_index(fichier_tof_index *F,char *field){
    if (strcmp(field, "num_block") == 0) return F->h.num_block;
    if (strcmp(field, "num_ins") == 0) return F->h.num_ins;
    if (strcmp(field, "num_del") == 0) return F->h.num_del;
    fprintf(stderr, "getHeader_index : Unknown headerName: \"%s\"\n", field);
}

void set_Header_index(fichier_tof_index *F,char *field,int val){
    if (strcmp(field, "num_block") == 0) {F->h.num_block = val ;return;}
    if (strcmp(field, "num_ins") == 0) {F->h.num_ins = val ; return;}
    if (strcmp(field, "num_del") == 0) {F->h.num_del = val ; return;}
    fprintf(stderr, "setHeader_index : Unknown headerName: \"%s\"\n", field);
}

void Read_Block_index(fichier_tof_index *F,block_index *buffer,int i){
    if (i<0 || i>get_Header_index(F,"num_block") ){fprintf(stderr, "Read_block_index : number of block does not exist: \"%d\"\n", i);; return;}
    fseek(F->f,sizeof(header_index) + i*sizeof(block_index),SEEK_SET);
    fread(buffer,sizeof(block_index),1,F->f);
}

void Write_Block_index(fichier_tof_index *F,block_index *buffer,int i){
    if (i<0 || i>get_Header_index(F,"num_block") ){fprintf(stderr, "Read_block_index : number of block does not exist: \"%d\"\n", i);; return;}
    fseek(F->f,sizeof(header_index) + i*sizeof(block_index),SEEK_SET);
    fwrite(buffer,sizeof(block_index),1,F->f);
}

void Alloc_block_index (fichier_tof_index *F){
    block_index buf;
    buf.nb =0;
    set_Header_index(F,"num_block",get_Header_index(F,"num_block")+1);
    Write_Block_index(F,&buf,get_Header_index(F,"num_block"));
}

void Alloc_block_index_1024 (fichier_tof_index *F){
    block_index buf;
    buf.nb =1024;
    set_Header_index(F,"num_block",get_Header_index(F,"num_block")+1);
    Write_Block_index(F,&buf,get_Header_index(F,"num_block"));
}

//* tof structure

typedef struct type_enreg_tof {
    long int Document_id;
    char Title[71];
    char Author[31];
    char Domaine[82];
    int Pub_year;
    int Available_qty;
    bool Del;
} enreg_tof ;

typedef struct type_block_tof {
    enreg_tof Tab[b];
    int nb;
} block_tof;

typedef struct type_hdr_tof {
    int num_block;
    int nrec;
    int ndel;
} header_tof ;

typedef struct tof_file{
    FILE *f;
    header_tof h;
} fichier_tof ;

void open_tof(fichier_tof **F,char filename[], char mode){ //F->f will be pointing after the header
    *F = (fichier_tof *)malloc(sizeof (fichier_tof));
    if (mode == 'E' || mode == 'e'){ // create a nwe lof file
        (*F)->f = fopen(filename,"rb+");
        if ((*F)->f == NULL) {
            printf("there was a problem opening the file");
            exit( EXIT_FAILURE );
        }
        fread(&((*F)->h),sizeof(header_tof),1,(*F)->f);
    }
    else { //mode New 
        (*F)->f = fopen(filename,"wb+");
        if ((*F)->f == NULL) {
            printf("there was a problem opening the file");
            exit( EXIT_FAILURE );
        }
        //initialize the header
        (*F)->h.num_block = 0;
        (*F)->h.nrec = 0;
        (*F)->h.ndel = 0;
        fwrite(&((*F)->h),sizeof(header_tof),1,(*F)->f);
        block_tof buffer;
        buffer.nb =0;
        fwrite(&buffer,sizeof(block_tof),1,(*F)->f);
    }
    printf("--tOF file opened succesfuly\n\n");
}

void close_tof(fichier_tof *F){
    rewind(F->f);
    fwrite(&F->h, sizeof(header_tof),1,F->f);
    fclose(F->f);
    free(F);
    printf("--tOF file closed succesfuly\n");
}

int get_Header_tof(fichier_tof *F,char *field){
    if (strcmp(field, "num_block") == 0) return F->h.num_block;
    if (strcmp(field, "nrec") == 0) return F->h.nrec;
    if (strcmp(field, "ndel") == 0) return F->h.ndel;
    fprintf(stderr, "getHeader : Unknown headerName: \"%s\"\n", field);
}

void set_Header_tof(fichier_tof *F,char *field,int val){
    if (strcmp(field, "num_block") == 0) {F->h.num_block = val ;return;}
    if (strcmp(field, "nrec") == 0) {F->h.nrec = val ; return;}
    if (strcmp(field, "ndel") == 0) {F->h.ndel = val ; return;}
    fprintf(stderr, "setHeader : Unknown headerName: \"%s\"\n", field);
}

void Read_block_tof(fichier_tof *F,block_tof *buffer,int i){
    if (i<0 || i>get_Header_tof(F,"num_block") ){fprintf(stderr, "Read_block_tof : number of block does not exist: \"%d\"\n", i);; return;}
    fseek(F->f,sizeof(header_tof) + i*sizeof(block_tof),SEEK_SET);
    fread(buffer,sizeof(block_tof),1,F->f);
}

void Write_block_tof(fichier_tof *F,block_tof *buffer,int i){
    if (i<0 || i>get_Header_tof(F,"num_block") ){fprintf(stderr, "Write_block_tof : number of block does not exist: \"%d\"\n", i);; return;}
    fseek(F->f,sizeof(header_tof) + i*sizeof(block_tof),SEEK_SET);
    fwrite(buffer,sizeof(block_tof),1,F->f);
    //fflush(F->f);
}

void Alloc_block_tof (fichier_tof *F){
    block_tof buf;
    buf.nb =0;
    set_Header_tof(F,"num_block",get_Header_tof(F,"num_block")+1);
    Write_block_tof(F,&buf,get_Header_tof(F,"num_block"));
}

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

void generate_random_title(char titre[71]){
    int i,taille_chaine = 50 + rand() % 21;
    for (i=0;i<taille_chaine;i++){
        titre[i] = 65 + rand()%26;
    }
    //printf("%d\n",taille_chaine);
    titre[i] = '\0';
}

void generate_random_author(char author[31]){
    int i,taille_chaine = 4 + rand() % 27;
    for (i=0;i<taille_chaine;i++){
        author[i] = 65 + rand()%26;
    }
    //printf("%d\n",taille_chaine);
    author[i] = '\0';
} 

char *generate_random_type(){
    char *type[7] = {
        "","Ouvrages et manuels","Revues et periodiques",
        "Actes de conferences","Rapports de recherche",
        "Memoires et theses","Polycopies et support de cours"
    } ;
    int num_type = 1 + rand() % (6);
    return type[num_type];
}

void generate_type(char *typ){
    char *type[7] = {
        "","Ouvrages et manuels","Revues et periodiques",
        "Actes de conferences","Rapports de recherche",
        "Memoires et theses","Polycopies et support de cours"
    } ;
    int num_type;
    printf("1. Ouvrages et manuels\n2. Revues et periodiques\n3. Actes de conferences\n4. Rapports de recherche\n5. Memoires et theses\n6. Polycopies et support de cours\n");
    printf("eneter the number of the type\n");
    scanf("%d",&num_type);
    while (num_type > 6 || num_type < 1){
        printf("oops number out of range please try again\n");
        scanf("%d",&num_type);
    }
    strcpy(typ,type[num_type]);
}

void generate_domaine(char *typ){
    char *domaine[13] = {
        "","Algorithmes et structures de donnees","Intelligence artificielle et apprentissage automatique","Systemes d'exploitation",
        "Cybersecurite","Bases de donnees et gestion des donnees","Reseaux et telecommunications",
        "Genie logiciel, programmation et developpement logiciel","Informatique graphique, multimedia,vision par ordinateur et traitement d'images",
        "Science des donnees et statistiques","Robotique et systemes embarques","Blockchain et technologies distribuees","Calcul haute performance et informatique quantique"
    } ;
    int num_type; 
    printf("1. Algorithmes et structures de donnees\n2. Intelligence artificielle et apprentissage automatique\n3. Systemes d'exploitation\n4. Cybersecurite\n5. Bases de donnees et gestion des donnees\n6. Reseaux et telecommunications\n7. Genie logiciel, programmation et developpement logiciel\n");
    printf("8. Informatique graphique, multimedia,vision par ordinateur et traitement d'images\n9. Science des donnees et statistiques\n10. Robotique et systemes embarques\n11. Blockchain et technologies distribuees\n12. Calcul haute performance et informatique quantique\n");
    printf("eneter the number of the type\n");
    scanf("%d",&num_type);
    while (num_type > 12 || num_type < 1)
    {
        printf("oops number out of range please try again\n");
        scanf("%d",&num_type);
    }
    
    strcpy(typ,domaine[num_type]);
}

char *generate_random_domaine(){
    char *domaine[13] = {
        "","Algorithmes et structures de donnees","Intelligence artificielle et apprentissage automatique","Systemes d'exploitation",
        "Cybersecurite","Bases de donnees et gestion des donnees","Reseaux et telecommunications",
        "Genie logiciel, programmation et developpement logiciel","Informatique graphique, multimedia,vision par ordinateur et traitement d'images",
        "Science des donnees et statistiques","Robotique et systemes embarques","Blockchain et technologies distribuees","Calcul haute performance et informatique quantique"
    } ;
    int num_type = 1 + rand() % (12);
    return domaine[num_type];
}

int generate_random_year(){
    return 1970 +  rand() % (55);
}

int generate_random_quantite(){
    return 1 +  rand() % (10);
}

void tocpy(){
    block_lof buffer;
    printf("%d %s, %s, %s, %s, %d %d \n\n",buffer.Tab[0].Document_id, buffer.Tab[0].Title, buffer.Tab[0].Author, buffer.Tab[0].Type, buffer.Tab[0].Domaine, buffer.Tab[0].Pub_year, buffer.Tab[0].Available_qty);
    /*Search_by_id(I,N,&found,&i,&j);
        if (found){
            Read_Block_lnof(F,&buffer,i);
            printf(" it was found in block %d position %d and value %d\n",i,j,buffer.Tab[j].Document_id);
        } else{
            printf("it was not found\n");
        }*/
}

int generate_random_id(int *list){
    int n,i;
    do {
        n = 110000 + (rand() * rand()) % (880001);
        i = n - 110000;
    }while(list[i] == 1);
    list[i] = 1;
    return n;
}

void fill_buffer(block_lof *buffer,int j,int *list,int *id){
    char titre[71];
    char author[31];
    generate_random_title(titre);
    generate_random_author(author);
    *id = generate_random_id(list); //to get the id of the book to make an index from it
    (*buffer).Tab[j].Document_id = *id;
    strcpy( (*buffer).Tab[j].Title,titre);
    strcpy( (*buffer).Tab[j].Author,author);
    strcpy( (*buffer).Tab[j].Type,generate_random_type());
    strcpy( (*buffer).Tab[j].Domaine,generate_random_domaine());
    (*buffer).Tab[j].Pub_year = generate_random_year();
    (*buffer).Tab[j].Available_qty = generate_random_quantite();
}

int min(int x,int y){
    if(x<y) return x;
    return y;
}
int max(int x,int y){
    if(x>y) return x;
    return y;
}

void sort_list(enreg_index *list_index,int len){
    int i,j;
    bool swap;
    enreg_index temp;

    for(i=0;i<len-1;i++){
    swap = false;
        for (j=0;j<len-i-1;j++){
                    
            if (list_index[j].key > list_index[j+1].key){
                temp = list_index[j];
                list_index[j] = list_index[j+1];
                list_index[j+1] = temp;
                        swap = true;
            }

        }
        if (!swap){
            break;
        }
    }
}

void sort_list_tof(enreg_tof *list,int len){
    int i,j;
    bool swap;
    enreg_tof temp;

    for(i=0;i<len-1;i++){
    swap = false;
        for (j=0;j<len-i-1;j++){
                    
            if (list[j].Document_id > list[j+1].Document_id){
                temp = list[j];
                list[j] = list[j+1];
                list[j+1] = temp;
                swap = true;
            }

        }
        if (!swap){
            break;
        }
    }
}

void bulk_load_lof(fichier_lnof *F,fichier_tof_index *I,int N){
    block_lof buffer;
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
                Write_Block_lnof(F,&buffer,get_Header_lnof(F,"Lastblk"));
                Alloc_block_lnof(F);
                Read_Block_lnof(F,&buffer,get_Header_lnof(F,"Lastblk"));
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
        Write_Block_lnof(F,&buffer,get_Header_lnof(F,"Lastblk"));
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
                Write_Block_index(I,&buffer_index,get_Header_index(I,"num_block"));
                Alloc_block_index(I);
                Read_Block_index(I,&buffer_index,get_Header_index(I,"num_block"));
                buffer_index.nb =1;
                buffer_index.Tab[0] = list_index[i];
                j=1;
            }
        }
    Write_Block_index(I,&buffer_index,get_Header_index(I,"num_block"));
    set_Header_index(I,"num_ins",N);
    printf("index terminated with success\n\n");
    }
}

void Recherche_dicho_bufer(block_index buff,int key,bool *found,int *block,int *position){
    int m,inf = 0,sup = buff.nb;
    *found = false;

    while(!(*found )&& (inf <= sup)){
        m = (inf + sup) / 2;
        if ( buff.Tab[m].key == key ){
            *found = true;
            *block = buff.Tab[m].adr_block;
            *position = buff.Tab[m].position;
            printf("key = %d  %d %d position in list_inedx = %d",key,*block,*position,m);
        } else {
            if (buff.Tab[m].key > key){
                sup = m - 1 ;
            } else {
                inf = m + 1;
            }
        }
    }
}

void Recherche_dicho_list(enreg_index *list_index,int len,int key,bool *found,int *block,int *position){
    int m,inf = 0,sup =len;
    *found = false;

    while(!(*found )&& (inf <= sup)){
        m = (inf + sup) / 2;
        if ( list_index[m].key == key ){
            *found = true;
            *block = list_index[m].adr_block;
            *position = list_index[m].position;
        } else {
            if (list_index[m].key > key){
                sup = m - 1 ;
            } else {
                inf = m + 1;
            }
        }
    }
}

bool search_list(enreg_index *list_index,int len,int key,int *position){
    int m,inf = 1,sup =len;
    bool found=false;

    while((!found) && (inf <= sup)){
        m = (inf + sup) / 2;
        //printf("inf = %d sup = %d m =%d\n",inf,sup,m);
        if ( list_index[m].key == key ){
            //printf("%d was found\n",key);
            found = true;
            *position = m;
        } else {
            if (list_index[m].key > key){
                sup = m - 1 ;
            } else {
                inf = m + 1;
            }
        }
    }
    return found;
}

void Search_by_id (fichier_tof_index *I,int key,bool *found,int *block,int *position){
    *found = false;
    if (key < 110000 || key > 990000){
        printf("\n\nkey out of range\n\n");

    } else {
        int m,inf_b = 0,sup_b = get_Header_index(I,"num_block");
        block_index buff;
        bool stop=false;

        while(!stop && (inf_b <= sup_b)){
            m = (inf_b + sup_b)/2;

            Read_Block_index(I,&buff,m);
            printf("nb in block index= %d is %d 1=%d and l=%d\n",m,buff.nb,buff.Tab[0].key,buff.Tab[buff.nb - 1].key);
            if (key >= buff.Tab[0].key && key <= buff.Tab[buff.nb - 1].key){
                Recherche_dicho_bufer(buff,key,found,block,position);//989206  989206  971391
                printf(" bloc in index_file=%d\n",m);
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

void read_changes(enreg *e){
    char field[50];

    int choice;
        printf("which field you want to change ?\nto stop type exit\n");
        scanf("%s",field);

        while (strcmp(field,"exit") != 0){
            
            if ((strcmp(field,"Title") == 0) || (strcmp(field,"title") == 0)){
                printf("\neneter the new value for %s\ndo you want to enter it: 1.manually or 2.generate randomly\nenter 1 or 2 to chose\n",field);
                scanf("%d",&choice);
                
                switch (choice){
                case 1:
                    scanf("%s",&(*e).Title);
                    while (strlen((*e).Title) < 50 || strlen((*e).Title) > 70 ){
                        printf("the length of the new title is out of range, please try again\n");
                        scanf("%s",&(*e).Title);
                    }
                    break;
                case 2:
                    generate_random_title((*e).Title);
                    printf("the title generated is %s\n",(*e).Title);
                    break;
                
                default:
                    break;
                }
                
            }

            if ((strcmp(field,"Author") == 0) || (strcmp(field,"author") == 0)){
                printf("\neneter the new value for %s\ndo you want to enter it: 1.manually or 2.generate randomly\nenter 1 or 2 to chose\n",field);
                scanf("%d",&choice);
                
                switch (choice){
                case 1:
                    scanf("%s",&(*e).Author);
                    while (strlen((*e).Author) < 4 || strlen((*e).Author) > 30 ){
                        printf("the length of the new %s is out of range, please try again\n",field);
                        scanf("%s",&(*e).Author);
                    }
                    break;
                case 2:
                    generate_random_author((*e).Author);
                    printf("the %s generated is %s\n",field,(*e).Author);
                    break;
                
                default:
                    break;
                }
                
            }

            if ((strcmp(field,"Type") == 0) || (strcmp(field,"type") == 0)){
                printf("eneter the new value for %s\n",field);
                generate_type((*e).Type);
            }

            if ((strcmp(field,"Domaine") == 0) || (strcmp(field,"domaine") == 0)){
                printf("eneter the new value for %s\n",field);
                generate_domaine((*e).Domaine);
                
            }

            if ((strcmp(field,"Pub_year") == 0) || (strcmp(field,"pub_year") == 0)){
                printf("eneter the new value for %s\n",field);
                scanf("%d",&(*e).Pub_year);

                while ((*e).Pub_year > 2024 || (*e).Pub_year < 1970)
                {
                    printf("%s must by between 1970 and 2024\n",field);
                    scanf("%d",&(*e).Pub_year);
                }
                
                
            }

            if ((strcmp(field,"Available_qty") == 0) || (strcmp(field,"available_qty") == 0)){
                printf("eneter the new value for %s\n",field);
                scanf("%d",&(*e).Available_qty);

                while ((*e).Available_qty > 10 || (*e).Available_qty < 0)
                {
                    printf("%s msut be postive and less than 10\n",field);
                    scanf("%d",&(*e).Available_qty);
                }
                
            }

            printf("\nwhich field you want to also modify?\nto finsh type exit\n");
            scanf("%s",field);
        }
    
}

void modify(fichier_lnof *F,fichier_tof_index *I){
    block_index buffer_index;
    block_lof buffer;
    enreg e;

    bool found;
    int key,block,position;

    printf("\ngive the id of the record you want to modify\n");
    scanf("%d",&key);

    Search_by_id(I,key,&found,&block,&position);
    if (found){
        //the prints are just to see the changes
        Read_Block_lnof(F,&buffer,block);
        printf("the %d is in block %d position %d\n",buffer.Tab[position].Document_id,block,position);
        e = buffer.Tab[position];
        printf("before | %d %s, %s, %s, %s, %d %d \n\n",buffer.Tab[position].Document_id, buffer.Tab[position].Title, buffer.Tab[position].Author, buffer.Tab[position].Type, buffer.Tab[position].Domaine, buffer.Tab[position].Pub_year, buffer.Tab[position].Available_qty);
        read_changes(&e);
        buffer.Tab[position] = e;
        printf("after | %d %s, %s, %s, %s, %d %d \n\n",buffer.Tab[position].Document_id, buffer.Tab[position].Title, buffer.Tab[position].Author, buffer.Tab[position].Type, buffer.Tab[position].Domaine, buffer.Tab[position].Pub_year, buffer.Tab[position].Available_qty);
        Write_Block_lnof(F,&buffer,block);
    } else{
        printf("\nThe record was not found\n");
    }
} 

void load_index(fichier_tof_index *I,enreg_index *list){
    int i,k=0,j,num = get_Header_index(I,"num_block");
    block_index buff;
   
    for (i=0;i<=num;i++){
        Read_Block_index(I,&buff,i);
        printf("\nnum = %d nb= %d\n",i,buff.nb);
        for (j=0;j<buff.nb;j++){
            list[k] = buff.Tab[j];
            k++;
        }
    }
}

enreg_index *resize_list_index (enreg_index *list,int len){
    enreg_index *newlist = realloc(list,len * sizeof(enreg_index));
    
    if( newlist == NULL) {
        fprintf(stderr, "ERROE, Memory allocation failed in resize_list_index\"%s\"\n");
        return NULL;
    }

    return newlist;
}

enreg_tof *resize_list_tof (enreg_tof *list,int len){
    enreg_tof *newlist = realloc(list,len * sizeof(enreg_tof));
    
    if( newlist == NULL) {
        fprintf(stderr, "ERROE, Memory allocation failed in resize_list_tof\"\"\n");
        return NULL;
    }

    return newlist;
}

void fill_document_manually(enreg *e,enreg_index *list_index,int len){
    int choice,id,i,j;
    bool found;

            //* generating new id 
            do {
                id = 110000 + (rand() * rand()) % (880001);
                Recherche_dicho_list(list_index,len,id,&found,&i,&j); //? maybe we can do search_list
            } while (found);
            (*e).Document_id = id;
            printf("the id generated for the new document is :%d\n\n",id);
            
            //* title
            printf("\neneter the new value for the title\ndo you want to enter it: 1.manually or 2.generate randomly\nenter 1 or 2 to chose\n");
            scanf("%d",&choice); 
            switch (choice){
            case 1:
                scanf("%s",&(*e).Title);
                while (strlen((*e).Title) < 50 || strlen((*e).Title) > 70 ){
                        printf("the length of the new title is out of range, please try again\n");
                        scanf("%s",&(*e).Title);
                }
                break;
            case 2:
                generate_random_title((*e).Title);
                printf("the title generated is %s\n",(*e).Title);
                break;
                
            default:
                break;
            }
                
            //* author 
            printf("\neneter the new value for the author\ndo you want to enter it: 1.manually or 2.generate randomly\nenter 1 or 2 to chose\n");
            scanf("%d",&choice); 
            switch (choice){
                case 1:
                    scanf("%s",&(*e).Author);
                    while (strlen((*e).Author) < 4 || strlen((*e).Author) > 30 ){
                        printf("the length of the new author is out of range, please try again\n");
                        scanf("%s",&(*e).Author);
                    }
                    break;
                case 2:
                    generate_random_author((*e).Author);
                    printf("the author generated is %s\n\n",(*e).Author);
                    break;
                
                default:
                    break;
            }
                
            //* type
                generate_type((*e).Type);


            //* domaine
                generate_domaine((*e).Domaine);

            //* pub year
                printf("\neneter the new value for pub year\n");
                scanf("%d",&(*e).Pub_year);
                while ((*e).Pub_year > 2024 || (*e).Pub_year < 1970)
                {
                    printf("pub year must by between 1970 and 2024\n");
                    scanf("%d",&(*e).Pub_year);
                }
                

            //* qty
                printf("eneter the new value for available quantity\n");
                scanf("%d",&(*e).Available_qty);
                while ((*e).Available_qty > 10 || (*e).Available_qty < 0)
                {
                    printf("the available quantity msut be postive and less than 10\n");
                    scanf("%d",&(*e).Available_qty);
                }               
}
    

void fill_document_random(enreg *e,enreg_index *list_index,int len){
    char titre[71];
    char author[31];
    int id,i,j;
    bool found;

    generate_random_title(titre);
    generate_random_author(author);
    strcpy( (*e).Title,titre);
    strcpy( (*e).Author,author);
    do {
        id = 110000 + (rand() * rand()) % (880001);
        Recherche_dicho_list(list_index,len,id,&found,&i,&j);
    } while (found);
    (*e).Document_id = id;
    strcpy( (*e).Type,generate_random_type());
    strcpy( (*e).Domaine,generate_random_domaine());
    (*e).Pub_year = generate_random_year();
    (*e).Available_qty = generate_random_quantite();
}

void Read_record(enreg *e,enreg_index *list_index,int len) {
    int num,cpt=0,choice;

    printf("how many documents you want to add ?\n");
    scanf("%d",&num);

    while (num > 0 && choice != 0){
        printf("do you want to generate it:\n1. manually\n2. automatic\n0. to exit\n");
        do{
            scanf("%d",&choice);
        } while (choice != 1 || choice != 2 || choice !=0);
        switch (choice){
            case 1:

                cpt++;
                break;
            
            case 2:
                fill_document_random(&e[cpt],list_index,len);
                cpt++;
                break;
            
            default :
                break;
        }

    }   
}

void Add(fichier_lnof *F,fichier_tof_index *I,enreg_index *list_index) {
    int num,choice,i,cpt=0,nrec=get_Header_lnof(F,"nrec"),adr = get_Header_lnof(F,"Lastblk"),min=990001,index,j,cnd,q,r;
    char respond[15];
    enreg document;
    block_lof buffer;
    block_index buffer_index;

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
            Read_Block_lnof(F,&buffer,get_Header_lnof(F,"Lastblk"));

            switch (choice){
                case 1: //manually
                    //* filling the lnof file
                    for (i=0;i<num;i++){
                        printf("\nfilling the infos of document number %d\n----------------------------------\n",i+1);
                        fill_document_manually(&document,list_index,nrec); //? we will change this 
                        if ( buffer.nb < b){
                            buffer.Tab[buffer.nb] = document;
                            buffer.nb++;
                        } else {
                            Write_Block_lnof(F,&buffer,get_Header_lnof(F,"Lastblk"));
                            Alloc_block_lnof(F);
                            adr++;
                            Read_Block_lnof(F,&buffer,get_Header_lnof(F,"Lastblk"));
                            buffer.nb = 1;
                            buffer.Tab[0] = document;
                        }
                        Write_Block_lnof(F,&buffer,get_Header_lnof(F,"Lastblk"));

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

                    Read_Block_index(I,&buffer_index,adr);
                    for (i=cnd;i<get_Header_index(I,"num_ins");i++){ 
                        if (j < b) {
                            buffer_index.Tab[j] = list_index[index];
                            //printf("nb = %d\n",buffer_index.nb);
                            j++;
                            index++;
                        } else {
                            Write_Block_index(I,&buffer_index,adr); 
                            adr++;
                            //printf("first adr = %d num_block=%d i=%d index= %d j= %d\n",adr,get_Header_index(I,"num_block"),i,index,j);
                            Read_Block_index(I,&buffer_index,adr);
                            buffer_index.Tab[0] = list_index[index];
                            j=1;
                            index++;
                        }
                    }
                    Write_Block_index(I,&buffer_index,adr);

                    cnd = i;
                    for (i=cnd;i<nrec;i++){ //adding the new values
                        if (j < b) {
                            buffer_index.Tab[j] = list_index[index];
                            //printf("nb = %d\n",buffer_index.nb);
                            index++;
                            j++;
                            buffer_index.nb++;
                        } else {
                            Write_Block_index(I,&buffer_index,adr); 
                            adr++;
                            Alloc_block_index(I);
                            Read_Block_index(I,&buffer_index,adr);
                            buffer_index.Tab[0] = list_index[index];
                            j=1;
                            index++;
                            buffer_index.nb = 1;
                        }
                    }
                    Write_Block_index(I,&buffer_index,get_Header_index(I,"num_block"));

                    set_Header_index(I,"num_ins",nrec);
                    set_Header_lnof(F,"nrec",nrec);
                    break;

                case 2: //autimatticlly

                    //* filling the lnof file
                    for (i=0;i<num;i++){
                        fill_document_random(&document,list_index,nrec); //? we will change this 
                        if ( buffer.nb < b){
                            buffer.Tab[buffer.nb] = document;
                            buffer.nb++;
                        } else {
                            Write_Block_lnof(F,&buffer,get_Header_lnof(F,"Lastblk"));
                            Alloc_block_lnof(F);
                            adr++;
                            Read_Block_lnof(F,&buffer,get_Header_lnof(F,"Lastblk"));
                            buffer.nb = 1;
                            buffer.Tab[0] = document;
                        }
                        Write_Block_lnof(F,&buffer,get_Header_lnof(F,"Lastblk"));

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
            
                    //* if the search had a problem
                    if (! search_list(list_index,nrec,min,&index)){
                        printf("%d was not found in the list\n",min); //* to check if the search didnt have problems
                    } 
                    adr = index / b; 
                    cnd = index; //b * adr
                    j = index % b;

                    Read_Block_index(I,&buffer_index,adr);
                    for (i=cnd;i<get_Header_index(I,"num_ins");i++){ //? i = index
                        if (j < b) {
                            buffer_index.Tab[j] = list_index[index];
                            //printf("nb = %d\n",buffer_index.nb);
                            j++;
                            index++;
                        } else {
                            Write_Block_index(I,&buffer_index,adr); 
                            adr++;
                            //printf("first adr = %d num_block=%d i=%d index= %d j= %d\n",adr,get_Header_index(I,"num_block"),i,index,j);
                            Read_Block_index(I,&buffer_index,adr);
                            buffer_index.Tab[0] = list_index[index];
                            j=1;
                            index++;
                        }
                    }
                    Write_Block_index(I,&buffer_index,adr);

                    cnd = i;
                    for (i=cnd;i<nrec;i++){ //adding the new values
                        if (j < b) {
                            buffer_index.Tab[j] = list_index[index];
                            //printf("nb = %d\n",buffer_index.nb);
                            index++;
                            j++;
                            buffer_index.nb++;
                        } else {
                            Write_Block_index(I,&buffer_index,adr); 
                            adr++;
                            Alloc_block_index(I);
                            Read_Block_index(I,&buffer_index,adr);
                            buffer_index.Tab[0] = list_index[index];
                            j=1;
                            index++;
                            buffer_index.nb = 1;
                        }
                    }
                    Write_Block_index(I,&buffer_index,get_Header_index(I,"num_block"));

                    set_Header_index(I,"num_ins",nrec);
                    set_Header_lnof(F,"nrec",nrec);
                    break;
            }  
    }
}

void delete(fichier_lnof *F,fichier_tof_index *I,enreg_index *list_index){ //? in this i am not using ndel in the meantime (propably i will take rid of it) 
    int id,i,j,pos,nrec = get_Header_index(I,"num_ins"),adr,cnd;            //? it maybe needs some optimisation
    bool same,last;
    block_lof buffer;
    block_index buffer_index;
    enreg e;
    enreg_index temp;

    printf("enter the id of the document you want to delete\n");
    scanf("%d",&id);

    if (search_list(list_index,nrec,id,&pos)){
        printf("id %d was found in position %d\n",id,pos);
        temp = list_index[pos];

        //* replace its value in the lnof file
        Read_Block_lnof(F,&buffer,get_Header_lnof(F,"Lastblk"));
        Read_Block_index(I,&buffer_index,get_Header_index(I,"num_block"));
        e = buffer.Tab[buffer.nb-1]; 
        buffer.nb--;
        buffer_index.nb--;
        same = (list_index[pos].key == e.Document_id); // if we are deleting the last elememt = maybe tsleh fl 2eme cas
        last = (same && buffer.nb ==0);

        Write_Block_index(I,&buffer_index,get_Header_index(I,"num_block"));
        Write_Block_lnof(F,&buffer,get_Header_lnof(F,"Lastblk"));
        if (buffer.nb == 0) { // the block will be deleted
            //update in both the lnof and the index file
            set_Header_index(I,"num_block",get_Header_index(I,"num_block")-1);
            set_Header_lnof(F,"Lastblk",get_Header_lnof(F,"Lastblk")-1);

            //? take in mind that it may delete the last block 
        }
        if (!same ){
        Read_Block_lnof(F,&buffer,list_index[pos].adr_block);
        //printf("berfor %d \n",buffer.Tab[list_index[pos].position].Document_id);
        buffer.Tab[list_index[pos].position] = e;
        Write_Block_lnof(F,&buffer,list_index[pos].adr_block);
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
                printf("id %d was found in position %d\n",e.Document_id,j);
            } else { printf("ther was a problem");}

            printf("before j=%d id=%d adr=%d pos=%d\n",j,list_index[j].key,list_index[j].adr_block,list_index[j].position);
            list_index[j].adr_block = temp.adr_block; //? temp => list_index[pos]
            list_index[j].position = temp.position;
            id=j;
            printf("after j=%d id=%d adr=%d pos=%d\n",j,list_index[j].key,list_index[j].adr_block,list_index[j].position);
        }
    
        set_Header_index(I,"num_ins",nrec-1);
        set_Header_lnof(F,"nrec",nrec-1);

        //update the index file
        adr = pos / b; 
        cnd = pos; //b * adr
        j = pos % b;

        printf("j=%d id=%d adr=%d\n",j,id,adr);
        Read_Block_index(I,&buffer_index,adr);
            for (i=cnd;i<nrec-1;i++){ //? i = index
                        if (j < b) {
                            buffer_index.Tab[j] = list_index[pos];
                            if (pos == id) {
                                printf("found j=%d id=%d adr=%d pos=%d\n",j,buffer_index.Tab[j].key,buffer_index.Tab[j].adr_block,buffer_index.Tab[j].position);
                            }
                            //printf("nb = %d\n",buffer_index.nb);
                            j++;
                            pos++;
                        } else {
                            Write_Block_index(I,&buffer_index,adr); 
                            adr++;
                            //printf("first adr = %d num_block=%d i=%d index= %d j= %d\n",adr,get_Header_index(I,"num_block"),i,pos,j);
                            Read_Block_index(I,&buffer_index,adr);
                            buffer_index.Tab[0] = list_index[pos];
                            j=1;
                            pos++;
                        }
                    }
                    Write_Block_index(I,&buffer_index,adr);
    } else {
        printf("the id was not found in the list_index\n");
    }
}

void bulk_laod_revue_periodique(fichier_lnof *F,fichier_tof *T){
    block_lof buff_lof;
    block_tof buff_tof;
    int i,j,k=0,link = get_Header_lnof(F,"Firstblk"),num=0,len=1024;
    enreg_tof *list = (enreg_tof*)malloc(len * sizeof(enreg_tof));

    Read_Block_lnof(F,&buff_lof,link);
    for ( i = 0; i <= get_Header_lnof(F,"Lastblk"); i++){ //Read_Block_lnof(F,&buff_lof,i); //if it doesnt work
        Read_Block_lnof(F,&buff_lof,i); 
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

    //* loading the file
    j=0;
    k=0;
    Read_block_tof(T,&buff_tof,0);
    buff_tof.nb=0;
    for(i=0;i<num;i++){
        if (k < b){
            buff_tof.Tab[k].Document_id = list[j].Document_id ; buff_tof.Tab[k].Pub_year = list[j].Pub_year; buff_tof.Tab[k].Available_qty = list[j].Available_qty;
            strcpy(buff_tof.Tab[k].Title, list[j].Title); strcpy(buff_tof.Tab[k].Author, list[j].Author); strcpy(buff_tof.Tab[k].Domaine, list[j].Domaine);
            buff_tof.nb++;
            k++;
            j++;     
        }else{
            Write_block_tof(T,&buff_tof,get_Header_tof(T,"num_block"));
            Alloc_block_tof(T);
            Read_block_tof(T,&buff_tof,get_Header_tof(T,"num_block"));
            k=0;
            buff_tof.Tab[k].Document_id = list[j].Document_id ; buff_tof.Tab[k].Pub_year = list[j].Pub_year; buff_tof.Tab[k].Available_qty = list[j].Available_qty;
            strcpy(buff_tof.Tab[k].Title, list[j].Title); strcpy(buff_tof.Tab[k].Author, list[j].Author); strcpy(buff_tof.Tab[k].Domaine, list[j].Domaine);
            buff_tof.nb++;
            k=1;
            j++;
        }
    }    
    Write_block_tof(T,&buff_tof,get_Header_tof(T,"num_block"));
    set_Header_tof(T,"nrec",num);
    
}

int main(){
    srand(time(NULL));
    fichier_lnof *F;
    block_lof buffer;
    bool found;
    buffer.link = -1;
    buffer.nb = 0;
    int i,j=0,N,r,*list,cpt = 1;
    fichier_tof_index *I;
    fichier_tof *T;
    enreg_index *list_index = (enreg_index*)malloc(sizeof(enreg_index));
    enreg e;
    block_index buffer_index;
    block_tof buffer_tof;

    //* writing the blocks

    open_lnof(&F,"test2",'e');
    
    if (F->f != NULL ){
        //* creating the index 
        open_index(&I,"index_test2",'e');
        open_tof(&T,"journal_magazine",'n');
        //bulk_load_lof(F,I,5000);

        list_index = resize_list_index(list_index,get_Header_index(I,"num_ins")); 
        if (list_index == NULL){
            printf("ERROR: Memory allocation failed for list_index in main\n");
            return 0;
        }
        load_index(I,list_index);

        printf("before: num_block in the file= %d nrec= %d ndel= %d\n\n",get_Header_tof(T,"num_block"),get_Header_tof(T,"nrec"),get_Header_tof(T,"ndel"));
        bulk_laod_revue_periodique(F,T);
        printf("after: num_block in the file= %d nrec= %d ndel= %d\n\n",get_Header_tof(T,"num_block"),get_Header_tof(T,"nrec"),get_Header_tof(T,"ndel"));
        
        Read_block_tof(T,&buffer_tof,0);
        r = rand() % (buffer_tof.nb + 1);
        printf("\nr=%d nb=%d\n",r,buffer_tof.nb);
        for (i=r;i<r+10;i++){
                printf("%d %s, %s, %s, %d %d \n\n",buffer_tof.Tab[i].Document_id, buffer_tof.Tab[i].Title, buffer_tof.Tab[i].Author, buffer_tof.Tab[i].Domaine, buffer_tof.Tab[i].Pub_year, buffer_tof.Tab[0].Available_qty);
        }

        close_index(I); 
        close_tof(T);   
        close_lnof(F);
    }
}