#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <conio.h>
#include <time.h>
#include <unistd.h>
#include <windows.h>
#include <winuser.h>
#include <sys/time.h>


struct timeval start, end;
typedef struct
{
	int hours;
	int minutes;
	int seconds;
}Time;
void new_timer()
{
	gettimeofday(&start, NULL);
}

long convert_time_to_long(Time timer)
{
	long mtime;
	mtime=timer.seconds*1000;
	mtime+=timer.minutes*60*1000;
	mtime+=timer.hours*60*1000*1000;
	return mtime;
}

void convert_long_to_time(long mtime, Time *timer)
{
	(timer)->seconds=(mtime/1000)%60;
    (timer)->minutes=(mtime/(1000*60))%60;
    (timer)->hours=(mtime/(1000*3600))%24;
}

long Update_timer ()
{
	long secs,usecs,mtime;
	
    gettimeofday(&end, NULL);
    secs  = end.tv_sec  - start.tv_sec;
    usecs = end.tv_usec - start.tv_usec;
    mtime = ((secs) * 1000 + usecs/1000.0) + 0.5;
    return mtime;
}

void percentage(int i,int j, int max,int curr)
{
	float perc;
	perc= (float)curr/max*100;
	printf("\033[%d;%dH%.1f%%",i,j,perc);
	printf("\033[%d;%dH",i,j+6);
	printf("\033[42m");
	for(curr=1;curr<(int)perc;curr++)
	{
		printf(" ");
	}
	printf("\033[0m");
}



void progress()
{
	system("cls");
	printf("\033[10;75HPress D to hide or show");
    printf("\033[25;72H Press Esc to abort the process");
}

void update_progress(int max,int curr,char message[])
{
		Time elapsed, remaining;
	    long temp,time;
	    float perc;
	
    			perc=(float)curr/max*100;
    			temp=Update_timer();
    			convert_long_to_time(temp,&elapsed);
    			time=temp*((((float)max-curr)/curr));
   			 	convert_long_to_time(time,&remaining);
   			 	printf("\033[15;84H\x1b[2K%.1f%%",perc);
   			 	printf("\033[17;80H\x1b[2K%s",message);
    			printf("\033[19;66H\x1b[2KTime elapsed: %d hours %d minutes %d seconds",elapsed.hours,elapsed.minutes,elapsed.seconds);
    			printf("\033[21;65H\x1b[2KTime remaining: %d hours %d minutes %d seconds",remaining.hours,remaining.minutes,remaining.seconds);
}




/*int main(void) { 

	int i,k=1000000,hide=0,j;  
    Time elapsed, remaining;
    long temp,bruh;
    float perc;
    
    fullscreen();
    new_timer();
    printf("\033[10;75HPress D to hide or show");
    printf("\033[25;72H Press Esc to abort the process");
    for(i=1;i<k;i++)
    {
    	if(!hide)
    	{
    	perc=(float)i/k*100;
    	temp=Update_timer();
    	convert_long_to_time(temp,&elapsed);
    	bruh=temp*((((float)k-i)/i));
    	convert_long_to_time(bruh,&remaining);
    	printf("\033[15;84H%.1f%%",perc);
    	printf("\033[17;66HTime elapsed: %d hours %d minutes %d seconds",elapsed.hours,elapsed.minutes,elapsed.seconds);
    	printf("\033[19;65HTime remaining: %d hours %d minutes %d seconds",remaining.hours,remaining.minutes,remaining.seconds);
    	}
    	if(kbhit())
    	{
    		if (Esc())
    		{
    			system("cls");
    			printf("Process canceled");
    			break;
			}
    		if (getch()=='d')
    		{
    			if(!hide)
    			{
    			system("cls");
    			printf("\033[10;75H Press D to hide or show");
    			printf("\033[25;72H Press Esc to abort the process");
    			hide=1;
    			}
    			else
    			{
    			hide=0;
    			}
    		}
		}
	}
	return 1;
}
*/