#include <SDL2/SDL.h>
#include <iostream>

//variables

bool running = true;
SDL_Window * window = nullptr;
SDL_Renderer * renderer = nullptr;
int screen_x = 800;
int screen_y = 600;
int minX = 0;
int minY = 0;
SDL_Event e;
const int CELL_SIZE = 20;
int dx = 0;
int dy = 0;
SDL_Rect myrect = {400, 300, 20, 20};
SDL_Rect myfood = {0, 0, 15 , 15};

void spawn_food() {
    myfood.x = (rand() % (800 / 20)) * 20;  // keeps it on grid
    myfood.y = (rand() % (600 / 20)) * 20;
}

//food function

void food_func(){

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &myfood);
    SDL_RenderFillRect(renderer, &myfood);
}

void snake(){

    myrect.x += 0;
    myrect.y += 0;
    //snake blob
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderDrawRect(renderer, &myrect);
    SDL_RenderFillRect(renderer, &myrect);

}

void gameover(){

}

void input(){
    if(e.type == SDL_KEYDOWN){
        switch (e.key.keysym.sym)
        {
        case SDLK_w:
            dy = -CELL_SIZE; dx = 0;
            break;
        case SDLK_s:
            dy = CELL_SIZE; dx = 0;
            break;
        case SDLK_d:
            dx = CELL_SIZE; dy = 0;
            break;
        case SDLK_a:
            dx = -CELL_SIZE; dy = 0;
            break;
        case SDLK_ESCAPE:
            running = false;
        
        default:
            break;
        }
    }

}


int main () {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Error: SDl Not found" << SDL_GetError() << std::endl;
        return 1;
    }

    window = SDL_CreateWindow("Snake_Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED ,screen_x , screen_y,  SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);




    //keeping the screen going


    while (running) {
        while (SDL_PollEvent(&e)) {
            switch (e.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            }
            input();
        }


        Uint32 lastMove = 0;
        int moveDelay = 150; // ms per step (speed)

        // inside game loop
            if (SDL_GetTicks() - lastMove > moveDelay) {
                myrect.x += dx;
                myrect.y += dy;
                lastMove = SDL_GetTicks();
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        snake();
        food_func();
        SDL_RenderPresent(renderer);
        SDL_Delay(16); //60 fps
    }

    SDL_Quit();

    return 0;
}