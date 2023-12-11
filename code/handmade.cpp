#include <SDL.h>

bool Running = true;

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
          case SDL_WINDOWEVENT_RESIZED: 
            {
              printf("SDL_WINDOWEVENT_RESIZED (%d, %d)\n", Event->window.data1, Event->window.data2); 
            } break;
          case SDL_WINDOWEVENT_EXPOSED: 
            {
              SDL_Window *Window = SDL_GetWindowFromID(Event->window.windowID);
              SDL_Renderer *Renderer = SDL_GetRenderer(Window);
              static bool IsWhite = true;
              if (IsWhite == true) {
                SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
                IsWhite = false;
              } else {
                SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
                IsWhite = true;
              }
              SDL_RenderClear(Renderer);
              SDL_RenderPresent(Renderer);
            } break;
        }
      }
  }

}

int main(int argc, char *argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_ShowSimpleMessageBox(
        SDL_MESSAGEBOX_INFORMATION, 
        "Handmade Penguin",
        SDL_GetError(),
        0
        );
  }

  // creating a window...
  // step: create window handle, i.e. a pointer to struct of type SDL_Window
  SDL_Window *Window;
  Window = SDL_CreateWindow(
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

    // get the events and send then to handler function?
    // 2 possible functions: SDL_PollEvent and SDL_WaitEvent
    if(Renderer) {
      while(Running) {
        SDL_Event Event;
        SDL_WaitEvent(&Event);
        HandleEvent(&Event);
      }
    }

  }

  SDL_Quit();
  return (0);
}
