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
 events, input, timers
 
 And csdint library which gives fixed-size integer types like uint32_t and uint8_t
 which is important for pixel math (uint32_t and uint8_t represents an unsigned
 integer with an exact size of 32 bits (8 bits))*/
#include<SDL2/SDL.h>
#include<iostream>
#include<csdint>

const int WIDTH = 800; //Width of the window
const int HEIGHT = 600; //Height of the window

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
    WIDTH, HEIGHT,
    SDL_WINDOW_SHOWN //Shows the window on the screen
  );

 //The below code block is to ensure that window is available, otherwise it closes
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

  //The below code block is to ensure that renderer is available, otherwise it closes
  if(!renderer){
    std::cout << "Renderer error: "<<SDL_GetError() << std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

 //This creates a GPU texture
 SDL_Texture*texture = SDL_CreateTexture(
    renderer,
    SDL_PIXELFORMAT_ARGB8888, //Imporrant parameter, each pixel is R + G + B + A = 32 bits in total (8 bits each)
    SDL_TEXTUREACCESS_STREAMING, //Ensures that this texture updates every frame form the CPU
    WIDTH, HEIGHT
  );

 //The below code block is to ensure that texture is available, otherwise it closes
  if(!texture) {
    std::cout<<"Texture Error: "<< SDL_GetError() << std::endl;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  uint32_t* pixels = new uint32_t[WIDTH * HEIGHT]; //This creates a dynamic block of memory, also called as framebuffer
                                                   //800 * 600 = 480,000 pixels, each pixel if 32 bits (4 bytes), the total memory used is ~1.8 MB
 
  bool running = true; //Controls loop, ensures the program doesn't exit immediately when executed 
  SDL_Event event; //Its a structure that stores input events such keyboard, mouse, window close etc
  int time = 0;
  
  while(running) { //Ensure the engine is running until it's closed
    while(SDL_PollEvent(&event)) { //Checks for events
      if(event.type == SDL_QUIT){
        running = false;
      }
    }
    //Initiating pixel drawing	
    for(int y = 0; y < HEIGHT; y++){ //Loop through rows
      for(int x = 0; x < WIDTH; x++) { //Loop through columns
        int index = y * WIDTH + x; //Converting 2D into 1D index because memory is a 1D array
							//Color calculation, each channel ranges from 0-255
							//The colors are based on x, y position and time for animation 
        uint32_t r = (x + time) % 256; //256 ensures that values are inside color range
        uint32_t g = (y + time) % 256; 
        uint32_t b = (x + y + time) % 256;
        
        pixels[index] = (255 << 24) | (r << 16) | (g << 8) | (b << 8); //Packing RGBA into 32 bits
																								//The animation is acheived by bit-shifting between r, g, b and a (Alpha)
																								//These are combined usinf a bitwise operator {Final layout in memory: [R][G][B][A]}
      }
    }
    time++; //This ensures that for each frame, color shifts slightly, the reason gradient moves
   
    //The below code is from the previous version to check if the created window can draw or not, feel free to use this to check
    /*SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //Creates a empty / Black window
    SDL_RenderClear(renderer); //Fills the entire screen with current color (Black)
    SDL_Rect rect = {300, 200, 200, 150}; //Creates a rectangle to draw
                    //position: x, y and width, height
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); //Sets drawing color to red
    SDL_RenderFillRect(renderer, &rect); //Draws a rectangle */
     
    SDL_UpdateTexture(texture, NULL, pixels, WIDTH * sizeof(uint32_t)); //VERY IMPORTANT: This copies the CPU frame buffer to GPU texture, the screen stays black without it
    SDL_RenderClear(renderer); //Clears renderer
    SDL_RenderCopy(renderer, texture, NULL, NULL); //Copies full texture onto screen
    SDL_RenderPresent(renderer); //Swaps buffers
  }

  delete[] pixels;
  //Memory cleanup (Dynamic)
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  //Close window
  SDL_Quit();
  
  std::cout<<"Window Destroyed"<<std::endl;
  return 0;
}

