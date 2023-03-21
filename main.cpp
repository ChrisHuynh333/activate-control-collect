#include <iostream>
#include "raylib.h"
#include <vector>

class Player 
{
    public:
        int height = 60;
        int width = 60;
        int pos_in_colors;
        Color color;
        int x;
        int y;
        void set_coords(int x_input, int y_input)
        {
            x = x_input;
            y = y_input;
        }
        void set_color(int position, Color &color_input)
        {
            pos_in_colors = position;
            color = color_input;
        }
};

class Game_Square: public Player
{
    public:
        void set_y_coord(int y_input)
        {
            y = y_input;
        }
        void set_x_coord(int x_input)
        {
            x = x_input;
        }
};


int get_random_color_position(const std::vector<Color> &available_colors)
{
    int number = rand() % (available_colors.size() - 2) + 2;
    return number;
}

void set_game_square_x_coords(std::vector<Game_Square> &game_squares)
{
    int configuration = rand() % (3);
    if (configuration == 0)
    {
        game_squares[0].set_x_coord(210);
        game_squares[1].set_x_coord(270);
    }
    else if (configuration == 1)
    {
        game_squares[0].set_x_coord(210);
        game_squares[1].set_x_coord(330);
    }
    else
    {
        game_squares[0].set_x_coord(270);
        game_squares[1].set_x_coord(330);
    }
}

void set_game_square_colors(std::vector<Game_Square> &game_squares, Player &player, const std::vector<Color> &colors, std::string game_square_type)
{
    if(game_square_type != "red")
    {
        int square_equal_to_player = rand() & 1;
        game_squares[square_equal_to_player].set_color(player.pos_in_colors, player.color);

        for(int i = 0; i < game_squares.size(); i++)
        {
            if (i != square_equal_to_player)
            {
                while(true)
                {
                    int color_number = get_random_color_position(colors);
                    if (color_number != player.pos_in_colors)
                    {   
                        Color color = colors[color_number];
                        game_squares[i].set_color(color_number, color);
                        break;
                    }
                }
                
            }
        }
    }
    else
    {
        Color color = RED;
        for(int i = 0; i < game_squares.size(); i++)
        {
            game_squares[i].set_color(0, color);
        }
    }
    
}

void create_starting_game_squares(std::vector<Game_Square> &game_squares, std::string game_square_type, int starting_position, const std::vector<Color> &colors, Player &player)
{
    set_game_square_x_coords(game_squares);
    set_game_square_colors(game_squares, player, colors, game_square_type);
    

    int starting_y_coord = 520 - (starting_position * 180);
    for(int i = 0; i < game_squares.size(); i++)
    {
        game_squares[i].set_y_coord(starting_y_coord);
    }

}

void initialize_game(const std::vector<Color> &available_colors, std::vector<std::vector<Game_Square>> &squares, Player &player)
{
    player.set_coords(270, 680);
    int vector_position = get_random_color_position(available_colors);
    Color color = available_colors[vector_position];
    player.set_color(vector_position, color);

    for(int i = 0; i < 3; i++)
    {
        Game_Square square_one;
        Game_Square square_two;
        std::vector<Game_Square> game_squares = {square_one, square_two};
        squares.push_back(game_squares);
        std::string game_square_type;
        if (i == 0 || i == 2)
        {
            game_square_type = "colors";
            
        }
        else
        {
            game_square_type = "red";
        }
        create_starting_game_squares(squares[i], game_square_type, i, available_colors, player);
    }
}




int main()
{
    srand (time(NULL));
    const std::vector<Color> colors {RED, WHITE, BLUE, GREEN, PURPLE, YELLOW, ORANGE};
    std::vector<std::vector<Game_Square>> game_squares;
    Player player;
    int windowHeight = 800;
    int windowWidth = 600;

    InitWindow(windowWidth, windowHeight, "Collect");
    initialize_game(colors, game_squares, player);
    SetTargetFPS(20);
    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);
        DrawRectangle(player.x, player.y, player.width, player.height, player.color);
        DrawLine(210, 100, 210, 800, BLACK);
        DrawLine(390, 100, 390, 800, BLACK);
        DrawLine(210, 100, 390, 100, BLACK);
        for(int i = 0; i < game_squares.size(); i++)
        {
            for(int j = 0; j < game_squares[i].size(); j++)
            {
                DrawRectangle(game_squares[i][j].x, game_squares[i][j].y, game_squares[i][j].width, game_squares[i][j].height, game_squares[i][j].color);
            }
        }
        if(IsKeyDown(KEY_LEFT))
        {
            if (player.x == 270)
            {
                player.x = 210;
            }
            else if (player.x == 330)
            {
                player.x = 270;
            }
        }
        if(IsKeyDown(KEY_RIGHT))
        {
            if (player.x == 210)
            {
                player.x = 270;
            }
            else if (player.x == 270)
            {
                player.x = 330;
            }
        }
        EndDrawing();
    }
}