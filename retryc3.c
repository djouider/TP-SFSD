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

void Alloc_block_index_1024 (fichier_tof_index *F){
    block_index buf;
    buf.nb =1024;
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
    int m,inf = 1,sup = buff.nb;
    *found = false;

    while(!(*found )&& (inf <= sup)){
        m = (inf + sup) / 2;
        if ( buff.Tab[m].key == key ){
            *found = true;
            *block = buff.Tab[m].adr_block;
            *position = buff.Tab[m].position;
            printf("key = %d  %d %d\n",key,*block,*position);
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
    int m,inf = 1,sup =len;
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
        if ( list_index[m].key == key ){
            printf("%d was found\n",key);
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

            if (key > buff.Tab[1].key && key < buff.Tab[buff.nb - 1].key){
                Recherche_dicho_bufer(buff,key,found,block,position);
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
        printf("\nnum = %d nb= %d",i,buff.nb);
        for (j=0;j<buff.nb;j++){
            list[k] = buff.Tab[j];
            k++;
        }
    }
}

enreg_index *resize_list_index (enreg_index *list,int len){
    enreg_index *newlist = realloc(list,len * sizeof(enreg_index));
    
    if( newlist == NULL) {
        printf("ERROE, Memory allocation failed\n");
        return NULL;
    }

    return newlist;
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
    enreg document;
    block_lof buffer;
    block_index buffer_index;

    printf("how many documents you want to add ?\n");
        scanf("%d",&num);

    if (num > 0 ){
        printf("how do you want to generate them ?\n1. manually\n2. automaticlly\n");
            scanf("%d",&choice);
            printf("\nenterign choice 2\n\n");
            switch (choice){
                case 1: //manually

                    break;

                case 2: //autimatticlly
                    printf("before list_index\n");
                    list_index = resize_list_index(list_index,nrec+num);
                    Read_Block_lnof(F,&buffer,get_Header_lnof(F,"Lastblk"));

                    //* filling the lnof file
                    for (i=0;i<num;i++){
                        fill_document_random(&document,list_index,nrec);
                        if ( buffer.nb < b){
                            buffer.Tab[buffer.nb] = document;
                            buffer.nb++;
                        } else {
                            Write_Block_lnof(F,&buffer,get_Header_lnof(F,"Lastblk"));
                            Alloc_block_lnof(F);
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
                    if (search_list(list_index,nrec,min,&index)){
                        printf("%d was found in position %d\n",min,index);
                    } else {
                        printf("%d was not found in the list\n",min);
                    }

                    adr = index / b; 
                    cnd = b*adr;


                    for (i=cnd;i<get_Header_index(I,"num_ins");i++){
                        if (index < b) {
                            buffer_index.Tab[index] = list_index[b*adr + index];
                            //printf("nb = %d\n",buffer_index.nb);
                        index++;
                        } else {
                            Write_Block_index(I,&buffer_index,adr); 
                            adr++;
                            printf("adr = %d num_block=%d i=%d index= %d\n",adr,get_Header_index(I,"num_block"),i,index);
                            Read_Block_index(I,&buffer_index,adr);
                            index = 0;
                            buffer_index.Tab[index] = list_index[b*adr + index];
                        }
                    }
                    Write_Block_index(I,&buffer_index,adr);

                    cnd = i;
                    for (i=cnd;nrec;i++){ //adding the new values
                        if (index < b) {
                            buffer_index.Tab[index] = list_index[b*adr + index];
                            //printf("nb = %d\n",buffer_index.nb);
                            index++;
                            buffer_index.nb++;
                        } else {
                            Write_Block_index(I,&buffer_index,adr); 
                            adr++;
                            Alloc_block_index(I);
                            printf("adr = %d num_block=%d i=%d index= %d\n",adr,get_Header_index(I,"num_block"),i,index);
                            Read_Block_index(I,&buffer_index,adr);
                            index = 0;
                            buffer_index.Tab[index] = list_index[b*adr + index];
                            buffer_index.nb = 1;
                        }
                    }
                    Write_Block_index(I,&buffer_index,adr);
                    /*q = nrec / b;
                    r = nrec % b;
                    while (get_Header_index(I,"num_block") != q){
                        Alloc_block_index_1024(I);
                        printf("1\n");
                    }
                    Read_Block_index(I,&buffer_index,get_Header_index(I,"num_block"));
                    buffer_index.nb = r;

                    printf("adr = %d cnd = %d nrec=%d\n",adr, cnd, nrec);
                    Read_Block_index(I,&buffer_index,adr);
                    printf("adr = %d index = %d nb= %d\n",adr,index,buffer_index.nb);
                    for (i=cnd; i<nrec; i++){
                            //printf("adr = %d index = %d nb= %d\n",adr,index,buffer_index.nb);
                            if (index < b) {
                                buffer_index.Tab[index] = list_index[b*adr + index];
                                //printf("nb = %d\n",buffer_index.nb);
                                if (buffer_index.nb < b){
                                    printf("d5l\n");
                                    buffer_index.nb++;
                                }
                                index++;
                            } else {
                                Write_Block_index(I,&buffer_index,adr); 
                                adr++;
                                printf("adr = %d num_block=%d i=%d index= %d\n",adr,get_Header_index(I,"num_block"),i,index);
                                Read_Block_index(I,&buffer_index,adr);
                                if (adr == get_Header_index(I,"num_block")) { // if a problem happens maybe it is here
                                    buffer.nb = r;
                                }

                                index = 0;
                                buffer_index.Tab[index] = list_index[b*adr + index];
                            }
                    }
                    Write_Block_index(I,&buffer_index,get_Header_index(I,"num_block"));*/

                    set_Header_index(I,"num_ins",nrec);
                    set_Header_lnof(F,"nrec",nrec);
                    break;

            }  
    }
}

int main(){
    srand(time(NULL));
    fichier_lnof *F;
    block_lof buffer;
    bool found;
    buffer.link = -1;
    buffer.nb = 0;
    int i,j=0,N,r,*list,cpt = 1;
    fichier_tof_index *I,*I2;
    enreg_index *list_index = (enreg_index*)malloc(sizeof(enreg_index));
    enreg e;
    block_index buffer_index;

    //* writing the blocks

    open_lnof(&F,"test2",'e');
    
    if (F->f != NULL ){
        //* creating the index 
        open_index(&I,"index_test2",'e');
        
        //bulk_load_lof(F,I,5000);

        list_index = resize_list_index(list_index,get_Header_index(I,"num_ins")); 
        if (list_index == NULL){
            printf("ERROR: Memory allocation failed for list_index in main\n");
            return 0;
        }

        load_index(I,list_index);
        /*r = rand() % (get_Header_index(I,"num_ins") + 1);

        for (i=r;i<r+10;i++){
            printf("i=%d | key= %d block= %d position= %d\n\n",i,list_index[i].key,list_index[i].adr_block,list_index[i].position);
        }
        printf("looking the list from i=%d\n",r);*/
        printf(" before the adding\nnum_block in the file= %d num_block_index= %d\n",get_Header_lnof(F,"Lastblk"),get_Header_index(I,"num_block"));
        printf("nrec in the file= %d nrec in the index= %d\n",get_Header_lnof(F,"nrec"),get_Header_index(I,"num_ins"));
        printf("looking in the last block in the file:\n");

        Read_Block_lnof(F,&buffer,get_Header_lnof(F,"Lastblk"));
        r = buffer.nb - 6;
        printf("last= %d\n",get_Header_lnof(F,"Lastblk"));

        for (i=r;i<buffer.nb;i++){
            printf("i= %d id= %d\n\n",i,buffer.Tab[i].Document_id);
        }
        printf("nb= %d,i=%d\n",buffer.nb,i);

        Add(F,I,list_index);
        
        printf("\n after the adding\nnum_block in the file= %d num_block_index= %d\n",get_Header_lnof(F,"Lastblk"),get_Header_index(I,"num_block"));
        printf("nrec in the file= %d nrec in the index= %d\n",get_Header_lnof(F,"nrec"),get_Header_index(I,"num_ins"));
        printf("looking in the last block in the file:\n");

        Read_Block_lnof(F,&buffer,get_Header_lnof(F,"Lastblk"));
        r=max(buffer.nb-6,0);
        
        printf("r= %d nb=%d\n",r,buffer.nb);
        for (i=r;i<buffer.nb;i++){
            N = buffer.Tab[i].Document_id;
            printf("%d %s, %s, %s, %s, %d %d \n\n",N, buffer.Tab[i].Title, buffer.Tab[i].Author, buffer.Tab[i].Type, buffer.Tab[i].Domaine, buffer.Tab[i].Pub_year, buffer.Tab[i].Available_qty);
        }
        printf("nb= %d,i=%d\n",buffer.nb,i);

        printf("enter the number of records:\n");
        scanf("%d",&N);

        Search_by_id(I,N,&found,&i,&j);
        if (found){
            Read_Block_lnof(F,&buffer,i);
            printf(" it was found in block %d position %d and value %d\n",i,j,buffer.Tab[j].Document_id);
        } else{
            printf("it was not found\n");
        }

        close_index(I);
        close_lnof(F);
    }
}