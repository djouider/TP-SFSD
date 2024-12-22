#include "History.c"

void scanf_time(char *c,int t)
{
	bool exit = false; 
int loops = 0; 
const int maxWait = 10; 
while( !exit ) 
{ 
    if( kbhit() ) // did user press a key? 
    { 
        *c = getch(); // we know user pressed a key, so getch() doesn't need to wait 
 
        // do with input whatever needs be done 
    } 
    Sleep(t); // wait 10ms 
    exit = exit || ( ++loops > maxWait ); 
}
}


int convert_mouse_to_line(int start_x,int start_y)
{
	if((p.y==start_y+7) && (p.x>=start_x+10) && (p.x<=start_x+83))
	{
	return 1;
}
	if((p.y==start_y+9) && (p.x>=start_x+10) && (p.x<=start_x+83))
	{
	return 2;
	}
	if((p.y==start_y+11) && (p.x>=start_x+10) && (p.x<=start_x+83))
	{
	return 3;
	}
	if((p.y==start_y+13) && (p.x>=start_x+10) && (p.x<=start_x+83))
	{
	return 4; 
	}
	if((p.y==start_y+15) && (p.x>=start_x+10) && (p.x<=start_x+83))
	{
	return 5;
	}
	return 0;
}


void check(int fs,int *u,int *d,int *c,int *m,int *k,int *exit)

{
	if(kbhit())
	{
	if(Up())
	{
		*u=1;
		clear_stdin();
		return;
	}
	if(Down())
	{
		*d=1;
		clear_stdin();
		return;
	}
	if(Enter())
	{
		*k=1;
		clear_stdin();
		return;
	}
	if(Esc())
	{
		*exit=1;
		clear_stdin();
		return;
	}
	}
	if(fs)
	{
	if(left_click())
	{
		*c=1;
		return;
	}
	if(mouse_move(&p))
	{
		*m=1;
		return;
	}
	}
	return;
}

void menu_0(int start_x,int start_y,const char Color1[],const char Color2[],const char Head[],const char command1[],const char command2[],const char command3[],const char command4[],const char command5[])
{
	char header_1[100],header_2[100],header_3[100],header_4[100],header_5[100],space1[75],space2[75],space3[75],space4[75],space5[75],space0b[75],space0a[75];
	int i;	
	if(command1==""){
		sprintf(header_1,"");
	}
	else{
		sprintf(header_1,"---");
	}
	
	if(command2==""){
		sprintf(header_2,"");
	}
	else{
		sprintf(header_2,"---");
	}
	
	if(command3==""){
		sprintf(header_3,"");
	}
	else{
		sprintf(header_3,"---");
	}
	
	if(command4==""){
		sprintf(header_4,"");
	}
	else{
		sprintf(header_4,"---");
	}
	
		if(command5==""){
		sprintf(header_5,"");
	}
	else{
		sprintf(header_5,"---");
	}
	///////////////////Head space////////////
		i=0;
	do
	{
		space0a[i]=' ';
		i++;
	}while(i<=(int)((71-strlen(Head))/2));
	space0a[i]='\0';
	
	i=0;
	do
	{
		space0b[i]=' ';
		i++;
	}while(i<=71-strlen(space0a)-strlen(Head));
	space0b[i]='\0';
	///////////////////////////////////////////////
	
		/////////////////////spaces///////////
	i=0;
	do
	{
		space1[i]=' ';
		i++;
	}while(i<=(69-strlen(header_1)-strlen(command1)));
	space1[i]='\0';
	i=0;
	do
	{
		space2[i]=' ';
		i++;
	}while(i<=(69-strlen(header_2)-strlen(command2)));
	space2[i]='\0';
	i=0;
	do
	{
		space3[i]=' ';
		i++;
	}while(i<=(69-strlen(header_3)-strlen(command3)));
	space3[i]='\0';
	i=0;
	do
	{
		space4[i]=' ';
		i++;
	}while(i<=(69-strlen(header_4)-strlen(command4)));
	space4[i]='\0';
	i=0;
	do
	{
		space5[i]=' ';
		i++;
	}while(i<=(69-strlen(header_5)-strlen(command5)));
	space5[i]='\0';
	

	/////////////////////////////////
		printf("\e[%d;%dH",start_y+1,start_x);
	printf("		%s//////////////////////////////////////////////////////////////////////////\033[1;0m",Color1);
		printf("\e[%d;%dH",start_y+2,start_x);
	printf("		%s/\033[1;0m                                                                        %s/\033[1;0m",Color1,Color1);
		printf("\e[%d;%dH",start_y+3,start_x);
	printf("		%s/\033[1;0m%s%s%s\033[1;0m%s%s/\033[1;0m",Color1,space0a,Color2,Head,space0b,Color1);
		printf("\e[%d;%dH",start_y+4,start_x);
	printf("		%s/\033[1;0m                                                                        %s/\033[1;0m",Color1,Color1);
		printf("\e[%d;%dH",start_y+5,start_x);
	printf("		%s//////////////////////////////////////////////////////////////////////////\033[1;0m",Color1);
		printf("\e[%d;%dH",start_y+6,start_x);
	printf("		%s/\033[1;0m                                                                        %s/\033[1;0m",Color1,Color1);
		printf("\e[%d;%dH",start_y+7,start_x);
	printf("		%s/\033[1;0m  %s%s%s%s/\033[1;0m",Color1,header_1,command1,space1,Color1);
		printf("\e[%d;%dH",start_y+8,start_x);
	printf("		%s/\033[1;0m                                                                        %s/\033[1;0m",Color1,Color1);
		printf("\e[%d;%dH",start_y+9,start_x);
	printf("		%s/\033[1;0m  %s%s%s%s/\033[1;0m",Color1,header_2,command2,space2,Color1);
		printf("\e[%d;%dH",start_y+10,start_x);
	printf("		%s/\033[1;0m                                                                        %s/\033[1;0m",Color1,Color1);
		printf("\e[%d;%dH",start_y+11,start_x);
	printf("		%s/\033[1;0m  %s%s%s%s/\033[1;0m",Color1,header_3,command3,space3,Color1);
		printf("\e[%d;%dH",start_y+12,start_x);
	printf("		%s/\033[1;0m                                                                        %s/\033[1;0m",Color1,Color1);
		printf("\e[%d;%dH",start_y+13,start_x);
	printf("		%s/\033[1;0m  %s%s%s%s/\033[1;0m",Color1,header_4,command4,space4,Color1);
		printf("\e[%d;%dH",start_y+14,start_x);
	printf("		%s/\033[1;0m                                                                        %s/\033[1;0m",Color1,Color1);
		printf("\e[%d;%dH",start_y+15,start_x);
	printf("		%s/\033[1;0m  %s%s%s%s/\033[1;0m",Color1,header_5,command5,space5,Color1);
		printf("\e[%d;%dH",start_y+16,start_x);
	printf("		%s/\033[1;0m                      	                                                 %s/\033[1;0m",Color1,Color1);
		printf("\e[%d;%dH",start_y+17,start_x);
	printf("		%s//////////////////////////////////////////////////////////////////////////\033[1;0m",Color1);
}

 int menu (int fs,const char Color1[],const char Color2[],const char Head[],const char command1[],const char command2[],const char command3[],const char command4[],const char command5[])
{
	char mov;
	int start_x=0,start_y=0;
	int l;
	
	int last=0,u=0,d=0,m=0,c=0,k=0,exit=0;
	l=0;
	if(fs)
	{
		start_x=40;
		start_y=10;
	}
	else
	{
		start_x=8;
		start_y=3;
	}
	menu_0(start_x,start_y,Color1,Color2,Head,command1,command2,command3,command4,command5);
	mouse_move(&p);

	do{	
		m=0;
		d=0;
		u=0;
		c=0;
		k=0;
	clear_stdin();
	
	switch(l)
	
		{
			case 0:
				do
				{
					check(fs,&u,&d,&c,&m,&k,&exit);
				}while((!m) && (!c) && (!d) && (!u) && (!k) && (!exit));

				break;
			
			case 1: 
				do{
					printf("\e[%d;%dH \033[0;31m-->%s\033[0m",start_y+7,start_x+11,command1);
					check(fs,&u,&d,&c,&m,&k,&exit);
					Sleep(100);
					printf("\e[%d;%dH ---%s",start_y+7,start_x+11,command1);
					check(fs,&u,&d,&c,&m,&k,&exit);
					Sleep(100);
					check(fs,&u,&d,&c,&m,&k,&exit);
					}while((!m) && (!c) && (!d) && (!u) && (!k) && (!exit));

				break;
			
			
	
			case 2:
				do{
					printf("\e[%d;%dH \033[0;31m-->%s\033[0m",start_y+9,start_x+11,command2);
					check(fs,&u,&d,&c,&m,&k,&exit);
					Sleep(100);
					printf("\e[%d;%dH ---%s",start_y+9,start_x+11,command2);
					check(fs,&u,&d,&c,&m,&k,&exit);
					Sleep(100);
					check(fs,&u,&d,&c,&m,&k,&exit);
				}while((!m) && (!c) && (!d) && (!u) && (!k) && (!exit));
	
				break;
			
			case 3:
				do{
					printf("\e[%d;%dH \033[0;31m-->%s\033[0m",start_y+11,start_x+11,command3);
					check(fs,&u,&d,&c,&m,&k,&exit);
					Sleep(100);
					printf("\e[%d;%dH ---%s",start_y+11,start_x+11,command3);
					check(fs,&u,&d,&c,&m,&k,&exit);
					Sleep(100);
					check(fs,&u,&d,&c,&m,&k,&exit);
				}while((!m) && (!c) && (!d) && (!u) && (!k) && (!exit));

				break;
				
			case 4:
				do{
					printf("\e[%d;%dH \033[0;31m-->%s\033[0m",start_y+13,start_x+11,command4);
					check(fs,&u,&d,&c,&m,&k,&exit);
					Sleep(100);
					printf("\e[%d;%dH ---%s",start_y+13,start_x+11,command4);
					check(fs,&u,&d,&c,&m,&k,&exit);
					Sleep(100);
					check(fs,&u,&d,&c,&m,&k,&exit);
				}while((!m) && (!c) && (!d) && (!u) && (!k) && (!exit));

				break;
			case 5:
				do{
					printf("\e[%d;%dH \033[0;31m-->%s\033[0m",start_y+15,start_x+11,command5);
					check(fs,&u,&d,&c,&m,&k,&exit);
					Sleep(100);
					printf("\e[%d;%dH ---%s",start_y+15,start_x+11,command5);
					check(fs,&u,&d,&c,&m,&k,&exit);
					Sleep(100);
					check(fs,&u,&d,&c,&m,&k,&exit);
				}while((!m) && (!c) && (!d) && (!u) && (!k) && (!exit));
				break;
				}
				
				if (d){
				last=l;
				l=l+1;
				}
				if(u){
				last=l;
				l=l-1;
				}
				if(m)
				{
					l=convert_mouse_to_line(start_x,start_y);
				}			
				
				if(l==-1)
				{
				 l=5;
				}
				if(l==6)
				{
					l=0;
				}
		
		if(u || d)
		{		
		if ((l==1) && (last==0) && (command1==""))
		{
			last=1;
			l=2;
		}
		if ((l==2) && (last==1) && (command2==""))
		{
			last=2;
			l=3;
		}

		if ((l==3) && (last==2) && (command3==""))
		{
			last=3;
			l=4;
		}

		if ((l==4) && (last==3) && (command4==""))
		{
			last=4;
			l=5;
		}
		if ((l==5) && (last==4) && (command5==""))
		{
			last=0;
			l=0;
		}
		
			if ((l==4) && (last==5) && (command4==""))
		{
			last=4;
			l=3;
		}
		
		if ((l==3) && (last==4) && (command3==""))
		{
			last=3;
			l=2;
		}
		
		if ((l==2) && (last==3) && (command2==""))
		{
			last=2;
			l=1;
		}
		
		if ((l==1) && (last==2) && (command1==""))
		{
			last=0;
			l=0;
		}
		}
		
			if((m))
		{
			if ((l==1) && (command1==""))
			{
				l=0;
			}
			if ((l==2) && (command2==""))
			{
				l=0;
			}
	
			if ((l==3) && (command3==""))
			{
				l=0;
			}
	
			if ((l==4) && (command4==""))
			{
				l=0;
			}
			if ((l==5) && (command5==""))
			{
				l=0;
			}
		}
		
		if(exit)
		{
			l=5;
		}
	}while ( (!exit)	&& ( (  (!k) && (!c) ) || (!l)	)	);///if Esc if pressed or (Enter or left click and not position 0) exit
	return l;
}

/*int main()
{
	int choice;
	fullscreen();
	choice=menu ("\033[0;32m","\033[0;34m", "BRUH","","ui","fu","DOO","haa");
	printf("\e[20;19H");
	return choice;
}*/


