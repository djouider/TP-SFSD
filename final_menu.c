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


int convert_mouse_to_line()
{
	if((p.y==8) && (p.x>=19) && (p.x<=89))
	{
	return 1;
}
	if((p.y==10) && (p.x>=19) && (p.x<=89))
	{
	return 2;
	}
	if((p.y==12) && (p.x>=19) && (p.x<=89))
	{
	return 3;
	}
	if((p.y==14) && (p.x>=19) && (p.x<=89))
	{
	return 4; 
	}
	if((p.y==16) && (p.x>=19) && (p.x<=89))
	{
	return 5;
	}
	return 0;
}


void check(int fs,int *u,int *d,int *c,int *m,int *k)

{
	if(kbhit())
	{
	if(Up())
	{
		*u=1;
		return;
	}
	if(Down())
	{
		*d=1;
		return;
	}
	if(Enter())
	{
		*k=1;
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

void menu_0(const char Color1[],const char Color2[],const char Head[],const char command1[],const char command2[],const char command3[],const char command4[],const char command5[])
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
	
	
	printf("\n		%s//////////////////////////////////////////////////////////////////////////\033[1;0m",Color1);
	printf("\n		%s/\033[1;0m                                                                        %s/\033[1;0m",Color1,Color1);
	printf("\n		%s/\033[1;0m%s%s%s\033[1;0m%s%s/\033[1;0m",Color1,space0a,Color2,Head,space0b,Color1);
	printf("\n		%s/\033[1;0m                                                                        %s/\033[1;0m",Color1,Color1);
	printf("\n		%s//////////////////////////////////////////////////////////////////////////\033[1;0m",Color1);
	printf("\n		%s/\033[1;0m                                                                        %s/\033[1;0m",Color1,Color1);
	printf("\n		%s/\033[1;0m  %s%s%s%s/\033[1;0m",Color1,header_1,command1,space1,Color1);
	printf("\n		%s/\033[1;0m                                                                        %s/\033[1;0m",Color1,Color1);
	printf("\n		%s/\033[1;0m  %s%s%s%s/\033[1;0m",Color1,header_2,command2,space2,Color1);
	printf("\n		%s/\033[1;0m                                                                        %s/\033[1;0m",Color1,Color1);
	printf("\n		%s/\033[1;0m  %s%s%s%s/\033[1;0m",Color1,header_3,command3,space3,Color1);
	printf("\n		%s/\033[1;0m                                                                        %s/\033[1;0m",Color1,Color1);
	printf("\n		%s/\033[1;0m  %s%s%s%s/\033[1;0m",Color1,header_4,command4,space4,Color1);
	printf("\n		%s/\033[1;0m                                                                        %s/\033[1;0m",Color1,Color1);
	printf("\n		%s/\033[1;0m  %s%s%s%s/\033[1;0m",Color1,header_5,command5,space5,Color1);
	printf("\n		%s/\033[1;0m                      	                                                 %s/\033[1;0m",Color1,Color1);
	printf("\n		%s//////////////////////////////////////////////////////////////////////////\033[1;0m",Color1);
}

 int menu (int fs,const char Color1[],const char Color2[],const char Head[],const char command1[],const char command2[],const char command3[],const char command4[],const char command5[])
{
	char mov;
	
	int l;
	
	int last=0,u=0,d=0,m=0,c=0,k=0;
	l=0;
	menu_0(Color1,Color2,Head,command1,command2,command3,command4,command5);
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
					check(fs,&u,&d,&c,&m,&k);
				}while((!m) && (!c) && (!d) && (!u) && (!k));

				break;
			
			case 1: 
				do{
					printf("\e[8;19H \033[0;31m-->%s\033[0m",command1);
					Sleep(100);
					printf("\e[8;19H ---%s",command1);
					Sleep(100);
					check(fs,&u,&d,&c,&m,&k);
					}while((!u) && (!d) && (!m) && (!c) && (!k));

				break;
			
			
	
			case 2:
				do{
					printf("\e[10;19H \033[0;31m-->%s\033[0m",command2);
					Sleep(100);
					printf("\e[10;19H ---%s",command2);
					Sleep(100);
					check(fs,&u,&d,&c,&m,&k);
				}while((!u) && (!d) && (!m) && (!c) && (!k));
	
				break;
			
			case 3:
				do{
					printf("\e[12;19H \033[0;31m-->%s\033[0m",command3);
					Sleep(100);
					printf("\e[12;19H ---%s",command3);
					Sleep(100);
					check(fs,&u,&d,&c,&m,&k);
				}while((!u) && (!d) && (!m) && (!c) && (!k));

				break;
				
			case 4:
				do{
					printf("\e[14;19H \033[0;31m-->%s\033[0m",command4);
					Sleep(100);
					printf("\e[14;19H ---%s",command4);
					Sleep(100);
					check(fs,&u,&d,&c,&m,&k);
				}while((!u) && (!d) && (!m) && (!c) && (!k));

				break;
			case 5:
				do{
					printf("\e[16;19H \033[0;31m-->%s\033[0m",command5);
					Sleep(100);
					printf("\e[16;19H ---%s",command5);
					Sleep(100);
					check(fs,&u,&d,&c,&m,&k);
				}while((!u) && (!d) && (!m) && (!c) && (!k));
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
					l=convert_mouse_to_line();
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
	}while (((!k) && (!c)) || (!(l)));
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


