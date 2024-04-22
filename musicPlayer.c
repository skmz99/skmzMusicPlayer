#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <vlc/vlc.h>
#include <string.h>
#include <fcntl.h>

#include "KEY_APP.h"
#include "OLED_GUI.h"
#include "OLED_Driver.h"

int play(char* musicFolder, char* song)
{
	libvlc_instance_t * inst;
	libvlc_media_player_t * mp;
	libvlc_media_t * m;
	libvlc_state_t state;

	inst = libvlc_new (0,NULL);
	char fileDir[300] = "Music/";
	strcat(fileDir,musicFolder);
	strcat(fileDir,"/");
	strcat(fileDir,song);

	int playingOption = 0;

	printf("%s is trying to be played\n",fileDir);

	int fd[2], pid;
	

	if(pipe(fd) < 0){
		exit(1);
	}


	pid = fork();

	if(pid > 0){

		m = libvlc_media_new_path (inst, fileDir);

		mp = libvlc_media_player_new_from_media (m);

		libvlc_media_release (m);

		libvlc_media_player_play (mp);

		libvlc_media_parse(m); //this function is depricated

		int length = libvlc_media_get_duration(m)/1000; //get the duration of song in ms and convert it into seconds.
		int isPlaying = 0;

		close(fd[0]);
		write(fd[1], &length, sizeof(int));

		do{
			state = libvlc_media_player_get_state(mp);
			
			//Pause Song
			if(KEY2_RD == 0){
				isPlaying = 100;
				libvlc_media_player_pause(mp);
				write(fd[1], &isPlaying, sizeof(int));
				gpioDelay(1050000);
			}
			
			//Play previous Song
			if(KEY1_RD == 0){
				isPlaying = 1;
				playingOption = 0;
				write(fd[1], &isPlaying,sizeof(int));
				break;
			}

			//play Next Song
			if(KEY3_RD == 0){
				isPlaying = 1;
				playingOption = 1;
				write(fd[1], &isPlaying,sizeof(int));
				break;
			}
			
			//Exit music player
			if(KEY_PRESS_RD == 0){
				isPlaying = 1;
				playingOption = 100;
				write(fd[1], &isPlaying, sizeof(int));
				gpioDelay(50000);
				break;
			}
		}
		while(state != libvlc_Ended && state != libvlc_Error);
		close(fd[1]);

		libvlc_media_player_stop (mp);

		libvlc_media_player_release (mp);

		libvlc_release (inst);

		wait(NULL);
	}
	else{
		int musicLength = 0;
		close(fd[1]);
		sleep(1);

		//keep reading fd[0] until something is sent else continue musicPlayer
		if( read(fd[0], &musicLength, sizeof(musicLength)) != sizeof(musicLength)){
			perror("read() failed in child 1\n");
			exit(EXIT_FAILURE);
		}
		

		if(fcntl(fd[0], F_SETFL, O_NONBLOCK) < 0){
			exit(2);
		}

		if(musicLength == 0) exit(0);

		int mediaOptions = 0;
		
		float loadingBarRate = musicLength / 120.0f;
		float timer = musicLength/ 120.0f;
		float timer2 = 0;
		int barLength = 3;
		CLEAR(BACK_COLOR);
		while(timer2 <= musicLength){

			if( read(fd[0], &mediaOptions, sizeof(mediaOptions)) == -1){
				if(errno = EAGAIN){
					usleep(100);
				}
				else{
					perror("read");
					exit(2);
				}
			}

			switch(mediaOptions){
				case 0://music is playing normally
					CLEAR(BACK_COLOR);
					DisString_EN(5,16, song, &Font8, BACK_COLOR, WORD_COLOR);
					DisString_EN(2,48, "[", &Font8, BACK_COLOR, WORD_COLOR);
					DisString_EN(60, 32, ">", &Font8, BACK_COLOR, WORD_COLOR);
					for(int i = 3; i <= barLength; i++){
						DisString_EN(i, 49, "=", &Font8, BACK_COLOR, WORD_COLOR);
					}
					DisString_EN(123, 48, "]", &Font8, BACK_COLOR, WORD_COLOR);

					OLED_Display();

					barLength += 1;
					timer2 += timer;
					
					gpioDelay(loadingBarRate * 1000000);
					break;

				case 1://exit out of musicPlayer
					close(fd[0]);
					exit(0);
					break;
					
				default://music is paused
					CLEAR(BACK_COLOR);
					DisString_EN(5, 16, song, &Font8, BACK_COLOR, WORD_COLOR);
					DisString_EN(2, 48, "[", &Font8, BACK_COLOR, WORD_COLOR);
					DisString_EN(60, 32, "||", &Font8, BACK_COLOR, WORD_COLOR);
					for(int i = 3; i <= barLength; i++){
						DisString_EN(i, 49, "=", &Font8, BACK_COLOR, WORD_COLOR);
					}
					OLED_Display();
					printf("media has been paused\n");
					usleep(500000);
					break;
			}
		}
		close(fd[0]);
		exit(0);
	}

	return playingOption;
}
