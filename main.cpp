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

#include<iostream>
#include<SDL2/SDL.h>
#include<cstdint>

const int WIDTH = 800; //Width of the window
const int HEIGHT = 600; //Height of the window

class Engine {
  public:
    bool init();
    void run();
    void cleanup();
    
  private:
    void handleEvents();
    void update();
    void render();
    void drawPixel(int x, int y, uint32_t color);
    void drawLine(int x1, int y1, int x2, int y2, uint32_t color);
    void drawTriangle(int x1, int y1, int z1,
                      int x2, int y2, int z2,
                      int x3, int y3, int z3,
                      uint32_t color);
    
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;
    uint32_t* pixels = nullptr;
    
    bool running = false;
    int time = 0;
};

bool Engine::init() { //The SDL_Init(SDL_INIT_VIDEO) starts the SDL video subsystem
  if(SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cout<<"SDL Init Error: "<< SDL_GetError() << std::endl;
    return 1; //SDL_GetErroe() says the reason of error if one occurs
  }
  
  window = SDL_CreateWindow(
  //Creates OS level window
    "Ajax Engine",
    SDL_WINDOWPOS_CENTERED,  //Position of window (X-axis)
    SDL_WINDOWPOS_CENTERED, //Position of window (Y-axis)
    WIDTH, HEIGHT,
    SDL_WINDOW_SHOWN //Shows the window on the screen
  );
  
   //The below code block is to ensure that window is available, otherwise it closes
  if(!window) {
    std::cout<<"Window Error: " << SDL_GetError() << std::endl;
    return 1;
  }
  
  renderer = SDL_CreateRenderer(
    window,
    -1,
    SDL_RENDERER_ACCELERATED
  );
  
   /*Creates a renderer (A drawing engine attached to the window)
  {window: Which window to draw on, -1: automatically chooses GPU, 
   SDL_RENDERER_ACCELERATED: Use hardware acceleration}
   -> These allows GPU Based drawing  */
  
  //The below code block is to ensure that renderer is available, otherwise it closes
  if(!renderer){
    std::cout<<"Renderer error: " << SDL_GetError() << std::endl;
    return 1;
  }
  
  texture = SDL_CreateTexture(
  //This creates a GPU texture
    renderer,
    SDL_PIXELFORMAT_RGBA8888, //Imporrant parameter, each pixel is R + G + B + A = 32 bits in total (8 bits each)
    SDL_TEXTUREACCESS_STREAMING,  //Ensures that this texture updates every frame form the CPU
    WIDTH, HEIGHT
  );
  
   //The below code block is to ensure that texture is available, otherwise it closes
  if(!texture){
    std::cout<<"Texture Error: " << SDL_GetError() << std::endl;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }
  
  pixels = new uint32_t[WIDTH * HEIGHT];  //This creates a dynamic block of memory, also called as framebuffer
                                           //800 * 600 = 480,000 pixels, each pixel if 32 bits (4 bytes), the total memory used is ~1.
  running = true; //Controls loop, ensures the program doesn't exit immediately when executed 
  return true;
}

void Engine::handleEvents(){
 //Its a structure that stores input events such keyboard, mouse, window close etc
  SDL_Event event;
    while(SDL_PollEvent(&event)) { //Checks for events
      if(event.type == SDL_QUIT) {
        running = false;
    }
  }
}

void Engine::update(){
  time++;  //This the frame changes to time
}

void Engine::drawPixel(int x, int y, uint32_t color){
  if(x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT){
    return;
  }
  pixels[y * WIDTH + x] = color;
}

void Engine::drawLine(int x1, int y1, int x2, int y2, uint32_t color){
  int dx = abs(x2 - x1);
  int dy = abs(y2 - y1);
  
  int sx = (x1 < x2) ? 1 : -1;
  int sy = (y1 < y2) ? 1 : -1;
  int err = dx - dy;
  while(true){
    drawPixel(x1, y1, color);
    if(x1 == x2 && y1 == y2){
      break;
    }
    int e2 = 2 * err;
    if(e2 > -dy){
      err -= dy;
      x1 += sx;
    }
     if(e2 < dx) {
      err += dx;
      y1 += sy;
     }
  }
}

void Engine::drawTriangle(int x1, int y1, int z1,
                          int x2, int y2, int z2,
                          int x3, int y3, int z3,
                          uint32_t color){
  drawLine(x1, y1, x2, y2, color);
  drawLine(x2, y2, x3, y3, color);
  drawLine(x3, y3, x1, y1, color);
}

uint32_t white = 0xFFFFFFFF;
void Engine::render(){

  for(int i = 0; i < WIDTH * HEIGHT; i++){
    pixels[i] = 0x000000FF;
  }
  
  drawTriangle(
    400, 150, 0,
    200, 450, 0,
    600, 450, 0,
    white
  );
  
  /* TEST FOR DRAWLINE
  drawLine(100, 100, 700, 500, white);
  drawLine(700, 100, 100, 500, white);
  drawLine(400, 0, 400, 600, white);
  drawLine(0, 300, 800, 300, white); */

 /* TEST FOR SDL2
 for(int y = 0; y < HEIGHT; y++){
    for(int x = 0; x < WIDTH; x++){
      int index = y * WIDTH + x;
      uint32_t r = (x + time) % 256;
      uint32_t g = (y + time) % 256;
      uint32_t b = (x + y + time) % 256;
      
      pixels[index] = 
        (r << 24) |
        (g << 16) |
        (b << 8) |
        255;
    }
  } */   
  
  SDL_UpdateTexture(texture, NULL, pixels, WIDTH * sizeof(uint32_t)); //VERY IMPORTANT: This copies the CPU frame buffer to GPU texture, the screen stays black without it
  SDL_RenderClear(renderer); //Clears renderer
  SDL_RenderCopy(renderer, texture, NULL, NULL); //Copies full texture onto screen
  SDL_RenderPresent(renderer); //Swaps buffers
}

void Engine::run(){
  while(running){
    handleEvents();
    update();
    render();
  }
}

void Engine::cleanup(){
  delete[] pixels;
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

int main(){
  Engine obj;
  if(!obj.init()){
    return 1;
  }
  obj.run();
  obj.cleanup();
}
