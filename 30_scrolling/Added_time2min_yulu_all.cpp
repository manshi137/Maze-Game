/*This source code copyrighted by Lazy Foo' Productions (2004-2022)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, vectors, and strings
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <iostream>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080
 
using namespace std;

//The dimensions of the level
const int LEVEL_WIDTH = 9952;
const int LEVEL_HEIGHT = 3936;

//Screen dimension constants
const int SCREEN_WIDTH = 1400;
const int SCREEN_HEIGHT = 800;

//Key press surfaces constants
enum KeyPressSurfaces
{
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};
//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );
		
		#if defined(SDL_TTF_MAJOR_VERSION)
		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
		#endif

		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );
		
		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		//Gets image dimensions
		int getWidth();
		int getHeight();
		bool isdone=false;
		bool taskdoneonce=true;
		bool dotask=false;
	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
};

//The dot that will move around on the screen
class Dot
{
    public:
		//The dimensions of the dot
		static const int DOT_WIDTH = 40;
		static const int DOT_HEIGHT = 40;
		//The dimensions of the dot
		static const int GHOST_WIDTH = 20;
		static const int GHOST_HEIGHT = 20;
		//Maximum axis velocity of the dot
		int DOT_VEL = 20;
		//Initializes the variables
		Dot();
		//Takes key presses and adjusts the dot's velocity
		void handleEvent( SDL_Event& e , int m2times);

		//Moves the dot and checks collision
		void move( SDL_Rect wall[] , int length1, SDL_Rect ghostCollider);

		//Shows the dot on the screen relative to the camera
		void render( int camX, int camY );

		//Dot's collision box
		SDL_Rect mCollider;

		//Position accessors
		int getPosX();
		int getPosY();
		
		//life attribute
		int life=5;
		int points = 0;
		bool pointsupdated = false;
		bool manshi=true; // true means no collision
		int manshi2=1;
		int manshi3=1;
		int manshi4=1;
		int manshi5=1;
		//false means collision
		
		void startVel();
		void stopVel();

		//The velocity of the dot
		int mVelX, mVelY;

    private:
		//The X and Y offsets of the dot
		int mPosX, mPosY;
		int prevmVelX, prevmVelY;
		
		//Ghost's collision box
		SDL_Rect ghostCollider;

		
};
//========================================================================================================
class Ghost
{
    public:
		//The dimensions of the dot
		static const int GHOST_WIDTH = 20;
		static const int GHOST_HEIGHT = 20;

		//Maximum axis velocity of the dot
		static const int GHOST_VEL = 1;

		//Initializes the variables
		Ghost(int x, int y);

		//Takes key presses and adjusts the ghost's velocity
		void handleEvent( SDL_Event& e );

		//Moves the ghost and checks collision
		void move( SDL_Rect wall[] , int length);

		//Shows the ghost on the screen relative to the camera
		void render( int camX, int camY, int isDoc );
		
		//Position accessors
		int getPosX();
		int getPosY();
		
		void startVel();
		void stopVel();
		//Ghost's collision box
		SDL_Rect ghostCollider;
		

    private:
		//The X and Y offsets of the ghost
		int ghostPosX, ghostPosY;

		//The velocity of the ghost
		int ghostVelX, ghostVelY;
		int prevghostVelX, prevghostVelY;
		
};
//=================================================================================================================


//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image
LTexture* loadSurface( std::string path );
//==============The images that correspond to a keypress
LTexture gKeyPressSurfaces[ KEY_PRESS_SURFACE_TOTAL ];

//==============
//Box collision detector
bool checkCollision( SDL_Rect a, SDL_Rect b[], int length );

bool checkCollisionC( SDL_Rect c, SDL_Rect b[], int length );
bool checkCollisionAC( SDL_Rect a, SDL_Rect c );
bool checkCollisionlife( SDL_Rect a, SDL_Rect b[], int length );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Globally used font
TTF_Font *gFont = NULL;

//Scene textures
LTexture gTimeTextTexture;
LTexture gPromptTextTexture;


//Scene textures
LTexture gDotTexture;
LTexture ghostTexture1;
LTexture ghostTexture2;
LTexture ghostTexture3;
LTexture candaceTexture;
LTexture isabellaTexture;
LTexture vanessaTexture;


LTexture gBGTexture;
LTexture gPauseTexture;
LTexture gStartTexture;
LTexture gMapWindowTexture;
LTexture gInstrWindowTexture;
LTexture gStopTexture;
LTexture oops;
LTexture yulu;
LTexture nocoins;
LTexture win;

LTexture lifeT1;
LTexture lifeT2;
LTexture lifeT3;
LTexture lifeT4;
LTexture lifeT5;
LTexture task1;
LTexture t1;
LTexture task2;
LTexture t2;
LTexture task22;
LTexture task3;
LTexture t3;
LTexture task41;
LTexture task42;
LTexture task43;
LTexture task44;
LTexture task5;
LTexture t5;
LTexture task61;
LTexture t61;
LTexture task62;
LTexture t62;
LTexture task63;
LTexture t63;
LTexture task7;
LTexture t7;
LTexture task8;
LTexture t8;
LTexture task9;
LTexture t9;

LTexture taskdone;
LTexture notask;
LTexture point1;
LTexture yululogo;
LTexture player2;
LTexture taskarray[11]={task1,task2,task22,task3,task5,task61,task62,task63,task7,task8,task9};
// int begin = rand();
int todotasks[5];



//The music that will be played
Mix_Music *gMusic = NULL;

//The sound effects that will be used
Mix_Chunk *gScratch = NULL;
Mix_Chunk *gHigh = NULL;
Mix_Chunk *gPlatypus = NULL;
Mix_Chunk *gCandace = NULL;
Mix_Chunk *gIsabella = NULL;
Mix_Chunk *gVanessa = NULL;
Mix_Chunk *gDog = NULL;
Mix_Chunk *gYulu = NULL;
Mix_Chunk *gEnergy = NULL;
Mix_Chunk *gPause = NULL;
Mix_Chunk *gMedium = NULL;
Mix_Chunk *gLow = NULL;


LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0x03, 0xFC, 0x00 ) );
		//SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0xFC, 0xFF, 0xFD ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface != NULL )
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	else
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}

	
	//Return success
	return mTexture != NULL;
}
#endif

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}
		
void LTexture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

Dot::Dot()
{
    //Initialize the offsets
    //initial position
    // mPosX = 300*32;//gate1
    // mPosY = 57*32;

    mPosX = 21*32; //gate6
    mPosY = 8*32;
	
    //Set collision box dimension
	mCollider.w = DOT_WIDTH;
	mCollider.h = DOT_HEIGHT;
	
	//Set collision box dimension
	ghostCollider.w = GHOST_WIDTH;
	ghostCollider.h = GHOST_HEIGHT;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;
    prevmVelX=0;
    prevmVelY=0;
}
//===========

Ghost::Ghost(int x, int y)
{
    //Initialize the offsets
    ghostPosX = x;
    ghostPosY = y;
    
    //Set collision box dimension
	ghostCollider.w = GHOST_WIDTH;
	ghostCollider.h = GHOST_HEIGHT;
	

    //Initialize the velocity
    ghostVelX = -5;
    ghostVelY = -5;
    prevghostVelX=0;
    prevghostVelY=0;
}
//========
void Dot::handleEvent( SDL_Event& e , int m2times)
{
    //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_m:
            		if(m2times==1)
            		{
            			DOT_VEL=0;
            			break;
            		}   
            		else{
            		DOT_VEL= 20;
            		break;
            		}
            case SDLK_y: DOT_VEL =35; break; //yulu increases speed by 10
            case SDLK_w: DOT_VEL= 0;break;
            case SDLK_s: DOT_VEL= 20;break;
            case SDLK_UP: mVelY = -DOT_VEL; break;
            case SDLK_DOWN: mVelY = DOT_VEL; break;
            case SDLK_LEFT: mVelX = -DOT_VEL; break;
            case SDLK_RIGHT: mVelX = DOT_VEL; break;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_y: DOT_VEL =35; break; //yulu increases speed by 10
            case SDLK_w: DOT_VEL= 0;break;
            case SDLK_s: DOT_VEL= 20;break;
            case SDLK_UP: mVelY = 0; break;
            case SDLK_DOWN: mVelY = 0; break;
            case SDLK_LEFT: mVelX = 0; break;
            case SDLK_RIGHT: mVelX = 0; break;
        }
    }
}
//===================
void Ghost::handleEvent( SDL_Event& e )
{
    //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: ghostVelY = rand()%16 -8; break;
            
            case SDLK_DOWN: ghostVelY = rand()%16 -8; break;
            case SDLK_LEFT: ghostVelX = rand()%16 -8; break;
            
            case SDLK_RIGHT: ghostVelX = rand()%16 -8; break;

        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: ghostVelY =rand()%16 -8; break;
            case SDLK_DOWN: ghostVelY = rand()%16 -8; break;
            case SDLK_LEFT: ghostVelX = rand()%16 -8; break;
            case SDLK_RIGHT: ghostVelX = rand()%16 -8; break;
           
        }
    }
}
//=====================

void Dot::move(SDL_Rect wall[], int length1 ,SDL_Rect ghostCollider)
{
    //Move the dot left or right
    mPosX += mVelX;
mCollider.x = mPosX;
    //If the dot went too far to the left or right
    if( ( mPosX < 0 ) || ( mPosX + DOT_WIDTH > LEVEL_WIDTH ) || checkCollision( mCollider, wall, length1 ) )
    {
        //Move back
        mPosX -= mVelX;
        mCollider.x = mPosX;
    }

    //Move the dot up or down
    mPosY += mVelY;
	mCollider.y = mPosY;    

    //If the dot went too far up or down
    if( ( mPosY < 0 ) || ( mPosY + DOT_HEIGHT > LEVEL_HEIGHT ) || checkCollision( mCollider, wall, length1 ))
    {
        //Move back
        mPosY -= mVelY;
        mCollider.y = mPosY;
    }
    if( !checkCollisionAC(mCollider, ghostCollider)){
    mPosX= 306	* 32;	
    mPosY= 57* 32;

    Mix_PlayChannel( -1, gLow, 0 );
    
	}
	// if(( mPosY < 0 ) || ( mPosY + DOT_HEIGHT > LEVEL_HEIGHT ) || !checkCollisionAC(mCollider, perryCollider)){
    // points+=2;
	// pointsupdated=true;
	
	// }
}

//=============

void Ghost::move(SDL_Rect wall[], int length)
{
    //Move the dot left or right
    ghostPosX += ghostVelX;
ghostCollider.x = ghostPosX;
    //If the dot went too far to the left or right
    if( ( ghostPosX < 0 ) || ( ghostPosX + GHOST_WIDTH > LEVEL_WIDTH ) || checkCollisionC( ghostCollider, wall, length ) )
    {
        //Move back
        ghostPosX -= ghostVelX;
        ghostCollider.x = ghostPosX;
    }

    //Move the dot up or down
    ghostPosY += ghostVelY;
	ghostCollider.y = ghostPosY;    

    //If the dot went too far up or down
    if( ( ghostPosY < 0 ) || ( ghostPosY + GHOST_HEIGHT > LEVEL_HEIGHT ) || checkCollisionC( ghostCollider, wall, length ))
    {
        //Move back
        ghostPosY -= ghostVelY;
        ghostCollider.y = ghostPosY;
    }
	
}


//=============
void Dot::render( int camX, int camY )
{
    //Show the dot relative to the camera
	gDotTexture.render( mPosX - camX, mPosY - camY );
}
//===========
void Ghost::render( int camX, int camY,int isDoc )
{
    //Show the dot relative to the camera
    if(isDoc==1){// render doctor
	ghostTexture1.render( ghostPosX - camX, ghostPosY - camY );}
	else if(isDoc==0){//render dog
	ghostTexture2.render( ghostPosX - camX, ghostPosY - camY );}
	else if(isDoc==2){
	ghostTexture3.render( ghostPosX - camX, ghostPosY - camY );}
	
	else if(isDoc==3){
	candaceTexture.render( ghostPosX - camX, ghostPosY - camY );}
	
	else if(isDoc==4){
	isabellaTexture.render( ghostPosX - camX, ghostPosY - camY );}
	
	else if(isDoc==5){
	vanessaTexture.render( ghostPosX - camX, ghostPosY - camY );}
	
}
//===========

void Dot::stopVel()
{
prevmVelX = mVelX;
mVelX= 0;
prevmVelY = mVelY;
mVelY= 0;	
}

void Dot::startVel()
{
mVelX = prevmVelX;
mVelY = prevmVelY;
}

void Ghost::startVel()
{
ghostVelX = prevghostVelX;
ghostVelY = prevghostVelY;	
}

int Dot::getPosX()
{
	return mPosX;
}

int Dot::getPosY()
{
	return mPosY;
}
//===========

void Ghost:: stopVel()
{
prevghostVelX = ghostVelX;
ghostVelX= 0;
prevghostVelY = ghostVelY;
ghostVelY= 0;
}

int Ghost::getPosX()
{
	return ghostPosX;
}

int Ghost::getPosY()
{
	return ghostPosY;
}
//=============
bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
				//Initialize SDL_mixer
				if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
				{
					printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
					success = false;
				}
				
				//Initialize SDL_ttf
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;
	
	//Open the font
	gFont = TTF_OpenFont( "lazy.ttf", 28 );
	if( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else
	{
		//Set text color as black
		SDL_Color textColor = { 0, 0, 0, 255 };
		
		//Load prompt texture
		if( !gPromptTextTexture.loadFromRenderedText( "Press M to display Map", textColor ) )
		{
			printf( "Unable to render prompt texture!\n" );
			success = false;
		}
	}
	
	//Load music
	gMusic = Mix_LoadMUS( "sounds/beat.wav" );
	if( gMusic == NULL )
	{
		printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	
	//Load sound effects
	gScratch = Mix_LoadWAV( "sounds/scratch.wav" );
	if( gScratch == NULL )
	{
		printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	
	gHigh = Mix_LoadWAV( "sounds/smb_coin.wav" );
	if( gHigh == NULL )
	{
		printf( "Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	
	gPlatypus = Mix_LoadWAV( "sounds/smb_1-up.wav" );
	if( gPlatypus == NULL )
	{
		printf( "Failed to load platypus sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	gCandace = Mix_LoadWAV( "sounds/Candace.wav" );
	if( gCandace == NULL )
	{
		printf( "Failed to load Candace sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	gIsabella = Mix_LoadWAV( "sounds/isabella.wav" );
	if( gIsabella == NULL )
	{
		printf( "Failed to load Isabella sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	gVanessa = Mix_LoadWAV( "sounds/vanessa.wav" );
	if( gVanessa == NULL )
	{
		printf( "Failed to load Vanessa sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	gDog = Mix_LoadWAV( "sounds/dog.wav" );
	if( gDog == NULL )
	{
		printf( "Failed to load dog sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	gYulu = Mix_LoadWAV( "sounds/yulu.wav" );
	if( gYulu == NULL )
	{
		printf( "Failed to load yulu sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}


	gEnergy = Mix_LoadWAV( "sounds/smb_powerup.wav" );
	if( gEnergy == NULL )
	{
		printf( "Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	gPause =Mix_LoadWAV( "sounds/smb_pause.wav" );
	if( gPause == NULL )
	{
		printf( "Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	

	gMedium = Mix_LoadWAV( "sounds/smb_mariodie.wav" );
	if( gMedium == NULL )
	{
		printf( "Failed to load medium sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	gLow = Mix_LoadWAV( "sounds/smb_flagpole.wav" );
	if( gLow == NULL )
	{
		printf( "Failed to load low sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	
	if( !gMapWindowTexture.loadFromFile( "pics/map2.png" ) )
	{
		printf( "Failed to load front map texture!\n" );
		success = false;
	}
	else
	{
		//Set standard alpha blending
		gMapWindowTexture.setBlendMode( SDL_BLENDMODE_BLEND );
	}
	if( !gInstrWindowTexture.loadFromFile( "pics/instructions.png" ) )
	{
	
		printf( "Failed to load front instruction texture!\n" );
		success = false;
	}
	else
	{
		//Set standard alpha blending
		gInstrWindowTexture.setBlendMode( SDL_BLENDMODE_BLEND );
	}

	

	if( !gPauseTexture.loadFromFile( "pics/Pause.png" ) )
	{
		printf( "Failed to load front pause texture!\n" );
		success = false;
	}
	else
	{
		//Set standard alpha blending
		gPauseTexture.setBlendMode( SDL_BLENDMODE_BLEND );
	}

	if( !gStartTexture.loadFromFile( "pics/Start.png" ) )
	{
		printf( "Failed to load front start texture!\n" );
		success = false;
	}
	else
	{
		//Set standard alpha blending
		gStartTexture.setBlendMode( SDL_BLENDMODE_BLEND );
	}
	if( !gStopTexture.loadFromFile( "pics/Stop.png" ) )
	{
		printf( "Failed to load front pause texture!\n" );
		success = false;
	}
	else
	{
		//Set standard alpha blending
		gStopTexture.setBlendMode( SDL_BLENDMODE_BLEND );
	}

	
	gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ].loadFromFile( "pics/ferbright.png" );
	gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ].loadFromFile( "pics/ferbleft.png");
	gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ].loadFromFile( "pics/ferbleft.png");
	gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ].loadFromFile( "pics/ferbleft.png");
	gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ].loadFromFile( "pics/ferbleft.png");
	
	//Load ghost texture1
	if( !ghostTexture1.loadFromFile( "pics/doc.png" ) )
	{
		printf( "Failed to load doc texture!\n" );
		success = false;
	}
	
	//Load ghost texture2
	if( !ghostTexture2.loadFromFile( "pics/dog(2).png" ) )
	{
		printf( "Failed to load dog texture!\n" );
		success = false;
	}
	if( !ghostTexture3.loadFromFile( "pics/perry.png" ) )
	{
		printf( "Failed to load dog texture!\n" );
		success = false;
	}
	if( !candaceTexture.loadFromFile( "pics/candace.png" ) )
	{
		printf( "Failed to load dog texture!\n" );
		success = false;
	}
	if( !isabellaTexture.loadFromFile( "pics/isabella.png" ) )
	{
		printf( "Failed to load dog texture!\n" );
		success = false;
	}
	if( !vanessaTexture.loadFromFile( "pics/vanessa.png" ) )
	{
		printf( "Failed to load dog texture!\n" );
		success = false;
	}
	//Load background texture
	if( !gBGTexture.loadFromFile( "pics/map.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !lifeT1.loadFromFile( "pics/1.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !lifeT2.loadFromFile( "pics/2.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !lifeT3.loadFromFile( "pics/3.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !lifeT4.loadFromFile( "pics/4.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !lifeT5.loadFromFile( "pics/5.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	// if( !task1.loadFromFile( "pics/task1.png" ) )
	if( !taskarray[0].loadFromFile( "pics/task1.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	// if( !task2.loadFromFile( "pics/task2.png" ) )
	if( !taskarray[1].loadFromFile( "pics/task2.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	// if( !task22.loadFromFile( "pics/task2.png" ) )
	if( !taskarray[2].loadFromFile( "pics/task2.png" ) )
 	{
 		printf( "Failed to load background texture!\n" );
 		success = false;
 	}
	// if( !task3.loadFromFile( "pics/task3.png" ) )
	if( !taskarray[3].loadFromFile( "pics/task3.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !task41.loadFromFile( "pics/task4.1.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !task42.loadFromFile( "pics/task4.2.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !task43.loadFromFile( "pics/task4.3.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !task44.loadFromFile( "pics/task4.4.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	// if( !task5.loadFromFile( "pics/task5.png" ) )
	if( !taskarray[4].loadFromFile( "pics/task5.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	// if( !task61.loadFromFile( "pics/task6.png" ) )
	if( !taskarray[5].loadFromFile( "pics/task6.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	// if( !task62.loadFromFile( "pics/task6.2.png" ) )
	if( !taskarray[6].loadFromFile( "pics/task6.2.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	// if( !task63.loadFromFile( "pics/task6.3.png" ) )
	if( !taskarray[7].loadFromFile( "pics/task6.3.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	// if( !task7.loadFromFile( "pics/task7.png" ) )
	if( !taskarray[8].loadFromFile( "pics/task7.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	// if( !task8.loadFromFile( "pics/task8.png" ) )
	if( !taskarray[9].loadFromFile( "pics/task8.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	// if( !task9.loadFromFile( "pics/task9.png" ) )
	if( !taskarray[10].loadFromFile( "pics/task9.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	
	if( !taskdone.loadFromFile( "pics/taskdone.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
    if( !player2.loadFromFile( "pics/phinleft.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !point1.loadFromFile( "pics/coin(1).png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	
	if( !yululogo.loadFromFile( "pics/yululogo.png" ) )
	{
		printf( "Failed to load yululogo texture!\n" );
		success = false;
	}
	
	
if( !oops.loadFromFile( "pics/oops.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
if( !yulu.loadFromFile( "pics/yulu.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}

if( !nocoins.loadFromFile( "pics/nocoins.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
if( !win.loadFromFile( "pics/win.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
if( !notask.loadFromFile( "pics/notask.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
if( !t1.loadFromFile( "pics/attend1.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
if( !t2.loadFromFile( "pics/food.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
if( !t3.loadFromFile( "pics/book3.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
if( !t5.loadFromFile( "pics/gym5.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
if( !t61.loadFromFile( "pics/cricket6.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
if( !t62.loadFromFile( "pics/hockey62.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
if( !t63.loadFromFile( "pics/tennis6.3.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
if( !t7.loadFromFile( "pics/assign7.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
if( !t8.loadFromFile( "pics/market8.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
if( !t9.loadFromFile( "pics/project9.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	return success;
}

void close()
{
	
	//Free loaded images
	gTimeTextTexture.free();
	gPromptTextTexture.free();

	//Free global font
	TTF_CloseFont( gFont );
	gFont = NULL;	
	
	//Free loaded images
	gDotTexture.free();
	ghostTexture1.free();
	ghostTexture2.free();
	ghostTexture3.free();
	candaceTexture.free();
	isabellaTexture.free();
	vanessaTexture.free();

	gBGTexture.free();
	gMapWindowTexture.free();
	gInstrWindowTexture.free();
	gPauseTexture.free();
	gStartTexture.free();
	gStopTexture.free();
	lifeT1.free();
	lifeT2.free();
	lifeT3.free();
	lifeT4.free();
	lifeT5.free();
	task1.free();
	t1.free();
	task2.free();
	t2.free();
	task22.free();
	task3.free();
	t3.free();
	task41.free();
	task42.free();
	task43.free();
	task44.free();
	task5.free();
	t5.free();
	task61.free();
	t61.free();
	task62.free();
	t62.free();
	task63.free();
	t63.free();
	task7.free();
	t7.free();
	task8.free();
	t8.free();
	task9.free();
	t9.free();
	taskdone.free();
    player2.free();
	point1.free();
	yululogo.free();
	oops.free();
	yulu.free();
	nocoins.free();
	win.free();
	notask.free();
	//Free the sound effects
	Mix_FreeChunk( gScratch );
	Mix_FreeChunk( gHigh );
	Mix_FreeChunk( gPlatypus );
	Mix_FreeChunk( gCandace );
	Mix_FreeChunk( gIsabella );
	Mix_FreeChunk( gVanessa );
	Mix_FreeChunk( gDog );
	Mix_FreeChunk( gYulu );
	Mix_FreeChunk( gEnergy );
	Mix_FreeChunk( gPause );
	Mix_FreeChunk( gMedium );
	Mix_FreeChunk( gLow );
	gScratch = NULL;
	gHigh = NULL;
	gPlatypus = NULL;
	gCandace= NULL;
	gIsabella= NULL;
	gVanessa= NULL;
	gDog= NULL;
	gYulu= NULL;
	gEnergy = NULL;
	gPause = NULL;
	gMedium = NULL;
	gLow = NULL;
	
	//Free the music
	Mix_FreeMusic( gMusic );
	gMusic = NULL;
	
	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}
//check collsion between player and walls

bool checkCollision( SDL_Rect a,SDL_Rect b[], int length )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;
    bool iscollisionA =  true;
 
    //true means no collision =  outside rectangles
for(  int i=0;i< length ;i++){
    //Calculate the sides of rect B
    leftB = b[i].x;
    rightB = b[i].x + b[i].w;
    topB = b[i].y;
    bottomB = b[i].y + b[i].h;
	
	if(!iscollisionA){return false;} 	
	else{
    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        iscollisionA= true;
    }

    else if( topA >= bottomB )
    {
        iscollisionA= true;
    }

    else if( rightA <= leftB )
    {
        iscollisionA= true;
    }

    else if( leftA >= rightB )
    {
        iscollisionA= true;
    }
    else{ iscollisionA= false;}}
}
    //If none of the sides from A are outside B
    
 return iscollisionA;   
}

//check collision between dogs and walls

bool checkCollisionC( SDL_Rect c, SDL_Rect b[], int length )
{
    //The sides of the rectangles
    int leftB, leftC;
    int rightB, rightC;
    int topB, topC;
    int bottomB, bottomC;

   
    //Calculate the sides of rect C
    leftC = c.x;
    rightC = c.x + c.w;
    topC = c.y;
    bottomC = c.y + c.h;
    
    bool iscollisionC =  true;
    
    //true means no collision =  outside rectangles
for(  int i=0;i< length ;i++){
    //Calculate the sides of rect B
    leftB = b[i].x;
    rightB = b[i].x + b[i].w;
    topB = b[i].y;
    bottomB = b[i].y + b[i].h;
	
	if(!iscollisionC){return false;} 	
	else{
    //If any of the sides from C are outside of B
    if( bottomC <= topB )
    {
        iscollisionC= true;
    }

    else if( topC >= bottomB )
    {
        iscollisionC= true;
    }

    else if( rightC <= leftB )
    {
        iscollisionC= true;
    }

    else if( leftC >= rightB )
    {
        iscollisionC= true;
    }
    else{ iscollisionC= false;}}
}
    //If none of the sides from C are outside B
    
 return iscollisionC;   
}

//check collision between dogs(b) and player (a)
bool checkCollisionlife( SDL_Rect a, SDL_Rect b[], int length )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A = player
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;
    bool iscollisionlife =  true;
    
    //Calculate the sides of rect B = dogs/ghosts
    for(int i=0;i<length;i++){

    leftB = b[i].x;
    rightB = b[i].x + b[i].w;
    topB = b[i].y;
    bottomB = b[i].y + b[i].h;

    //If any of the sides from A are outside of B
    if(!iscollisionlife){return false;} 	
	else{
    //If any of the sides from C are outside of B
    if( bottomA <= topB )
    {
        iscollisionlife= true;
    }

    else if( topA >= bottomB )
    {
        iscollisionlife= true;
    }

    else if( rightA <= leftB )
    {
        iscollisionlife= true;
    }

    else if( leftA >= rightB )
    {
        iscollisionlife= true;
    }
    else{ iscollisionlife= false;}}
    }

    //If none of the sides from A are outside B
    
    return iscollisionlife;
}

//check collision between two rectangles 
bool checkCollisionAC( SDL_Rect a, SDL_Rect b )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
 
        return true;
    }

    if( topA >= bottomB )
    {
    
        return true;
    }

    if( rightA <= leftB )
    {
    
        return true;
    }

    if( leftA >= rightB )
    {
    
        return true;
    }

    //If none of the sides from A are outside B
    
    return false;
}




int main( int argc, char* args[] )
{	//sockets start
 /*   int counter =0;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
 
    // Convert IPv4 and IPv6 addresses from text to binary
    if (inet_pton(AF_INET, "10.184.7.66", &serv_addr.sin_addr)
        <= 0) {
        printf(
            "\nInvalid address/ Address not supported \n");
        return -1;
    }
    if (connect(sock, (struct sockaddr*)&serv_addr,
                sizeof(serv_addr))
        < 0) {
        printf("\nConnection Failed \n");
        return -1;
    } */
    //sockets end
// int begin=rand()+1;

int begin=rand();
for(int i=0; i<5; i++){
	// todotasks[i]=taskarray[(begin +4)%10];
	todotasks[i]=(begin +4)%10;
	cout<<(begin +4)%10<<":index ";
	taskarray[(begin +4)%10].taskdoneonce=false;
	cout<<taskarray[i].taskdoneonce<<":taskarray"<<endl;
	taskarray[(begin +4)%10].dotask=true;
	begin+=4;
	cout<<begin<<endl;
}


cout<<endl;
for(int i=0;i<11;i++){
	cout<<taskarray[i].taskdoneonce<<" "<<taskarray[i].dotask<<endl;

}
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;
			
			//Set text color as black
			SDL_Color textColor = { 0, 0, 0, 255 };

			//Current time start time
			Uint32 startTime = 0;
			Uint32 yuluStart = 120000;

			//In memory text stream
			std::stringstream timeText;
			
			//Modulation component
			Uint8 a = 0;
			Uint8 b = 255;
			Uint8 c = 0;
			Uint8 d = 0;
			Uint8 ee = 0;


			//The dot that will be moving around on the screen
			Dot dot;
			Ghost doctor(192* 32-64,57* 32);
			Ghost perry(149*32,57*32);
			Ghost candace(26*32,75*32);//candace in nalanda
			Ghost isabella(266*32,84*32);//isabella in kailash
			Ghost vanessa(294*32,84*32-32);//vanessa in himadri

			Ghost ghost2(33* 32,99* 32 -32);

			Ghost ghost3(114* 32,57* 32);
			Ghost ghost4(150* 32,57* 32);
			Ghost ghost1(46* 32,57* 32);
			Ghost ghost5(70*32,76*32);
			Ghost ghost6(217*32,57*32);
			//Set the wall
			SDL_Rect wall1;
			wall1.x = 672;
			wall1.y = 224;
			wall1.w = 64;
			wall1.h = 3008;
			
			SDL_Rect wall2;
			wall2.x = 22* 32;
			wall2.y = 25* 32 -32;
			wall2.w = 61* 32;
			wall2.h = 2 * 32 +32;
			
			SDL_Rect wall3;
			wall3.x = 46* 32 - 32;
			wall3.y = 26* 32;
			wall3.w = 2* 32 + 32;
			wall3.h = 43* 32;
			//oops
			SDL_Rect wall4;
			wall4.x = 22* 32;
			wall4.y = 57* 32 -32 ;
			wall4.w = 281* 32;
			wall4.h = 2* 32 + 32;
			
			SDL_Rect wall5;
			wall5.x = 22* 32;
			wall5.y = 99* 32 -32;
			wall5.w = 49* 32;
			wall5.h = 2 * 32 +32;
			
			SDL_Rect wall6;
			wall6.x = 70* 32-32;
			wall6.y = 26* 32;
			wall6.w = 2* 32+32;
			wall6.h = 86* 32;
			
			SDL_Rect wall7;
			wall7.x = 71* 32;
			wall7.y = 110*32 -32;
			wall7.w = 232* 32;
			wall7.h = 2*32 +32;
			
			SDL_Rect wall8;
			wall8.x = 255* 32 -32;
			wall8.y = 27* 32;
			wall8.w = 2* 32 +32;
			wall8.h = 84 * 32;
			
			SDL_Rect wall9;
			wall9.x = 150* 32;
			wall9.y = 27* 32 -32;
			wall9.w = 107* 32;
			wall9.h = 2* 32 +32;
			
			SDL_Rect wall10;
			wall10.x = 217* 32 -32;
			wall10.y = 28* 32;
			wall10.w = 2* 32 +32;
			wall10.h = 31* 32;
			
			SDL_Rect wall11;
			wall11.x = 165* 32 -32;
			wall11.y = 28* 32;
			wall11.w = 2* 32 +32;
			wall11.h = 31* 32;
			
			SDL_Rect wall12;
			wall12.x = 150* 32 -32;
			wall12.y = 28* 32;
			wall12.w = 2* 32 +32;
			wall12.h = 31 * 32;
			
			SDL_Rect wall13;
			wall13.x = 192* 32 -64;
			wall13.y = 25* 32;
			wall13.w = 1* 32 +64;
			wall13.h = 38* 32;
			
			SDL_Rect wall14;
			wall14.x = 22* 32;
			wall14.y = 54* 32 -64;
			wall14.w = 25* 32;
			wall14.h = 1* 32 +64;
			
			SDL_Rect wall15;
			wall15.x = 3* 32;
			wall15.y = 94* 32-32;
			wall15.w = 19* 32;
			wall15.h = 2* 32 +32;
			
			SDL_Rect wall16;
			wall16.x = 24* 32;
			wall16.y = 76* 32-64;
			wall16.w = 4* 32;
			wall16.h = 1* 32+64;
			
			SDL_Rect wall17;
			wall17.x = 29* 32-64;
			wall17.y = 62* 32;
			wall17.w = 1* 32+64;
			wall17.h = 15* 32;
			
			SDL_Rect wall18;
			wall18.x = 29* 32;
			wall18.y = 62* 32-64;
			wall18.w = 18* 32;
			wall18.h = 1* 32+64;
			
			SDL_Rect wall19;
			wall19.x = 82* 32-64;
			wall19.y = 25* 32;
			wall19.w = 1* 32+64;
			wall19.h = 6* 32;
			
			SDL_Rect wall20;
			wall20.x = 82* 32;
			wall20.y = 30* 32-64;
			wall20.w = 12* 32;
			wall20.h = 1* 32+64;
			
			SDL_Rect wall21;
			wall21.x = 131* 32-64;
			wall21.y = 58* 32;
			wall21.w = 1* 32+64;
			wall21.h = 10* 32;
			
			SDL_Rect wall22;
			wall22.x = 134* 32-64;
			wall22.y = 58* 32;
			wall22.w = 1* 32+64;
			wall22.h = 10* 32;
			
			SDL_Rect wall23;
			wall23.x = 131* 32;
			wall23.y = 67* 32-64;
			wall23.w = 4* 32;
			wall23.h = 1* 32+64;
			
			SDL_Rect wall24;
			wall24.x = 140* 32-64;
			wall24.y = 51* 32;
			wall24.w = 1* 32+64;
			wall24.h = 7* 32;
			
			SDL_Rect wall25;
			wall25.x = 140* 32-64;
			wall25.y = 28* 32;
			wall25.w = 1* 32+64;
			wall25.h = 11* 32;
			
			SDL_Rect wall26;
			wall26.x = 140* 32;
			wall26.y = 28* 32-64;
			wall26.w = 11* 32;
			wall26.h = 1* 32+64;
			
			SDL_Rect wall27;
			wall27.x = 151* 32;
			wall27.y = 44* 32-64;
			wall27.w = 7* 32;
			wall27.h = 1* 32+64;
			
			SDL_Rect wall28;
			wall28.x = 157* 32-64;
			wall28.y = 44* 32;
			wall28.w = 1* 32+64;
			wall28.h = 14* 32;
			
			SDL_Rect wall29;
			wall29.x = 173* 32-64;
			wall29.y = 58* 32;
			wall29.w = 1* 32+64;
			wall29.h = 5* 32;
			
			SDL_Rect wall30;
			wall30.x = 211* 32-64;
			wall30.y = 58* 32;
			wall30.w = 1* 32+64;
			wall30.h = 5* 32;
			
			SDL_Rect wall31;
			wall31.x = 173* 32;
			wall31.y = 62* 32-64;
			wall31.w = 39* 32;
			wall31.h = 1* 32+64;
			
			SDL_Rect wall32;
			wall32.x = 190* 32-64;
			wall32.y = 62* 32;
			wall32.w = 1* 32+64;
			wall32.h = 16* 32;
			
			SDL_Rect wall33;
			wall33.x = 194* 32-64;
			wall33.y = 62* 32;
			wall33.w = 1* 32+64;
			wall33.h = 16* 32;
			
			SDL_Rect wall34;
			wall34.x = 194* 32;
			wall34.y = 74* 32-64;
			wall34.w = 23* 32;
			wall34.h = 1* 32+64;
			
			SDL_Rect wall35;
			wall35.x = 216* 32-64;
			wall35.y = 74* 32;
			wall35.w = 1* 32+64;
			wall35.h = 26* 32;
			
			SDL_Rect wall36;
			wall36.x = 193* 32-64;
			wall36.y = 99* 32;
			wall36.w = 24* 32;
			wall36.h = 1* 32+64;
			
			SDL_Rect wall37;
			wall37.x = 193* 32-64;
			wall37.y = 98* 32;
			wall37.w = 1* 32+64;
			wall37.h = 2* 32;
			
			SDL_Rect wall38;
			wall38.x = 198* 32-64;
			wall38.y = 99* 32;
			wall38.w = 1* 32+64;
			wall38.h = 12* 32;
			
			SDL_Rect wall39;
			wall39.x = 277* 32-64;
			wall39.y = 35* 32;
			wall39.w = 1* 32+64;
			wall39.h = 23* 32;
			
			SDL_Rect wall40;
			wall40.x = 283* 32-32;
			wall40.y = 111* 32;
			wall40.w = 2* 32+32;
			wall40.h = 12* 32;
			
			SDL_Rect wall41;
			wall41.x = 256* 32;
			wall41.y = 67* 32-64;
			wall41.w = 30* 32;
			wall41.h = 1* 32+64;

			SDL_Rect wall42;
			wall42.x = 256* 32;
			wall42.y = 86* 32-64;
			wall42.w = 47* 32;
			wall42.h = 1* 32+64;

			SDL_Rect wall43;
			wall43.x = 256* 32;
			wall43.y = 89* 32-64;
			wall43.w = 47* 32;
			wall43.h = 1* 32+64;

			SDL_Rect wall44;
			wall44.x = 256* 32;
			wall44.y = 95* 32-64;
			wall44.w = 47* 32;
			wall44.h = 1* 32+64;

			SDL_Rect wall45;
			wall45.x = 256* 32;
			wall45.y = 103* 32-64;
			wall45.w = 47* 32;
			wall45.h = 1* 32+64;
			
			SDL_Rect wall46;
			wall46.x = 286* 32-64;
			wall46.y = 58* 32;
			wall46.w = 1* 32+64;
			wall46.h = 10* 32;

			SDL_Rect wall47;
			wall47.x = 302* 32-64;
			wall47.y = 86* 32;
			wall47.w = 1* 32+64;
			wall47.h = 4* 32;

			SDL_Rect wall48;
			wall48.x = 302* 32-64;
			wall48.y = 95* 32;
			wall48.w = 1* 32+64;
			wall48.h = 17* 32;
			
			SDL_Rect wall49;//lhc
			wall49.x = 190* 32;
			wall49.y = 78*32 -32;
			wall49.w = 5*32;
			wall49.h = 2*32 +32;
			
			SDL_Rect wall50;//shivalik
			wall50.x = 52* 32 -32;
			wall50.y = 24*32 -32;
			wall50.w = 2*32 +32;
			wall50.h = 2*32 +32;
			
			SDL_Rect wall51;//nalanda
			wall51.x = 20* 32 -32;
			wall51.y = 84*32 -32;
			wall51.w = 2*32 +32;
			wall51.h = 2*32 +32;
			
			SDL_Rect wall52;//lib
			wall52.x = 164* 32 -32;
			wall52.y = 48*32 -32;
			wall52.w = 2*32 +32;
			wall52.h = 2*32 +32;
			
			SDL_Rect wall53;//sac
			wall53.x = 46* 32 -32;
			wall53.y = 67*32 -32;
			wall53.w = 2*32 +32;
			wall53.h = 2*32 +32;
			
			SDL_Rect wall54;//csc
			wall54.x = 215* 32 -32;
			wall54.y = 36*32 -32;
			wall54.w = 2*32 +32;
			wall54.h = 2*32 +32;
			
			SDL_Rect wall55;//iit market
			wall55.x = 244* 32 -32;
			wall55.y = 108*32 -32;
			wall55.w = 2*32 +32;
			wall55.h = 2*32 +32;
			
			SDL_Rect wall75;//bharti building
			wall75.x = 140* 32 -32;
			wall75.y = 51*32 -32;
			wall75.w = 2*32 +32;
			wall75.h = 2*32 +32;

			SDL_Rect wall56;//amul
			wall56.x = 163* 32 -32;
			wall56.y = 29*32 -32;
			wall56.w = 2*32 +32;
			wall56.h = 2*32 +32;
			
			SDL_Rect wall57;//ccd
			wall57.x = 191* 32 -32;
			wall57.y = 25*32 -32;
			wall57.w = 2*32 +32;
			wall57.h = 2*32 +32;
			
			SDL_Rect wall58;//delhi 16
			wall58.x = 23* 32 -32;
			wall58.y = 38*32 -32;
			wall58.w = 2*32 +32;
			wall58.h = 2*32 +32;
			
			SDL_Rect wall59;//masala mix
			wall59.x = 68* 32 -32;
			wall59.y = 50*32 -32;
			wall59.w = 2*32 +32;
			wall59.h = 2*32 +32;
			
			SDL_Rect wall60;//tennis
			wall60.x = 34* 32 -32;
			wall60.y = 52*32 -32;
			wall60.w = 2*32 +32;
			wall60.h = 2*32 +32;
			
			SDL_Rect wall62;//hockey
			wall62.x = 136* 32 -32;
			wall62.y = 67*32 -32;
			wall62.w = 2*32 +32;
			wall62.h = 2*32 +32;
			
			SDL_Rect wall61;//cricket
			wall61.x = 120* 32;
			wall61.y = 67*32 -32;
			wall61.w = 10*32 ;
			wall61.h = 2*32 +32;
			
			SDL_Rect wall63;//oops
			wall63.x = 303* 32;
			wall63.y = 57* 32 -32 ;
			wall63.w = 4* 32;
			wall63.h = 2* 32 + 32;
			
			//yulu1
			SDL_Rect wall64;
			wall64.x = 294* 32;
			wall64.y = 53* 32 ;
			wall64.w = 4* 32;
			wall64.h = 3* 32 ;
			//yulu2
			SDL_Rect wall65;
			wall65.x = 184* 32;
			wall65.y = 62* 32 ;
			wall65.w = 4* 32;
			wall65.h = 3* 32 ;
			//yulu3
			SDL_Rect wall66;
			wall66.x = 142* 32;
			wall66.y = 28* 32 ;
			wall66.w = 4* 32;
			wall66.h = 3* 32 ;
			//yulu4
			SDL_Rect wall67;
			wall67.x = 90* 32;
			wall67.y = 57* 32 ;
			wall67.w = 4* 32;
			wall67.h = 3* 32 ;
			//yulu5
			SDL_Rect wall68;
			wall68.x = 72* 32;
			wall68.y = 57* 32 ;
			wall68.w = 4* 32;
			wall68.h = 3* 32 ;
			//yulu10
			SDL_Rect wall69;
			wall69.x = 20* 32;
			wall69.y = 95* 32 ;
			wall69.w = 3* 32;
			wall69.h = 4* 32 ;
			//yulu6
			SDL_Rect wall70;
			wall70.x = 40* 32;
			wall70.y = 62* 32 ;
			wall70.w = 4* 32;
			wall70.h = 3* 32 ;
			//yulu7
			SDL_Rect wall71;
			wall71.x = 82* 32;
			wall71.y = 30* 32 ;
			wall71.w = 4* 32;
			wall71.h = 3* 32 ;
			//yulu8
			SDL_Rect wall72;
			wall72.x = 277* 32;
			wall72.y = 84* 32 ;
			wall72.w = 4* 32;
			wall72.h = 3* 32 ;
			//yulu9
			SDL_Rect wall73;
			wall73.x = 20* 32;
			wall73.y = 10* 32 ;
			wall73.w = 3* 32;
			wall73.h = 4* 32 ;

			
			
			
			//The camera area
			SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
			int flag=0;
			int flag1=0;
			int m2times =0;
			int alpha=0;
			int song_only1=0;
			int y2times =0;
			int end_screen1=0;
			int yulu_in_use=0;
			int yulu_counter=0; 
			// int begin =rand();
			//Set default current surface
			gDotTexture = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];

			//While application is running
			int ind1,ind2;
			while( !quit )
			{
				
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT || e.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
					{
						quit = true;
					}
					//User presses a key
					else if( e.type == SDL_KEYDOWN )
					{
					
						if(e.key.keysym.sym == SDLK_RETURN and flag1==1)
						{
						d= 0;
						flag=1;
						startTime = SDL_GetTicks();
						flag1=0;
						}
						else if( e.key.keysym.sym == SDLK_RETURN )
							{
							b=0;
							flag1 =1;
							d=255;
							}
						//Increase alpha on w
						else if( e.key.keysym.sym == SDLK_w and flag == 1)
						{
							Mix_PlayChannel( -1, gPause, 0 );
							a= 255;
							dot.stopVel();
							doctor.stopVel();
						}
						//Decrease alpha on s
						else if( e.key.keysym.sym == SDLK_s and flag == 1)
						{
							a=0;
							dot.startVel();
							doctor.startVel();
						}
						else if( e.key.keysym.sym == SDLK_m and flag == 1
						and m2times==1)
						{
							c=0;
							dot.startVel();
							ghost1.startVel();
							m2times =0;
						}
						//Increase alpha on w
						else if( e.key.keysym.sym == SDLK_m and flag == 1)
						{
							c= 255;
							dot.stopVel();
							ghost1.stopVel();
							m2times=1;
						}
						
						else if( e.key.keysym.sym == SDLK_y and flag == 1 and yulu_in_use==0 and yulu_counter <4)
						{
							if(!checkCollisionAC(dot.mCollider, wall64) or !checkCollisionAC(dot.mCollider, wall65) or !checkCollisionAC(dot.mCollider, wall66) or !checkCollisionAC(dot.mCollider, wall67) or !checkCollisionAC(dot.mCollider, wall68) or !checkCollisionAC(dot.mCollider, wall69) or !checkCollisionAC(dot.mCollider, wall70) or !checkCollisionAC(dot.mCollider, wall71) or !checkCollisionAC(dot.mCollider, wall72) or !checkCollisionAC(dot.mCollider, wall73) ){
							Mix_PlayChannel( -1, gYulu, 0 );
							yuluStart = SDL_GetTicks();
							y2times =1;
							yulu_in_use =1;
							end_screen1=0;
							yulu_counter++;
							}
						}
						
						else if( e.key.keysym.sym == SDLK_UP and flag == 1)
						{
							gDotTexture = gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ];
						}
						else if( e.key.keysym.sym == SDLK_DOWN and flag == 1)
						{
							gDotTexture = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ];
						}
						else if( e.key.keysym.sym == SDLK_LEFT and flag == 1)
						{
							gDotTexture = gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ];
						}
						else if( e.key.keysym.sym == SDLK_RIGHT and flag == 1)
						{
							gDotTexture = gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ];
						}
						else if( e.key.keysym.sym == SDLK_1 and flag == 1)
						{
						//task1
							if(!checkCollisionAC(dot.mCollider, wall49)){
								// if(!task1.taskdoneonce){
								if(!taskarray[0].taskdoneonce){

								dot.points+=1;
								taskarray[0].taskdoneonce=true;
								}
							Mix_PlayChannel( -1, gHigh, 0 );
							dot.pointsupdated=true;
							taskarray[0].isdone=true;
							//TASK COMPLETED
							}
						}
						else if( e.key.keysym.sym == SDLK_2 and flag == 1)
						{
						//task2
							if(!checkCollisionAC(dot.mCollider, wall50)){
								// if(!task2.taskdoneonce){
									if(!taskarray[1].taskdoneonce){
								dot.points+=1;
								taskarray[1].taskdoneonce=true;
								}
							Mix_PlayChannel( -1, gHigh, 0 );
							dot.pointsupdated=true;
							taskarray[1].isdone=true;
							//TASK COMPLETED
							}
							else if(!checkCollisionAC(dot.mCollider, wall51)){
								// if(!task22.taskdoneonce){
								if(!taskarray[2].taskdoneonce){
								dot.points+=1;
								taskarray[2].taskdoneonce=true;
								}
							Mix_PlayChannel( -1, gHigh, 0 );
							dot.pointsupdated=true;
							taskarray[2].isdone=true;
							//TASK COMPLETED
							}
						}	
						else if( e.key.keysym.sym == SDLK_3 and flag == 1)
						{
						//task3
							if(!checkCollisionAC(dot.mCollider, wall52)){
								// if(!task3.taskdoneonce){
									if(!taskarray[3].taskdoneonce){
								dot.points+=1;
								taskarray[3].taskdoneonce=true;
								}
							Mix_PlayChannel( -1, gHigh, 0 );
							dot.pointsupdated=true;
							taskarray[3].isdone=true;
							//TASK COMPLETED
							}
						}
						else if( e.key.keysym.sym == SDLK_4 and flag == 1)
						{
						//task4
							if(!checkCollisionAC(dot.mCollider, wall56)){
							
							if(  dot.points!=0)
							{dot.life=5;
							Mix_PlayChannel( -1, gEnergy, 0 );
							task41.isdone=true;}
							dot.points-=1;
							
							dot.pointsupdated=true;
							//TASK COMPLETED
							}
							else if(!checkCollisionAC(dot.mCollider, wall57)){
							
							if(  dot.points!=0)
							{dot.life=5;
							Mix_PlayChannel( -1, gEnergy, 0 );
							task43.isdone=true;}
							dot.points-=1;
							
							dot.pointsupdated=true;
							//TASK COMPLETED
							}
							else if(!checkCollisionAC(dot.mCollider, wall58)){
							
							if(  dot.points!=0)
							{dot.life=5;
							Mix_PlayChannel( -1, gEnergy, 0 );
							task44.isdone=true;}

							dot.points-=1;
							
							dot.pointsupdated=true;
							//TASK COMPLETED
							}
							else if(!checkCollisionAC(dot.mCollider, wall59)){
				
							if(  dot.points!=0)
							{dot.life=5;
							Mix_PlayChannel( -1, gEnergy, 0 );
							task42.isdone=true;}
							dot.points-=1;
							
							dot.pointsupdated=true;
							//TASK COMPLETED
							}
						}
						else if( e.key.keysym.sym == SDLK_5 and flag == 1)
						{
						//task5
							if(!checkCollisionAC(dot.mCollider, wall53)){
								// if(!task5.taskdoneonce){
									if(!taskarray[4].taskdoneonce){
								dot.points+=1;
								taskarray[4].taskdoneonce=true;
								}
							Mix_PlayChannel( -1, gHigh, 0 );
							dot.pointsupdated=true;
							taskarray[4].isdone=true;
							//TASK COMPLETED
							}
						}
						else if( e.key.keysym.sym == SDLK_6 and flag == 1)
						{
						//task6
							if(!checkCollisionAC(dot.mCollider, wall61)){
								// if(!task61.taskdoneonce){
									if(!taskarray[5].taskdoneonce){
								dot.points+=1;
								taskarray[5].taskdoneonce=true;
								}
							Mix_PlayChannel( -1, gHigh, 0 );
							dot.pointsupdated=true;
							taskarray[5].isdone=true;
							//TASK COMPLETED
							}
							else if(!checkCollisionAC(dot.mCollider, wall62)){
								// if(!task62.taskdoneonce){
									if(!taskarray[6].taskdoneonce){
								dot.points+=1;
								taskarray[6].taskdoneonce=true;
								}
							Mix_PlayChannel( -1, gHigh, 0 );
							dot.pointsupdated=true;
							taskarray[6].isdone=true;
							//TASK COMPLETED
							}
							else if(!checkCollisionAC(dot.mCollider, wall60)){
								// if(!task63.taskdoneonce){
									if(!taskarray[7].taskdoneonce){
								dot.points+=1;
								taskarray[7].taskdoneonce=true;
								}
							Mix_PlayChannel( -1, gHigh, 0 );
							dot.pointsupdated=true;
							taskarray[7].isdone=true;
							//TASK COMPLETED
							}
						}
						else if( e.key.keysym.sym == SDLK_7 and flag == 1)
						{
						//task7
							if(!checkCollisionAC(dot.mCollider, wall54)){
								// if(!task7.taskdoneonce){
									if(!taskarray[8].taskdoneonce){
								dot.points+=1;
								taskarray[8].taskdoneonce=true;
								}
							Mix_PlayChannel( -1, gHigh, 0 );
							dot.pointsupdated=true;
							taskarray[8].isdone=true;
							//TASK COMPLETED
							}
						}
						
						else if( e.key.keysym.sym == SDLK_8 and flag == 1)
						{
						//task8
							if(!checkCollisionAC(dot.mCollider, wall55)){
								// if(!task8.taskdoneonce){
									if(!taskarray[9].taskdoneonce){
								dot.points+=1;
								taskarray[9].taskdoneonce=true;
								}
							Mix_PlayChannel( -1, gHigh, 0 );
							dot.pointsupdated=true;
							taskarray[9].isdone=true;
							//TASK COMPLETED
							}
						}
						else if( e.key.keysym.sym == SDLK_9 and flag == 1)
						{
						//task9
							if(!checkCollisionAC(dot.mCollider, wall75)){
								// if(!task9.taskdoneonce){
									if(!taskarray[10].taskdoneonce){
								dot.points+=1;
								taskarray[10].taskdoneonce=true;
								}
							Mix_PlayChannel( -1, gHigh, 0 );
							dot.pointsupdated=true;
							taskarray[10].isdone=true;
							//TASK COMPLETED
							}
						}
						
						
						
						else if(flag == 1)
						{
							gDotTexture = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];
						}
						
					}
				if(flag==1){
					//Handle input for the dot
					dot.handleEvent( e, m2times );
					doctor.handleEvent( e);
					ghost2.handleEvent( e);
					ghost3.handleEvent( e);
					perry.handleEvent( e);
					candace.handleEvent( e);
					isabella.handleEvent( e);
					vanessa.handleEvent( e);
					ghost1.handleEvent( e);
					ghost4.handleEvent( e);
					ghost5.handleEvent( e);
					ghost6.handleEvent( e);
					}
				}
				
				//Set text to be rendered
				timeText.str( "" );
				timeText << "Time left: " << (120000-(SDL_GetTicks() - startTime))/60000<<"min "<<(((60000-(SDL_GetTicks() - startTime))%60000)/1000) <<"sec"; 
				
				//Render text
				if( !gTimeTextTexture.loadFromRenderedText( timeText.str().c_str(), textColor ) )
				{
					printf( "Unable to render time texture!\n" );
				}
				
				//Move the dot and check collision
				SDL_Rect wallarray[73]={wall1, wall2, wall3, wall4, wall5, wall6, wall7, wall8, wall9, wall10, wall11, wall12, wall13, wall14, wall15, wall16, wall17, wall18, wall19, wall20, wall21, wall22, wall23, wall24, wall25, wall26, wall27, wall28, wall29, wall30, wall31, wall32, wall33, wall34, wall35, wall36, wall37, wall38, wall39, wall40, wall41, wall42, wall43, wall44, wall45, wall46, wall47, wall48,wall49,wall50,wall51,wall52,wall53, wall54,wall55 ,wall56, wall57, wall58, wall59, wall60, wall61, wall62, wall63,wall64, wall65, wall66, wall67, wall68, wall69, wall70, wall71, wall72, wall73};
				// int walltaskdone[74]={0};
				SDL_Rect candacearray[1]={wall16};
				SDL_Rect isabellaarray[1]={wall50};
				SDL_Rect vanessaarray[1]={wall51};

				/*for(int i=0;i<2;i++){
				dot.move( wallarray ,48,ghostarray[i].ghostCollider );}*/
				
				dot.move( wallarray ,73,doctor.ghostCollider );
				
				doctor.move(wallarray, 63);
				ghost2.move(wallarray, 63);
				ghost3.move(wallarray, 63);

				perry.move(wallarray, 63);
				candace.move(candacearray,1);//nalanda
				isabella.move(isabellaarray,1);//kailash
				vanessa.move(vanessaarray,1);//himadri

				ghost1.move(wallarray, 63);
				ghost4.move(wallarray, 63);
				ghost5.move(wallarray, 63);
				ghost6.move(wallarray, 63);

				//Center the camera over the dot
				camera.x = ( dot.getPosX() + Dot::DOT_WIDTH / 2 ) - SCREEN_WIDTH / 2;
				camera.y = ( dot.getPosY() + Dot::DOT_HEIGHT / 2 ) - SCREEN_HEIGHT / 2;

				//Keep the camera in bounds
				if( camera.x < 0 )
				{ 
					camera.x = 0;
				}
				if( camera.y < 0 )
				{
					camera.y = 0;
				}
				if( camera.x > LEVEL_WIDTH - camera.w )
				{
					camera.x = LEVEL_WIDTH - camera.w;
				}
				if( camera.y > LEVEL_HEIGHT - camera.h )
				{
					camera.y = LEVEL_HEIGHT - camera.h;
				}

			/*	//sockets start
				string ss = "POINTS="+to_string(dot.points)+"." +to_string(dot.getPosX())+","+ to_string(dot.getPosY());
				char* hello2 =  const_cast<char*>(ss.c_str());
				    
				send(sock, hello2, strlen(hello2), 0);
				printf("seding coordinates from client: \n");
                char buffer2[1024] = { 0 };
                valread = read(sock, buffer2, 1024);
                //printf("%s\n", buffer2);
                string strr=buffer2;
                int index1 = strr.find('=');
                int index2 = strr.find('.');
                int index = strr.find(',');
				//points=5. index1=6,index2=8
                string str3 =strr.substr(index1+1 ,index2-index1-1);
                string str1= strr.substr(index2+1, index-index2-1);
                string str2 =strr.substr(index+1 ,strr.length()-1-index);
				cout<<"points:"<<stoi(str3)<<" ";
                ind1 = stoi(str1);
                ind2 = stoi(str2);
                cout<< "ind1 :"<< ind1 << endl;
                cout<< "ind2 :"<< ind2<< endl; */ 
				//sockets end
				
				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render background
				gBGTexture.render( 0, 0, &camera );
				
				//initialize life of player
				//manshi = false means already colliding , manshi = true means no collision till now
				//checkcollisionlife = false means collision
				SDL_Rect ghostColliderarray[6]={ghost2.ghostCollider, ghost3.ghostCollider, ghost1.ghostCollider, ghost4.ghostCollider, ghost5.ghostCollider, ghost6.ghostCollider};
				
				
				if(!(checkCollisionlife(dot.mCollider,ghostColliderarray,6)) ){
				if(dot.manshi){
				//cout<<dot.life<<":life ";
				//dot.DOT_VEL-=3;
				dot.life-=1;
				// dot.mVelY-=3;
				Mix_PlayChannel( -1, gDog, 0 );
				cout<<dot.DOT_VEL<<" "<< dot.mVelX<<" " <<dot.mVelY<<" "<<endl;
				dot.manshi = false;
				}}
				else{
				dot.manshi = true;}

				//manshi 2 =1 means it has not collided with perry yet , manshi 2=2 means it has collided with perry alrady and got points
				if(!(checkCollisionAC(dot.mCollider,perry.ghostCollider)) ){
				if(dot.manshi2==1){
				// cout<<dot.life<<":life ";
				
				dot.points+=2;
				Mix_PlayChannel( -1, gPlatypus, 0 );
				dot.manshi2 = 2;
				}}
				if(!(checkCollisionAC(dot.mCollider,candace.ghostCollider)) ){
				if(dot.manshi3==1){
				// cout<<dot.life<<":life ";
				cout<<"candace:"<<dot.points;
				Mix_PlayChannel( -1, gCandace, 0 );
				dot.points+=1;//collision with candace increases 1 point
				cout<<dot.points;
				dot.manshi3 = 2;
				}}
				if(!(checkCollisionAC(dot.mCollider,isabella.ghostCollider)) ){
				if(dot.manshi4==1){
				// cout<<dot.life<<":life ";
				cout<<"isabella:"<<dot.points;
				Mix_PlayChannel( -1, gIsabella, 0 );
				// dot.points+=1;//collision with candace increases 1 point
				cout<<dot.points;
				dot.manshi4 = 2;
				}}
				if(!(checkCollisionAC(dot.mCollider,vanessa.ghostCollider)) ){
				if(dot.manshi5==1){
				// cout<<dot.life<<":life ";
				cout<<"vanessa:"<<dot.points;
				Mix_PlayChannel( -1, gVanessa, 0 );
				dot.points+=1;//collision with vanessa increases 1 point
				cout<<dot.points;
				dot.manshi5 = 2;
				}}
				if(dot.life==5){
				lifeT5.render( 0,0+48);}
				else if(dot.life==2){
				lifeT2.render( 0,0+48);}
				else if(dot.life==3){
				lifeT3.render( 0,0+48);}
				else if(dot.life==4){
				lifeT4.render( 0,0+48);}
				else {
				lifeT1.render( 0,0+48);
				if(alpha==0) {
				Mix_PlayChannel( -1, gMedium, 0 );
				ee= 255;
				alpha++;
				}
				}
				
				gPauseTexture.setAlpha( a );
				gStartTexture.setAlpha( b);
				gMapWindowTexture.setAlpha(c);
				gInstrWindowTexture.setAlpha(d);
				gStopTexture.setAlpha(ee);
				

				//Render objects
				//gDotTexture.render(camera.x, camera.y );
				dot.render( camera.x, camera.y );
				doctor.render( camera.x, camera.y,1 );
				perry.render( camera.x, camera.y,2);
				candace.render( camera.x, camera.y,3);
				isabella.render( camera.x, camera.y,4);
				vanessa.render( camera.x, camera.y , 5);
			
				ghost2.render( camera.x, camera.y,0 );
				ghost3.render( camera.x, camera.y,0 );
				ghost1.render( camera.x, camera.y,0 );
				ghost4.render( camera.x, camera.y,0 );
				ghost6.render( camera.x, camera.y,0 );
				ghost5.render( camera.x, camera.y,0 );
				player2.render(ind1-camera.x,ind2-camera.y);
				
				int counter=0;
				//TASKS
				if(!checkCollisionAC(dot.mCollider, wall49)&& ( taskarray[0].dotask)){
					//task1
				taskarray[0].render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if((dot.pointsupdated || taskarray[0].isdone ) ){
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					// if(! taskarray[0].dotask){
					// 	notask.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					// }
					}
				}
				
				else if(!checkCollisionAC(dot.mCollider, wall50)&& ( taskarray[1].dotask)){
					//task2
				taskarray[1].render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if(dot.pointsupdated  or taskarray[1].isdone ){
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					// if(! taskarray[1].dotask){
					// 	notask.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					// }
					}
				}
				else if(!checkCollisionAC(dot.mCollider, wall51) && ( taskarray[2].dotask)){
				// task22
				taskarray[2].render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if(dot.pointsupdated or taskarray[2].isdone ){
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					// if(! taskarray[2].dotask){
					// 	notask.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					// }
					}
				}
				
				else if(!checkCollisionAC(dot.mCollider, wall52)&& ( taskarray[3].dotask)){
				// task3
				taskarray[3].render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if(dot.pointsupdated or taskarray[3].isdone ){
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					// if( !taskarray[2].dotask){
					// 	notask.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					// }
					}
				}
				
				else if(!checkCollisionAC(dot.mCollider, wall56)){//amul
				task41.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if(dot.pointsupdated or task41.isdone ){
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					if(dot.points<0){
					nocoins.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					dot.points+=1;
					dot.pointsupdated=true;}
					}
				}
				else if(!checkCollisionAC(dot.mCollider, wall57)){
				task43.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if(dot.pointsupdated or task43.isdone ){
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					if(dot.points<0){
					nocoins.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					dot.points+=1;
					dot.pointsupdated=true;}
					}
				}
				else if(!checkCollisionAC(dot.mCollider, wall58)){//d16
				task44.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if(dot.pointsupdated or task44.isdone ){
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					if(dot.points<0){
					nocoins.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					dot.points+=1;
					dot.pointsupdated=true;}
					}
				}
				else if(!checkCollisionAC(dot.mCollider, wall59)){//masala mix
				task42.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if(dot.pointsupdated or task42.isdone ){
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					if(dot.points<0){
					nocoins.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					dot.points+=1;
					dot.pointsupdated=true;
					}
					}
				}
				
				else if(!checkCollisionAC(dot.mCollider, wall53)&& ( taskarray[4].dotask)){
				// task5
				taskarray[4].render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if(dot.pointsupdated or taskarray[4].isdone ){
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					// if( !taskarray[4].dotask){
					// 	notask.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					// }
					}
				}
				else if(!checkCollisionAC(dot.mCollider, wall61)&& ( taskarray[5].dotask)){
				// task61
				taskarray[5].render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if(dot.pointsupdated or taskarray[5].isdone ){
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					// if( !taskarray[5].dotask){
					// 	notask.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					// }
					}
				}
				else if(!checkCollisionAC(dot.mCollider, wall62)&& ( taskarray[6].dotask)){
				// task62
				taskarray[6].render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if(dot.pointsupdated or taskarray[6].isdone ){
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					// if( !taskarray[6].dotask){
					// 	notask.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					// }
					}
				
				}
				else if(!checkCollisionAC(dot.mCollider, wall60)&& ( taskarray[7].dotask)){
				// task63
				taskarray[7].render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if(dot.pointsupdated or taskarray[7].isdone ){
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					// if(! taskarray[7].dotask){
					// 	notask.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					// }
					}
				}
				
				else if(!checkCollisionAC(dot.mCollider, wall54)&& ( taskarray[8].dotask)){
				// task7
				taskarray[8].render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if(dot.pointsupdated or taskarray[8].isdone ){
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					// if( !taskarray[8].dotask){
					// 	notask.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					// }
					}
				}
				
				else if(!checkCollisionAC(dot.mCollider, wall55)&& ( taskarray[9].dotask)){
				// task8
				taskarray[9].render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if(dot.pointsupdated or taskarray[9].isdone ){
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					// if(! taskarray[9].dotask){
					// 	notask.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					// }
					}
				}
				else if(!checkCollisionAC(dot.mCollider, wall75)&& ( taskarray[10].dotask)){
				// task9
				taskarray[10].render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if(dot.pointsupdated or taskarray[10].isdone ){
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					// if(! taskarray[10].dotask){
					// 	notask.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					// }
					}
				}
				else if(!checkCollisionAC(dot.mCollider, wall63)){
				oops.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				dot.points=0;
				if(dot.pointsupdated){
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					
					}
				}
				else if(!checkCollisionAC(dot.mCollider, wall64) and yulu_in_use==0 and yulu_counter<4 ){
				yulu.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if(dot.pointsupdated){
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					
					}
				}
				else if(!checkCollisionAC(dot.mCollider, wall65) and yulu_in_use==0 and yulu_counter<4){
				yulu.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if(dot.pointsupdated){
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					
					}
				}
				else if(!checkCollisionAC(dot.mCollider, wall66) and yulu_in_use==0 and yulu_counter<4){
				yulu.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if(dot.pointsupdated){
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					
					}
				}
				else if(!checkCollisionAC(dot.mCollider, wall67) and yulu_in_use==0 and yulu_counter<4){
				yulu.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if(dot.pointsupdated){
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					
					}
				}
				else if(!checkCollisionAC(dot.mCollider, wall68) and yulu_in_use==0 and yulu_counter<4){
				yulu.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if(dot.pointsupdated){
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					
					}
				}
				else if(!checkCollisionAC(dot.mCollider, wall69) and yulu_in_use==0 and yulu_counter<4){
				yulu.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if(dot.pointsupdated){
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					
					}
				}
				else if(!checkCollisionAC(dot.mCollider, wall70) and yulu_in_use==0 and yulu_counter<4){
				yulu.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if(dot.pointsupdated){
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					
					}
				}
				else if(!checkCollisionAC(dot.mCollider, wall71) and yulu_in_use==0 and yulu_counter<4){
				yulu.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if(dot.pointsupdated){
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					
					}
				}
				else if(!checkCollisionAC(dot.mCollider, wall72) and yulu_in_use==0 and yulu_counter<4){
				yulu.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if(dot.pointsupdated){
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					
					}
				}
				else if(!checkCollisionAC(dot.mCollider, wall73) and yulu_in_use==0 and yulu_counter<4){
				yulu.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if(dot.pointsupdated){
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					
					}
				}
				

				else {dot.pointsupdated = false;}
				//POINTS
				if(dot.points>=1){
				point1.render(0,4+64+48);}
				if(dot.points>=2){
				point1.render(48,4+64+48);}
				if(dot.points>=3){
				point1.render(48*2,4+64+48);}
				if(dot.points>=4){
				point1.render(48*3,4+64+48);}
				if(dot.points>=5){
				point1.render(48*4,4+64+48);}
				if(dot.points>=6){
				point1.render(0,48+64+48);}
				if(dot.points>=7){
				point1.render(48,48+64+48);}
				if(dot.points>=8){
				point1.render(48*2,48+64+48);}
				if(dot.points>=9){
				point1.render(48*3,48+64+48);}
				if(dot.points>=10){
				point1.render(48*4,48+64+48);
				win.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);//player wins
				}

			// LTexture taskarray[11]={task1,task2,task22,task3,task5,task61,task62,task63,task7,task8,task9};

			for(int i=0;i<5;i++){
				if(todotasks[i]==0){t1.render(0,64*2+64*i+48);}
				if(todotasks[i]==1){t2.render(0,64*2+64*i+48);}
				if(todotasks[i]==2){t2.render(0,64*2+64*i+48);}
				if(todotasks[i]==3){t3.render(0,64*2+64*i+48);}
				if(todotasks[i]==4){t5.render(0,64*2+64*i+48);}
				if(todotasks[i]==5){t61.render(0,64*2+64*i+48);}
				if(todotasks[i]==6){t62.render(0,64*2+64*i+48);}
				if(todotasks[i]==7){t63.render(0,64*2+64*i+48);}
				if(todotasks[i]==8){t7.render(0,64*2+64*i+48);}
				if(todotasks[i]==9){t8.render(0,64*2+64*i+48);}
				if(todotasks[i]==10){t9.render(0,64*2+64*i+48);}
	}
				
				if(SDL_GetTicks()-startTime > 120000)
				{
				ee=255;
				if(song_only1==0){
				Mix_PlayChannel( -1, gMedium, 0 );
				
				song_only1 =1;
				
				}
				}
				
				if(yuluStart + 10000 < SDL_GetTicks() )
				{
				//ye=255;
				dot.DOT_VEL=20;
				end_screen1=1;
				yulu_in_use =0;
				y2times=0;
				}
				else if(yulu_in_use==1)
				{
				yululogo.render(64,48);
				
				}
				//gYuluEndTexture.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				gPromptTextTexture.render( ( SCREEN_WIDTH - gPromptTextTexture.getWidth() ) / 2, 0 );
				gMapWindowTexture.render( 0, 0);
				//Render textures
				
				gTimeTextTexture.render( 4,20);
				
				gMapWindowTexture.render( 0, 0);
				
				gPauseTexture.render( 0, 0 );
				gStartTexture.render(0, 0);
				gInstrWindowTexture.render(0,0);
				gStopTexture.render(0,0);
				//Update screen
				SDL_RenderPresent( gRenderer );
				//if(dot.pointsupdated){cout<<dot.points<<" ";}
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
