/*******************************************************************************
	Sega Dreamcast Project

	Project name : Crob
	Created on   : 2020-09-11
*******************************************************************************/

#define PROJECT_NAME "Crob"

#include <kos.h>
#include <dc/sound/sound.h>
#include <dc/sound/sfxmgr.h>

// SDL
#include <SDL/SDL.h>

#ifdef DEBUG
#include <arch/gdb.h>
#endif

/* These macros tell KOS how to initialize itself. All of this initialization
   happens before main() gets called, and the shutdown happens afterwards. So
   you need to set any flags you want here. Here are some possibilities:

   INIT_NONE         -- don't do any auto init
   INIT_IRQ          -- Enable IRQs
   INIT_THD_PREEMPT  -- Enable pre-emptive threading
   INIT_NET          -- Enable networking (doesn't imply lwIP!)
   INIT_MALLOCSTATS  -- Enable a call to malloc_stats() right before shutdown

   You can OR any or all of those together. If you want to start out with
   the current KOS defaults, use INIT_DEFAULT (or leave it out entirely). */
KOS_INIT_FLAGS(INIT_DEFAULT | INIT_MALLOCSTATS);

/* Declaration of the romdisk
   You can access the files inside it by using the "/rd" mounting point. */
extern uint8 romdisk[];
KOS_INIT_ROMDISK(romdisk);

/* Your program's main entry point */
int main(int argc, char *argv[]) {
#ifdef DEBUG
	/* This is needed for the Debug target.
	   Please don't remove this part of code if you want to use the Code::Blocks debugger.
	   Also, you'll need to configure Dreamcast Tool (dc-tool) from the DreamSDK Manager. */
	gdb_init();
	printf("Connection established to %s!", PROJECT_NAME);
#endif

    /* Your program start here... */
    printf("\nHello world from %s!\n\n", PROJECT_NAME);

    SDL_Surface * screen;
    SDL_Surface * image;
    Uint32 flags = SDL_SWSURFACE|SDL_FULLSCREEN;
    int randX,randY;
    int crobW = 128;
    int crobH = 119;

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        return -1;
    }
    snd_init();

    screen = SDL_SetVideoMode(640, 480, 16, flags);
    SDL_ShowCursor(SDL_DISABLE);
    srand(time(NULL));

    image = SDL_LoadBMP("/rd/crob.bmp");
    sfxhnd_t crob = snd_sfx_load("/rd/crob.wav");

    while(true) {
        SDL_Rect * crobPos = new SDL_Rect();
        randX = rand() % 511;
        randY = rand() % 360;

        crobPos->x = randX;
        crobPos->y = randY;
        crobPos->w = crobW;
        crobPos->h = crobH;

        SDL_FillRect(screen, NULL, 0x000000); // Clear the screen
        SDL_BlitSurface(image, NULL, screen, crobPos); // Draw the crob
        SDL_Flip(screen); // Show the screen
        snd_sfx_play(crob, 255, 128);

        SDL_Delay(1000);
        delete crobPos;
    }

	/* Bye... */
	snd_shutdown();
    return 0;
}
