#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

const int SCREEN_X = 800;
const int SCREEN_Y = 600;
const int CELL_SIZE = 20;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Event e;
bool running = true;

// direction: one cell per tick
int dx = CELL_SIZE;
int dy = 0;

struct Cell {
    int x;
    int y;
};

// snake body (each cell is one segment)
std::vector<Cell> snake;

// food
Cell food;

void spawn_food() {
    food.x = (rand() % (SCREEN_X / CELL_SIZE)) * CELL_SIZE;
    food.y = (rand() % (SCREEN_Y / CELL_SIZE)) * CELL_SIZE;
}

void handle_input() {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
        case SDLK_w: dy = -CELL_SIZE; dx = 0; break;
        case SDLK_s: dy = CELL_SIZE; dx = 0; break;
        case SDLK_a: dx = -CELL_SIZE; dy = 0; break;
        case SDLK_d: dx = CELL_SIZE; dy = 0; break;
        case SDLK_ESCAPE: running = false; break;
        }
    }
}

void update_snake() {
    // new head
    Cell newHead = { snake.front().x + dx, snake.front().y + dy };

    // check wall collision
    if (newHead.x < 0 || newHead.x >= SCREEN_X ||
        newHead.y < 0 || newHead.y >= SCREEN_Y) {
        running = false; // game over
        return;
    }
    // check self collision
    for (const auto& c : snake) {
        if (newHead.x == c.x && newHead.y == c.y) {
            running = false;
            return;
        }
    }

    // add head
    snake.insert(snake.begin(), newHead);

    // check food
    if (newHead.x == food.x && newHead.y == food.y) {
        spawn_food(); // grow snake (don’t pop tail)
    } else {
        // remove tail
        snake.pop_back();
    }
}

void draw() {
    // background
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // draw food
    SDL_Rect foodRect = { food.x, food.y, CELL_SIZE, CELL_SIZE };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &foodRect);

    // draw snake
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (const auto& c : snake) {
        SDL_Rect r = { c.x, c.y, CELL_SIZE, CELL_SIZE };
        SDL_RenderFillRect(renderer, &r);
    }

    SDL_RenderPresent(renderer);
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL init error: " << SDL_GetError() << std::endl;
        return 1;
    }

    window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_X, SCREEN_Y, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // start snake with 3 cells
    snake.push_back({ SCREEN_X / 2, SCREEN_Y / 2 });
    snake.push_back({ SCREEN_X / 2 - CELL_SIZE, SCREEN_Y / 2 });
    snake.push_back({ SCREEN_X / 2 - 2 * CELL_SIZE, SCREEN_Y / 2 });

    spawn_food();

    Uint32 lastMove = 0;
    const int moveDelay = 150; // ms per step

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
            handle_input();
        }

        // move only every moveDelay ms
        if (SDL_GetTicks() - lastMove > moveDelay) {
            update_snake();
            lastMove = SDL_GetTicks();
        }

        draw();
        SDL_Delay(16); // ~60 fps
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
