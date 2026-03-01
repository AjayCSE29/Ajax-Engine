/* --DOCUMENTATION SECTION--
This is the Ajax Engine, an open-source project built by the user AjayCSE29.
This engine is based on C++'s SDL2 library to make it work, this project's
main motive is to create a 3d engine which could run on lower-end
hardware.

Since this is an open-source project, you can contribute to this project 
and make this a better engine, the main goal is to minimize everything so 
that it could run on any lower-end hardware.

The project right now, as it is, creates a window and draws a rectangle
in it in which the window only exits when the user closes it, updates will 
take place on a daily basis so look forward to the code, thank you.
*/

/*This includes the entire SDL2 library which is used
 for various functionalities such as window creation, rendering,
 events, input, timers*/
#include<SDL2/SDL.h>
#include<iostream>

int main() { //The SDL_Init(SDL_INIT_VIDEO) starts the SDL video subsystem
  if(SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cout<<"SDL Init Error: "<<SDL_GetError() << std::endl;
    return 1; //SDL_GetErroe() says the reason of error if one occurs
  }
  
  std::cout<<"Window created"<<std::endl;
  
  SDL_Window* window = SDL_CreateWindow( //Creates OS level window
    "SDL Test", //Window title
    SDL_WINDOWPOS_CENTERED, //Position of window (X-axis)
    SDL_WINDOWPOS_CENTERED, //Position of window (Y-axis)
    800, 600, //Width and Height of the window
    SDL_WINDOW_SHOWN //Shows the window on the screen
  );
  
  if(!window) {
    std::cout<<"Window Error: "<< SDL_GetError << std::endl;
    SDL_Quit();
    return 1;
  }
  
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  /*Creates a renderer (A drawing engine attached to the window)
  {window: Which window to draw on, -1: automatically chooses GPU, 
   SDL_RENDERER_ACCELERATED: Use hardware acceleration}
   -> These allows GPU Based drawing  */
   
  if(!renderer){
    std::cout << "Renderer error: "<<SDL_GetError() << std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }
  
  bool running = true; //Controls loop, ensures the program doesn't exit immediately when executed 
  SDL_Event event; //Its a structure that stores input events such keyboard, mouse, window close etc
  
  while(running) { //Ensure the engine is running until it's closed
    while(SDL_PollEvent(&event)) { //Checks for events
      if(event.type == SDL_QUIT){
        running = false;
      }
    }
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //Creates a empty / Black window
    SDL_RenderClear(renderer); //Fills the entire screen with current color (Black)
    
    SDL_Rect rect = {300, 200, 200, 150}; //Creates a rectangle to draw
                    //position: x, y and width, height
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); //Sets drawing color to red
    SDL_RenderFillRect(renderer, &rect); //Draws a rectangle
    
    SDL_RenderPresent(renderer); //Makes the drawn rectangle visible on the screen
  }
  
  //Memory cleanup (Dynamic)
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  //Close window
  SDL_Quit();
  
  std::cout<<"Window Destroyed"<<std::endl;
  return 0;
}

