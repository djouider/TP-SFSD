#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "time.c"

typedef struct typef_filtre {
    long int Document_id;
    char Author[31];
    char Type[30];
    char Domaine[82];
    int Pub_year_max;
    int Pub_year_min;
    bool Del;
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


void mise_en_page(int fs)
{
	int i;
	if(fs)//fullscreen mode
	{
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
			printf("\033[%d;%dH ",i,42);
			}
			printf("\033[%d;%dH ",i,100);
		}
		
		printf("\033[0m\033[%d;%dHID",3,5);
		printf("\033[%d;%dHType",3,24);
		printf("\033[%d;%dHDomaine",3,65);
		printf("\033[%d;%dHPage :",46,3);
		printf("\033[%d;%dH<-- Previous ; Next--> ",46,35);
		printf("\033[%d;%dHNumber of books: ",46,70);
		printf("\033[%d;%dHPress D to show/hide filter: ",1,121);
	}
	
	else /////Windowed mode
	{
		printf("\033[44m");
		for(i=1;i<75;i++)
		{
			printf("\033[%d;%dH ",1,i);
			printf("\033[%d;%dH ",5,i);
			printf("\033[%d;%dH ",26,i);
			printf("\033[%d;%dH ",30,i);
		}
		for(i=1;i<31;i++)
		{
			printf("\033[%d;%dH ",i,1);
			if(i<=25)
			{
			printf("\033[%d;%dH ",i,12);
			printf("\033[%d;%dH ",i,40);
			}
			printf("\033[%d;%dH ",i,75);
		}
	
		printf("\033[0m\033[%d;%dHID",3,5);
		printf("\033[%d;%dHType",3,24);
		printf("\033[%d;%dHDomaine",3,55);
		printf("\033[%d;%dHPage :",28,3);
		printf("\033[%d;%dH<-- Previous ; Next--> ",28,20);
		printf("\033[%d;%dHNumber of books: ",28,48);
	}
}

void filter(int fs) ///to show filter prompt
{
	if (fs) ///Fullscreen mode
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

		printf("\033[45;135H\033[44mSearch\033[0m ");
	}
}
void unfilter(int fs) ///to hide filter prompt
{
	system("cls");
	mise_en_page(fs);
}


void press_affect(filtre *block,temporary *temp)
{
	char *type[7] = {"Ouvrages et manuels","Revues et periodiques",
    "Actes de conferences","Rapports de recherche",
    "Memoires et theses","Polycopies et support de cours",""
    } ;
	
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
	}
	
	if(p.y==28 && p.x>=128 && p.x<=133)////Type's prompt
	{
		p.x=108;
		p.y=29;
		list(p,type);
	}
	
	if(p.y==28 && p.x>=140 && p.x<=148)////Domaine's prompt
	{
		p.x=108;
		p.y=29;
		list(p,domaine);
	}
	
	
	
}
//\033[%d;%dH
int main()
{
	int i;
	bool f;
	filtre value;
	char string[100]="";
	temporary temp=(temporary){{'\0'},{'\0'},{'\0'},{'\0'},{'\0'},{'\0'} };
	fullscreen();
	sleep(1);
	
	
	mise_en_page(1);
	f=true;
	filter(1);
	do
	{
		if(left_click())
		{
		 mouse_move(&p);
		 press_affect(&value,&temp);
		}
			
		if(mouse_move(&p))
		{
		 printf("\033[46;70H%d,%d,",p.x,p.y);
		}
		
		
		 
			
		if(kbhit())
		{
		
			if(getch()=='d')
			{
				if(f)
				{
					unfilter(1);
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