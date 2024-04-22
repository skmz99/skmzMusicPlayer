#include <stdio.h>
#include <pigpio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "KEY_APP.h"
#include "OLED_Driver.h"
#include "OLED_GUI.h"
#include "dynamicArray.h"

int main(){


	if(gpioInitialise() < 0){
		printf("Failure\n");
		exit(0);
	}

	printf("Successfull\n");

	/******** Initiate a Dynamic Array with size 10 ******/
	Array stringList;
	initArray(&stringList, 10);

	/******** Initiate GPIO ********/
	GPIO_Init();
	gpioDelay(2000000); //Delay program for 2 seconds

	/******** Initiate Oled ********/
	OLED_Init();
	gpioDelay(2000000); 
	
	/******* Program initial start read and save Music 
	 *			 file names into Dynamic Array
	 */

	struct dirent *entry;
	DIR *dir;
	dir = opendir("./Music");
	if(dir){
		while((entry = readdir(dir)) != NULL)
		{
			if(!strcmp(entry->d_name,".")) continue;
			if(!strcmp(entry->d_name,"..")) continue;
			
			insertArray(&stringList,entry->d_name);//insert original name in Array
			
			//cuts original name to a max of 24 characters to fit into 1.3 inch oled screen
			if(strlen(entry->d_name) >= 24){
				entry->d_name[21] = '.';
				entry->d_name[22] = '.';
				entry->d_name[23] = '.';
				entry->d_name[24] = '\0';
			}			
			insertAltName(&stringList,entry->d_name); //insert alternative name in AltArray
		}
	}
	else{
		printf("Failed opendir\n");
	}
	closedir(dir);
	gpioDelay(5000000);//Delay for 5 seconds


	/****** Erase any drawings on screen *******/
	OLED_Clear(0x00);
	OLED_Display();
	gpioDelay(1000000);

	printf("Sketching SKMZ MUSIC PLAYER\n");

	GUI_DisString_EN(25,25, "Skmz Music Player ", &Font8, FONT_BACKGROUND, WHITE);
	OLED_Display();
	gpioDelay(1000000);
	
	OLED_Clear(0X00);
	OLED_Display();
	gpioDelay(1000000);

	printf("-----------------OLED SCREEN------------\n");
	
	//makes a copy of dyanmic array for KEY_APP.c
	keyAppArrayinit(&stringList);
	gpioDelay(100000);
	//Menu_Key_Listen();
	Music_Key_Listen();

	//Free Dyanmic Array 
	freeArray(&stringList);
	gpioTerminate();
	return 0;
}
