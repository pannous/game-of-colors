
// gcc  -I /usr/local/include/SDL2 -lSDL2 game_of_colors.c -o game_of_colors && ./game_of_colors

// #include "SDL.h"
#include <SDL.h>
#include <stdio.h>

SDL_Surface *surface;
SDL_Window *window; 
SDL_Renderer *renderer;

// const int SCREEN_WIDTH = 1600;
// const int SCREEN_HEIGHT = 1000;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;
// const int SCREEN_WIDTH = 400;
// const int SCREEN_HEIGHT = 400;
// const int SCREEN_WIDTH = 1280;
// const int SCREEN_HEIGHT = 1024;

const int BYTES_PER_PIXEL=4;
int pitch=SCREEN_WIDTH;
int iteration=0;
float f=0;// coupling factor

const Uint8 *state;
void dot(int x, int y, Uint32 pixel){
	Uint32 *p=surface->pixels + y * pitch + x*BYTES_PER_PIXEL;
	*(Uint32 *)p = pixel;
}

void fill(){
	for(int x=0;x<SCREEN_WIDTH;x++){
		for(int y=0;y<SCREEN_HEIGHT;y++){
			int c=rand ();
			// if(c%100>60)c=0;else c=128;
			dot(x,y,c);
		}
	}
	SDL_UpdateWindowSurface(window);
}

int get_safe(int x, int y){
	if(x<0)x=SCREEN_WIDTH-1;
	if(y<0)y=SCREEN_HEIGHT-1;	
	Uint32 *p=surface->pixels + (y%SCREEN_HEIGHT) * pitch + (x%SCREEN_WIDTH)*BYTES_PER_PIXEL;
	return *p;//((int*)surface->pixels+y%SCREEN_HEIGHT * surface->pitch + (x%SCREEN_WIDTH)*4);
}

uint get(int x, int y){
	return get_safe(x,y);
	// Uint32 *p=surface->pixels + y * pitch + x *BYTES_PER_PIXEL;
	// return *p;
}

Uint8 get_r(int x, int y){
	Uint8 *p=surface->pixels + y * pitch + x *BYTES_PER_PIXEL;
	return *(p+2);
}
Uint8 get_g(int x, int y){
	Uint8 *p=surface->pixels + y * pitch + x *BYTES_PER_PIXEL;
	return *(p+1);
}
Uint8 get_b(int x, int y){
	Uint8 *p=surface->pixels + y * pitch + x *BYTES_PER_PIXEL;
	return *p;
}

uint calc(int x, int y){
	
	// r,g,b color values around center 'c11'
	// 00 10 20
	// 01 11 21
	// 02 12 22
	
	uint c00=get(x-1,y-1);
	uint c01=get(x-1,y);
	uint c02=get(x-1,y+1);
	uint c10=get(x,y-1);
	uint c11=get(x,y);
	uint c12=get(x,y+1);
	uint c20=get(x+1,y-1);
	uint c21=get(x+1,y);
	uint c22=get(x+1,y+1);
	
	// red
	uint r00=get_r(x-1,y-1);
	uint r01=get_r(x-1,y)  ;
	uint r02=get_r(x-1,y+1);
	uint r10=get_r(x,y-1)  ;
	uint r11=get_r(x,y)	;
	uint r12=get_r(x,y+1)  ;
	uint r20=get_r(x+1,y-1);
	uint r21=get_r(x+1,y)  ;
	uint r22=get_r(x+1,y+1);
	
	// green
	uint g00=get_g(x-1,y-1);
	uint g01=get_g(x-1,y)  ;
	uint g02=get_g(x-1,y+1);
	uint g10=get_g(x,y-1)  ;
	uint g11=get_g(x,y)	;
	uint g12=get_g(x,y+1)  ;
	uint g20=get_g(x+1,y-1);
	uint g21=get_g(x+1,y)  ;
	uint g22=get_g(x+1,y+1);
	
	// blue
	uint b00=get_b(x-1,y-1);
	uint b01=get_b(x-1,y)  ;
	uint b02=get_b(x-1,y+1);
	uint b10=get_b(x,y-1)  ;
	uint b11=get_b(x,y)	;
	uint b12=get_b(x,y+1)  ;
	uint b20=get_b(x+1,y-1);
	uint b21=get_b(x+1,y)  ;
	uint b22=get_b(x+1,y+1);
	
	// center colors of THIS dot
	uint r=r11;
	uint g=g11;
	uint b=b11;
	
	// random colors
	uint zr=rand()%256;
	uint zg=rand()%256;
	uint zb=rand()%256;	
	
	// mean color value of surrounding
	uint r0=((r10+r01+r21+r12)/4);
	uint g0=((g10+g01+g21+g12)/4);
	uint b0=(b10+b01+b21+b12)/4;
	
	// Including this dot
	// uint r0=((r10+r01+r11+r21+r12)/5);
	// uint g0=((g10+g01+g11+g21+g12)/5);
	// uint b0=(b10+b01+b11+b21+b12)/5;
	
	float fr=r0/256.0;
	float fg=g0/256.0;
	float fb=b0/256.0;
	
	uint k=128;
	float h=100.0;

//////////
// done with initialization, 
// now updated the dot's color based on some crazy experimental whatever function
// feel free to wildly experiment with this algorithm!

// POPULATION UPDATE

	//  inspired by Conway's game of life:

	//  if there is a sufficient population, then grow in numbers:
	if(b0>=100)b=b0*1.01;
	//  if the population is small then shrink
	if(b0<100)b=b0*(0.99-fr/100);
	//  if the population is too big then collapse
	if(b0>240)b=0;
	//  repopulate collapsed populations
	if(b0<3)b=120;//zb*((1.5-f)+fr*f);
	
	if(r0>=100)r=r0*1.01;
	if(r0<100)r=r0*(0.99-fg/100);
	if(r11>240)r=0;
	if(r11<3)r=120*(1-fr*fg);//zb*((1.5-f)+fr*f);
	
	if(g0>=100)g=g0*1.01;
	if(g0<100)g=g0*(0.99-f*fb/100);
	if(g11>240){g=0;b=b/2;}
	if(g11<3)g=120*(1-fb*fr);//zb*((1.5-f)+fr*f);

// END OF POPULATION UPDATE 

	if(r>255)r=255;
	if(g>255)g=255;
	if(b>255)b=255;
	return (r<<16)+(g<<8)+b;
}


void loop(){
	iteration++;
	f=(iteration%200)/200.0;
	// f1=
	for(int x=1;x<SCREEN_WIDTH-1;x++){
		for(int y=1;y<SCREEN_HEIGHT-1;y++){
			uint c=calc(x,y);
			// if(c!=0)
			dot(x,y,c);
		}
	}
	for(int x=SCREEN_WIDTH-1;x>0;x--){
		for(int y=SCREEN_HEIGHT-1;y>0;y--){
			uint c=calc(x,y);
			// if(c!=0)
			dot(x,y,c);
		}
	}
	
	SDL_PumpEvents();
	if (state[SDL_SCANCODE_RETURN]||state[SDL_SCANCODE_ESCAPE])exit(0);
	
	// SDL_Event test_event;
	// SDL_KeyboardEvent key;
	// while(SDL_PollEvent(&test_event)) {
	// 	switch(test_event.type) {
	// 		case SDL_KEYDOWN:
	// 		key=test_event;
	// 		if(key.keysym.sym==SDLK_ESCAPE)exit(0);
	// 		// case SDL_MOUSEMOTION:
	// 		break;
	// 	    default:
	// 		break;
	// 	}
	// }
	SDL_UpdateWindowSurface(window);
	// SDL_Delay(10);	
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2
    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "Game of dots",                  // window title
        800,           // initial x position
        800,           // initial y position
        SCREEN_WIDTH,                               // width, in pixels
        SCREEN_HEIGHT,                               // height, in pixels
        SDL_WINDOW_OPENGL //|SDL_GL_DOUBLEBUFFER
    );
    // The window is open: enter program loop (see SDL_PollEvent)
	surface=SDL_GetWindowSurface(window);// SDL_GetVideoSurface();
	pitch=surface->pitch;
	state= SDL_GetKeyboardState(NULL);
	fill();
	while(true){
		loop();	
		SDL_PumpEvents();
		if (state[SDL_SCANCODE_RETURN]||state[SDL_SCANCODE_ESCAPE])exit(0);
	}
	
    SDL_Delay(3000);  // Pause execution for 3000 milliseconds, for example

    
    // Close and destroy the window
    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();
    return 0;
}
