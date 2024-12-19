#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "lof_modal.c"

// la structure TOF (pour l'index)

typedef struct type_enreg_index{
    long int key;
    int num_block;
    int position;
    bool del;
}enreg_index;

typedef struct type_bloc_tof_index
{
    enreg_index Tab[b];
    int nb;
}block_index ;

typedef struct type_hdr_index{
    int num_block;
    int num_ins;
    int num_del;
} header_index;

typedef struct index_file {
    FILE *f;
    header_index h;
} fichier_tof_index;

void open_index(fichier_tof_index **F,char *filename,char mode){
    *F = malloc(sizeof (fichier_tof_index));
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
}

void close_index( fichier_tof_index *F )
{
   // saving header part in secondary memory (at the begining of the stream F->f)
   rewind(F->f);
   fwrite( &F->h, sizeof(header_index), 1, F->f );
   fclose(F->f);
   printf("index file closed succesfuly");
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
    set_Header_index(F,"Lastblk",get_Header_index(F,"num_block")+1);
    Write_Block_index(F,&buf,get_Header_index(F,"num_block"));
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

void fill_buffer(block_lof *buffer,int j){
    char titre[71];
    char author[31];
    generate_random_title(titre);
    generate_random_author(author);
    //(*buffer).Tab[j].Document_id = generate_random_id();
    strcpy( (*buffer).Tab[j].Title,titre);
    strcpy( (*buffer).Tab[j].Author,author);
    strcpy( (*buffer).Tab[j].Type,generate_random_type());
    strcpy( (*buffer).Tab[j].Domaine,generate_random_domaine());
     (*buffer).Tab[j].Pub_year = generate_random_year();
     (*buffer).Tab[j].Available_qty = generate_random_quantite();
}

void initial_load_lof(fichier_lnof *F,int N){
    block_lof buffer;
    buffer.nb = 0;
    int i,j=0;
    open_lnof(&F,"test1",'n');
    if (F->f !=NULL ){
        printf("lastblk=%d\n",get_Header_lnof(F,"Lastblk"));
        //Read_Block_lnof(F,&buffer,0);
        printf("link=%d nb=%d\n",buffer.link,buffer.nb);
        //starting to fill
        printf("\nenter the number of records:\n");
        scanf("%d",&N);
        for (i=0;i<N;i++){
            if ( j< b){
                fill_buffer(&buffer,j);
                buffer.nb++;
                j++;
            }else {
                Write_Block_lnof(F,&buffer,get_Header_lnof(F,"Lastblk"));
                Alloc_block_lnof(F);
                Read_Block_lnof(F,&buffer,get_Header_lnof(F,"Lastblk"));
                buffer.nb = 1;
                j=0;
                fill_buffer(&buffer,j);
                j++;
            }
        }
        // writing the last block
        Write_Block_lnof(F,&buffer,get_Header_lnof(F,"Lastblk"));
    }
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

int generate_random_id(int *list, int *len){
    int n;bool f;
    do {
        n = 110000 +  rand() % (880001);
        f = find_list(list,*len,n);
        printf("%x\n",f);
    }while(!f);
    (*len)++;
    list[(*len)] = n;
    return n;
}

int main(){
    srand(time(NULL));
    fichier_lnof *F;
    block_lof buffer;
    buffer.link = -1;
    buffer.nb = 0;
    int i,j=0,N;
    printf("enter the number of records:\n");
    scanf("%d",&N);
    //int list[N],len=0;
   // printf("%d",generate_random_id(list,&len));
    //resizing
    initial_load_lof(F,N);
    /*open_lnof(&F,"test1",'n');
    if (F->f !=NULL ){
        printf("lastblk=%d\n",get_Header_lnof(F,"Lastblk"));
        //Read_Block_lnof(F,&buffer,0);
        printf("link=%d nb=%d\n",buffer.link,buffer.nb);
        //starting to fill
        printf("\nenter the number of records:\n");
        scanf("%d",&N);
        for (i=0;i<N;i++){
            if ( j< b){
                fill_buffer(&buffer,j);
                buffer.nb++;
                j++;
            }else {
                Write_Block_lnof(F,&buffer,get_Header_lnof(F,"Lastblk"));
                Alloc_block_lnof(F);
                Read_Block_lnof(F,&buffer,get_Header_lnof(F,"Lastblk"));
                buffer.nb = 1;
                j=0;
                fill_buffer(&buffer,j);
                j++;
            }
        }
        // writing the last block
        Write_Block_lnof(F,&buffer,get_Header_lnof(F,"Lastblk"));
        printf("lastblk in the file=%d\n",get_Header_lnof(F,"Lastblk"));
        N = rand() % (get_Header_lnof(F,"Lastblk")+1);  
        Read_Block_lnof(F,&buffer,N);
        //Read_Block_lnof(F,&buffer,0);
        printf("printing the values of block %d\n",N);
        printf("link=%d nb=%d\n",buffer.link,buffer.nb);
        for (i=0;i<buffer.nb;i++){
        printf("i=%d: %d %s, %s, %s, %s, %d %d \n\n",i,buffer.Tab[i].Document_id, buffer.Tab[i].Title, buffer.Tab[i].Author, buffer.Tab[i].Type, buffer.Tab[i].Domaine, buffer.Tab[i].Pub_year, buffer.Tab[i].Available_qty);
        }
        close_lnof(F);
    }*/
    close_lnof(F);
}