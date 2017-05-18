#ifndef PONG_H_INCLUDED
#define PONG_H_INCLUDED
// Copyright Arnaud d'Orchymont 2017

/*  The game logic (no view code or direct user interaction) for a pong game:
a table tennis sports game featuring simple two-dimensional graphics */

#endif // PONG_H_INCLUDED

#include "Define.h"
/** position, speed and size values are in pixels by default, unless specified otherwise. **/

//Paddle data, for for position, d for direction
typedef struct PongPaddle{
    float px,py,dy;

}PongPaddle;

//Player VS AI game, get 12 points to win.
typedef struct PongScore{
    int player, AI;

}PongScore;

//Ball data, p for position, s for speed, value are in pixels
typedef struct PongBall{
    float px, py, sx, sy;
    float radius;

}PongBall;

// SDL2 pointer
typedef struct DisplayPongGame{

     SDL_Window *g_pWindow;
     SDL_Renderer *g_pRenderer;
     SDL_Texture *g_pTexture;
     SDL_Surface *g_pSurface;

}DisplayPongGame;

// Pong Game structure
typedef struct PongGame{
    DisplayPongGame displayGame;
    PongBall ball;
    PongPaddle paddle1;
    PongPaddle paddle2;
    PongScore score;

}PongGame;

//Used to load a font
typedef struct font{
    TTF_Font *g_font;
}font;

enum Collision {
    Top, Bottom, Right, Left, None
};

enum BOOL{
    True,False
};


//prototypes
void initPongGame (PongGame *myGame);
void initFont (font *myFont);
int initSDL(char *title, int xpos,int ypos,int width, int height,int flags,PongGame *myGame);

void handleIntroEvents(int *introIsRunning, int *gameIsRunning, PongGame *myGame);
void introWindow(PongGame *myGame, font myFont);
void handleGameEvents(int *gameIsRunning, PongGame *myGame);
void playerPaddleMove (PongGame *myGame);
void handleAI(PongGame *myGame);
void AIPaddleMove (PongGame *myGame);

void renderPaddles(PongGame *myGame);
void renderBoundaryLine(PongGame *myGame, int colorR, int colorG, int colorB);
void renderCircle(PongGame *myGame, int R, int G, int B);
void renderAIScore (PongGame *myGame, font myFont);
void renderPlayerScore (PongGame *myGame, font myFont);
void renderPongGame (PongGame myGame, font myFont);

enum Collision CheckCollisionBallWalls (PongGame myGame);
void checkVictoryConditions (int *gameIsRunning, PongGame *myGame, font myFont);
void resetBall (PongGame *myGame);
enum BOOL CheckCollisionBallPaddles (PongGame myGame);
void ballMovementAndScore(PongGame *myGame);
void delay(unsigned int frameLimit);
void endWindow (PongGame *myGame, font myFont, int winner);
void destroy(PongGame *myGame);
void releaseFont (font *myFont);
