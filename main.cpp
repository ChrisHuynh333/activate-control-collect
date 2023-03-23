#include <iostream>
#include "raylib.h"
#include <vector>
#include <chrono>
#include <thread>

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

class Lives
{
    public:
        Color color;
        std::string color_str;
        int x;
        void set_color(Color &color_input, std::string &color_str_input, int x_input)
        {
            color = color_input;
            color_str = color_str_input;
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
    

    int starting_y_coord = 500 - (starting_position * 180);
    for(int i = 0; i < game_squares.size(); i++)
    {
        game_squares[i].set_y_coord(starting_y_coord);
    }

}

void initialize_game(const std::vector<Color> &available_colors, std::vector<std::vector<Game_Square>> &squares, Player &player, std::vector<Lives> &player_lives)
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
    int lives_x_coord = 305;
    for(int i = 0; i < player_lives.size(); i++)
    {
        Lives life;
        Color color = GREEN;
        std::string color_str = "green";
        life.set_color(color, color_str, lives_x_coord);
        player_lives[i] = life;
        lives_x_coord += 25;
    }
}

void move_game_squares(std::vector<std::vector<Game_Square>> &game_squares, const std::vector<Color> &available_colors, Player &player, std::string &color_tracker)
{
    for(int i = 0; i < game_squares.size(); i++)
    {
        for(int j = 0; j < game_squares[i].size(); j++)
        {
            if (game_squares[i][j].height == 30)
            {
                game_squares[i][j].height = 60;
            }
            game_squares[i][j].set_y_coord(game_squares[i][j].y + 3);

            
        }
    }
    if (game_squares[0][1].y >= 800)
    {
        game_squares.erase(game_squares.begin());
    }

    if (game_squares.back()[1].y >= 260)
    {
        Game_Square new_game_square_one;
        Game_Square new_game_square_two;
        std::vector<Game_Square> new_game_squares = {new_game_square_one, new_game_square_two};
        set_game_square_x_coords(new_game_squares);
        set_game_square_colors(new_game_squares, player, available_colors, color_tracker);
        if (color_tracker == "red")
        {
            color_tracker = "not red";
        }
        else
        {
            color_tracker = "red";
        }
        for (int i = 0; i < new_game_squares.size(); i++)
        {
            new_game_squares[i].height = 30;
            new_game_squares[i].set_y_coord(80);
        }
        game_squares.push_back(new_game_squares);
    }
}

int main()
{
    srand (time(NULL));
    const std::vector<Color> colors {RED, WHITE, BLUE, GREEN, PURPLE, ORANGE};
    std::vector<std::vector<Game_Square>> game_squares;
    Player player;
    auto start_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_difference;
    double time_tracker = 0.5;
    int seconds_tracker = 1;
    double score_time_tracker = 0;
    double score_increase_cooldown = 0;
    double life_loss_cooldown = 0;
    std::string color_tracker = "red";
    int minutes = 2;
    int seconds = 0;
    std::vector<Lives> lives(5);
    int remaining_squares = 50;
    int score = 1000;
    bool game_over = false;
    int windowHeight = 800;
    int windowWidth = 600;

    InitWindow(windowWidth, windowHeight, "Collect");
    initialize_game(colors, game_squares, player, lives);
    SetTargetFPS(240);
    while(!WindowShouldClose() && !game_over)
    {
        BeginDrawing();
        ClearBackground(WHITE);

        DrawRectangle(player.x, player.y, player.width, player.height, player.color);
        DrawText("Time", 50, 15, 25, BLACK);
        DrawText(TextFormat("%02d:%02d", minutes, seconds), 40, 40, 35, BLACK);
        DrawText("Score", 175, 15, 25, BLACK);
        DrawText(TextFormat("%d", score), 180, 40, 35, BLACK);
        DrawText("Lives", 325, 15, 25, BLACK);
        
        for(int i = 0; i < lives.size(); i++)
        {
            DrawCircle(lives[i].x, 55, 10, lives[i].color);
        }
        DrawText("Remaining", 450, 15, 25, BLACK);
        DrawText(TextFormat("%d", remaining_squares), 485, 40, 35, BLACK);
        DrawLine(210, 80, 210, 800, BLACK);
        DrawLine(390, 80, 390, 800, BLACK);
        DrawLine(210, 80, 390, 80, BLACK);
        for(int i = 0; i < game_squares.size(); i++)
        {
            for(int j = 0; j < game_squares[i].size(); j++)
            {
                DrawRectangle(game_squares[i][j].x, game_squares[i][j].y, game_squares[i][j].width, game_squares[i][j].height, game_squares[i][j].color);
            }
        }

        if(lives[0].color_str == "red")
        {
            DrawText("Game Over! Out Of Lives", 100, 350, 35, BLACK);
            game_over = true;
        }
        if(seconds == 0 && minutes == 0)
        {
            DrawText("Game Over! Out Of Time", 100, 350, 35, BLACK);
            game_over = true;
        }
        if(remaining_squares == 0)
        {
            int lives_counter = 0;
            for(int i = 0; i < 5; i++)
                {
                    if(lives[i].color_str == "green")
                    {
                        lives_counter ++;
                    }
                    else
                    {
                        break;
                    }
                }
                int lives_points = lives_counter * 50;
                DrawText("Congratulations, you won!", 150, 325, 25, BLACK);
                DrawText(TextFormat("Your score is %d plus %d more from lives.", score, lives_points), 50, 350, 25, BLACK);
                game_over = true;
        }


        auto end = std::chrono::high_resolution_clock::now();
        time_difference = end - start_time;
        if (time_difference.count() > time_tracker)
        {
            time_tracker += 0.01;
            move_game_squares(game_squares, colors, player, color_tracker);
            if (time_tracker > seconds_tracker)
            {
                seconds_tracker += 1;
                if (seconds == 0) 
                {
                    if (minutes > 0)
                    {
                        seconds = 59;
                        minutes -= 1;
                    }
                }
                else
                {
                    seconds -= 1;
                }
            }
        }

        if (time_difference.count() > score_time_tracker && score > 0)
        {
            score_time_tracker += 0.12;
            score -= 1;
        }

        for (int i = 0; i < game_squares.size(); i++)
        {
            for (int j = 0; j < game_squares[i].size(); j++)
            {
                if(game_squares[i][j].x == player.x && player.y >= game_squares[i][j].y && player.y <= game_squares[i][j].y + 60)
                {
                    if(game_squares[i][j].pos_in_colors == 0 && life_loss_cooldown + 0.35 < time_tracker)
                    {
                        for (int k = (lives.size() - 1); k > -1; k--)
                        {
                            if (lives[k].color_str == "green")
                            {
                                std::cout << k << "\n";
                                Color color = RED;
                                std::string color_str = "red";
                                lives[k].set_color(color, color_str, lives[k].x);
                                break;
                            }
                        }
                        life_loss_cooldown = time_tracker;
                    }
                    else if (game_squares[i][j].pos_in_colors == player.pos_in_colors && score_increase_cooldown + 0.35 < time_tracker)
                    {
                        remaining_squares -= 1;
                        score_increase_cooldown = time_tracker;
                    }
                }
            }
        }

        if(IsKeyDown(KEY_LEFT))
        {
            player.x = 210;
        }
        else if (IsKeyDown(KEY_RIGHT))
        {
            player.x = 330;
        }
        else
        {
            player.x = 270;
        }
        EndDrawing();
        if(IsKeyPressed(KEY_ESCAPE))
        {
            CloseWindow();
            break;
        }
    }
    if(game_over)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        CloseWindow();
    }
}