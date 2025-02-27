#include <iostream>
#include "Chip8.cpp"
#include <SDL2/SDL.h>

using namespace std;

bool init();
bool loop();
void kill();

SDL_Surface* winSurface = nullptr;
SDL_Window* window = nullptr;
Chip8 ch8;

uint color = 0;

int main(int argc, char const *argv[])
{
  if(argc < 2)
  {
    std::cout<<"Please include path to ROM"<<std::endl;
    return -1;
  }
  ch8.init(argv[1]);

  if ( !init() ) return 1;

  while ( loop() ) {
    ch8.step();

    //getchar();

	// wait before processing the next frame
	SDL_Delay(10);
	}

	kill();
	return 0;
}

bool loop() {

	SDL_Event e;

	while( SDL_PollEvent( &e ) != 0 ) {
		switch (e.type) {
			case SDL_QUIT:
        break;
			case SDL_KEYDOWN:
        switch(e.key.keysym.sym)
        {
          case SDLK_ESCAPE:
            kill();
            return 0;
          break;
        }
			case SDL_KEYUP:
				break;
		}
	}

  // Fill the window with a white rectangle
  SDL_FillRect( winSurface, NULL, SDL_MapRGB( winSurface->format, 0, 0, 0) );

  // Update the window display
  SDL_UpdateWindowSurface( window );

	return true;
}

bool init()
{
	// Initialize SDL. SDL_Init will return -1 if it fails.
	if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
		cout << "Error initializing SDL: " << SDL_GetError() << endl;
		return 1;
	}

	// Create our window
	window = SDL_CreateWindow( "Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 320, SDL_WINDOW_SHOWN );

	// Make sure creating the window succeeded
	if ( !window ) {
		cout << "Error creating window: " << SDL_GetError()  << endl;
		return 1;
	}

	// Get the surface from the window
	winSurface = SDL_GetWindowSurface( window );

	// Make sure getting the surface succeeded
	if ( !winSurface ) {
		cout << "Error getting surface: " << SDL_GetError() << endl;
		return 1;
	}

  return true;
}


void kill() {
	// Quit
	SDL_DestroyWindow( window );
	SDL_Quit();
}
