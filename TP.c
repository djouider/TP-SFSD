
#include "retry.c"



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
	fichier_lnof *F;
	int fs=1;
	int choice,N=100000;
	int list[N],len=0;
	char Color1[]="\033[0;104m",Color2[]="\033[1;32m",time[20]; //colors to use in the menu
	

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
		choice=menu(fs,Color1,Color2,"Main Menu","File Management","Extras","History","Settings","Exit");
		
		switch(choice)
		{
			case 1:
				{
					if(check_file_existance("DOCUMENTS_LIBRARY.BIN"))
					{
					do
					{
					/////////////File Management////////////
					system("cls");
					choice=menu(fs,Color1,Color2,"File Management","Overview","Modify file","Generate new file","Magazine","Exit");
					switch(choice)
					{
						
						case 1:
						{
							////////Overview//////////	
						}
						choice=0;
						break;
						
						case 2:
							{
								do ////////Modify//////////
								{
									system("cls");
									choice=menu(fs,Color1,Color2,"Modify file","Modify value","Add to file","Delete from file","","Exit");
									switch(choice)
									{
										case 1:
										{
											//////Modify Value//////
										}
										choice=0;
										break;
										
										case 2:
										{
											///////Add to file///////
										}
										choice=0;
										break;
										
										case 3:
										{
											////////Delete from file/////		
										}
										choice=0;
										break;
										
									}
								}while(choice!=5);	
							}
							choice=0;
							break;
							
							
							
						case 3:
							{
								////////////Generate new file/////////
							}
							choice=0;
							break;
							
						case 4:
							{
								do		///////////////Magazine//////////////
								{
									system("cls");
									choice=menu(fs,Color1,Color2,"Magazine","Update Magazines","overview","Delete from Magazines","","Exit");
									switch(choice)									
									{
										case 1:
											{
												///////////Update magazine///////
											}
											choice=0;
											break;
											
											
										case 2:
											{
												//////////Overview Magazine//////////
											}
											choice=0;
											break;
											
										case 3:
											{
												////////////Delete Magazine////////
											}
											choice=0;
											break;
											
										
									}
								}while(choice!=5);
								
							}
							choice=0;
							break;
						}
					}while(choice!=5);
					}
					else
					{
						////////////Bulk Loading//////////
						system("cls");
						printf("					Your file deosnt exist \n\n\n			Would you like to open a new one and bulk load it?\n\n\n Press enter if yes and Esc if no");
						do
						{
							if(Esc())
							{
								clear_stdin();break;
							}
							if(Enter())
							{
								clear_stdin();
								system("cls");
								printf("Pls choose the number of books that u want to initialise your file with or enter 0 to default value(100,000)");
								scanf("%d", &N);
								if(!N)
								{
									N=100000;///Default value
								}
								system("cls");
								open_lnof(&F,"DOCUMENTS_LIBRARY.BIN",'N');
								printf("here");
								bulk_load_lof(F,N,list,&len,time);
								break;
								
							}
						}while(1);
					}
				
				}
				choice=0;
				break;
			
			case 2:
				{
					////////////Extras///////////////
				}
				choice=0;
				break;
			
			
			case 3:
				{
					///////////History////////////////////
					if(check_file_existance("History.bin")) ///File exists
					{
					open_htof(&H,"History.bin",'E');
					show_history(H,fs);
					close_htof(H);
					}
					else
					{
						system("cls");
						if(fs)
						{
						printf("\n\n\n\n\n\n\n\n\n					Your history is empty\n\n\n					Make some Operations to add something");
						printf("\n\n\n\nPress  Anything to go back");
						getch();
						}
					}
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
										choice=0;
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
									choice=0;
									break;
									
							}
							}while(choice!=5);
						}
						choice=0;
						break;
						
						
					case 2:
					{
						////////////Reset History/////////////	
						system("cls");
						if(check_file_existance("History.bin"))
						{
							if(!(remove("History.bin")))///Delete the file
							printf("your history has been deleted successfully\n\npress anything to go back");
							else
							{
								printf("There was a problem deleting your history pls retry later");
							}
						}
						else
						{
							printf("your history is already empty\n\npres anything to go back");
						}
						getch();
					}	
					choice=0;
					break;
						
						
						
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
										if(!fs)
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
										if(fs)
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
				}
				choice=0;
				break;
	
		}
	}while(choice!=5);
}