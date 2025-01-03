#ifndef data_definition
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define b 1024

typedef struct _cost
{
	int read;
	int write;
} cost;

typedef struct type_enreg {
    long int Document_id;
    char Title[71];
    char Author[31];
    char Type[31];
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
    //fseek(F->f,0,SEEK_SET);
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
    fprintf(stderr, "getHeader_lnof : Unknown headerName: \"%s\"\n", field);
}

void set_Header_lnof(fichier_lnof *F,char *field,int val){
    if (strcmp(field, "Firstblk") == 0) {F->h.Firstblk = val ;return;}
    if (strcmp(field, "Lastblk") == 0) {F->h.Lastblk = val ; return;}
    if (strcmp(field, "nrec") == 0) {F->h.nrec = val ; return;}
    if (strcmp(field, "ndel") == 0) {F->h.ndel = val ; return;}
    fprintf(stderr, "setHeader_lnof : Unknown headerName: \"%s\"\n", field);
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

void open_index(fichier_tof_index **F,char *filename,char mode){ //? 1 read if exists / 2 writes if new
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

void close_index( fichier_tof_index *F ) //? 1 write
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
} //? 1 read

void Write_Block_index(fichier_tof_index *F,block_index *buffer,int i){
    if (i<0 || i>get_Header_index(F,"num_block") ){fprintf(stderr, "Read_block_index : number of block does not exist: \"%d\"\n", i);; return;}
    fseek(F->f,sizeof(header_index) + i*sizeof(block_index),SEEK_SET);
    fwrite(buffer,sizeof(block_index),1,F->f);
} //? 1 write

void Alloc_block_index (fichier_tof_index *F){
    block_index buf;
    buf.nb =0;
    set_Header_index(F,"num_block",get_Header_index(F,"num_block")+1);
    Write_Block_index(F,&buf,get_Header_index(F,"num_block"));
} //? 1 write

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
    char filename[50];
    bool updated;
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
        (*F)->h.updated = 0;
        strcpy((*F)->h.filename,filename);
        fwrite(&((*F)->h),sizeof(header_tof),1,(*F)->f);
        block_tof buffer;
        buffer.nb =0;
        fwrite(&buffer,sizeof(block_tof),1,(*F)->f);
    }
    printf("--TOF file opened succesfuly\n\n");
}

void close_tof(fichier_tof *F){
    rewind(F->f);
    fwrite(&F->h, sizeof(header_tof),1,F->f);
    fclose(F->f);
    free(F);
    printf("--TOF file closed succesfuly\n");
}

int get_Header_tof(fichier_tof *F,char *field){
    if (strcmp(field, "num_block") == 0) return F->h.num_block;
    if (strcmp(field, "nrec") == 0) return F->h.nrec;
    if (strcmp(field, "ndel") == 0) return F->h.ndel;
    if (strcmp(field, "updated") == 0) return F->h.updated;
    fprintf(stderr, "getHeader_tof : Unknown headerName: \"%s\"\n", field);
}

void set_Header_tof(fichier_tof *F,char *field,int val){
    if (strcmp(field, "num_block") == 0) {F->h.num_block = val ;return;}
    if (strcmp(field, "nrec") == 0) {F->h.nrec = val ; return;}
    if (strcmp(field, "ndel") == 0) {F->h.ndel = val ; return;}
    if (strcmp(field, "updated") == 0) {F->h.updated = (bool)val ; return;}
    fprintf(stderr, "setHeader_tof : Unknown headerName: \"%s\"\n", field);
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
#endif