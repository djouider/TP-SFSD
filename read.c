#include "mouse.c"



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
	while(start<=max)
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
	sprintf(temp,string);
	do{
		printf("\033[%d;%dH", Start.y, (Start.x)+i);
		
		
		
		
		if(left_click()) //change position by mouse
		{
			mouse_move(&p);
			if((p.y==Start.y) && ((p.x<=(Start.x+max)) && (p.x>=Start.x))) // check if the click is inside the read area
			{
				printf("%c",temp[i]);
				i=p.x-Start.x;
				clear_stdin();
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
		if(i<max-2)
		{
		printf("\033[0m%c",temp[i]);
		i++;
		}
		clear_stdin();
		continue;
		}
		
		if(Back()) // Delete (go back + decalage)
		{
			decalage(i,max,temp);

			printf("\033[%d;%dH", Start.y, Start.x);
			print_void(max-1);
			printf("\033[%d;%dH", Start.y, Start.x);
			printf("%s",temp);
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
			
		if(right_click()) //technical problem
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
				if (i==max-1)i--;
		}
		}
	}while(1!=2);
}


/*int main()
{
	POINT p;
	p.x=0;
	p.y=0;
	mouse_coordinates(&p);
	fullscreen();
	char string[]="bruh";
	printf("bruh");
	Back_read(p,string,5);
}*/