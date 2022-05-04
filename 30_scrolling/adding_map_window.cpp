/*This source code copyrighted by Lazy Foo' Productions (2004-2022)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, vectors, and strings
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080
using namespace std;

//The dimensions of the level
const int LEVEL_WIDTH = 9952;
const int LEVEL_HEIGHT = 3936;

//Screen dimension constants
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 960;

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
		int DOT_VEL = 25;

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
		//false means collision
		
		void startVel();
		void stopVel();

    private:
		//The X and Y offsets of the dot
		int mPosX, mPosY;

		//The velocity of the dot
		int mVelX, mVelY;
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
		static const int GHOST_VEL = -5;

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

//Scene textures
LTexture gDotTexture;
LTexture ghostTexture1;
LTexture ghostTexture2;
LTexture gBGTexture;
LTexture gPauseTexture;
LTexture gStartTexture;
LTexture gMapWindowTexture;
LTexture oops;
LTexture lifeT1;
LTexture lifeT2;
LTexture lifeT3;
LTexture lifeT4;
LTexture lifeT5;
LTexture task1;
LTexture task2;

LTexture task3;
LTexture task41;

LTexture task42;
LTexture task43;
LTexture task44;
LTexture task5;
LTexture task61;
LTexture task62;
LTexture task63;

LTexture task7;
LTexture task8;

LTexture taskdone;
LTexture point1;

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
    mPosX = 300*32;
    mPosY = 57*32;
    
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
    ghostVelX = 1;
    ghostVelY = 1;
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
            		DOT_VEL= 25;
            		break;
            		}
            case SDLK_w: DOT_VEL= 0;break;
            case SDLK_s: DOT_VEL= 25;break;
            case SDLK_UP: mVelY -= DOT_VEL; break;
            case SDLK_DOWN: mVelY += DOT_VEL; break;
            case SDLK_LEFT: mVelX -= DOT_VEL; break;
            case SDLK_RIGHT: mVelX += DOT_VEL; break;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_w: DOT_VEL= 0;break;
            case SDLK_s: DOT_VEL= 25;break;
            case SDLK_UP: mVelY += DOT_VEL; break;
            case SDLK_DOWN: mVelY -= DOT_VEL; break;
            case SDLK_LEFT: mVelX += DOT_VEL; break;
            case SDLK_RIGHT: mVelX -= DOT_VEL; break;
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
            case SDLK_UP: ghostVelY -= rand()%10 ; break;
            
            case SDLK_DOWN: ghostVelY += rand()%10 ; break;
            case SDLK_LEFT: ghostVelX -= rand()%10 ; break;
            
            case SDLK_RIGHT: ghostVelX += rand()%10 ; break;

        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: ghostVelY +=rand()%10 ; break;
            case SDLK_DOWN: ghostVelY -= rand()%10 ; break;
            case SDLK_LEFT: ghostVelX += rand()%10 ; break;
            case SDLK_RIGHT: ghostVelX -= rand()%10 ; break;
           
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
    if(( mPosY < 0 ) || ( mPosY + DOT_HEIGHT > LEVEL_HEIGHT ) || !checkCollisionAC(mCollider, ghostCollider)){
    mPosX= 306	* 32;
			
    mPosY= 57* 32;}
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
    if(isDoc==1){
	ghostTexture1.render( ghostPosX - camX, ghostPosY - camY );}
	else{
	ghostTexture2.render( ghostPosX - camX, ghostPosY - camY );}
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
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
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
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;
	
	if( !gMapWindowTexture.loadFromFile( "map2.png" ) )
	{
		printf( "Failed to load front map texture!\n" );
		success = false;
	}
	else
	{
		//Set standard alpha blending
		gMapWindowTexture.setBlendMode( SDL_BLENDMODE_BLEND );
	}
	

	if( !gPauseTexture.loadFromFile( "Pause.png" ) )
	{
		printf( "Failed to load front pause texture!\n" );
		success = false;
	}
	else
	{
		//Set standard alpha blending
		gPauseTexture.setBlendMode( SDL_BLENDMODE_BLEND );
	}

	if( !gStartTexture.loadFromFile( "Start.png" ) )
	{
		printf( "Failed to load front start texture!\n" );
		success = false;
	}
	else
	{
		//Set standard alpha blending
		gStartTexture.setBlendMode( SDL_BLENDMODE_BLEND );
	}
	
	gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ].loadFromFile( "phinright.png" );
	gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ].loadFromFile( "phinleft.png");
	gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ].loadFromFile( "phinleft.png");
	gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ].loadFromFile( "phinleft.png");
	gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ].loadFromFile( "phinleft.png");
	
	//Load ghost texture1
	if( !ghostTexture1.loadFromFile( "doc.png" ) )
	{
		printf( "Failed to load doc texture!\n" );
		success = false;
	}
	
	//Load ghost texture2
	if( !ghostTexture2.loadFromFile( "dog(2).png" ) )
	{
		printf( "Failed to load dog texture!\n" );
		success = false;
	}

	//Load background texture
	if( !gBGTexture.loadFromFile( "map.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !lifeT1.loadFromFile( "1.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !lifeT2.loadFromFile( "2.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !lifeT3.loadFromFile( "3.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !lifeT4.loadFromFile( "4.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !lifeT5.loadFromFile( "5.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !task1.loadFromFile( "task1.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !task2.loadFromFile( "task2.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !task3.loadFromFile( "task3.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !task41.loadFromFile( "task4.1.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !task42.loadFromFile( "task4.2.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !task43.loadFromFile( "task4.3.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !task44.loadFromFile( "task4.4.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !task5.loadFromFile( "task5.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !task61.loadFromFile( "task6.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !task62.loadFromFile( "task6.2.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !task63.loadFromFile( "task6.3.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !task7.loadFromFile( "task7.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !task8.loadFromFile( "task8.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	
	if( !taskdone.loadFromFile( "taskdone.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !point1.loadFromFile( "coin(1).png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	
if( !oops.loadFromFile( "oops.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}

	return success;
}

void close()
{
	//Free loaded images
	gDotTexture.free();
	ghostTexture1.free();
	ghostTexture2.free();
	gBGTexture.free();
	gMapWindowTexture.free();
	gPauseTexture.free();
	gStartTexture.free();
	lifeT1.free();
	lifeT2.free();
	lifeT3.free();
	lifeT4.free();
	lifeT5.free();
	task1.free();
	task2.free();
	task3.free();
	task41.free();
	task42.free();
	task43.free();
	task44.free();
	task5.free();
	task61.free();
	task62.free();
	task63.free();
	task7.free();
	task8.free();
	taskdone.free();
	point1.free();
	oops.free();

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
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



//check collision between dogs and player
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
{
/*	int counter=0;
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = { 0 };
    char* hello = "Hello from server";
 
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0))
        == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
 
    // Forcefully attaching socket to the port 8080 
if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
 
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr*)&address,
             sizeof(address))
        < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket
         = accept(server_fd, (struct sockaddr*)&address,
                  (socklen_t*)&addrlen))
        < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    valread = read(new_socket, buffer, 1024);
    printf("%s\n", buffer);
    send(new_socket, hello, strlen(hello), 0);
    printf("Hello message sent\n");*/

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
			
			//Modulation component
			Uint8 a = 0;
			Uint8 b = 255;
			Uint8 c = 0;

			//The dot that will be moving around on the screen
			Dot dot;
			Ghost ghost1(131* 32-64,58* 32);
			//ghost1.Ghost(672,224);
			Ghost ghost2(22* 32,25* 32 -32);
			Ghost ghost3(131* 32-64,58* 32);
			//ghost2.Ghost(22* 32,25* 32 -32);
			
			//Ghost ghostarray[2]={ghost1,ghost2};
			
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
			wall16.x = 22* 32;
			wall16.y = 76* 32-64;
			wall16.w = 8* 32;
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
			wall41.w = 47* 32;
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
			
			SDL_Rect wall50;//kailash
			wall50.x = 266* 32 -32;
			wall50.y = 84*32 -32;
			wall50.w = 2*32 +32;
			wall50.h = 2*32 +32;
			
			SDL_Rect wall51;//himadri
			wall51.x = 297* 32 -32;
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
			
			SDL_Rect wall61;//hockey
			wall61.x = 136* 32 -32;
			wall61.y = 67*32 -32;
			wall61.w = 2*32 +32;
			wall61.h = 2*32 +32;
			
			SDL_Rect wall62;//cricket
			wall62.x = 120* 32;
			wall62.y = 67*32 -32;
			wall62.w = 10*32 ;
			wall62.h = 2*32 +32;
			//oops
			SDL_Rect wall63;
			wall63.x = 303* 32;
			wall63.y = 57* 32 -32 ;
			wall63.w = 4* 32;
			wall63.h = 2* 32 + 32;
			
			//The camera area
			SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
			int flag=0;
			int m2times =0;
			//Set default current surface
			gDotTexture = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];
			
			//While application is running
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
					
					if( e.key.keysym.sym == SDLK_RETURN )
						{
						b=0;
						flag =1;
						}
						//Increase alpha on w
						else if( e.key.keysym.sym == SDLK_w and flag == 1)
						{
							a= 255;
							dot.stopVel();
							ghost1.stopVel();
						}
						//Decrease alpha on s
						else if( e.key.keysym.sym == SDLK_s and flag == 1)
						{
							a=0;
							dot.startVel();
							ghost1.startVel();
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
						//Decrease alpha on s
						
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
							dot.points+=1;
							//cout<<dot.points;
							
							dot.pointsupdated=true;
							//TASK COMPLETED
							}
						}
						else if( e.key.keysym.sym == SDLK_2 and flag == 1)
						{
						//task2
							if(!checkCollisionAC(dot.mCollider, wall50)){
							dot.points+=1;
							//cout<<dot.points;
							dot.pointsupdated=true;
							//TASK COMPLETED
							}
							else if(!checkCollisionAC(dot.mCollider, wall51)){
							dot.points+=1;
							//cout<<dot.points;
							dot.pointsupdated=true;
							//TASK COMPLETED
							}
						}	
						else if( e.key.keysym.sym == SDLK_3 and flag == 1)
						{
						//task3
							if(!checkCollisionAC(dot.mCollider, wall52)){
							dot.points+=1;
							//cout<<dot.points;
							dot.pointsupdated=true;
							//TASK COMPLETED
							}
						}
						else if( e.key.keysym.sym == SDLK_4 and flag == 1)
						{
						//task4
							if(!checkCollisionAC(dot.mCollider, wall56)){
							dot.points-=1;
							dot.life=5;
							//cout<<dot.points;
							dot.pointsupdated=true;
							//TASK COMPLETED
							}
							else if(!checkCollisionAC(dot.mCollider, wall57)){
							dot.points-=1;
							dot.life=5;
							//cout<<dot.points;
							dot.pointsupdated=true;
							//TASK COMPLETED
							}
							else if(!checkCollisionAC(dot.mCollider, wall58)){
							dot.points-=1;
							dot.life=5;
							//cout<<dot.points;
							dot.pointsupdated=true;
							//TASK COMPLETED
							}
							else if(!checkCollisionAC(dot.mCollider, wall59)){
							dot.points-=1;
							dot.life=5;
							//cout<<dot.points;
							dot.pointsupdated=true;
							//TASK COMPLETED
							}
						}
						else if( e.key.keysym.sym == SDLK_5 and flag == 1)
						{
						//task5
							if(!checkCollisionAC(dot.mCollider, wall53)){
							dot.points+=1;
							//cout<<dot.points;
							dot.pointsupdated=true;
							//TASK COMPLETED
							}
						}
						else if( e.key.keysym.sym == SDLK_6 and flag == 1)
						{
						//task6
							if(!checkCollisionAC(dot.mCollider, wall61)){
							dot.points+=1;
							//cout<<dot.points;
							dot.pointsupdated=true;
							//TASK COMPLETED
							}
							else if(!checkCollisionAC(dot.mCollider, wall60)){
							dot.points+=1;
							//cout<<dot.points;
							dot.pointsupdated=true;
							//TASK COMPLETED
							}
							else if(!checkCollisionAC(dot.mCollider, wall62)){
							dot.points+=1;
							//cout<<dot.points;
							dot.pointsupdated=true;
							//TASK COMPLETED
							}
						}
						else if( e.key.keysym.sym == SDLK_7 and flag == 1)
						{
						//task7
							if(!checkCollisionAC(dot.mCollider, wall54)){
							dot.points+=1;
							//cout<<dot.points;
							dot.pointsupdated=true;
							//TASK COMPLETED
							}
						}
						
						else if( e.key.keysym.sym == SDLK_8 and flag == 1)
						{
						//task8
							if(!checkCollisionAC(dot.mCollider, wall55)){
							dot.points+=1;
							//cout<<dot.points;
							dot.pointsupdated=true;
							//TASK COMPLETED
							}
						}
						else if(flag == 1)
						{
							gDotTexture = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];
						}
						
					}


			if(flag==1){
				/*				
			char buffer3[1024] = { 0 };
			valread = read(new_socket, buffer3, 1024);
			printf("%s\n", buffer3);
			string s(buffer3);
			int intcounter=stoi(s);
			cout<<intcounter<<endl;
				if(intcounter%2==0){
				char buffer2[1024] = { 0 };
					valread = read(new_socket, buffer2, 1024);
					printf("%s\n", buffer2);
								}
				else{
				string ss = to_string(dot.getPosX())+","+ to_string(dot.getPosY())+"Hello2 from server=manshi";
					char* hello2 = const_cast<char*>(ss.c_str());

					send(new_socket, hello2, strlen(hello2), 0);
					printf("Hello2 message sent\n");}
*/
//Handle input for the dot
					dot.handleEvent( e, m2times );
					ghost1.handleEvent( e);
					ghost2.handleEvent( e);
					ghost3.handleEvent( e);
/*if(counter%2==1){
					char buffer2[1024] = { 0 };
					valread = read(new_socket, buffer2, 1024);
    				printf("%s\n", buffer2);
					cout<<endl;
}
else{
					string ss = to_string(dot.getPosX())+","+ to_string(dot.getPosY())+"Hello2 from server=manshi";
					char* hello2 = const_cast<char*>(ss.c_str());

					send(new_socket, hello2, strlen(hello2), 0);
					printf("sending coordinates from server: \n");
}
counter++;
cout<<counter<<" :counter"<<endl;
*/					

					}
				
			}
				//Move the dot and check collision
				SDL_Rect wallarray[63]={wall1, wall2, wall3, wall4, wall5, wall6, wall7, wall8, wall9, wall10, wall11, wall12, wall13, wall14, wall15, wall16, wall17, wall18, wall19, wall20, wall21, wall22, wall23, wall24, wall25, wall26, wall27, wall28, wall29, wall30, wall31, wall32, wall33, wall34, wall35, wall36, wall37, wall38, wall39, wall40, wall41, wall42, wall43, wall44, wall45, wall46, wall47, wall48,wall49,wall50,wall51,wall52,wall53, wall54,wall55 ,wall56, wall57, wall58, wall59, wall60, wall61, wall62, wall63};
				//dot.move( wall );
				
				/*for(int i=0;i<2;i++){
				dot.move( wallarray ,48,ghostarray[i].ghostCollider );}*/
				
				dot.move( wallarray ,63,ghost1.ghostCollider );
				
				ghost1.move(wallarray, 63);
				ghost2.move(wallarray, 63);
				ghost3.move(wallarray, 63);

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

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render background
				gBGTexture.render( 0, 0, &camera );
				
				//initialize life of player
				//manshi = false means already colliding , manshi = true means no collision till now
				//checkcollisionlife = false means collision
				SDL_Rect ghostColliderarray[2]={ghost2.ghostCollider,ghost3.ghostCollider};
				
				
				if(!(checkCollisionlife(dot.mCollider,ghostColliderarray,2)) ){
				if(dot.manshi){
				//cout<<dot.life<<" ";
				dot.life-=1;
				dot.manshi = false;
				}}
				else{
				dot.manshi = true;}
				
				
				if(dot.life==5){
				lifeT5.render( 0,0);}
				else if(dot.life==2){
				lifeT2.render( 0,0);}
				else if(dot.life==3){
				lifeT3.render( 0,0);}
				else if(dot.life==4){
				lifeT4.render( 0,0);}
				else {
				lifeT1.render( 0,0);}
				
				gPauseTexture.setAlpha( a );
				gStartTexture.setAlpha( b);
				gMapWindowTexture.setAlpha(c);

				//Render objects
				//gDotTexture.render(camera.x, camera.y );
				dot.render( camera.x, camera.y );
				ghost1.render( camera.x, camera.y,1 );
				ghost2.render( camera.x, camera.y,0 );
				ghost3.render( camera.x, camera.y,0 );
				
				//POINTS
				if(dot.points){
				point1.render(SCREEN_WIDTH - 48,4);}
				if(dot.points>=1){
				point1.render(SCREEN_WIDTH - 48,4);}
				if(dot.points>=2){
				point1.render(SCREEN_WIDTH - 48*2,4);}
				if(dot.points>=3){
				point1.render(SCREEN_WIDTH - 48*3,4);}
				if(dot.points>=4){
				point1.render(SCREEN_WIDTH - 48*4,4);}
				if(dot.points>=5){
				point1.render(SCREEN_WIDTH - 48*5,4);}
				if(dot.points>=6){
				point1.render(SCREEN_WIDTH - 48,48);}
				if(dot.points>=7){
				point1.render(SCREEN_WIDTH - 48*2,48);}
				if(dot.points>=8){
				point1.render(SCREEN_WIDTH - 48*3,48);}
				if(dot.points>=9){
				point1.render(SCREEN_WIDTH - 48*4,48);}
				if(dot.points>=10){
				point1.render(SCREEN_WIDTH - 48*5,48);
				lifeT5.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				}
				
				//TASKS
				if(!checkCollisionAC(dot.mCollider, wall49)){
				task1.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if(dot.pointsupdated){
				//cout<<"wall49 ";
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					////cout<<"wall50done ";
					}
				}
				
				else if(!checkCollisionAC(dot.mCollider, wall50)){
				task2.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if(dot.pointsupdated){
				//cout<<"wall50 ";
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					//cout<<"wall50done ";
					}
				}
				else if(!checkCollisionAC(dot.mCollider, wall51)){
				task2.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if(dot.pointsupdated){
				//cout<<"wall51 ";
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					////cout<<"wall50done ";
					}
				}
				
				else if(!checkCollisionAC(dot.mCollider, wall52)){
				task3.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if(dot.pointsupdated){
				//cout<<"wall52 ";
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					////cout<<"wall50done ";
					}
				}
				
				else if(!checkCollisionAC(dot.mCollider, wall56)){
				task41.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if(dot.pointsupdated){
				//cout<<"wall56 ";
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					////cout<<"wall50done ";
					}
				}
				else if(!checkCollisionAC(dot.mCollider, wall57)){
				task43.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if(dot.pointsupdated){
				//cout<<"wall57 ";
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					////cout<<"wall50done ";
					}
				}
				else if(!checkCollisionAC(dot.mCollider, wall58)){
				task44.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if(dot.pointsupdated){
				//cout<<"wall58 ";
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					////cout<<"wall50done ";
					}
				}
				else if(!checkCollisionAC(dot.mCollider, wall59)){
				task42.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if(dot.pointsupdated){
				//cout<<"wall59 ";
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					////cout<<"wall50done ";
					}
				}
				
				else if(!checkCollisionAC(dot.mCollider, wall53)){
				task5.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if(dot.pointsupdated){
				//cout<<"wall53 ";
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					////cout<<"wall50done ";
					}
				}
				
				else if(!checkCollisionAC(dot.mCollider, wall60)){
				task63.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if(dot.pointsupdated){
				//cout<<"wall60 ";
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					////cout<<"wall50done ";
					}
				}
				else if(!checkCollisionAC(dot.mCollider, wall61)){
				task62.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if(dot.pointsupdated){
				//cout<<"wall61 ";
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					////cout<<"wall50done ";
					}
				
				}
				else if(!checkCollisionAC(dot.mCollider, wall62)){
				task61.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if(dot.pointsupdated){
				//cout<<"wall62 ";
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					////cout<<"wall50done ";
					}
				}
				
				else if(!checkCollisionAC(dot.mCollider, wall54)){
				task7.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if(dot.pointsupdated){
				//cout<<"wall54 ";
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					////cout<<"wall50done ";
					}
				}
				
				else if(!checkCollisionAC(dot.mCollider, wall55)){
				task8.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if(dot.pointsupdated){
				//cout<<"wall55 ";
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					////cout<<"wall50done ";
					}
				}
				else if(!checkCollisionAC(dot.mCollider, wall63)){
				oops.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
				if(dot.pointsupdated){
				//cout<<"wall63 ";
					taskdone.render(SCREEN_WIDTH/2 - 193,SCREEN_HEIGHT/2 - 322);
					
					}
				}
				else {dot.pointsupdated = false;}
				
				gMapWindowTexture.render( 0, 0);
				gPauseTexture.render( 0, 0 );
				gStartTexture.render(0, 0);
				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
