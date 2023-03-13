#include <iostream>
#include "raylib.h"
#include <vector>

class Player {
    public:
        const int height = 60;
        const int width = 60;
        int pos_in_colors;
        std::string color_str;
        int x;
        int y;
        void set_coords(int x_input, int y_input)
        {
            x = x_input;
            y = y_input;
        }
        void set_color(int color_input, std::string &color_str_input)
        {
            pos_in_colors = color_input;
            color_str = color_str_input;
        }
};

class Game_Square: public Player{};

void initialize_game(const std::vector<Color> &available_colors, std::vector<std::vector<Game_Square>> &squares, Player &player)
{
    player.set_coords(60, 1080);

}

void select_color(const std::vector<Color> &available_colors, Game_Square &square)
{}


int main()
{
    const std::vector<Color> colors {WHITE, BLUE, GREEN, PURPLE, YELLOW, ORANGE, RED};
    std::vector<std::vector<Game_Square>> game_squares;
    Player player;
    int windowHeight = 1200;
    int windowWidth = 180;

    InitWindow(windowWidth, windowHeight, "Collect");
    initialize_game(colors, game_squares, player);
    while(!WindowShouldClose())
    {

    }
}