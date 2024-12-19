#include "read.c"

#define b 40
typedef struct _cost
{
	int read;
	int write;
} cost;

typedef struct h_enreg 
{
    char date[20];
    char Operation[20];
    char number;
    cost cout;
    char time[20];
    char status[10];
} henreg;

typedef struct type_block_htof {
    henreg Tab[b];
    int nb;
} block_htof;

typedef struct type_hhdr {
	
	int nblck;
    int nrec;
    
} header_htof ;

typedef struct htof_file{
    FILE *f;
    header_htof h;
} fichier_htof ;

void open_htof(fichier_htof **F,char filename[], char mode){ //F->f will be pointing after the header
    *F = malloc(sizeof (fichier_htof));
    if (mode == 'E' || mode == 'e'){ // create a new htof file
        (*F)->f = fopen(filename,"rb+");
        if ((*F)->f == NULL) {
            printf("there was a problem opening the file");
            exit( EXIT_FAILURE );
        }
        fread(&((*F)->h),sizeof(header_htof),1,(*F)->f);
    }
    else { //mode New 
        (*F)->f = fopen(filename,"wb+");
        if ((*F)->f == NULL) {
            printf("there was a problem opening the file");
            exit( EXIT_FAILURE );
        }
        //initialize the header
        (*F)->h.nblck = 0;
        (*F)->h.nrec = 0;
        fwrite(&((*F)->h),sizeof(header_htof),1,(*F)->f);
        block_htof buffer;
        buffer.nb =0;
        fwrite(&buffer,sizeof(block_htof),1,(*F)->f);
    }

}

void close_htof(fichier_htof *F){
    rewind(F->f);
    fwrite(&F->h, sizeof(header_htof),1,F->f);
    fclose(F->f);
    printf("\nfile closed succefly\n");
    free(F);
}

int get_Header_htof(fichier_htof *F,char *field){
	
    if (strcmp(field, "nrec") == 0) return F->h.nrec;
    if (strcmp(field, "nblck") == 0) return F->h.nblck;
    
    fprintf(stderr, "setHeader : Unknown headerName: \"%s\"\n", field);
}

void set_Header_htof(fichier_htof *F,char *field,int val){

	if (strcmp(field, "nblck") == 0) {F->h.nblck = val ; return;}
    if (strcmp(field, "nrec") == 0) {F->h.nrec = val ; return;}
 
    fprintf(stderr, "setHeader : Unknown headerName: \"%s\"\n", field);
}

void Read_Block_htof(fichier_htof *F,block_htof *buffer,int i){
    if (i<0 || i>get_Header_htof(F,"nblck") ){fprintf(stderr, "Read_block_lnof : number of block does not exist: \"%d\"\n", i);; return;}
    fseek(F->f,sizeof(header_htof) + i*sizeof(block_htof),SEEK_SET);
    fread(buffer,sizeof(block_htof),1,F->f);
}

void Write_Block_htof(fichier_htof *F,block_htof *buffer,int i){
    if (i<0 || i>get_Header_htof(F,"nblck") ){fprintf(stderr, "Write_block_lnof : number of block does not exist: \"%d\"\n", i);; return;}
    fseek(F->f,sizeof(header_htof) + i*sizeof(block_htof),SEEK_SET);
    fwrite(buffer,sizeof(block_htof),1,F->f);
}

void Alloc_block_htof (fichier_htof *F){
    block_htof buf;
    buf.nb =0;
    set_Header_htof(F,"nblck",get_Header_htof(F,"nblck")+1);
    Write_Block_htof(F,&buf,get_Header_htof(F,"nblck"));
}

void write_hbuffer(block_htof *buf,henreg val,int i)///write a value val in buffer buf in position i
{
	/*sprintf(buf->Tab[i].date,val.date);
	(buf->Tab[i]).number=val.number;
	sprintf(buf->Tab[i].Operation,val.Operation);
	sprintf(buf->Tab[i].status,val.status);
	sprintf(buf->Tab[i].time,val.time);
	(buf->Tab[i]).cout=val.cout;*/
	buf->Tab[i]=val;
	return;
} 

void Add_history(fichier_htof *F,henreg val)
{
	int i;
	block_htof buf;
	i=get_Header_htof(F,"nblck");
	if(!i)
	{
		Alloc_block_htof(F);
		buf.nb=1;
		i++;
		set_Header_htof(F,"nblck",i);
		set_Header_htof(F,"nrec",get_Header_htof(F,"nrec")+1);
		write_hbuffer(&buf,val,0);
		Write_Block_htof(F,&buf,i);
		return;
	}
	Read_Block_htof(F,&buf,i);
	if(buf.nb==b)
	{
		Alloc_block_htof(F);
		buf.nb=1;
		i++;
		set_Header_htof(F,"nblck",i);
		set_Header_htof(F,"nrec",get_Header_htof(F,"nrec")+1);
		write_hbuffer(&buf,val,0);
		Write_Block_htof(F,&buf,i);
		return;	
	}
	set_Header_htof(F,"nrec",get_Header_htof(F,"nrec")+1);
	write_hbuffer(&buf,val,buf.nb);
	buf.nb=buf.nb+1;
	Write_Block_htof(F,&buf,i);
	return;
}

void set_date(henreg *val)
{
   time_t timestamp = time( NULL );
    struct tm * timeInfos = localtime( & timestamp );

    sprintf(val->date, "%04d/%02d/%02d %02d:%02d:%02d\0",
        timeInfos->tm_year+1900, timeInfos->tm_mon+1, timeInfos->tm_mday,
        timeInfos->tm_hour, timeInfos->tm_min, timeInfos->tm_sec
    );     
}




void Show_details(fichier_htof *F,int fs,int blck,int enreg)///Show full details of the value at block number blck and record number enreg
	{
		block_htof buf;
		henreg val;
		
		Read_Block_htof(F,&buf,blck);
		val=buf.Tab[enreg];
		system("cls");
		printf("%d %d",blck,enreg);
		if(fs)
		{
			if(strcmp(val.Operation,""))
			{
			}///to determine later
			
			
			printf("\n\n\n\n\n\n										Status : %s\n\n\n",val.status);
			printf("								Time of start : %s\n\n\n",val.date);
			printf("									Time took :%s\n\n\n",val.time);
			printf("								Cost : %d writes/ %d read\n\n\n",val.cout.write,val.cout.read);
			printf("							Operation : %s\n",val.Operation);
		}
		else
		{
			printf("\n\n\n						Status : %s\n\n\n",val.status);
			printf("				Time of start : %s\n\n\n",val.date);
			printf("					Time took :%s\n\n\n",val.time);
			printf("					Cost : %d writes/ %d read\n\n\n",val.cout.write,val.cout.read);
			printf("			Operation : %s\n",val.Operation);
		}
		
		printf("\n\n click anything to go back");
		getch();
	}




void show_history(fichier_htof *F,int fs)
{
	block_htof buf;
	int i=0,j=0,k=0,max,page,curr=0,curr_page=0,max_page=0,select=0;
	int s=0,exit=0;
	
	
	if(fs)
	{
		page=40;
	}
	else{
		page=12;
	}
	max=get_Header_htof(F,"nblck");
	max_page=get_Header_htof(F,"nrec")/page+1;
	if(max) //file no empty read first block
	{
		i=1;
		Read_Block_htof(F,&buf,i);
	}
	else ///empty file
	{
		system("cls");
		printf("			The history is empty");
		printf("\n\n\nPress anything to continue");
		getch();
		return;
	}

	do
	{
		select=0;
		system("cls");
		printf("						Commands history\n\nchoose a specific command to see full details \n\n");
		i=(curr)/b+1;
		Read_Block_htof(F,&buf,i);
		for(j=0;j<page;j++)
		{
			if(	( (j+curr)%b==0 || (j+curr)%b==buf.nb)  )//End of block
			{
				
				if((j+curr)%b==buf.nb) // block not full
				{
					i=curr/b+2;	
				}
				else{ //block  full
				i=(curr+j)/b+1;
				}
				if(i<=max)
				{
				Read_Block_htof(F,&buf,i);
				}
				else	break;
			}
			
			printf("%s : %s ;\n",buf.Tab[(j+curr)%b].date,buf.Tab[(j+curr)%b].Operation);
	
		}
		
		///////next/last page layout///////
		
		curr_page=curr/page+1;
		
		if((curr_page<max_page) && (curr_page>1))	printf("\n\n				 		<--%d-%d-->",curr+1,curr+j);
		
		if((curr_page<max_page) && (curr_page==1))	printf("\n\n						%d-%d-->",curr+1,curr+j);
		
		if((curr_page==max_page) && (curr_page>1))	printf("\n\n						<--%d-%d",curr+1,curr+j);
		
		if((curr_page>max_page) && (curr_page==1))	printf("\n\n						%d-%d",curr+1,curr+j);
		
		printf("\npage: %d/%d",curr_page,max_page);
		/////////////////
		
		
		do
		{
			if(kbhit())
			{
				if (Esc())
				{
					exit=1;
					clear_stdin();
					break;
				}
				
				if (Enter())
				{
					
					clear_stdin();
					if(select)
					{
						s=1;
						break;
					}
				}
				
				if(Left())
				{
					if(curr)
					{
						curr-=page;
						clear_stdin();
						break;
					}
					clear_stdin();
				}
				if(Right())
				{
					if((get_Header_htof(F,"nrec")-curr)>page)
					{
						curr+=j;
						clear_stdin();
						break;
					}
					clear_stdin();
				}
				
				
				if(Up())
				{
					if(select>1)
					{
					printf("\033[%d;%dH", select+4,1);
					printf("%s : %s ;    \n",buf.Tab[(select+curr-1)%b].date,buf.Tab[(select+curr-1)%b].Operation); ///print normale sentence in its place and go next one
					select--;
					printf("\033[%d;%dH", select+4,1);
					printf("--> %s : %s ;\n",buf.Tab[(select+curr-1)%b].date,buf.Tab[(select+curr-1)%b].Operation); ///print new sentence marked
					}
					clear_stdin();
				}
				
				
				if(Down())
				{
					if(!select)
					{
						printf("\033[%d;%dH",5,1);
					printf("--> %s : %s ;\n",buf.Tab[(curr)%b].date,buf.Tab[(curr)%b].Operation); ///print the first sentence marked
					select=1;
					}
					else
					{
					if(select<page)
					{
					printf("\033[%d;%dH", select+4,1);
					printf("%s : %s ;    \n",buf.Tab[(select+curr-1)%b].date,buf.Tab[(select+curr-1)%b].Operation); ///print normale sentence in its place and go next one
					select++;
					printf("\033[%d;%dH", select+4,1);
					printf("--> %s : %s ;\n",buf.Tab[(select+curr-1)%b].date,buf.Tab[(select+curr-1)%b].Operation); ///print new sentence marked
					}
					}
					clear_stdin();
				}
			}
			
		if(fs) /// mouse features
		{
			if(mouse_move(&p))
			{
				if	((p.y<=(buf.nb+4)) && (p.y>4))
				{
					if(select)
					{
					printf("\033[%d;%dH", select+4,1);
					printf("%s : %s ;    \n",buf.Tab[(select+curr-1)%b].date,buf.Tab[(select+curr-1)%b].Operation); ///print normale sentence in its place and go next one
					}
					select=p.y-4;
				}
				
				if(select)
				{
					printf("\033[%d;%dH", select+4,1);
					printf("--> %s : %s ;\n",buf.Tab[(select+curr-1)%b].date,buf.Tab[(select+curr-1)%b].Operation); ///print new sentence marked
				}
				
			}
			
			if(left_click())
			{
				if	((p.y<=(buf.nb+4)) && (p.y>4))
				{
					if(select)	
					{
						s=1;
						break;
					}
				}
			}
			
			if(right_click())
			{
				exit=1;
				break;
			}
		}
		}while(1);
		
		if(s)
		{
			Show_details(F,fs,curr/b+1,curr%b+select-1);
			s=0;
		}
	}while(!exit);
	
}


void Reset_History(fichier_htof *H)
{
	set_Header_htof(H,"nblck",0);
	set_Header_htof(H,"nrec",0);
}

/*int main()
{
	int j=0,fs=0;
	henreg val;
	fichier_htof *F;
	cost cout;
	cout.read=0;
	cout.write=69;
	
	
	fullscreen();
	fs=1;
	open_htof(&F,"History,bin",'N');
	for(j=0;j<=150;j++)
	{
		set_date(&val);
		sprintf(val.Operation,"hzhz%d",j+1);
		val.cout=cout;
		sprintf(val.time,"N/A");
		sprintf(val.status,"Successfull");
		Add_history(F,val);
		//Sleep(10);
	}
	show_history(F,fs);
	close_htof(F);
	return 1;
}*/