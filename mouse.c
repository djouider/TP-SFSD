#include "time.c"
int last_x=0;
int last_y=0;
POINT p;

void scanf_time_stdin(char *c,int t)
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
    	else
	{
	*c=(char)10;	
	} 
    Sleep(t); // wait 10ms 
    exit = exit || ( ++loops > maxWait ); 
} 
}

void clear_stdin()
{
	char c;
	while (kbhit())
	{
		getch();
	}
}



void fullscreen()
{
keybd_event(VK_MENU,0x38,0,0);
keybd_event(VK_RETURN,0x1c,0,0);
keybd_event(VK_RETURN,0x1c,KEYEVENTF_KEYUP,0);
keybd_event(VK_MENU,0x38,KEYEVENTF_KEYUP,0);
}

void resolution( int *width, int *height)
{   
  CONSOLE_SCREEN_BUFFER_INFOEX info = {0};
    HANDLE hConsole = NULL;

    // maximize window
    ShowWindow(GetConsoleWindow(), SW_SHOWMAXIMIZED);

    // print width/height of this window in terms of columns and rows
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    info.cbSize = sizeof(info);
    GetConsoleScreenBufferInfoEx(hConsole, &info);

    *width = info.srWindow.Right+1;
    *height = info.srWindow.Bottom+1;
}
void mouse_coordinates(POINT *p)
{
	if((p->x)>1891)
	{
		p->x=1891;	}
	else
	{
		if((p->x)<9)
		{
			p->x=9;
		}
	}
	
	if((p->y)>1064)
	{
		p->y=1064;
	}
	else
	{
		if((p->y<8))
		{
			p->y=8;
		}
	}
	p->y=(last_y+8)/22;
	p->x=(last_x)/11;
}

int mouse_move (POINT *p)
{
	int x,y;
	GetCursorPos(p);
	x=p->x;
	y=p->y;
	
	if ((last_y!=y) || (last_x!=x))
	{
		last_y=y;
		last_x=x;
		mouse_coordinates(p);
		return 1;
	}
	else
	{
		mouse_coordinates(p);
		return 0;
	}
}

int right_click()
{
	if (GetKeyState(VK_RBUTTON) < 0) {
	return 1;
	}
	return 0;
}

int left_click()
{
	if (GetKeyState(VK_LBUTTON) < 0) {
	return 1;
	}
	return 0;
}


/*int main()
{
	int width, height;
	POINT p;
	srand(time(NULL));
	fullscreen();
    
	do{
	mouse_move(&p);
	/*system("cls");
printf("x:%d y:%d",p.x,p.y);*/
    /*if (GetKeyState(VK_LBUTTON) < 0) {
	printf("\033[%d;%dH", p.y, p.x);
	printf("\033[47m%c",rand());
	}
	if (GetKeyState(VK_RBUTTON) < 0) {
	printf("\033[0m \033[%d;%dH", p.y, p.x);
	printf("%c",' ');
	}
	if(kbhit())
	{
		if(getch()=='r')
		{
			printf("\033[0m");
			system("cls");
		}
	}
	}while(1!=0);
	return 0;
}*/