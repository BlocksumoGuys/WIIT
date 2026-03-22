#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <string.h>
#define WPAD_BUTTON_COUNT 11
#define NCK_BUTTON_COUNT 2

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;
u8 menu = 0;
u32 wpad_buttons[WPAD_BUTTON_COUNT] = {
	WPAD_BUTTON_1,
    WPAD_BUTTON_2,
    WPAD_BUTTON_B,
    WPAD_BUTTON_A,
    WPAD_BUTTON_MINUS,
    WPAD_BUTTON_HOME,
    WPAD_BUTTON_LEFT,
    WPAD_BUTTON_RIGHT,
    WPAD_BUTTON_DOWN,
    WPAD_BUTTON_UP,
    WPAD_BUTTON_PLUS
};
u32 nck_buttons[NCK_BUTTON_COUNT] = {
	WPAD_NUNCHUK_BUTTON_C,
	WPAD_NUNCHUK_BUTTON_Z
};
char *nck_buttons_char[NCK_BUTTON_COUNT] = {
	"Button C   :   Waiting for Input..",
	"Button Z   :   Waiting for Input.."
};
char *nck_buttons_char_2[NCK_BUTTON_COUNT] = {
	"Button C   :   CHECKED",
	"Button Z   :   CHECKED"
};
char *wpad_buttons_char[WPAD_BUTTON_COUNT] = {
	"Button 1   :   Waiting for Input..",
    "Button 2   :   Waiting for Input..",
    "Button B   :   Waiting for Input..",
    "Button A   :   Waiting for Input..",
    "Button minus (-)   :   Waiting for Input..",
    "Button Home   :   Waiting for Input..",
    "Button Left   :   Waiting for Input..",
    "Button Right   :   Waiting for Input..",
    "Button Down   :   Waiting for Input..",
    "Button Up   :   Waiting for Input..",
    "Button plus (+)   :   Waiting for Input.."
};

char *wpad_buttons_char_2[WPAD_BUTTON_COUNT] = {
	"Button 1   :   CHECKED",
    "Button 2   :   CHECKED",
    "Button B   :   CHECKED",
    "Button A   :   CHECKED",
    "Button minus (-)   :   CHECKED",
    "Button Home   :   CHECKED (Press again to return)",
    "Button Left   :   CHECKED",
    "Button Right   :   CHECKED",
    "Button Down   :   CHECKED",
    "Button Up   :   CHECKED",
    "Button plus (+)   :   CHECKED"
};


void clear(){
	printf("\x1b[2J");
	printf("                 _______ _____ _______ \n");
    printf("    \\ \\        / /_   _|_   _|__   __|\n");
    printf("     \\ \\  /\\  / /  | |   | |    | |   \n");
    printf("      \\ \\/  \\/ /   | |   | |    | |   \n");
    printf("       \\  /\\  /   _| |_ _| |_   | |   \n");
    printf("        \\/  \\/   |_____|_____|  |_|   \n");
    printf("                                       \n");
    printf("                                       \n");

}


void replaceString(char *str, char *newStr) {
    u32 i = 0;
    while (newStr[i] != '\0') {
        str[i] = newStr[i];
        i++;
    }
    str[i] = '\0';
}

int fwiimote(){
	if (menu != 1){return -1;}
	clear();
	for(u8 i = 0; i < WPAD_BUTTON_COUNT; i++){
		printf("%s\n", wpad_buttons_char[i]);
	}
}
int fnunchuk(){
	if (menu != 2){return -1;}
	clear();
	for(u8 i = 0; i < NCK_BUTTON_COUNT; i++){
		printf("%s\n", nck_buttons_char[i]);
	}
	printf("\n\n\n\n\n");
	printf("Press HOME twice to go back.");
}
int fmenu(u8 opt){
	if (menu != 0){return -1;}
	switch (opt)
		{
			case 0:
				clear();
				printf(">   WiiMote Test.\n");
				printf("    Nunchuk Test.\n");
				printf("    Quit.\n");
				break;
			case 1:
				clear();
				printf("    WiiMote Test.\n");
				printf(">   Nunchuk Test.\n");
				printf("    Quit.\n");
				break;

			case 2:
				clear();
				printf("    WiiMote Test.\n");
				printf("    Nunchuk Test.\n");
				printf(">   Quit.\n");
				break;
			
			default:
				break;
		}
}

int main(int argc, char **argv) {
	VIDEO_Init();
	WPAD_Init();
	rmode = VIDEO_GetPreferredMode(NULL);
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);
	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(xfb);
	VIDEO_SetBlack(false);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	clear();
	u8 opt = 0;
	u8 opt_limit = 2;
	u8 bback = 0;
	while(SYS_MainLoop()) {
		WPAD_ScanPads();
		u32 pressed = WPAD_ButtonsDown(0);
		if (pressed && menu == 1){
			for(u8 i = 0; i < WPAD_BUTTON_COUNT; i++){
				if(pressed & wpad_buttons[i]){		
					replaceString(wpad_buttons_char[i], wpad_buttons_char_2[i]);
					fwiimote();
				}
			}
		}
		if (pressed & WPAD_BUTTON_HOME){
			if(menu != 0){
				bback +=1;
				if (bback == 2){
					bback = 0;
					menu = 0;
				}
			}
		}
		if (pressed & WPAD_BUTTON_A) {
			switch (opt)
			{
				case 0:
					clear();
					menu = 1;
					fwiimote();
					break;
				case 1:
					clear();
					menu = 2;
					fnunchuk();
					break;
				case 2:
					clear();
					printf("Quitting...");
					exit(0);
					break;
				default:
					break;
			}
		}
		if (pressed & WPAD_BUTTON_DOWN){
			opt += 1;
		}
		if (pressed & WPAD_BUTTON_UP){
			opt -= 1;
		}
		if (opt > opt_limit){
			opt = 0;
		}
		fmenu(opt);
		VIDEO_WaitVSync();
	}

	return 0;
}
