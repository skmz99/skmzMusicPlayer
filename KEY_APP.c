#include <pigpio.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "KEY_APP.h"
#include "OLED_GUI.h"
#include "OLED_Driver.h"
#include "musicPlayer.h"

//divides the screen into 10 sections for width
uint16_t W = WIDTH/10;
//divides the screen into 5 sections for height
uint16_t H = HEIGHT/5;
//Navi is used to highlight which current option is being hovered over
uint8_t  Navi = 0;
//Maximum number of folders/song names to be shown on screen 
const int MAXNUM = 6;

//dynamic Array to be copied from main.c
Array *testList;

/************* Functions ******************/
void Options_Content();
int  Music_Content(int pgNum, int folderSelec);
void Music_Key_Listen();
int drawMusic_Files(Array *musicFiles, int pgNum, int folderSelec);
void musicFile_Key_Listen(int musicFolderSelec);
/******************************************/

// called by main.c to copy stored music in the Music folder
void keyAppArrayinit(Array *stringList){
	testList = stringList;
}

void GPIO_Init()
{
	gpioSetMode(OLED_RST,		1);
	gpioSetMode(OLED_DC,		1);
	gpioSetMode(OLED_CS,		1);
	gpioSetMode(KEY_UP_PIN,		0);
	gpioSetPullUpDown(KEY_UP_PIN,	PI_PUD_UP);

	gpioSetMode(KEY_DOWN_PIN,	0);
	gpioSetPullUpDown(KEY_DOWN_PIN,	PI_PUD_UP);

	gpioSetMode(KEY_LEFT_PIN,	0);	
	gpioSetPullUpDown(KEY_LEFT_PIN, PI_PUD_UP);

	gpioSetMode(KEY_RIGHT_PIN,	0);
	gpioSetPullUpDown(KEY_RIGHT_PIN,	PI_PUD_UP);

	gpioSetMode(KEY_PRESS_PIN,	0);
	gpioSetPullUpDown(KEY_PRESS_PIN,	PI_PUD_UP);

	gpioSetMode(KEY1_PIN,		0);
	gpioSetPullUpDown(KEY1_PIN, 	PI_PUD_UP);

	gpioSetMode(KEY2_PIN,		0);
	gpioSetPullUpDown(KEY2_PIN,	PI_PUD_UP);

	gpioSetMode(KEY3_PIN,		0);
	gpioSetPullUpDown(KEY3_PIN,	PI_PUD_UP);
}



void Menu_Draw(uint8_t Option)
{
	CLEAR(BACK_COLOR);

	switch(Option){
		//highlights Music when user presses left
		case 1:
			DrawRectangle(W+2,H*3+1, W*4-1, H*2+1, WORD_COLOR, DRAW_EMPTY, DOT_PIXEL_DFT);
			DisString_EN(W+Font_Offset+1, H*2+Font_Offset, "Music", &Font12, WORD_COLOR, BACK_COLOR);

			DrawRectangle(W*5+1, H*3+1, W*9-1, H*2+1, WORD_COLOR, DRAW_EMPTY, DOT_PIXEL_DFT);
			DisString_EN(W*5+Font_Offset, H*2+Font_Offset, "Options", &Font12, BACK_COLOR, WORD_COLOR);

			break;
		//highlights Options when user presses right 
		case 2:
			DrawRectangle(W+2,H*3+1, W*4-1, H*2+1, WORD_COLOR, DRAW_EMPTY, DOT_PIXEL_DFT);
			DisString_EN(W+Font_Offset+1, H*2+Font_Offset, "Music", &Font12, BACK_COLOR, WORD_COLOR);

			DrawRectangle(W*5+1, H*3+1, W*9-1, H*2+1, WORD_COLOR, DRAW_EMPTY, DOT_PIXEL_DFT);
			DisString_EN(W*5+Font_Offset, H*2+Font_Offset, "Options", &Font12, WORD_COLOR, BACK_COLOR);
			
			break;
		//neither options will be highlighted 
		default:
			DrawRectangle(W+2,H*3+1, W*4-1, H*2+1, WORD_COLOR, DRAW_EMPTY, DOT_PIXEL_DFT);
			DisString_EN(W+Font_Offset+1, H*2+Font_Offset, "Music", &Font12, BACK_COLOR, WORD_COLOR);
			
			DrawRectangle(W*5+1, H*3+1, W*9-1, H*2+1, WORD_COLOR, DRAW_EMPTY, DOT_PIXEL_DFT);
			DisString_EN(W*5+Font_Offset, H*2+Font_Offset, "Options", &Font12, BACK_COLOR, WORD_COLOR);

			break;
	}

	OLED_Display();
}	


void Menu_Key_Listen(void)
{
	Menu_Draw(Navi); //initally will be zero to draw initial options to choose

	for(;;){
		if(KEY_UP_RD == 0){
			printf("UP key pressed\n");
			freeArray(testList);
			break;
		}
		if(KEY_DOWN_RD == 0){
			printf("DOWN key pressed returned\n");
			freeArray(testList);
			return;
		}
		if(KEY_LEFT_RD == 0){ 
			printf("LEFT key pressed\n");
			Navi = 1;
			Menu_Draw(Navi);
			continue;
		}
		if(KEY_RIGHT_RD == 0){
			printf("RIGHT key pressed\n");
			Navi = 2;
			Menu_Draw(Navi);
			continue;
		}
		if(KEY1_RD == 0){
			printf("KEY1 pressed\n");

			switch(Navi){
			
				case 1:
					printf("Music\n");
					gpioDelay(500000);
					Music_Key_Listen();
					Menu_Draw(1);
					break;
				case 2:
					printf("Options\n");
					Options_Content();
					Menu_Draw(2);
					break;

				default: 
					printf("Default"); 
					break;
		       	}
		}
	}
}
//********************************************************************************
//********************************************************************************
//**										**
//**				Music Content					**
//**										**
//********************************************************************************
//********************************************************************************

int Music_Content(int pgNum, int folderSelec){
	CLEAR(BACK_COLOR);
	
	//offset placement of song titles to fit MAXIMUM on screen
	int offset = 0;
	
	int i = pgNum;
	int pgSize = 0;
	while(testList->altName[i]){
		//highlight musicFolder if user hovers over it
		if(folderSelec == i){
			DisString_EN(5,5 + offset, testList->altName[i], &Font8, WORD_COLOR, BACK_COLOR);
		}
		//no highlight on musicFolder
		else{
			DisString_EN(5,5 + offset, testList->altName[i], &Font8, BACK_COLOR, WORD_COLOR);
		}

		OLED_Display();
		//offset next line on screen by 10 pixels
		offset = offset + 10;
		i++;
		pgSize++;
		
		//return once i is a divisable of MAXNUM that isnt 0
		if( i % MAXNUM == 0 && i != 0){
			return MAXNUM; //statement is only true if there has been 6 drawn strings on screen
		}
	}
	//return will be less than MAXNUM 
	return pgSize;
}


void Music_Key_Listen(){
	CLEAR(BACK_COLOR);

	int musicFolderSelect = 0; 			     //initially highlights first folder shown
	int pgSize = Music_Content(0,musicFolderSelect); //Number of files showing on screen after function call
	int prevPgSize = 0;

	for(;;){
		if(KEY_UP_RD == 0 && musicFolderSelect > prevPgSize && musicFolderSelect < pgSize){
			//highlight folder above
		       	musicFolderSelect--;
			//redraw musicFolders with new highlight
			Music_Content(prevPgSize, musicFolderSelect);
			gpioDelay(900000);
		}

		if(KEY_DOWN_RD == 0 && musicFolderSelect < pgSize-1){
			//highlight folder below
		       	musicFolderSelect++;
			Music_Content(prevPgSize, musicFolderSelect);
			gpioDelay(900000);
		}

		//draw the next MAXNUM of musicFolders
		if(KEY_RIGHT_RD == 0 && pgSize < testList->used){
			musicFolderSelect = pgSize ;
			prevPgSize = pgSize;
			pgSize += Music_Content(pgSize, musicFolderSelect);
			gpioDelay(900000);
		}
		
		//draw the previous MAXNUM of musicFolders
		if(KEY_LEFT_RD == 0 && prevPgSize > 0){
			musicFolderSelect = prevPgSize - MAXNUM;
			pgSize = prevPgSize;
			prevPgSize = prevPgSize - MAXNUM;
			Music_Content(prevPgSize, musicFolderSelect);
			gpioDelay(900000);
		} 
		
		//selects the highlighted musicFolder to access its content
		if(KEY1_RD == 0){
			musicFile_Key_Listen(musicFolderSelect);
			Music_Content(prevPgSize, musicFolderSelect);
			gpioDelay(900000);
		}

		if(KEY_PRESS_RD == 0){
			CLEAR(BACK_COLOR);
			OLED_Display();
			return;
		}
	}
}

int drawMusic_Files(Array *musicFiles,int pgNum, int folderSelec){
	CLEAR(BACK_COLOR);

	int offset = 0;

	int i = pgNum;
	int pgSize = 0;
	while(musicFiles->used > i){

		if(folderSelec == i){
			DisString_EN(5,5 + offset, musicFiles->altName[i], &Font8, WORD_COLOR, BACK_COLOR);
		}
		else{
			DisString_EN(5,5 + offset, musicFiles->altName[i], &Font8, BACK_COLOR, WORD_COLOR);
		}
		OLED_Display();
		offset = offset + 10;
		i++;
		pgSize++;

		if(i % MAXNUM == 0 && i != 0){
			return MAXNUM;
		}
	}
	return pgSize;
		
}

void musicFile_Key_Listen(int folderSelec){
	CLEAR(BACK_COLOR);
	
	//Create a new dynamic array for the song files within musicfolder
	Array musicFiles;
	initArray(&musicFiles,10);
	char fileDir[100] = "./Music/";
	strcat(fileDir,testList->array[folderSelec]);

	struct dirent *entry;
	DIR *dir;
	dir = opendir(fileDir);
	
	//save original and alt song titles same way as musicFolder were saved
	if(dir){
		while((entry = readdir(dir)) != NULL)
		{
			if(!strcmp(entry->d_name, ".")) continue;
			if(!strcmp(entry->d_name, "..")) continue;
			if(entry->d_name[strlen(entry->d_name) - 3] != 'w') continue;
			if(entry->d_name[strlen(entry->d_name) - 2] != 'a') continue;
			if(entry->d_name[strlen(entry->d_name) - 1] != 'v') continue;

			
			insertArray(&musicFiles, entry->d_name);

			if(strlen(entry->d_name) >= 24){
				entry->d_name[21] = '.';
				entry->d_name[22] = '.';
				entry->d_name[23] = '.';
				entry->d_name[24] = '\0';
			}
			insertAltName(&musicFiles,entry->d_name);
		}
	}
	else{
		printf("Failed opendir\n");
	}
	closedir(dir);
	gpioDelay(1000000);
	
	/******** Same way of highlighting and selecting as musicFolders ********/
	int musicFileSelec = 0;
	int prevPgSize = 0;
	int pgSize = drawMusic_Files(&musicFiles, 0, musicFileSelec);
	for(;;){
		if(KEY_UP_RD == 0 && musicFileSelec > prevPgSize && musicFileSelec < pgSize){
			musicFileSelec--;
			drawMusic_Files(&musicFiles, prevPgSize, musicFileSelec);
			gpioDelay(900000);
		}

		if(KEY_DOWN_RD == 0 && musicFileSelec < pgSize-1){
			musicFileSelec++;
			drawMusic_Files(&musicFiles, prevPgSize, musicFileSelec);
			gpioDelay(900000);
		}

		if(KEY_LEFT_RD == 0 && prevPgSize > 0){
			musicFileSelec = prevPgSize - MAXNUM;
			pgSize = prevPgSize;
			prevPgSize = prevPgSize - MAXNUM;
			drawMusic_Files(&musicFiles, prevPgSize, musicFileSelec);
			gpioDelay(900000);
		}

		if(KEY_RIGHT_RD == 0 && pgSize < musicFiles.used){
			musicFileSelec = pgSize;
			prevPgSize = pgSize;
			pgSize += drawMusic_Files(&musicFiles, pgSize, musicFileSelec);
			gpioDelay(900000);
		}
		//selected song will play with vlc in musicPlayer.c
		if(KEY1_RD == 0){
			int fileNum = musicFileSelec;
			int playOptions = 0;
			bool remainLooping = true;
			while(remainLooping){
				//play song and return integer value to playOptions
				playOptions = play(testList->array[folderSelec], musicFiles.array[fileNum]);

				switch(playOptions){

					case 0://plays next song/loops to start if user skips foward or lets current song play through
						if(fileNum < musicFiles.used){
							fileNum++;
						}
						else{
							fileNum = 0;
						}
						break;

					case 1://plays previous song or loops to the end of the files list
						if(fileNum > 0){
							fileNum--;
						}
						else{
							fileNum = musicFiles.used -1;
						}

						break;
					default:
						remainLooping = false;
						break;
				}
						
			}
			//Draw all musicSongs when user exits loop
			drawMusic_Files(&musicFiles, prevPgSize, musicFileSelec);
			gpioDelay(900000);
		}
		//free's dynamicArray and returns to musicFolder 
		if(KEY_PRESS_RD == 0){
			freeArray(&musicFiles);
			gpioDelay(15000);
			return;	
		}

	}
}

 void Options_Content()
{
	CLEAR(BACK_COLOR);

	DrawRectangle(5, H, W*4+1, H*2-2, WORD_COLOR, DRAW_EMPTY, DOT_PIXEL_DFT);
	DisString_EN(5+Font_Offset, H+Font_Offset,"Bluetooth",&Font8, BACK_COLOR, WORD_COLOR);

	DisString_EN(5,55,"Skmz Music Player V1.0", &Font8, BACK_COLOR, WORD_COLOR);

	OLED_Display();
}

