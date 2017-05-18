#include "Pong.h"
// Copryright Arnaud d'Orchymont 2017

int main(int argc, char *argv[])
{


    PongGame myGame;
    int gameIsRunning;
    int introIsRunning;
    font myFont;
    unsigned int frameLimit = SDL_GetTicks() + 16;

    //Init myGame
    initPongGame (&myGame);
    initFont(&myFont);
    //Init SDL
    if(initSDL("Pong",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN,&myGame)){
            gameIsRunning=1;
            introIsRunning=1;
            srand (time(NULL)); // initialize random seed
    }else{
            fprintf(stdout,"Unable to initialize SDL (%s)\n",SDL_GetError());
            return EXIT_FAILURE;
    }

    while (introIsRunning){
        handleIntroEvents(&introIsRunning,&gameIsRunning,&myGame);
        introWindow(&myGame, myFont);
    }

    do {
            handleGameEvents(&gameIsRunning,&myGame);
            playerPaddleMove (&myGame);
            handleAI(&myGame);
            AIPaddleMove (&myGame);
            ballMovementAndScore(&myGame);
            renderPongGame (myGame, myFont);
            checkVictoryConditions (&gameIsRunning, &myGame, myFont);

            delay(frameLimit);
            frameLimit = SDL_GetTicks() + 16; //60 fps cap

    } while (gameIsRunning);

        // free pointer
        destroy(&myGame);
        releaseFont (&myFont);

        //free memory
        TTF_Quit();
        SDL_Quit();


    return EXIT_SUCCESS;
}
