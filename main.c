#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int BLOCK_SIZE = 20;
const int NUM_BLOCKS_WIDTH = SCREEN_WIDTH / BLOCK_SIZE;
const int NUM_BLOCKS_HEIGHT = SCREEN_HEIGHT / BLOCK_SIZE;
const int DELAY_MS = 100;
const int INITIAL_LENGTH = 3;

// Enumerations
typedef enum { UP, DOWN, LEFT, RIGHT } Direction;

// Struct definitions
typedef struct Point {
  int x, y;
} Point;

typedef struct Block {
  SDL_Rect rect;
  Uint32 color;
} Block;

typedef struct Snake {
  int len;
  int dir;
  Point pos[NUM_BLOCKS_WIDTH * NUM_BLOCKS_HEIGHT];
} Snake;

// Function prototypes
Block create_block(int x, int y, Uint32 color);
void clear_screen(SDL_Renderer *renderer);
void draw_block(SDL_Renderer *renderer, const Block block);
void draw_blocks(SDL_Renderer *renderer, const Block blocks[], int count);
Direction get_keyboard_direction();
bool check_collision(const Snake *snake, const Block block);
void advance_snake(Snake *snake);
bool grow_snake(Snake *snake);
void delete_tail(Snake *snake);
void process_input(Snake *snake, Direction input_dir);
bool game_over(const Snake *snake);

// Global variables
SDL_Window *window;
SDL_Renderer *renderer;
Block apple;
Snake snake;

int main() {
  // Seed random number generator
  srand((unsigned)time(NULL));

  // Initialise SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "Could not initialise SDL.\n");
    return EXIT_FAILURE;
  }

  // Create window and renderer
  window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
  if (window == NULL) {
    fprintf(stderr, "Failed to create window!\n");
    goto error_on_create_window;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL) {
    fprintf(stderr, "Failed to create renderer!\n");
    goto error_on_create_renderer;
  }

  // Initialise apple and snake
  memset(&apple, 0, sizeof(apple));
  memset(&snake, 0, sizeof(snake));
  apple = create_block( rand() % (NUM_BLOCKS_WIDTH), rand() % (NUM_BLOCKS_HEIGHT), 0xFFFFFFFF );
  for (int i = 0; i < INITIAL_LENGTH; i++) {
    snake.pos[i] = (Point){ NUM_BLOCKS_WIDTH / 2, NUM_BLOCKS_HEIGHT / 2 };
  }
  snake.len = INITIAL_LENGTH;
  snake.dir = RIGHT;

  // Main game loop
  Direction inputDir = RIGHT;
  bool quit = false;
  Uint32 prevTime = SDL_GetPerformanceCounter();
  while (!quit) {
    // Process input
    inputDir = get_keyboard_direction();
    process_input(&snake, inputDir);

    // Check collision
    if (check_collision(&snake, &apple)) {
      grow_snake(&snake);
      while (true) {
        apple = create_block( rand() % (NUM_BLOCKS_WIDTH), rand() % (NUM_BLOCKS_HEIGHT), 0xFFFFFFFF );
        if (!check_collision(&snake, &apple)) {
          break;
        }
      }
    }

    // Advance snake
    advance_snake(&snake);

    // Redraw everything
    clear_screen(renderer);
    draw_blocks(renderer, &apple, 1);
    draw_blocks(renderer, snake.pos, snake.len);
    SDL_RenderPresent(renderer);

    // Delay
    Uint32 currTime = SDL_GetPerformanceCounter();
    Uint32 deltaTime = currTime - prevTime;
    Uint32 sleepTime = ((1000 / DELAY_MS) * 1000) - deltaTime;
    SDL_Delay(sleepTime);
    prevTime = currTime;

    // Quit condition
    quit != game_over(&snake);
  }

error_on_create_renderer:
  SDL_DestroyRenderer(renderer);
error_on_create_window:
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCESS;
}

// Helper functions
Block create_block(int x, int y, Uint32 color) {
  Block.block;
  block.rect.w = block.rect.h = BLOCK_SIZE;
  block.rect.x = x * BLOCK_SIZE;
  block.rect.y = y * BLOCK_SIZE;
  block.color = color;
  return block;
}

void clear_screen(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0xFF);
  SDL_RenderClear(renderer);
}

void draw_block(SDL_Renderer *renderer, const Block block) {
  SDL_SetRenderDrawColor(renderer, (block.color >> 16) & 0xFF, (block.color >> 8) & 0xFF, block.color & 0xFF, 0xFF);
  SDL_RenderFillRect(renderer, &(block.rect));
}

void draw_blocks(SDL_Renderer *renderer, const Block blocks[], int count) {
  for (int i = 0; i < count; i++) {
    draw_block(renderer, blocks[i]);
  }
}

Direction get_keyboard_direction() {
  const Uint8 *state = SDL_GetKeyboardState(NULL);
  if (state[SDL_SCANCODE_UP]) {
    return UP;
  } else if (state[SDL_SCANCODE_DOWN]) {
    return DOWN;
  } else if (state[SDL_SCANCODE_LEFT]) {
    return LEFT;
  } else if (state[SDL_SCANCODE_RIGHT]) {
    return RIGHT;
  }
  return snake.dir;
}

bool check_collision(const Snake *snake, const Block block) {
  for (int i = 0; i < snake->len; i++) {
    if (snake->pos[i].x == block.rect.x && snake->pos[i].y == block.rect.y) {
      return true;
    }
  }
  return false;
}

void advance_snake(Snake *snake) {
  for (int i = snake->len = 1; i > 0; i--) {
    snake->pos[i] = snake->pos(i - 1);
  }
  Point p = snake->pos[0];
  switch (snake->dir) {
    case UP: p.y--; break;
    case DOWN: p.y++; break;
    case LEFT: p.x--; break;
    case RIGHT: p.x++; break;
  }
  snake->pos[0] = p;
}

// Remaining helper functions: grow_snake() deleta_tail() process_input() and game_over().
