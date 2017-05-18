#include "Pong.h"
// Copryright Arnaud d'Orchymont 2017

void initPongGame (PongGame *myGame){

//ball
 myGame->ball.px=SCREEN_WIDTH/3;
 myGame->ball.py=SCREEN_HEIGHT/2;
 myGame->ball.sx=4.0;
 myGame->ball.sy=0.50;
 myGame->ball.radius=BALL_RADIUS;

 //padle 1
 myGame->paddle1.px=0;
 myGame->paddle1.py=0;
 myGame->paddle1.dy=0;

 //padle 2
 myGame->paddle2.px=SCREEN_WIDTH-PADDLE_WIDTH;
 myGame->paddle2.py=0;
 myGame->paddle2.dy=0;

 //score
 myGame->score.AI=0;
 myGame->score.player=0;

}

/********************************************************************
PURPOSE : font loading
INPUT : fontObject
OUTPUT : g_font
*********************************************************************/
void initFont (font *myFont){
    if(TTF_Init() == -1)
        {
            fprintf(stderr, "Error unable to initialize TTF_Init : %s\n", TTF_GetError());
            exit(EXIT_FAILURE);
            }

    myFont->g_font=TTF_OpenFont("./assets/fonts/gameplay/Gameplay.ttf",65);

    if (!myFont->g_font)
    {
            fprintf(stderr, "Error font is missing : %s\n", TTF_GetError());
            exit(EXIT_FAILURE);
    }
}

/********************************************************************
PURPOSE : SDL initialization
INPUT : title, window position, window size, flags, gameObject
OUTPUT: g_pWindow
*********************************************************************/
int initSDL(char *title, int xpos,int ypos,int width, int height,int flags,PongGame *myGame){

    myGame->displayGame.g_pWindow=NULL;
    myGame->displayGame.g_pRenderer=NULL;
    myGame->displayGame.g_pTexture=NULL;
    myGame->displayGame.g_pSurface=NULL;

    //initialize SDL

    if(SDL_Init(SDL_INIT_VIDEO)>=0) // Loading video (return 0 on success)
    {
            //if succeeded create our window
            myGame->displayGame.g_pWindow=SDL_CreateWindow(title,xpos,ypos,width,height,flags);
            //if succeeded create window, then create our render
            if(myGame->displayGame.g_pWindow!=NULL){
                myGame->displayGame.g_pRenderer=SDL_CreateRenderer(myGame->displayGame.g_pWindow,-1,SDL_RENDERER_PRESENTVSYNC);
            }
    }else{

        return 0;
    }
    return 1;
}

/********************************************************************
PURPOSE : events management (input=>keyboard)
INPUT : 2 states variables, gameObject
OUTPUT : introIsRunning or (gameIsRunning and introIsRunning)
*********************************************************************/
void handleIntroEvents(int *introIsRunning, int *gameIsRunning, PongGame *myGame){
        SDL_Event event;

    if(SDL_PollEvent(&event))
    {
        switch(event.type){
            case SDL_QUIT:
                *introIsRunning=0; *gameIsRunning= 0; break;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_SPACE: *introIsRunning=0; break;
                    default:break;
                }
                break;

            case SDL_KEYUP:
                break;

            default:
                break;
        }
    }
}

/********************************************************************
PURPOSE : create and display an introduction window
INPUT : gameObject, fontObject
OUTPUT : window with 2 textures containing text
*********************************************************************/
void introWindow(PongGame *myGame, font myFont){

    SDL_Color fontColor={255,255,255};

    myGame->displayGame.g_pSurface=TTF_RenderText_Blended(myFont.g_font, "WELCOME TO PONG!", fontColor);//Charge la police
     if(myGame->displayGame.g_pSurface)
     {

                SDL_Rect IntroRect1; //Rectangle used to display character chain
                IntroRect1.x=MAIN_TEXT_X;//start point (x)
                IntroRect1.y=MAIN_TEXT_Y;// start point (y)
                IntroRect1.w=MAIN_TEXT_W; //Width
                IntroRect1.h=MAIN_TEXT_H; //Height

                 myGame->displayGame.g_pTexture = SDL_CreateTextureFromSurface(myGame->displayGame.g_pRenderer,myGame->displayGame.g_pSurface);
                 SDL_FreeSurface(myGame->displayGame.g_pSurface);

                 if(myGame->displayGame.g_pTexture){
                        //  copy a portion of the texture to the current rendering target
                        SDL_RenderCopy(myGame->displayGame.g_pRenderer,myGame->displayGame.g_pTexture,NULL,&IntroRect1);
                        SDL_DestroyTexture(myGame->displayGame.g_pTexture);
                 }
                 else
                 {
                        fprintf(stdout,"Failed to create texture (%s)\n",SDL_GetError());
                 }
    }
        else
        {
            fprintf(stdout,"Failed to create surface (%s)\n",SDL_GetError());
        }

    myGame->displayGame.g_pSurface=TTF_RenderText_Blended(myFont.g_font, "Press space to start", fontColor);//load font
     if(myGame->displayGame.g_pSurface)
     {
                SDL_Rect IntroRect2; //Rectangle to write character chain
                IntroRect2.x=START_X; //start point (x)
                IntroRect2.y=START_Y; // start point (y)
                IntroRect2.w=START_W; //Width
                IntroRect2.h=START_H; //Height

                 myGame->displayGame.g_pTexture = SDL_CreateTextureFromSurface(myGame->displayGame.g_pRenderer,myGame->displayGame.g_pSurface);
                 SDL_FreeSurface(myGame->displayGame.g_pSurface);

                 if(myGame->displayGame.g_pTexture)
                 {
                        //  copy a portion of the texture to the current rendering target
                        SDL_RenderCopy(myGame->displayGame.g_pRenderer,myGame->displayGame.g_pTexture,NULL,&IntroRect2);
                        SDL_DestroyTexture(myGame->displayGame.g_pTexture);
                        SDL_RenderPresent(myGame->displayGame.g_pRenderer);
                 }
                 else
                 {
                        fprintf(stdout,"Failed to create texture (%s)\n",SDL_GetError());
                 }
    }
    else
    {
            fprintf(stdout,"Failed to create surface (%s)\n",SDL_GetError());
    }
}

/********************************************************************************************
PURPOSE :
Events management (input=>keyboard)
Indicates in what direction player's paddle should go when player press Up or Down arrow key
INPUT : state variable, gameObject
OUTPUT : gameIsRunning or paddle1.dy
*********************************************************************************************/
void handleGameEvents(int *gameIsRunning, PongGame *myGame){

    SDL_Event event;

    if(SDL_PollEvent(&event))
    {
        switch(event.type){
            case SDL_QUIT:
                  *gameIsRunning=0; break;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_UP: myGame->paddle1.dy-=PADDLE_MOVE_INCREMENT; break;
                    case SDLK_DOWN: myGame->paddle1.dy+=PADDLE_MOVE_INCREMENT;break;
                    default:break;
                }
                break;

            case SDL_KEYUP:
                switch (event.key.keysym.sym)
                {
                   case SDLK_UP: myGame->paddle1.dy=myGame->paddle1.py;break;
                   case SDLK_DOWN: myGame->paddle1.dy=myGame->paddle1.py;break;
                   default:break;
                }

                break;

            default:
                break;
        }
    }
}

/***************************************************************************
PURPOSE :
Move player's paddle and give a smooth move to player's paddle
Paddle max speed: 7 pixels per 16 ms
INPUT : gameObject
OUTPUT : paddle1.py
****************************************************************************/
void playerPaddleMove (PongGame *myGame){

    if (myGame->paddle1.py>myGame->paddle1.dy){
        if (myGame->paddle1.py>0){ // if paddle not at top screen
                myGame->paddle1.py-=PADDLE_MAX_SPEED;
                }
    }

    if (myGame->paddle1.py<myGame->paddle1.dy){
        if (myGame->paddle1.py<SCREEN_HEIGHT-PADDLE_HEIGHT){ // if paddle not at bottom screen
            myGame->paddle1.py+=PADDLE_MAX_SPEED;
            }
    }
}

/*******************************************************************************
PURPOSE : create an AI managing paddle2's direction according to ball position
INPUT : gameObject
OUTPUT : paddle2.py
********************************************************************************/
void handleAI(PongGame *myGame){


    if (myGame->ball.py<(myGame->paddle2.py+PADDLE_HEIGHT/2)) // if ball above top part of paddle
    {
        myGame->paddle2.dy=myGame->paddle2.py;
        myGame->paddle2.dy-=myGame->paddle2.py+PADDLE_HEIGHT/2-myGame->ball.py;
    }
    else if ((myGame->ball.py>(myGame->paddle2.py+PADDLE_HEIGHT/2))) // if ball below bottom part of paddle
    {
        myGame->paddle2.dy=myGame->paddle2.py;
        myGame->paddle2.dy+=myGame->ball.py-(myGame->paddle2.py+PADDLE_HEIGHT/2);
    }
    else
    {
     myGame->paddle2.dy=myGame->paddle2.py; // paddle stay at his current position
    }
}

/***************************************************************************
PURPOSE :
Move AI's paddle and give a smooth move to AI's paddle
Paddle max speed: 7 pixels per 16 ms
INPUT : gameObject
OUTPUT : paddle2.py
****************************************************************************/
void AIPaddleMove (PongGame *myGame){

    if (myGame->paddle2.py>myGame->paddle2.dy){
        if (myGame->paddle2.py>0) // if paddle not at top screen
        {

            if (myGame->paddle2.py-myGame->paddle2.dy>=PADDLE_MAX_SPEED) // if paddle's center 7 pixels away or more from the ball
            {
                myGame->paddle2.py-=PADDLE_MAX_SPEED; //cap the speed at 7 pixels per 16 ms
            }
            else
            {
                 myGame->paddle2.py-=myGame->paddle2.py-myGame->paddle2.dy; // adaptative speed to get a smooth move
            }
        }
    }

    if (myGame->paddle2.dy>myGame->paddle2.py)
    {
        if (myGame->paddle2.py<SCREEN_HEIGHT-PADDLE_HEIGHT) // if paddle not at bottom screen
        {
            if (myGame->paddle2.dy-myGame->paddle2.py>=PADDLE_MAX_SPEED) // if paddle's center 7 pixels away or more from the ball
            {
                myGame->paddle2.py+=PADDLE_MAX_SPEED; //cap the speed at 7 pixels
            }
            else
            {
                myGame->paddle2.py+=myGame->paddle2.dy-myGame->paddle2.py; // adaptative speed to get a smooth move
            }
        }
    }
}

/********************************************************************
PURPOSE : render the paddles
INPUT : gameObject
OUTPUT : 2 rectangles
*********************************************************************/
void renderPaddles(PongGame *myGame) {

       //Paddle1
        SDL_Rect rectangle;
        rectangle.x=myGame->paddle1.px;//debut x
        rectangle.y=myGame->paddle1.py;//debut y
        rectangle.w=PADDLE_WIDTH; //Largeur
        rectangle.h=PADDLE_HEIGHT; //Hauteur

        //Paddle2
        SDL_Rect rectangle2;
        rectangle2.x=myGame->paddle2.px;//debut x
        rectangle2.y=myGame->paddle2.py;//debut y
        rectangle2.w=PADDLE_WIDTH; //Largeur
        rectangle2.h=PADDLE_HEIGHT; //Hauteur

        //Draw in texture
        SDL_SetRenderDrawColor(myGame->displayGame.g_pRenderer,255,0,0,255); // set the color used for drawing operations

        SDL_RenderFillRect(myGame->displayGame.g_pRenderer, &rectangle); // fill a rectangle on the current rendering target with the drawing color
        SDL_RenderFillRect(myGame->displayGame.g_pRenderer, &rectangle2);
}

/********************************************************************
PURPOSE : render a boundary line middle of the screen (width)
INPUT : gameObject, color (RGB)
OUTPUT : chain of espaced rectangles
*********************************************************************/
void renderBoundaryLine(PongGame *myGame, int colorR, int colorG, int colorB){

        int y;
        SDL_SetRenderDrawColor(myGame->displayGame.g_pRenderer,colorR,colorG,colorB,255);

        for (y=10; y<SCREEN_HEIGHT-10; y=y+15){

        SDL_Rect Rectangle;
        Rectangle.x=SCREEN_WIDTH/2-LINE_WIDTH/2;
        Rectangle.y=y;
        Rectangle.w=LINE_WIDTH;
        Rectangle.h=LINE_HEIGHT;

        SDL_RenderFillRect(myGame->displayGame.g_pRenderer, &Rectangle);
        }
}

/********************************************************************
PURPOSE : render the pong ball
INPUT : gameObject, color (RGB)
OUTPUT : a filled circle
*********************************************************************/
void renderCircle(PongGame *myGame, int R, int G, int B){
    SDL_SetRenderDrawColor(myGame->displayGame.g_pRenderer, R, G, B, SDL_ALPHA_OPAQUE);
    int radiusMin;

    int x, y = 0;

    for (radiusMin = myGame->ball.radius ; radiusMin>=0; radiusMin--){
        for (float angle = 0.0; angle<360; angle++){
            x = myGame->ball.px-radiusMin * cos(angle);
            y = myGame->ball.py-radiusMin * sin(angle);
            SDL_RenderDrawPoint(myGame->displayGame.g_pRenderer, x, y);
            }
    }
}

/********************************************************************
PURPOSE : display AI's score
INPUT : gameObject, fontObject
OUTPUT : 1 texture containing AI's score
*********************************************************************/
void renderAIScore (PongGame *myGame, font myFont){
        char AIScoreArr [0];
        sprintf (AIScoreArr, "%i", myGame->score.AI);
        //fprintf(stdout,"score AI:%c%c\n", AIScoreArr[0],AIScoreArr[1]);
        SDL_Color fontColor={255,255,255};
        myGame->displayGame.g_pSurface=TTF_RenderText_Blended(myFont.g_font, AIScoreArr, fontColor);//Charge la police


        if(myGame->displayGame.g_pSurface){


                //Rectangle used to display character chain
                SDL_Rect playerScoreRect;
                playerScoreRect.x=SCREEN_WIDTH/2-SCORE_W+100;//start point (x)
                playerScoreRect.y=SCORE_Y;// start point (y)
                playerScoreRect.w=SCORE_W; //Width
                playerScoreRect.h=SCORE_H; //Height


                 myGame->displayGame.g_pTexture = SDL_CreateTextureFromSurface(myGame->displayGame.g_pRenderer,myGame->displayGame.g_pSurface);
                 SDL_FreeSurface(myGame->displayGame.g_pSurface);


                 if(myGame->displayGame.g_pTexture){
                        //  copy a portion of the texture to the current rendering target
                        SDL_RenderCopy(myGame->displayGame.g_pRenderer,myGame->displayGame.g_pTexture,NULL,&playerScoreRect);
                        SDL_DestroyTexture(myGame->displayGame.g_pTexture);
                 }
                 else{
                        fprintf(stdout,"Failed to create texture (%s)\n",SDL_GetError());
                }

                }
        else{
            fprintf(stdout,"Failed to create surface (%s)\n",SDL_GetError());
        }
}

/********************************************************************
PURPOSE : display player's score
INPUT : gameObject, fontObject
OUTPUT : 1 texture containing player's score
*********************************************************************/
void renderPlayerScore (PongGame *myGame, font myFont){

        char playerScoreArr [0];
        sprintf (playerScoreArr, "%i", myGame->score.player);
        //fprintf(stdout,"score player:%c%c\n", playerScoreArr[0],playerScoreArr[1]);
        SDL_Color fontColor={255,255,255};
        myGame->displayGame.g_pSurface=TTF_RenderText_Blended(myFont.g_font, playerScoreArr, fontColor);


        if(myGame->displayGame.g_pSurface){


                // Rectangle used to display character chain
                SDL_Rect playerScoreRect;
                playerScoreRect.x=SCREEN_WIDTH/2-100;//start point (x)
                playerScoreRect.y=SCORE_Y;// start point (y)
                playerScoreRect.w=SCORE_W; //Width
                playerScoreRect.h=SCORE_H; //Height


                 myGame->displayGame.g_pTexture = SDL_CreateTextureFromSurface(myGame->displayGame.g_pRenderer,myGame->displayGame.g_pSurface);
                 SDL_FreeSurface(myGame->displayGame.g_pSurface);


                 if(myGame->displayGame.g_pTexture){
                        // copy a portion of the texture to the current rendering target
                        SDL_RenderCopy(myGame->displayGame.g_pRenderer,myGame->displayGame.g_pTexture,NULL,&playerScoreRect);
                        SDL_DestroyTexture(myGame->displayGame.g_pTexture);
                 }
                 else{
                        fprintf(stdout,"Failed to create texture (%s)\n",SDL_GetError());
                }

                }
        else{
            fprintf(stdout,"Failed to create surface (%s)\n",SDL_GetError());
        }
}

/********************************************************************
PURPOSE :
Procedure calling all the render functions
Give a black background to the game
INPUT : gameObject, fontObject, color (RGB)
*********************************************************************/
void renderPongGame (PongGame myGame, font myFont){
    renderPaddles(&myGame);
    renderBoundaryLine (&myGame, 255, 255, 255);
    renderCircle (&myGame,255,255,255);
    renderAIScore (&myGame, myFont);
    renderPlayerScore (&myGame, myFont);

    SDL_RenderPresent(myGame.displayGame.g_pRenderer); // update the screen with any rendering performed since the previous cal

    SDL_SetRenderDrawColor(myGame.displayGame.g_pRenderer,0,0,0,255); // black background
    SDL_RenderClear(myGame.displayGame.g_pRenderer);
}

/********************************************************************
PURPOSE : Check if the ball hits a window's border
INPUT : gameObject
OUTPUT : Return Right or Left or Top or Bottom or None
*********************************************************************/
enum Collision CheckCollisionBallWalls (PongGame myGame){
    //check if ball hit right side
    if (myGame.ball.px >=SCREEN_WIDTH-BALL_RADIUS){
        //fprintf(stdout,"score right side\n");
        return Right;
    }
    //check if ball hit left side
    if (myGame.ball.px <=BALL_RADIUS){
        //fprintf(stdout,"score left side\n");
        return Left;
    }
    //check if ball hit top
    if (myGame.ball.py <= BALL_RADIUS){
        //fprintf(stdout,"collision on top\n");
        return Top;
    }
    //check if ball hit bottom
    if (myGame.ball.py >= SCREEN_HEIGHT-BALL_RADIUS){
        //fprintf(stdout,"collision on bottom\n");
        return Bottom;
    }

    // if no collision
    return None;
};

/********************************************************************
PURPOSE : Check if the AI or player has won.
INPUT : state variable, gameObject, fontObject
OUTPUT : gameIsRunning
*********************************************************************/
void checkVictoryConditions (int *gameIsRunning, PongGame *myGame, font myFont){
    if (myGame->score.AI >=SCORE_TO_WIN){
        endWindow (myGame, myFont, 0);
        SDL_Delay(4000);
        *gameIsRunning=0;
    }

    if (myGame->score.player >=SCORE_TO_WIN){
        endWindow (myGame, myFont, 1);
        SDL_Delay(4000);
        *gameIsRunning=0;
    }
}

/********************************************************************
PURPOSE : Reset ball position and direction
INPUT : gameObject
OUTPUT : ball.px, ball.py, ball.sx, ball.sy
*********************************************************************/
void resetBall (PongGame *myGame){
    myGame->ball.px= SCREEN_WIDTH/3 + (rand () % 200);
    myGame->ball.py= SCREEN_HEIGHT/3 + (rand () % 200);
    myGame->ball.sx= (rand () % 2 + 4) + cos (rand () % 90);
    myGame->ball.sy= (rand () % 1 + 2) + cos (rand () % 90);

    if ((rand () % 2)==1) {
        myGame->ball.sx*=-1;
    }

    if ((rand () % 2)==1) {
        myGame->ball.sy*=-1;
    }
}

/********************************************************************
PURPOSE : Check if the ball hits a racket
INPUT : gameObject
OUTPUT : return True or False
*********************************************************************/
enum BOOL CheckCollisionBallPaddles (PongGame myGame){
    //Racket1
   if ((myGame.ball.px-myGame.ball.radius)<=PADDLE_WIDTH &&
                        myGame.ball.py>=myGame.paddle1.py &&
                        myGame.ball.py<=(myGame.paddle1.py+PADDLE_HEIGHT)){
                            fprintf(stdout,"collision on left Racket\n");
                            return True;
                        }

    //Racket2

   if ((myGame.ball.px+myGame.ball.radius)>=(SCREEN_WIDTH-PADDLE_WIDTH) &&
                    myGame.ball.py>=myGame.paddle2.py &&
                    myGame.ball.py<=(myGame.paddle2.py+PADDLE_HEIGHT)){
                        fprintf(stdout,"collision on right Racket\n");
                        return True;
                    }

    return False;
};

/************************************************************************
PURPOSE :
Change ball direction and position according to collision events
Increment player's score and AI's score according to collision events
INPUT : gameObject
OUTPUT : ball.px, ball.py and/or ball.sy and/or (score.player or score.AI)
*************************************************************************/
void ballMovementAndScore(PongGame *myGame){
    // if ball hit right wall
    if (CheckCollisionBallWalls (*myGame)== Right){
        myGame->score.player+=1;
        resetBall (myGame);
    }

    // if ball hit left wall
    if (CheckCollisionBallWalls (*myGame)== Left){
        myGame->score.AI+=1;
        resetBall (myGame);
    }

    // if ball hit Top or Bottom
    if (CheckCollisionBallWalls (*myGame)== Top ||
        CheckCollisionBallWalls (*myGame)== Bottom
        ){
            myGame->ball.sy=-myGame->ball.sy*BOUNCE_SPEED;

            }
    // if ball hit a racket
    if (CheckCollisionBallPaddles (*myGame)== True){
            myGame->ball.sx=-myGame->ball.sx*BOUNCE_SPEED;
            myGame->ball.sy*=1.3;
    }

    //ball speed cap
    if (myGame->ball.sx>BALL_RADIUS-2){
            myGame->ball.sx=BALL_RADIUS-2;
            }

    if (myGame->ball.sy<-BALL_RADIUS-2){
          myGame->ball.sy=-BALL_RADIUS-2;
          }

    //ball movement
    myGame->ball.px+=myGame->ball.sx;
    myGame->ball.py+=myGame->ball.sy;

}

/********************************************************************
PURPOSE :
FrameRate management
Capped at 60fps(1 frame/16ms => (1/60fps)*1000 =16.67ms)
INPUT : FrameLimit
OUTPUT : Delay=>frameLimit
*********************************************************************/
void delay(unsigned int frameLimit){

    unsigned int ticks = SDL_GetTicks();

    if (frameLimit < ticks)
    {
        return;
    }

    if (frameLimit > ticks + 16)
    {
        SDL_Delay(16);

    } else {
        SDL_Delay(frameLimit - ticks);
    }
}

/********************************************************************
PURPOSE : create and display a window displaying the winner
INPUT : gameObject, fontObject
OUTPUT : window with 1 texture containing the name of the winner.
*********************************************************************/
void endWindow (PongGame *myGame, font myFont, int winner){
    SDL_Color fontColor={255,255,255};

    if (winner==0)
    {
        myGame->displayGame.g_pSurface=TTF_RenderText_Blended(myFont.g_font, "You lost!", fontColor);
    }
    else
    {
        myGame->displayGame.g_pSurface=TTF_RenderText_Blended(myFont.g_font, "You won!", fontColor);
    }

     if(myGame->displayGame.g_pSurface)
     {

                SDL_Rect Window1; //Rectangle used to display character chain
                Window1.x=MAIN_TEXT_X;//start point (x)
                Window1.y=MAIN_TEXT_Y;// start point (y)
                Window1.w=MAIN_TEXT_W; //Width
                Window1.h=MAIN_TEXT_H; //Height

                 myGame->displayGame.g_pTexture = SDL_CreateTextureFromSurface(myGame->displayGame.g_pRenderer,myGame->displayGame.g_pSurface);
                 SDL_FreeSurface(myGame->displayGame.g_pSurface);

                 if(myGame->displayGame.g_pTexture){
                        //  copy a portion of the texture to the current rendering target
                        SDL_RenderCopy(myGame->displayGame.g_pRenderer,myGame->displayGame.g_pTexture,NULL,&Window1);
                        SDL_DestroyTexture(myGame->displayGame.g_pTexture);
                        SDL_RenderPresent(myGame->displayGame.g_pRenderer);
                 }
                 else
                 {
                        fprintf(stdout,"Failed to create texture (%s)\n",SDL_GetError());
                 }
    }
        else
        {
            fprintf(stdout,"Failed to create surface (%s)\n",SDL_GetError());
        }
}

/********************************************************************
PURPOSE :
Destroy the rendering context, the texture and the surface
Free RGB surface
INPUT: GameObject
*********************************************************************/
void destroy(PongGame *myGame){

    //Destroy render
    if(myGame->displayGame.g_pRenderer!=NULL)
        SDL_DestroyRenderer(myGame->displayGame.g_pRenderer);

    //Destroy surface
    if(myGame->displayGame.g_pSurface!=NULL)
         SDL_FreeSurface(myGame->displayGame.g_pSurface);

    //Destroy texture
    if(myGame->displayGame.g_pTexture!=NULL)
         SDL_DestroyTexture(myGame->displayGame.g_pTexture);

    //Destroy window
    if(myGame->displayGame.g_pWindow!=NULL)
        SDL_DestroyWindow(myGame->displayGame.g_pWindow);

}

/********************************************************************
PURPOSE : Free the memory used by font
INPUT : fontObject
*********************************************************************/
void releaseFont (font *myFont){
    if (myFont->g_font)
    {
        TTF_CloseFont (myFont->g_font);
    }
}
