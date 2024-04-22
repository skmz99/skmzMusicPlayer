gcc -Wall -pthread -o main main.c KEY_APP.c OLED_Driver.h OLED_Driver.c OLED_GUI.c OLED_GUI.h dynamicArray.h dynamicArray.c fonts.h font8.c font12.c font16.c font20.c font24.c musicPlayer.h musicPlayer.c -lvlc -lpigpio -lrt
sudo ./main
