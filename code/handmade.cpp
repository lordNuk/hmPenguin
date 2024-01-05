#include <cstdint>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <SDL.h>

#define internal static
#define local_persist static
#define gloabl_variable static

gloabl_variable bool Running;
gloabl_variable SDL_Texture *Texture;
gloabl_variable void *BitmapMemory;
gloabl_variable int BitmapWidth;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

void SDLResizeTexture (SDL_Renderer *Renderer, int Width, int Height) {
    if(Texture) {
        SDL_DestroyTexture(Texture);
    }
    if(BitmapMemory) {
        free(BitmapMemory);
    }
    Texture =  SDL_CreateTexture(  
            Renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            Width,
            Height);
    BitmapWidth = Width;
    int BytesPerPixel = 4;
    BitmapMemory = malloc(Width * Height * BytesPerPixel);
    int Pitch = Width*BytesPerPixel;
    uint8 *Row = (uint8 *)BitmapMemory;
    for(int y = 0; y < Height; y++) {
        uint8 *Pixel = (uint8 *)Row;
        for(int x = 0; x < Width; x++) {
            *Pixel = (uint8)x; 
            ++Pixel;
            *Pixel = (uint8)y; 
            ++Pixel;
            *Pixel = 0; 
            ++Pixel;
            *Pixel = 255; 
            ++Pixel;
        }
        Row += Pitch;
    }
}

void SDLUpdateWindow (SDL_Renderer *Renderer){
    if(SDL_UpdateTexture(Texture,
            0,
            BitmapMemory,
            BitmapWidth*4)) {
        // do something about error
    }
    SDL_RenderCopy(Renderer,
            Texture,
            0, 0);
    SDL_RenderPresent(Renderer);
}

void HandleEvent(SDL_Event *Event) {
    switch(Event->type) {
        case SDL_QUIT: 
            {
                printf("SDL_QUIT\n");
                Running = false;
            } break;
        case SDL_WINDOWEVENT: 
            {
                switch(Event->window.event) 
                {
                    case SDL_WINDOWEVENT_SIZE_CHANGED: 
                        {
                            SDL_Window *Window = SDL_GetWindowFromID(Event->window.windowID);
                            SDL_Renderer *Renderer = SDL_GetRenderer(Window);
                            printf("SDL_WINDOWEVENT_SIZE_CHANGED (%d, %d)\n", Event->window.data1, Event->window.data2); 
                            SDLResizeTexture(Renderer, Event->window.data1, Event->window.data2); 
                        } break;
                    case SDL_WINDOWEVENT_EXPOSED: 
                        {
                            SDL_Window *Window = SDL_GetWindowFromID(Event->window.windowID);
                            SDL_Renderer *Renderer = SDL_GetRenderer(Window);
                            SDLUpdateWindow(Renderer);
                        } break;
                }
            }
    }

}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *Window = SDL_CreateWindow(
            "Handmade Penguid", //title
            SDL_WINDOWPOS_UNDEFINED, // horizontal pos of window
            SDL_WINDOWPOS_UNDEFINED, // vertical pos of window
            640,
            480,  // width and height (int)
            SDL_WINDOW_RESIZABLE// uint32 flags for extra options
            );

    if (Window) {
        // rendering something?
        SDL_Renderer *Renderer = SDL_CreateRenderer(
                Window, // window pointer
                -1,     // index of driver (-1 for autodetect)
                0);     // flags

        Running = true;
        if(!Renderer) {
            Running = false;
            //handle error;
        }

        // get the events and send then to handler function?
        // 2 possible functions: SDL_PollEvent and SDL_WaitEvent
        while(Running) {
            SDL_Event Event;
            SDL_WaitEvent(&Event);
            HandleEvent(&Event);
        }

    }

    SDL_Quit();
    return (0);
}

