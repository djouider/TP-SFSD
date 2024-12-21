#include "mouse.c"

void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = number_of_seconds;

    // Storing start time
    clock_t start_time = clock();

    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}


int Up()
{
	if (GetKeyState(VK_UP) < 0) {
	return 1;
	}
	return 0;
}

int Down()
{
	if (GetKeyState(VK_DOWN) < 0) {
	return 1;
	}
	return 0;
}
int Left()
{
	if (GetKeyState(VK_LEFT) < 0) {
	return 1;
	}
	return 0;
}
int Right()
{
	if (GetKeyState(VK_RIGHT) < 0) {
	return 1;
	}
	return 0;
}
int Back()
{
	if (GetKeyState(VK_BACK) < 0) {
	return 1;
	}
	return 0;
}

int Enter()
{
	if (GetKeyState(VK_RETURN) < 0) {
	return 1;
	}
	return 0;
}

int Esc()
{
	if (GetKeyState(VK_ESCAPE) < 0) {
	return 1;
	}
	return 0;
}


void decalage(int start, int max, char string[])
{
	while(start<max-1)
	{
		string[start]=string[start+1];
		start++;
	}
}

void print_void(int max)
{
	int i=0;
	while(i<max)
	{
		printf("%c",' ');
		i++;
	}
}

void Back_read(POINT Start,char string[], int max)
{
	char temp[max], c;
	int i=0,j=0;
	POINT p;
	Start.x++;
	Start.y++;
	delay(100);
	for(i=0;i<max;i++)
	{
		temp[i]='\0';
	}
	i=0;
	sprintf(temp,string);
	printf("\033[%d;%dH%s", Start.y, (Start.x),temp);
	do{
		printf("\033[%d;%dH", Start.y, (Start.x)+i);
		
		
		
		
		if(left_click()) //change position by mouse
		{
			mouse_move(&p);
			if((p.y==Start.y) && ((p.x<=(Start.x+max)) && (p.x>=Start.x))) // check if the click is inside the read area
			{
				if(p.x-Start.x<=(strlen(temp)))
				{
				printf("%c",temp[i]);
				i=p.x-Start.x;
				clear_stdin();
				}
				continue;
			}
			else // if not leave and apply changes
			{
				temp[max]='\0';
				sprintf(string,temp);
				printf("\033[%d;%dH", Start.y, Start.x);
				printf("%s",string);
				break;
			}
		}
		
		if(kbhit())
		{
			
			
		if(Left())
		{
		if(i>0)
		{
		printf("\033[0m%c",temp[i]);
		i--;
		}
		clear_stdin();
		continue;
		}
		
		if(Right())
		{
		if(i<max-1 && i<strlen(temp))
		{
		printf("\033[0m%c",temp[i]);
		i++;
		}
		clear_stdin();
		continue;
		}
		
		if(Back()) // Delete (go back + decalage)
		{
			if(i)
			{
			i--;
			decalage(i,max,temp);

			printf("\033[%d;%dH", Start.y, Start.x);
			print_void(max-1);
			printf("\033[%d;%dH", Start.y, Start.x);
			printf("%s",temp);
			}
			clear_stdin();
			continue;
		}
		
		if (Esc()) //esc(leave without applaying changes)
		{
			printf("\033[0m \033[%d;%dH", Start.y, Start.x);
			printf("%s",string);
			break;
		}
		if(Enter()) //enter(apply changes and leave)
			{
				temp[max]='\0';
				sprintf(string,temp);
				printf("\033[0m \033[%d;%dH", Start.y, Start.x);
				printf("%s",string);
				break;
			}
			
		if(right_click()) //technical problem( to not be read with normale chars)
		{
			clear_stdin();
			continue;
		}
			
			
		//normale char
		{
			c=getch();
				if(i<max-1)
				{
				temp[i]=c;
				printf("\033[0m%c", c);
				i++;
				}
		}
		}
	}while(1!=2);
}

void read_prompt(int start_x,int start_y,int length)
{
	int i;
	printf("\033[%d;%dH",start_y,start_x);
	for(i=0;i<length;i++)
	{
	printf("\033[%d;%dH*",start_y,start_x+i);
	printf("\033[%d;%dH*",start_y+2,start_x+i);
	}
	printf("\033[%d;%dH*",start_y+1,start_x);
	printf("\033[%d;%dH*",start_y+1,start_x+length-1);
	
	printf("\033[1;37m\033[%d;%dH Enter here\033[0m",start_y+1,start_x+1);
}

int length(char *string[])
{
	int i=0;
	while( string[i][0]!='\0')
	{
		i++;
	}
	return i;
}

void unlist(POINT Start)
{
	int i,j;
	
	for(i=106;i<=170;i++)
	{
		for(j=29;j<=43;j++)
		{
			printf("\033[%d;%dH ",j,i);
		}
	}
}


int list(POINT Start,char *choices[])
{	
	int i,j,phase;
	POINT position;//wich choices and at what part of it
	int curr=1,curr_line=1,max=31,lines=14;
	bool exit=false;
	position.x=0;
	position.y=0;
	delay(100);
	if(length(choices)<10)
	{
		max=31;
		phase=15;
	}
	else
	{
		max=29;
		phase=-2;
	}
	
	while(!exit)
	{	
		while(curr_line<=lines && position.x<length(choices))
		{
			printf("\033[%d;%dH",Start.y+curr_line,Start.x+phase);
			if(position.y==0) ///new choice
			{
				if(curr==position.x+1)	printf("\033[0;31m%**-\033[0m");
				else	printf("%d-",position.x+1);
				i=0;
				while(i<=max && choices[position.x][i]!='\0')
				{
					printf("%c",choices[position.x][i]);
					i++;
				}
				if(choices[position.x][i]=='\0') ///next choice
				{
					position.y=0;
					position.x++;
				}
				else
				{
					position.y=i;
				}
			}
			else ///continue last choice
			{
				while(i%max !=0 && choices[position.x][i]!='\0')
				{
					i--;
					printf("%c",choices[position.x][i]);
					i++;
					i++;
				}
				printf("%c",choices[position.x][i-1]);
				if(choices[position.x][i]=='\0') ///next choice
				{
					position.y=0;
					position.x++;
				}
				else
				{
					i++;
					position.y=i;
				}
				
			}
			curr_line++;
		}
		if(position.x<length(choices))
		{
			for(j=0;j<lines;j++)
			{
				printf("\033[%d;%dH*",Start.y+j,Start.x+29);
			}
			phase=30;
			curr_line=1;
			continue;
		}
		
		do
		{
			if(length(choices)>=10)
			{
				max=29;
				phase=-2;
			}
			curr_line=1;
			position.x=0;
			position.y=0;
			
			if(kbhit()) /////keyboard navigation
			{
				if(Up())
				{
					if(curr>1) curr--;
					clear_stdin();
					break;
				}
				if(Down())
				{
					if (curr<length(choices)) curr++;
					clear_stdin();
					break;
				}
			}
			
			if(mouse_move(&p))
			{
				if(phase==-2) //////////Domaine
				{
					if(p.y>=26 && p.y<=43 && p.x>=106 && p.x<=165) //mouse inside the box
					{
						if(p.x>=136)
						{
							curr=(p.y-30)/2+9;
							if(curr>13) curr=0;
						}
						else
						{
							if(p.y ==30 || p.y==31) curr=1;
							if(p.y ==32 || p.y==33) curr=2;	
							if(p.y ==34 ) curr=3;
							if(p.y ==35) curr=4;
							if(p.y ==36 || p.y==37) curr=5;
							if(p.y ==38) curr=6;
							if(p.y ==39 || p.y==40) curr=7;
							if(p.y ==41 || p.y==42 || p.y==43) curr=8;
						}
						break;
					}
					else //// mouse outside the box
					{
						if(curr)
						{
							curr=0;
							break;
						}
						
					}
				}
				else ///////Type
				{
					if(p.y>=26 && p.y<=35 && p.x>=123 && p.x<=154) //inside the box
					{
						curr=p.y-30+1;
						break;
					}
					else /////mouse outside the box
					{
						if(curr)
						{
							curr=0;
							break;
						}
						
					}
				}
			}
		if(left_click())
		{
			exit=1;
			break;
		}
		if(Enter())
		{
			exit=1;
			break;
		}

		}while(1);
	}
	unlist(Start);
	return curr;
}

/*int main()
{
	POINT p;
	p.x=0;
	p.y=0;
	mouse_coordinates(&p);
	fullscreen();
	char string[10]="bruh";
	printf("bruh");
	Back_read(p,string,10);
}*/