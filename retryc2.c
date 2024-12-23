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
    printf("\n--index file opened succesuly\n\n");
}

void close_index( fichier_tof_index *F )
{
   // saving header part in secondary memory (at the begining of the stream F->f)
   rewind(F->f);
   fwrite( &F->h, sizeof(header_index), 1, F->f );
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
                printf("lastblk=%d\n",get_Header_lnof(F,"Lastblk"));
            }
        }
        // writing the last block
        Write_Block_lnof(F,&buffer,get_Header_lnof(F,"Lastblk"));
    printf("terminated with success\n\n");

    //* sorting the list_index
    bool swap;
            for(i=0;i<N-1;i++){
                swap = false;
                for (j=0;j<N-i-1;j++){
                    
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

    //* bulkloading the index file
    j=0;
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
        printf("index terminated with success\n\n");
}

int main(){
    srand(time(NULL));
    fichier_lnof *F;
    block_lof buffer;
    buffer.link = -1;
    buffer.nb = 0;
    int i,j=0,N,r,*list,*list_index;
    fichier_tof_index *I;
    block_index buffer_index;

    printf("enter the number of records:\n");
    scanf("%d",&N);

    //* writing the blocks

    open_lnof(&F,"test1",'n');
    
    if (F->f != NULL ){
        //* creating the index 
            open_index(&I,"index_test1",'n');
        bulk_load_lof(F,I,N);
        
        
        printf("num_blocks=%d\n",get_Header_index(I,"num_block"));
        //* looking through the index
        r = rand() % (get_Header_index(I,"num_block") + 1);
        Read_Block_index(I,&buffer_index,r);
        j = rand() % (buffer_index.nb + 1);
        for (i=j;i<(j+30) % (buffer_index.nb + 1);i++){
            
            printf("j=%d | key=%d\n",i,buffer_index.Tab[i].key);
        }
        printf("reading index block = %d\n",r);


        //* looking throught some buffer
        r = rand() % (get_Header_lnof(F,"Lastblk") + 1);
        Read_Block_lnof(F,&buffer,r);
        for (i=0;i<20;i++){
            j = rand() % (buffer.nb + 1);
            printf("j=%d | %d %s, %s, %s, %s, %d %d \n\n",j,buffer.Tab[j].Document_id, buffer.Tab[j].Title, buffer.Tab[j].Author, buffer.Tab[j].Type, buffer.Tab[j].Domaine, buffer.Tab[j].Pub_year, buffer.Tab[j].Available_qty);
        }

        printf("reading block = %d\n",r);
        printf("link=%d nb=%d num_block=%d\n",buffer.link,buffer.nb,get_Header_lnof(F,"Lastblk"));

        close_index(I);
        close_lnof(F);
    }
}