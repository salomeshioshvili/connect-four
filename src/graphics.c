#include "graphics.h"
#include <stdio.h>
#include <math.h>

static const SDL_Color COLOR_BACKGROUND = {40, 44, 52, 255};
static const SDL_Color COLOR_BOARD = {33, 100, 209, 255};
static const SDL_Color COLOR_EMPTY = {25, 30, 40, 255};
static const SDL_Color COLOR_PLAYER1 = {220, 50, 50, 255};   // Red
static const SDL_Color COLOR_PLAYER2 = {255, 220, 50, 255};  // Yellow
static const SDL_Color COLOR_HOVER = {80, 90, 110, 255};
static const SDL_Color COLOR_TEXT_BG = {50, 55, 65, 255};
static const SDL_Color COLOR_WHITE = {255, 255, 255, 255};

int graphics_init(Graphics *gfx, const char *title) {
    if (!gfx) return -1;
    
    gfx->window = NULL;
    gfx->renderer = NULL;
    gfx->running = 1;
    gfx->selected_column = -1;
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return -1;
    }
    
    gfx->window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    
    if (!gfx->window) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }
    
    gfx->renderer = SDL_CreateRenderer(gfx->window, -1, 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!gfx->renderer) {
        fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(gfx->window);
        SDL_Quit();
        return -1;
    }
    
    return 0;
}

void graphics_cleanup(Graphics *gfx) {
    if (!gfx) return;
    
    if (gfx->renderer) {
        SDL_DestroyRenderer(gfx->renderer);
        gfx->renderer = NULL;
    }
    
    if (gfx->window) {
        SDL_DestroyWindow(gfx->window);
        gfx->window = NULL;
    }
    
    SDL_Quit();
}

static void set_render_color(SDL_Renderer *renderer, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

static void draw_filled_circle(SDL_Renderer *renderer, int cx, int cy, int radius) {
    for (int y = -radius; y <= radius; y++) {
        int half_width = (int)sqrt((double)(radius * radius - y * y));
        SDL_RenderDrawLine(renderer, cx - half_width, cy + y, cx + half_width, cy + y);
    }
}

static void draw_circle_outline(SDL_Renderer *renderer, int cx, int cy, int radius, int thickness) {
    for (int t = 0; t < thickness; t++) {
        int r = radius - t;
        for (int angle = 0; angle < 360; angle++) {
            double rad = angle * M_PI / 180.0;
            int x = cx + (int)(r * cos(rad));
            int y = cy + (int)(r * sin(rad));
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
}

// Simple game over overlay - just a banner with winner's color
static void draw_game_over_overlay(SDL_Renderer *renderer, CellState winner, int is_draw) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    // Dark overlay bar
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_Rect overlay = {0, WINDOW_HEIGHT / 2 - 40, WINDOW_WIDTH, 80};
    SDL_RenderFillRect(renderer, &overlay);
    
    // Winner circle in center
    int cx = WINDOW_WIDTH / 2;
    int cy = WINDOW_HEIGHT / 2;
    
    if (is_draw) {
        // Show both colors for draw
        set_render_color(renderer, COLOR_PLAYER1);
        draw_filled_circle(renderer, cx - 25, cy, 25);
        set_render_color(renderer, COLOR_PLAYER2);
        draw_filled_circle(renderer, cx + 25, cy, 25);
    } else if (winner == PLAYER1) {
        set_render_color(renderer, COLOR_PLAYER1);
        draw_filled_circle(renderer, cx, cy, 35);
    } else if (winner == PLAYER2) {
        set_render_color(renderer, COLOR_PLAYER2);
        draw_filled_circle(renderer, cx, cy, 35);
    }
}

int graphics_get_column_from_x(int mouse_x) {
    int x = mouse_x - BOARD_PADDING;
    if (x < 0 || x >= COLS * CELL_SIZE) {
        return -1;
    }
    return x / CELL_SIZE;
}

void graphics_render(Graphics *gfx, const Board *board, CellState current_player,
                     int game_over, CellState winner, int is_draw) {
    if (!gfx || !gfx->renderer || !board) return;
    
    set_render_color(gfx->renderer, COLOR_BACKGROUND);
    SDL_RenderClear(gfx->renderer);
    
    // Highlight selected columnx
    for (int col = 0; col < COLS; col++) {
        if (!game_over && col == gfx->selected_column) {
            set_render_color(gfx->renderer, COLOR_HOVER);
            SDL_Rect highlight = {
                BOARD_PADDING + col * CELL_SIZE + 2,
                BOARD_PADDING - 5,
                CELL_SIZE - 4,
                ROWS * CELL_SIZE + 10
            };
            SDL_RenderFillRect(gfx->renderer, &highlight);
        }
    }
    
    // Hover piece at top
    if (!game_over && gfx->selected_column >= 0 && gfx->selected_column < COLS) {
        int cx = BOARD_PADDING + gfx->selected_column * CELL_SIZE + CELL_SIZE / 2;
        int cy = BOARD_PADDING / 2 + 5;
        int radius = CELL_SIZE / 2 - 10;
        
        if (current_player == PLAYER1) {
            set_render_color(gfx->renderer, COLOR_PLAYER1);
        } else {
            set_render_color(gfx->renderer, COLOR_PLAYER2);
        }
        draw_filled_circle(gfx->renderer, cx, cy, radius);
    }
    
    // Board background
    SDL_Rect board_rect = {
        BOARD_PADDING - 8,
        BOARD_PADDING - 8,
        COLS * CELL_SIZE + 16,
        ROWS * CELL_SIZE + 16
    };
    set_render_color(gfx->renderer, COLOR_BOARD);
    SDL_RenderFillRect(gfx->renderer, &board_rect);
    
    // Cells
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            int cx = BOARD_PADDING + col * CELL_SIZE + CELL_SIZE / 2;
            int cy = BOARD_PADDING + row * CELL_SIZE + CELL_SIZE / 2;
            int radius = CELL_SIZE / 2 - 6;
            
            CellState cell = board->cells[row][col];
            
            if (cell == EMPTY) {
                set_render_color(gfx->renderer, COLOR_EMPTY);
            } else if (cell == PLAYER1) {
                set_render_color(gfx->renderer, COLOR_PLAYER1);
            } else {
                set_render_color(gfx->renderer, COLOR_PLAYER2);
            }
            
            draw_filled_circle(gfx->renderer, cx, cy, radius);
        }
    }
    
    // Status bar
    SDL_Rect status_rect = {
        BOARD_PADDING - 8,
        WINDOW_HEIGHT - 60,
        COLS * CELL_SIZE + 16,
        50
    };
    set_render_color(gfx->renderer, COLOR_TEXT_BG);
    SDL_RenderFillRect(gfx->renderer, &status_rect);
    
    // Turn indicator
    int indicator_x = BOARD_PADDING + 15;
    int indicator_y = WINDOW_HEIGHT - 35;
    if (current_player == PLAYER1) {
        set_render_color(gfx->renderer, COLOR_PLAYER1);
    } else {
        set_render_color(gfx->renderer, COLOR_PLAYER2);
    }
    draw_filled_circle(gfx->renderer, indicator_x, indicator_y, 15);
    
    // Game over overlay
    if (game_over) {
        draw_game_over_overlay(gfx->renderer, winner, is_draw);
    }
    
    SDL_RenderPresent(gfx->renderer);
}

void graphics_handle_events(Graphics *gfx, int *col_out, int *quit_out, int *undo_out) {
    if (!gfx) return;
    
    *col_out = -1;
    *quit_out = 0;
    *undo_out = 0;
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                gfx->running = 0;
                *quit_out = 1;
                break;
                
            case SDL_MOUSEMOTION:
                gfx->selected_column = graphics_get_column_from_x(event.motion.x);
                break;
                
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int col = graphics_get_column_from_x(event.button.x);
                    if (col >= 0 && col < COLS) {
                        *col_out = col;
                    }
                }
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                    case SDLK_q:
                        gfx->running = 0;
                        *quit_out = 1;
                        break;
                    case SDLK_u:
                    case SDLK_z:
                        *undo_out = 1;
                        break;
                    case SDLK_1:
                    case SDLK_2:
                    case SDLK_3:
                    case SDLK_4:
                    case SDLK_5:
                    case SDLK_6:
                    case SDLK_7:
                        *col_out = event.key.keysym.sym - SDLK_1;
                        break;
                }
                break;
        }
    }
}

int graphics_wait_for_restart(Graphics *gfx) {
    if (!gfx) return 0;
    
    SDL_Event event;
    while (gfx->running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    gfx->running = 0;
                    return 0;
                    
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                        case SDLK_q:
                        case SDLK_n:
                            return 0;
                        case SDLK_RETURN:
                        case SDLK_SPACE:
                        case SDLK_y:
                            return 1;
                    }
                    break;
                    
                case SDL_MOUSEBUTTONDOWN:
                    return 1;
            }
        }
        SDL_Delay(16);
    }
    return 0;
}