/*This source code copyrighted by Lazy Foo' Productions (2004-2022)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, vectors, and strings
#include<SDL2/SDL.h>
//#include <SDL.h>
#include<SDL2/SDL_image.h>
//#include <SDL_image.h>
#include <stdio.h>
#include <string>

//The dimensions of the level
const int LEVEL_WIDTH = 9952;
const int LEVEL_HEIGHT = 3936;

//Screen dimension constants
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 960;

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
		static const int DOT_WIDTH = 20;
		static const int DOT_HEIGHT = 20;

		//Maximum axis velocity of the dot
		static const int DOT_VEL = 10;

		//Initializes the variables
		Dot();

		//Takes key presses and adjusts the dot's velocity
		void handleEvent( SDL_Event& e );

		//Moves the dot and checks collision
		void move( SDL_Rect wall[] , int length);

		//Shows the dot on the screen relative to the camera
		void render( int camX, int camY );

		//Position accessors
		int getPosX();
		int getPosY();

    private:
		//The X and Y offsets of the dot
		int mPosX, mPosY;

		//The velocity of the dot
		int mVelX, mVelY;
		
		//Dot's collision box
		SDL_Rect mCollider;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Box collision detector
bool checkCollision( SDL_Rect a, SDL_Rect b[], int length );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
LTexture gDotTexture;
LTexture gBGTexture;

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
    mPosX = 672;
    mPosY = 224;
    
    //Set collision box dimension
	mCollider.w = DOT_WIDTH;
	mCollider.h = DOT_HEIGHT;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;
}

void Dot::handleEvent( SDL_Event& e )
{
    //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
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
            case SDLK_UP: mVelY += DOT_VEL; break;
            case SDLK_DOWN: mVelY -= DOT_VEL; break;
            case SDLK_LEFT: mVelX += DOT_VEL; break;
            case SDLK_RIGHT: mVelX -= DOT_VEL; break;
        }
    }
}

void Dot::move(SDL_Rect wall[], int length)
{
    //Move the dot left or right
    mPosX += mVelX;
mCollider.x = mPosX;
    //If the dot went too far to the left or right
    if( ( mPosX < 0 ) || ( mPosX + DOT_WIDTH > LEVEL_WIDTH ) || checkCollision( mCollider, wall, length ) )
    {
        //Move back
        mPosX -= mVelX;
        mCollider.x = mPosX;
    }

    //Move the dot up or down
    mPosY += mVelY;
	mCollider.y = mPosY;    

    //If the dot went too far up or down
    if( ( mPosY < 0 ) || ( mPosY + DOT_HEIGHT > LEVEL_HEIGHT ) || checkCollision( mCollider, wall, length ))
    {
        //Move back
        mPosY -= mVelY;
        mCollider.y = mPosY;
    }
}

void Dot::render( int camX, int camY )
{
    //Show the dot relative to the camera
	gDotTexture.render( mPosX - camX, mPosY - camY );
}

int Dot::getPosX()
{
	return mPosX;
}

int Dot::getPosY()
{
	return mPosY;
}

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

	//Load dot texture
	if( !gDotTexture.loadFromFile( "phin(1).bmp" ) )
	{
		printf( "Failed to load dot texture!\n" );
		success = false;
	}

	//Load background texture
	if( !gBGTexture.loadFromFile( "map.png" ) )
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
	gBGTexture.free();

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

bool checkCollision( SDL_Rect a, SDL_Rect b[], int length )
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
    bool iscollision =  true;
    
    //true means no collision =  outside rectangles
for(  int i=0;i< length ;i++){
    //Calculate the sides of rect B
    leftB = b[i].x;
    rightB = b[i].x + b[i].w;
    topB = b[i].y;
    bottomB = b[i].y + b[i].h;
	
	if(!iscollision){return false;} 	
	else{
    //If any of the sides from A are outside of B
    if( topA < topB )
    {
        iscollision= true;
    }

    else if( bottomA > bottomB )
    {
        iscollision= true;
    }

    else if( leftA < leftB )
    {
        iscollision= true;
    }

    else if( rightA > rightB )
    {
        iscollision= true;
    }
    else{ iscollision= false;}}
}
    //If none of the sides from A are outside B
    
 return iscollision;   
}

int main( int argc, char* args[] )
{
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

			//The dot that will be moving around on the screen
			Dot dot;
			
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
			
			SDL_Rect wall4;
			wall4.x = 22* 32;
			wall4.y = 57* 32 -32 ;
			wall4.w = 285* 32;
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
			
			//The camera area
			SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}

					//Handle input for the dot
					dot.handleEvent( e );
				}

				//Move the dot and check collision
				SDL_Rect wallarray[48]={wall1, wall2, wall3, wall4, wall5, wall6, wall7, wall8, wall9, wall10, wall11, wall12, wall13, wall14, wall15, wall16, wall17, wall18, wall19, wall20, wall21, wall22, wall23, wall24, wall25, wall26, wall27, wall28, wall29, wall30, wall31, wall32, wall33, wall34, wall35, wall36, wall37, wall38, wall39, wall40, wall41, wall42, wall43, wall44, wall45, wall46, wall47, wall48};
				//dot.move( wall );
				dot.move( wallarray ,48);


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

				//Render wall
				SDL_SetRenderDrawColor(gRenderer,0x00,0x00,0x00,0xFF);
				SDL_RenderDrawRect(gRenderer, &wall1);
				SDL_RenderDrawRect(gRenderer, &wall2);
				SDL_RenderDrawRect(gRenderer, &wall3);				
				SDL_RenderDrawRect(gRenderer, &wall4);
				SDL_RenderDrawRect(gRenderer, &wall5);
				SDL_RenderDrawRect(gRenderer, &wall6);
				SDL_RenderDrawRect(gRenderer, &wall7);
				SDL_RenderDrawRect(gRenderer, &wall8);
				SDL_RenderDrawRect(gRenderer, &wall9);
				SDL_RenderDrawRect(gRenderer, &wall10);
				SDL_RenderDrawRect(gRenderer, &wall11);
				SDL_RenderDrawRect(gRenderer, &wall12);
				SDL_RenderDrawRect(gRenderer, &wall13);
				SDL_RenderDrawRect(gRenderer, &wall14);
				SDL_RenderDrawRect(gRenderer, &wall15);
				SDL_RenderDrawRect(gRenderer, &wall16);				
				SDL_RenderDrawRect(gRenderer, &wall17);
				SDL_RenderDrawRect(gRenderer, &wall18);
				SDL_RenderDrawRect(gRenderer, &wall19);
				SDL_RenderDrawRect(gRenderer, &wall20);
				SDL_RenderDrawRect(gRenderer, &wall21);
				SDL_RenderDrawRect(gRenderer, &wall22);
				SDL_RenderDrawRect(gRenderer, &wall23);
				SDL_RenderDrawRect(gRenderer, &wall24);
				SDL_RenderDrawRect(gRenderer, &wall25);
				SDL_RenderDrawRect(gRenderer, &wall26);
				SDL_RenderDrawRect(gRenderer, &wall27);
				SDL_RenderDrawRect(gRenderer, &wall28);
				SDL_RenderDrawRect(gRenderer, &wall29);
				SDL_RenderDrawRect(gRenderer, &wall30);
				SDL_RenderDrawRect(gRenderer, &wall31);				
				SDL_RenderDrawRect(gRenderer, &wall32);
				SDL_RenderDrawRect(gRenderer, &wall33);
				SDL_RenderDrawRect(gRenderer, &wall34);
				SDL_RenderDrawRect(gRenderer, &wall35);
				SDL_RenderDrawRect(gRenderer, &wall36);
				SDL_RenderDrawRect(gRenderer, &wall37);
				SDL_RenderDrawRect(gRenderer, &wall38);
				SDL_RenderDrawRect(gRenderer, &wall39);
				SDL_RenderDrawRect(gRenderer, &wall40);
				SDL_RenderDrawRect(gRenderer, &wall41);
				SDL_RenderDrawRect(gRenderer, &wall42);
				SDL_RenderDrawRect(gRenderer, &wall43);
				SDL_RenderDrawRect(gRenderer, &wall44);
				SDL_RenderDrawRect(gRenderer, &wall45);
				SDL_RenderDrawRect(gRenderer, &wall46);
				SDL_RenderDrawRect(gRenderer, &wall47);
				SDL_RenderDrawRect(gRenderer, &wall48);
				
				//Render background
				gBGTexture.render( 0, 0, &camera );

				//Render objects
				dot.render( camera.x, camera.y );

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
