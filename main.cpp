/* 
--DOCUMENTATION SECTION--
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

This includes the entire SDL2 library which is used
 for various functionalities such as window creation, rendering,
 events, input, timers
 
 And csdint library which gives fixed-size integer types like uint32_t and uint8_t
 which is important for pixel math (uint32_t and uint8_t represents an unsigned
 integer with an exact size of 32 bits (8 bits))
 
Vector library - This is used to create the functions inside a class
is used to store positions and directions in 3D space. It also provides basic 
vector operations like addition, subtraction, scalar multiplication, 
dot product and cross product which are important for 3D graphics calculations.
 */

#include<iostream>
#include<SDL2/SDL.h>
#include<cstdint>
#include<cmath>
#include<vector>

class Vec3 {
//Vector 3 class, its used for 3D vertices, rotation, projection, camera math
  public:
    float x, y, z;
    //Initial contrustors (contructor overloading)
    Vec3() : x(0), y(0), z(0) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    //Vector arithmetic implementation
    //Vector addition (operator overloading)
    Vec3 operator+(const Vec3& v) const {
      return Vec3(x+v.x, y+v.y, z+v.z);
    }

    //Vector subtraction (operator overloading)
    Vec3 operator-(const Vec3& v) const {
      return Vec3(x-v.x, y-v.y, z-v.z);
    }

    //Vector division (operator overloading)
    Vec3 operator/(float s) const {
      return Vec3(x / s, y / s, z / s);
    }

    //Vector multiplication (operator overloading)
    Vec3 operator*(float s) const {
      return Vec3(x * s, y * s, z * s);
    }

    //Dot product
    float dot(const Vec3& v) const {
      return x * v.x + y * v.y + z * v.z;
    }

    //Cross product
    Vec3 cross(const Vec3& v) const {
      return Vec3(
        y * v.z - z * v.y,
        z * v.x - x * v.z,
        x * v.y - y * v.x
      );
    }

    //Length(Magnitude of the vector)
    float length() const {
      return std::sqrt(x*x + y*y + z*z);
    }

    //Noramalising vectors
    Vec3 normalize() const {
      float len = length();
      if(len == 0){
        return Vec3();
      }
      return *this / len;
    }
};

class Mat4 {
/* The Mat4 class represents a 4x4 matrix used for performing
transformations in 3D graphics. These matrices are used to rotate, scale and transform Vec3
objects in 3D space. The class also includes rotation matrices for the X, Y and Z axes
which are used to rotate objects like triangles and cubes. */
  public:
    float m[4][4];
    Mat4(){
      for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
          m[i][j] = (i == j) ? 1.0f : 0.0f;
        }
      }
    }
    
    Mat4 operator*(const Mat4& other) const {
      Mat4 result;
      for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
          result.m[i][j] = 0;
          for(int k = 0; k < 4; k++){
            result.m[i][j] += m[i][k] * other.m[k][j];
          }
        }
      }
      return result;
    }
    
    Vec3 operator*(const Vec3& v) const {
      float x = v.x * m[0][0] + v.y * m[0][1] + v.z * m[0][2] + m[0][3];
      float y = v.x * m[1][0] + v.y * m[1][1] + v.z * m[1][2] + m[1][3];
      float z = v.x * m[2][0] + v.y * m[2][1] + v.z * m[2][2] + m[2][3];
      float w = v.x * m[3][0] + v.y * m[3][1] + v.z * m[3][2] + m[3][3];
      
      if(w != 0.0f){
        x /= w;
        y /= w;
        z /= w;
      }
      return Vec3(x, y, z);
    }

    //To make rotations at X-axis
    static Mat4 rotationX(float angle){
      Mat4 r;
      r.m[1][1] = cos(angle);
      r.m[1][2] = -sin(angle);
      r.m[2][1] = sin(angle);
      r.m[2][2] = cos(angle);
      
      return r;
    }

    //To make rotations at X-axis
    static Mat4 rotationY(float angle){
      Mat4 r;
      r.m[0][0] = cos(angle);
      r.m[0][2] = sin(angle);
      r.m[2][0] = -sin(angle);
      r.m[2][2] = cos(angle);
      
      return r;
    }

    //To make rotations at X-axis
    static Mat4 rotationZ(float angle){
      Mat4 r;
      r.m[0][0] = cos(angle);
      r.m[0][1] = -sin(angle);
      r.m[1][0] = sin(angle);
      r.m[1][1] = cos(angle);
      
      return r;
    }
    
};

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

    //Vertices of Cube
    std::vector<Vec3> cubeVertices = {
      {-1, -1, -1},
      {1, -1, -1},
      {1, 1, -1},
      {-1, 1, -1},
      {-1, -1, 1},
      {1, -1, 1},
      {1, 1, 1},
      {-1, 1, 1},
    };

    //Edges of cube
    std::vector<std::pair<int, int>> cubeEdges = {
      {0,1}, {1,2}, {2,3}, {3,0},
      {4,5}, {5,6}, {6,7}, {7,4},
      {0,4}, {1,5}, {2,6}, {3,7}
    };

    //Projection of values
    Vec3 project(const Vec3& v){
      float distance = 3.0f;
      float scale = 300.0f;
      float z = v.z + distance;
      float x = (v.x / z) * scale + WIDTH / 2;
      float y = (v.y / z) * scale + HEIGHT / 2;
      
      return Vec3(x, y, z);
    }

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

//Implementation of Bresenham line algorithm 
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

//To draw triangle
void Engine::drawTriangle(int x1, int y1, int z1,
                          int x2, int y2, int z2,
                          int x3, int y3, int z3,
                          uint32_t color){
  drawLine(x1, y1, x2, y2, color);
  drawLine(x2, y2, x3, y3, color);
  drawLine(x3, y3, x1, y1, color);
}

uint32_t white = 0xFFFFFFFF; //Color of the Cube's wire frame

void Engine::render(){
  for(int i = 0; i < WIDTH * HEIGHT; i++){
    pixels[i] = 0x000000FF;
  }

		//Actual animation of the cube
  float angle = time * 0.005f;
  Mat4 rotX = Mat4::rotationX(angle);
  Mat4 rotY = Mat4::rotationY(angle);
  Mat4 rotZ = Mat4::rotationZ(angle);
  
  Mat4 rotation = rotZ * rotY * rotX;
  std::vector<Vec3> transformed;
  
  for(const auto& v : cubeVertices){
    transformed.push_back(rotation * v);
  }
  
  for(const auto& edge : cubeEdges){
    Vec3 v1 = project(transformed[edge.first]);
    Vec3 v2 = project(transformed[edge.second]);
    
    drawLine(
      (int)v1.x, (int)v1.y,
      (int)v2.x, (int)v2.y,
      0xFFFFFFFF
    );
  } 

 /* TEST FOR 2D TRIANGLE
  drawTriangle(
    400, 150, 0,
    200, 450, 0,
    600, 450, 0,
    white
  ); */ 
  
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
