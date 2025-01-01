#include "retry.c"



typedef struct _show_list {
	enreg Val;
	POINT coordinates;
	struct _show_list* Next;
}show_list;

void Allocate_Li (show_list **P)
{
*P = (show_list*) malloc( sizeof(show_list)) ;
(*P)->Next = NULL;
}



typedef struct typef_filtre {
    long int Document_id;
    char Author[31];
    char Type[30];
    char Domaine[82];
    int Pub_year_max;
    int Pub_year_min;
} filtre;

typedef struct typef_temp {
    char Document_id[11];
    char Author[31];
    char Type[30];
    char Domaine[82];
    char Pub_year_max[5];
    char Pub_year_min[5];
    bool Del;
} temporary;


void clear_table()
{
	int i,j;
	for(i=4;i<97;i++)
	{
		if(i!=12 && i!=50 & i!=87)
		{
			for(j=6;j<43;j++)
			{
			printf("\033[%d;%dH ",j,i);
			}
		}
		printf("\033[%d;%dH ",46,i);
	}
}



void add_list(show_list **Head,show_list *Node)
{
	show_list *ptr;
	
	if(*Head==NULL) 
	{
		*Head=Node;
		return;
	} 
	ptr=*Head;
	while(ptr->Next!=NULL)
	{
		ptr=ptr->Next;
	}///go to end
	
	ptr->Next= Node;
	return;
}



void search_by_filter(fichier_lnof *F,filtre filter,show_list **result) ////Searches for all the values by the given filter and insert them into multiple lists
{
	bool Document_Id,Author,Type,Domaine,Pub_year_max,Pub_year_min,add; //to select which fields to filter
	
	int i,j,max;
	show_list * Node;
	block_lof buffer;
	POINT coordinates;
	
	*result=NULL;
	Document_Id=true;
	Author=true;
	Type=true;
	Domaine=true;
	Pub_year_max=true;
	Pub_year_min=true;
	
	if(filter.Document_id==0)
	{
		Document_Id=false;
	}
	
	if(filter.Author[0]=='\0')
	{
		Author=false;
	}
	
	if(filter.Type[0]=='\0')
	{
		Type=false;
	}
	
	if(filter.Domaine[0]=='\0')
	{
		Domaine=false;
	}
	
	if(filter.Pub_year_max==0)
	{
		Pub_year_max=false;
	}
	
	if(filter.Pub_year_min==0)
	{
		Pub_year_min=false;
	}
	max=get_Header_lnof(F,"Lastblk");
	//////The file Must be already opened
//	i=get_Header_lnof(F,"Firstblk");
i=1;
	Read_Block_lnof(F,&buffer,i);
	while(i<=max)
	{
		for(j=0;j<buffer.nb;j++)
		{
			if( Document_Id )
			{
				if(filter.Document_id!=buffer.Tab[j].Document_id)
				{
					continue;
				}
			
			}
			
			if(Author)
			{
				if(strcmp(filter.Author,buffer.Tab[j].Author)!=0)
				{
					continue;
				}
			}
			
			if(Type)
			{
				if(strcmp(filter.Type,buffer.Tab[j].Type)!=0)
				{
					continue;
				}
			}
			
			if(Domaine)
			{
				if(strcmp(filter.Domaine,buffer.Tab[j].Domaine)!=0)
				{
					continue;
				}
			}	
			
			if(Pub_year_max)
			{
				if(filter.Pub_year_max<buffer.Tab[j].Pub_year)
				{
					continue;
				}
			}
			
			if(Pub_year_min)
			{
				if(filter.Pub_year_min>buffer.Tab[j].Pub_year)
				{
					continue;
				}
			}
			////add the record to the list because he have all the criterias
					Node=NULL;
					Allocate_Li(&Node);
					Node->Val=buffer.Tab[j];
					coordinates.x=i-1;
					coordinates.y=j;
					Node->coordinates=coordinates;
					Node->Next=NULL;
					add_list(result,Node);
		}
		
		 ///go to next block
//			i=buffer.link;
			i++;
			if(i<max)	Read_Block_lnof(F,&buffer,i);
	}
	return;
}





void mise_en_page()
{
	int i;
	printf("\033[44m");
	for(i=1;i<100;i++)
	{
		printf("\033[%d;%dH ",1,i);
		printf("\033[%d;%dH ",5,i);
		printf("\033[%d;%dH ",44,i);
		printf("\033[%d;%dH ",49,i);
	}
	for(i=1;i<49;i++)
	{
		printf("\033[%d;%dH ",i,1);
		if(i<=44)
		{
		printf("\033[%d;%dH ",i,12);
		printf("\033[%d;%dH ",i,50);
		printf("\033[%d;%dH ",i,87);
		}
		printf("\033[%d;%dH ",i,100);
	}
	printf("\033[0m");
	printf("\033[0m\033[%d;%dHID",3,5);
	printf("\033[%d;%dHType",3,24);
	printf("\033[%d;%dHAuthor",3,65);
	printf("\033[%d;%dHPub_date",3,90);
	printf("\033[%d;%dHPage :",46,3);
	printf("\033[%d;%dH<-- Previous ; Next--> ",46,35);
	printf("\033[%d;%dHNumber of books: ",46,70);
	printf("\033[%d;%dHPress D to show/hide filter: ",1,121);

}

void filter() ///to show filter prompt
{
		printf("\033[4;115HFilter:");
		printf("\033[7;118HDocument Id :");
		read_prompt(118,9,15);
		
		printf("\033[14;118HAuthor :");
		read_prompt(118,16,35);
		
		printf("\033[20;118HPublication year :");
		printf("\033[23;118HMin:");
		read_prompt(123,22,15);
		printf("\033[23;140HMax:");
		read_prompt(145,22,15); 
		
		printf("\033[28;128HType +");
		
		printf("\033[28;140HDomaine +");

		printf("\033[45;130H\033[44mSearch\033[0m ");
		
		printf("\033[45;148H\033[44mClear Filter\033[0m ");
}                                              

void unfilter() ///to hide filter prompt
{
	system("cls");
	mise_en_page();
}




int max_list(show_list *Head)
{
	int cpt=0;
	while(Head!=NULL)
	{
		Head=Head->Next;
		cpt++;
	}
	return cpt;
}


void access_list(show_list *Head,show_list **position,int number)
{
	*position=Head;
	while(number &&  (*position)->Next!=NULL)
	{
		*position=(*position)->Next;
		number--;
	}
	
}


void show_pages(fichier_lnof *F,show_list *Head,int *page,bool filter)
{
	int i,j,l,max_page;
	block_lof buffer;
	show_list *P;
	
	if(!filter)//show the whole file
	{
		if(*page<1) *page=1;
		max_page=get_Header_lnof(F,"nrec")/19;
		if(get_Header_lnof(F,"nrec")%19);
		{
			max_page++;
		}
		if(*page>max_page)
		{
			*page=(max_page);
		}
		
		
//		j=get_Header_lnof(F,"Firstblk");
		j=1;
		for(i=0;i<=(19*(*page-1))/b;i++)///go to the right block
		{
			j=buffer.link;
			Read_Block_lnof(F,&buffer,j);
		}
		l=j;
		j=(19*(*page-1))%b;//go to the right value in the block
		for(i=0;(i<19)&& (i+j-1<buffer.nb);i++)
		{
			if(((j+i)%b)%buffer.nb==0)
				{
						if((j+i)!=0)///not in the start
				{
					l=buffer.link;
					if(l!=-1)
					{
						Read_Block_lnof(F,&buffer,l);
						j=-i;
					}
					else break;
				}
				
			}
			printf("\033[%d;%dH",6+2*i,3);//position the cursor to the start of the line
			printf(" %ld", buffer.Tab[(j+i)%b].Document_id);
			printf("\033[%d;%dH",6+2*i,13);
			printf(" %s", buffer.Tab[(j+i)%b].Type);
			printf("\033[%d;%dH",6+2*i,52);
			printf(" %s", buffer.Tab[(j+i)%b].Author);
			printf("\033[%d;%dH",6+2*i,91);
			printf(" %d", buffer.Tab[(j+i)%b].Pub_year);
		}
			printf("\033[%d;%dH",46,10);
			printf("%d %d",*page,max_page);
			
			printf("\033[%d;%dH",46,87);
			printf("%d/%d",(*page-1)*19,get_Header_lnof(F,"nrec"));
	}
	else ///show the list
	{
		if(Head==NULL)
		{
			printf("\033[%d;%dHSorry we haven't found anything",22,16);
			return;
		}
		if(*page<1) *page=1;
		max_page=max_list(Head)/19;
		if(max_list(Head)%19);
		{
			max_page++;
		}
		if(*page>max_page)
		{
			*page=(max_page);
		}
		access_list(Head,&P,19*(*page-1));
		for(i=0;i<19 && P!=NULL;i++)
		{
			printf("\033[%d;%dH",6+2*i,3);//position the cursor to the start of the line
			printf(" %ld", P->Val.Document_id);
			printf("\033[%d;%dH",6+2*i,13);
			printf(" %s", P->Val.Type);
			printf("\033[%d;%dH",6+2*i,52);
			printf(" %s", P->Val.Author);
			printf("\033[%d;%dH",6+2*i,91);
			printf(" %d", P->Val.Pub_year);
			P=P->Next;
		}
			printf("\033[%d;%dH",46,10);
			printf("%d/%d",*page,max_page);
			printf("\033[%d;%dH",46,87);
			printf("%d/%d",(*page-1)*19,max_list(Head));
		
		
	}
}


void show_book(fichier_lnof *F,bool filt,show_list *Head,int page,int number)
{
	int i,j,l,max_page;
	block_lof buffer;
	show_list *P;
	if(filt)///search in the list
	{
		if(19*page+number>max_list(Head)) return;

		access_list(Head,&P,19*page+number);
		
		system("cls");
		printf("\033[%d;%dH",4,80); printf("Book ID:%ld",P->Val.Document_id);
		printf("\033[%d;%dH",7,70);printf("Author's name:%s",P->Val.Author);
		printf("\033[%d;%dH",10,49);printf("Title :%s",P->Val.Title);
		printf("\033[%d;%dH",14,65);printf("Domaine:%s",P->Val.Domaine);
		printf("\033[%d;%dH",16,68);printf("Type:%s",P->Val.Type);
		printf("\033[%d;%dH",20,77);printf("Released in:%d",P->Val.Pub_year);
		printf("\033[%d;%dH",22,75);printf("Available quantity :%d",P->Val.Available_qty);
		printf("\033[%d;%dH",28,65);printf("Situated at : block number %d position number %d",P->coordinates.x-1,P->coordinates.y);
		printf("\033[%d;%dHPress anything to go back",39,75);
		getch();
		
	}
	else//search in the file
	{
		if(page<1) page=1;
		max_page=get_Header_lnof(F,"nrec")/19;
		
		if(get_Header_lnof(F,"nrec")%19);
		{
			max_page++;
		}
		if(page>max_page)
		{
			page=(max_page);
		}
		//		l=get_Header_lnof(F,"Firstblk");
		l=1;
		for(i=0;i<=(19*(page-1))/b;i++)///go to the right block
		{
			Read_Block_lnof(F,&buffer,l);
			l=buffer.link;
		}
		j=(19*(page-1))%b;//go to the right value in the block
		j+=number;
		if(j/b)//next block
		{
			j=j/b;
			l=buffer.link;
			if(l>get_Header_lnof(F,"Lastblk")) return;
			Read_Block_lnof(F,&buffer,l);
		}
		printf("%d",j);
		if(j>=buffer.nb) return; //incomplete page
		system("cls");
		printf("\033[%d;%dH",4,80); printf("Book ID:%ld",buffer.Tab[j].Document_id);
		printf("\033[%d;%dH",7,70);printf("Author's name:%s",buffer.Tab[j].Author);
		printf("\033[%d;%dH",10,49);printf("Title :%s",buffer.Tab[j].Title);
		printf("\033[%d;%dH",14,65);printf("Domaine:%s",buffer.Tab[j].Domaine);
		printf("\033[%d;%dH",16,68);printf("Type:%s",buffer.Tab[j].Type);
		printf("\033[%d;%dH",20,77);printf("Released in:%d",buffer.Tab[j].Pub_year);
		printf("\033[%d;%dH",22,75);printf("Available quantity :%d",buffer.Tab[j].Available_qty);
		printf("\033[%d;%dH",28,65);printf("Situated at : block number %d position number %d",l,j);
		printf("\033[%d;%dHPress anything to go back",39,75);
		getch();
		
	}
}


bool empty_filter(filtre block)
{
	if(block.Author[0]=='\0' && block.Domaine[0]=='\0' && block.Type[0]=='\0' && block.Document_id==0 && block.Pub_year_max==0 && block.Pub_year_min==0) return true;
	return false;
}

void press_affect(filtre *block,temporary *temp,bool *filt,fichier_lnof *F,show_list **Head,int *page)
{
	char *type[7] = {"Ouvrages et manuels","Revues et periodiques",
    "Actes de conferences","Rapports de recherche",
    "Memoires et theses"," Polycopies et support de cours" , ""} ;
	
	char *domaine[13] = {
    "Algorithmes et structures de donnees","Intelligence artificielle et apprentissage automatique","Systemes d'exploitation",
	"Cybersecurite","Bases de donnees et gestion des donnees","Reseaux et telecommunications",
    "Genie logiciel, programmation et developpement logiciel","Informatique graphique, multimedia,vision par ordinateur et traitement d'images",
    "Science des donnees et statistiques","Robotique et systemes embarques","Blockchain et technologies distribuees","Calcul haute performance et informatique quantique",""
    } ;
	
	char *end;
	int id;
	if((p.x>=2 && p.x<=99) && (p.y>=6 && p.y<=43))//Inside the table ( 2<=x<=99  6<=y<=43)
	{
		
		if(p.y%2==0)
		{
			show_book(F,*filt,*Head,*page,(p.y-6)/2);
			system("cls");
			filter();
			mise_en_page();
			show_pages(F,*Head,page,*filt);
			
		}
		
		return;
	}
	
	if(p.y==10 && p.x>=119 && p.x<=131)////Document ID's prompt
	{
		printf("\033[%d;%dH           \033[0m",10,119);
		p.x=119;
		p.y=9;
		Back_read(p,temp->Document_id,11);
		block->Document_id=strtol(temp->Document_id,&end,10);
		if(*end)
		{
			printf("\033[12;118H\033[0;31mId must only contain numbers\033[0m");
			temp->Document_id[0]='\0';
			block->Document_id=0;
		}
		else
		{
		printf("\033[12;118H                            ");
		}
		if(block->Document_id==0)
		{
			printf("\033[1;37m\033[%d;%dH Enter here\033[0m",10,119);
		}
		return;
	}
	
	
	if(p.y==17 && p.x>=119 && p.x<=146)////Authour's prompt
	{
		printf("\033[%d;%dH           \033[0m",17,119);
		p.x=119;
		p.y=16;
		Back_read(p,block->Author,31);
		if(!block->Author[0])
		{
			printf("\033[1;37m\033[%d;%dH Enter here\033[0m",17,119);
		}
		return;
	}
	
	
	if(p.y==23 && p.x>=124 && p.x<=136)////Pub_Year Min's prompt
	{
		printf("\033[%d;%dH           \033[0m",23,124);
		p.x=124;
		p.y=22;
		Back_read(p,temp->Pub_year_min,5);

		block->Pub_year_min=strtol(temp->Pub_year_min,&end,10);
		if(*end)
		{
			printf("\033[26;124H                                     ");
			printf("\033[26;124H\033[0;31mMin and Max must only contain numbers\033[0m");
			temp->Pub_year_min[0]='\0';
			block->Pub_year_min=0;
		}
		else
		{
		printf("\033[26;124H                                     ");
		if(strlen(temp->Pub_year_min)<4)
		{
			printf("\033[26;124H\033[0;31mInvalid year values\033[0m");
			temp->Pub_year_min[0]='\0';
			block->Pub_year_min=0;
		}
		
		
		}
		if(block->Pub_year_min==0)
		{
			printf("\033[1;37m\033[%d;%dH Enter here\033[0m",23,124);
		}
		return;
	}
	
	if(p.y==23 && p.x>=146 && p.x<=158)////Pub_Year Max's prompt
	{
		printf("\033[%d;%dH           \033[0m",23,146);
		p.x=146;
		p.y=22;
		Back_read(p,temp->Pub_year_max,5);
		block->Pub_year_max=strtol(temp->Pub_year_max,&end,10);
		if(*end)
		{
			printf("\033[26;124H\033[0;31mMin and Max must only contain numbers\033[0m");
			temp->Pub_year_max[0]='\0';
			block->Pub_year_max=0;
		}
		else
		{
		printf("\033[26;124H                                     ");
		if(strlen(temp->Pub_year_max)<4)
		{
			printf("\033[26;124H\033[0;31mInvalid year values\033[0m");
			temp->Pub_year_max[0]='\0';
			block->Pub_year_max=0;
		}
		
		
		}
		if(block->Pub_year_max==0)
		{
			printf("\033[1;37m\033[%d;%dH Enter here\033[0m",23,146);
		}
		return;
	}
	
	if(p.y==28 && p.x>=128 && p.x<=133)////Type's prompt
	{
		p.x=108;
		p.y=29;
		sprintf(block->Type,type[0,list(p,type)]);
		sprintf(temp->Type,block->Type);
		return;
	}
	
	if(p.y==28 && p.x>=140 && p.x<=148)////Domaine's prompt
	{
		p.x=108;
		p.y=29;
		sprintf(block->Domaine,domaine[0,list(p,domaine)]);
		sprintf(temp->Domaine,block->Domaine);
		return;
	}
	
	if(p.y==45 && p.x>=130 && p.x<=135)///Search
	{
		delay(100);
		*filt=!(empty_filter(*block));
		if(*filt)	search_by_filter(F,*block,Head);
		clear_table();
		mise_en_page();
		*page=1;
		show_pages(F,*Head,page,*filt);
		return;
	}
	
	if(p.y==45 && p.x>=148 && p.x<=159)///Clear filter
	{
		delay(100);
		block->Pub_year_max=0;
		block->Pub_year_min=0;
		block->Document_id=0;
		sprintf(block->Domaine,"");
		sprintf(block->Author,"");
		sprintf(block->Type,"");
		sprintf(temp->Domaine,"");
		sprintf(temp->Author,"");		
		sprintf(temp->Document_id,"");
		sprintf(temp->Pub_year_max,"");
		sprintf(temp->Pub_year_min,"");
		sprintf(temp->Type,"");
		*filt=false;
		unfilter();
		filter();
		show_pages(F,*Head,page,*filt);
		return;
	}
	
	
}
//\033[%d;%dH
int main()
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	show_list *result=NULL;
	int i,page=480;
	bool f,filt=false;
	
	fichier_lnof *F;
	filtre value=(filtre){0,{'\0'},{'\0'},{'\0'},0,0};
	char string[100]="";
	temporary temp=(temporary){{'\0'},{'\0'},{'\0'},{'\0'},{'\0'},{'\0'} };
	fullscreen();
	sleep(1);
	open_lnof(&F,"DOCUMENTS_LIBRARY.BIN",'E');
	
	mise_en_page();
	f=true;
	filter();
	show_pages(F,result,&page,filt);
	do
	{	
		if(left_click())
		{
		 mouse_move(&p);
		 press_affect(&value,&temp,&filt,F,&result,&page);
		}	
		if(mouse_move(&p))
		{
			printf("\033[46;70H%d,%d,",p.x,p.y);
		}
		
		
		 
			
		if(kbhit())
		{
			if(Right())
			{
				page++;
				clear_table();
				mise_en_page();
				show_pages(F,result,&page,filt);
				clear_stdin();
				continue;
			}
			
			
			if(Left())
			{
				page--;
				clear_table();
				mise_en_page();
				show_pages(F,result,&page,filt);
				clear_stdin();
				continue;
			}
			
			if(getch()=='d')
			{
				if(f)
				{
					unfilter(1);
					show_pages(F,result,&page,filt);
					f=false;
				}
				else
				{
					filter(1);
					f=true;
				}
			}
			clear_stdin();
		}
	}while(1);
	
	getch();
}