#include "data_definition.h"
//#include "History.c"
#ifndef base_modules

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
        "Ouvrages et manuels","Revues et periodiques",
        "Actes de conferences","Rapports de recherche",
        "Memoires et theses","Polycopies et support de cours",""
    } ;
    int num_type = rand() % (6);
    return type[num_type];
}

void generate_type(char *typ){
    char *type[7] = {
        "Ouvrages et manuels","Revues et periodiques",
        "Actes de conferences","Rapports de recherche",
        "Memoires et theses","Polycopies et support de cours",""
    } ;
    int num_type;
    printf("1. Ouvrages et manuels\n2. Revues et periodiques\n3. Actes de conferences\n4. Rapports de recherche\n5. Memoires et theses\n6. Polycopies et support de cours\n");
    printf("eneter the number of the type\n");
    scanf("%d",&num_type);
    while (num_type > 6 || num_type < 1){
        printf("oops number out of range please try again\n");
        scanf("%d",&num_type);
    }
    strcpy(typ,type[num_type-1]);
}

void generate_domaine(char *typ){
    char *domaine[13] = {
        "Algorithmes et structures de donnees","Intelligence artificielle et apprentissage automatique","Systemes d'exploitation",
        "Cybersecurite","Bases de donnees et gestion des donnees","Reseaux et telecommunications",
        "Genie logiciel, programmation et developpement logiciel","Informatique graphique, multimedia,vision par ordinateur et traitement d'images",
        "Science des donnees et statistiques","Robotique et systemes embarques","Blockchain et technologies distribuees","Calcul haute performance et informatique quantique",""
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
    
    strcpy(typ,domaine[num_type-1]);
}

char *generate_random_domaine(){
    char *domaine[13] = {
        "Algorithmes et structures de donnees","Intelligence artificielle et apprentissage automatique","Systemes d'exploitation",
        "Cybersecurite","Bases de donnees et gestion des donnees","Reseaux et telecommunications",
        "Genie logiciel, programmation et developpement logiciel","Informatique graphique, multimedia,vision par ordinateur et traitement d'images",
        "Science des donnees et statistiques","Robotique et systemes embarques","Blockchain et technologies distribuees","Calcul haute performance et informatique quantique",""
    } ;
    int num_type =  rand() % (12);
    return domaine[num_type];
}

int generate_random_year(){
    return 1970 +  rand() % (55);
}

int generate_random_quantite(){
    return 1 +  rand() % (10);
}

void tocpy(){ //! it is just a place to put prints that are often used
    block_lof buffer;
    printf("%d %s, %s, %s, %s, %d %d \n\n",buffer.Tab[0].Document_id, buffer.Tab[0].Title, buffer.Tab[0].Author, buffer.Tab[0].Type, buffer.Tab[0].Domaine, buffer.Tab[0].Pub_year, buffer.Tab[0].Available_qty);
    /*Search_by_id(I,N,&found,&i,&j);
        if (found){
            Read_Block_lnof(F,&buffer,i);
            printf(" it was found in block %d position %d and value %d\n",i,j,buffer.Tab[j].Document_id);
        } else{
            printf("it was not found\n");

            Read_Block_index(I2,&buffer_index,0);
        for (i=0;i<get_Header_index(I2,"num_ins");i++){
            printf("i=%d id=%d blk=%d pos=%d\n",i,buffer_index.Tab[i].key,buffer_index.Tab[i].adr_block,buffer_index.Tab[i].position);
        }
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


void Recherche_dicho_bufer(block_index buff,int key,bool *found,int *block,int *position){
    int m,inf = 0,sup = buff.nb; //! sup = buff.nb - 1
    *found = false;

    while(!(*found )&& (inf <= sup)){
        m = (inf + sup) / 2;
        if ( buff.Tab[m].key == key ){
            *found = true;
            *block = buff.Tab[m].adr_block;
            *position = buff.Tab[m].position;
            //printf("key = %d  %d %d position in list_inedx = %d",key,*block,*position,m);
        } else {
            if (buff.Tab[m].key > key){
                sup = m - 1 ;
            } else {
                inf = m + 1;
            }
        }
    }
}

bool Recherche_dicho_bufer_tof(block_tof buff,int key,int *position){ // returns the last visited index in the buffer
    int m,inf = 0,sup = buff.nb-1; //! sup et inf
    bool found = false;

    while(!found && (inf <= sup)){
        m = (inf + sup) / 2;
        if ( buff.Tab[m].Document_id == key ){
            found = true;
            //printf("key = %d  %d position in = %d",key,*position,m);
        } else {
            if (buff.Tab[m].Document_id > key){
                sup = m - 1 ;
            } else {
                inf = m + 1;
            }
        }
    }
    *position = m;
    return found;
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
};

bool search_list(enreg_index *list_index,int len,int key,int *position){  //it returns also the last visited inedx in the list
    int m,inf = 1,sup =len;
    bool found=false;

    while((!found) && (inf <= sup)){
        m = (inf + sup) / 2;
        //printf("inf = %d sup = %d m =%d\n",inf,sup,m);
        if ( list_index[m].key == key ){
            //printf("%d was found\n",key);
            found = true;
        } else {
            if (list_index[m].key > key){
                sup = m - 1 ;
            } else {
                inf = m + 1;
            }
        }
    }
    *position = m;
    return found;
}

bool search_list_journal(fichier_tof *T,enreg_index *list_index,int key,int *block,int *position){ //search in the list_index fo journal_magazine file and returning its coordinats in the file

    if (search_list(list_index, (get_Header_tof(T,"nrec") / b)+1,key,position)){  // this will check first if the key in the list_index //position is the position of the key in list index
        *block = list_index[*position].adr_block;
        *position = list_index[*position].position; //now it returns the position in the tof file
        return true;
    } //else 
    block_tof buffer;
    *block = list_index[*position].adr_block;
    Read_block_tof(T,&buffer,list_index[*position].adr_block);
    printf("position %d\n-------\n",*position);
    if ( Recherche_dicho_bufer_tof(buffer,key,position) ){ //now this looks the position in the buffer
        return true;
    } //if not found in the buffer
    return false;
    
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


void load_index(fichier_tof_index *I,enreg_index *list){
    int i,k=0,j,num = get_Header_index(I,"num_block");
    block_index buff;
   
    for (i=0;i<=num;i++){
        Read_Block_index(I,&buff,i);
        //printf("\nnum = %d nb= %d\n",i,buff.nb);
        for (j=0;j<buff.nb;j++){
            list[k] = buff.Tab[j];
            k++;
        }
    }
}

void load_index_journal_du_tof(fichier_tof *T,enreg_index *list){
    int i,k=0,j,num = get_Header_tof(T,"num_block");
    block_tof buff;
   
    for (i=0;i<=num;i++){
        Read_block_tof(T,&buff,i);
        //printf("\nnum = %d nb= %d\n",i,buff.nb);
            list[k].key = buff.Tab[buff.nb-1].Document_id;
            list[k].adr_block = i;
            list[k].position = buff.nb-1;
            k++;
    }
}

enreg_index *resize_list_index (enreg_index *list,int len){
    enreg_index *newlist = (enreg_index *)realloc(list,len * sizeof(enreg_index));
    
    if( newlist == NULL) {
        fprintf(stderr, "ERROE, Memory allocation failed in resize_list_index\"\"\n");
        return NULL;
    }

    return newlist;
}

enreg_tof *resize_list_tof (enreg_tof *list,int len){
    enreg_tof *newlist = (enreg_tof*)realloc(list,len * sizeof(enreg_tof));
    
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

void affich_entet_lnof(fichier_lnof *F){
    printf("\nfichier principale\n------------------\nnumber of blocks in the file: %d\nnumber of documents: %d\nnumber of deletes: %d\n\n",get_Header_lnof(F,"Lastblk")+1,get_Header_lnof(F,"nrec"),get_Header_lnof(F,"ndel"));
}

void affich_entet_index(fichier_tof_index *I){
    printf("\nfichier index\n--------------\nnumber of blocks in the file: %d\nnumber of documents: %d\nnumber of deletes: %d\n\n",get_Header_index(I,"num_block")+1,get_Header_index(I,"num_ins"),get_Header_index(I,"num_del"));
}

void affich_entet_tof(fichier_tof *T){
    printf("\nfichier journal_magazine\n------------------\nnumber of blocks in the file: %d\nnumber of documents: %d\nnumber of deletes: %d\nupdated: %x\n\n",get_Header_tof(T,"num_block")+1,get_Header_tof(T,"nrec"),get_Header_tof(T,"ndel"),get_Header_tof(T,"updated"));
}

#endif