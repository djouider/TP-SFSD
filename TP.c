#include "final_menu.c"
#include "lof_modal.c"



void slow_printf(const char message[],int time) /// write a message slowly
{
	int i=0;
	if(time==0)
	{
		printf("%s",message);
	}
	else{
	do{
		printf("%c",message[i]);
			Sleep(time);
		i++;
		if(kbhit())
		{
			do{
				printf("%c",message[i]);
					i++;
				}while(message[i-1]!='\0');
			clear_stdin();
			break;
			
		}
	}while(message[i-1]!='\0');
	
	}
}


int main()
{
	fichier_htof *H;
	int fs=1;
	int choice;
	char Color1[]="\033[0;104m",Color2[]="\033[1;32m"; //colors to use in the menu
	
	
	open_htof(&H,"History,bin",'E');
	fullscreen(); //making the cmd appear in fullscreen
	
	///Introduction
	printf("										");
	slow_printf("\033[0;31mHello\033[0m\n\n\n",130);
	slow_printf("Before we start we want you to take into concideration the following tips:\n\n",50);
	slow_printf("1- You may skip a dialogue by pressing any key\n\n",50);
	slow_printf("2- You may use your mouse to navigate throught all the parts of the program as long as you are in fullscreen mode\n\n",50);
	slow_printf("3- To navigate using the keyboard you need to use the arrow keys\n\n",50);
	slow_printf("4- You can change personal preferences in the settings option in the main menu \n\n",50);
	printf("\n\n\n										");
	slow_printf("\033[0;32mWe hope you enjoy it\033[0m\n",50);
		printf("\n\nPress anything to continue",50);
		getch();
		clear_stdin();
	////
	do //////////main menu///////////
	{
		system("cls");
		choice=menu(fs,Color1,Color2,"Main Menu","File Management","File Browsing","History","Settings","Exit");
		
		switch(choice)
		{
			case 1:
				{
					do
					{
					/////////////File Management////////////
					system("cls");
					choice=menu(fs,Color1,Color2,"File Management","Add/Remove to file","","","","Exit");
					switch(choice)
					{
						case 1:
							{
								/////////Bulk Loading/////////
								
							}
							choice=0;
							break;
					}
					}while(choice!=5);
				}
				choice=0;
				break;
			
			case 2:
				{
					////////////File Browsing///////////////
				}
				choice=0;
				break;
			
			
			case 3:
				{
					///////////History////////////////////
					
					show_history(H,fs);
				}
				choice=0;
				break;
			
			case 4: 
			{
				do
				{
					//////////////Settings///////////////
				system("cls");
				choice=menu(fs,Color1,Color2,"Settings","Colors","Reset History","Resolution","","Exit");
				switch (choice)
				{
					case 1:
						{
						////////////////Colors////////////////
							do
							{
							system("cls");
							choice=menu(fs,Color1,Color2,"Colors Settings","Borders Color","","Cursor & Head Color","","Exit");	
							switch(choice)
							{
								case 1:
									{
										////////////////Borders Color////////////////
										system("cls");
										choice=menu(fs,Color1,Color2,"Borders Settings","Red","Green","Blue","White","Yellow");
										switch(choice)
										{
											case 1 :
												sprintf(Color1,"\033[0;101m");
												break;
											case 2 :
												sprintf(Color1,"\033[0;102m");
												break;
											case 3 :
												sprintf(Color1,"\033[0;104m");
												break;
											case 4 :
												sprintf(Color1,"\033[0;107m");
												break;
											case 5 :
												sprintf(Color1,"\033[0;103m");
												break;
										}
										break;
									
									case 3 :
										{
											////////////////Cursor & head color////////////////
											system("cls");
											choice=menu(fs,Color1,Color2,"Cursor & Head Settings","Red","Green","Blue","White","Purple");
											switch(choice)
											{
											case 1 :
												sprintf(Color2,"\033[1;91m");
												break;
											case 2 :
												sprintf(Color2,"\033[1;92m");
												break;
											case 3 :
												sprintf(Color2,"\033[1;94m");
												break;
											case 4 :
												sprintf(Color2,"\033[1;97m");
												break;
											case 5 :
												sprintf(Color2,"\033[1;95m");
												break;
											}
										}
									}
									
									break;
									
							}
							}while(choice!=5);
							choice=0;
						}
						break;
						
						
					case 2:
					{
						////////////Reset History/////////////	
						system("cls");
						Reset_History(H);
						printf("your history has been deleted successfully\n\npress anything to go back");
						getch();
					}	
						
						
						
					case 3:
						{
							////////////////Resolution////////////////
							do{
								system("cls");
								printf("%d",fs);
								choice=menu(fs,Color1,Color2,"Resolution","","Fullscreen","Windowed","","Exit");
								switch(choice)
								{
									case 2 :
										{
										//////////Fullscreen/////////
										if(fs==1)
										{
											perror("You are already in fullscreen mode");
										}
										else
										{

											fullscreen();
											fs=1;
											choice=5;
										}
										}
										break;
									
									case 3 :
										{
										////////Windowed//////////
										if(fs==0)
										{
											perror("You are already in windowed mode");
										}
										else
										{
											fullscreen();
											fs=0;
											choice=5;
										}
										}
										break;
								}
																						
						}while(choice!=5);
						choice =0;
						}
						break;
					}
		
				}while(choice!=5);
				choice=0;
				}
				break;
	
		}
	}while(choice!=5);
}