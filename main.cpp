#include <iostream>
#include <cstdlib>
#include "Player.h"
#include "Enemy.h"
#include <time.h>
#include <cmath>
#include <limits>
#include <thread>
#include "constants.h"

using namespace std;

typedef struct Level
{
    int LEVEL = 1;
    double EMPTY_FIELDS_PROBABILITY = 0.4;
    double DANGER_FIELDS_PROBABILITY = 0.4;
    double FOUNTAIN_PROBABILITY = 0.1;
    double RELICS_PROBABILITY = 0.1;
    int ENEMIES= DIMENSION_X * DIMENSION_Y / 100;
    int ENEMY_DAMAGE = 1;
};
Level level;

typedef struct ItemProbability
{
    double PATHFINDER_PROBABILITY = 5;
    double NUMBER_WIZARD_PROBABILITY = 3;
    double HINT_HELPER_PROBABILITY = 2;
};
ItemProbability itemProbability;

typedef struct
{
    char coordinates[DIMENSION_X][DIMENSION_Y];
    int relicsCount = 0;
    int dangerCount = 0;
    int fountainsCount = 0;
} World;


enum PLAYER_CONTROL
{
    UP = 'w',
    DOWN = 's',
    LEFT = 'a',
    RIGHT = 'd'
};

World create_world(Player* player, Enemy* enemies[])
{
    World newWorld;
    bool relicsGenerated = false;
    int player_x = player->get_x_position(),player_y = player->get_y_position();

    // Set enemies on fields
    for(int e= 0; e < level.ENEMIES; e++)
    {
        Enemy* enemy = enemies[e];
        int enemy_x = enemy->get_x_position(),enemy_y = enemy->get_y_position();
        newWorld.coordinates[enemy_x][enemy_y] = ENEMY_FIELD;
    }

    // Set random fields
    for (int y = 0; y < DIMENSION_Y; y++)
    {
        for (int x = 0; x < DIMENSION_X; x++)
        {
            if((x != player_x || y != player_y) && (newWorld.coordinates[x][y] != ENEMY_FIELD))
            {
                // Random double number between 0 and 1
                double probability = (double)rand() / RAND_MAX;
                if (probability < level.RELICS_PROBABILITY || !relicsGenerated)
                {
                    newWorld.coordinates[x][y] = RELIC_FIELD;
                    relicsGenerated = true;
                    newWorld.relicsCount++;
                }
                else if (probability < level.RELICS_PROBABILITY + level.FOUNTAIN_PROBABILITY)
                {
                    newWorld.coordinates[x][y] = FOUNTAIN_FIELD;
                    newWorld.fountainsCount++;
                }
                else if (probability < level.RELICS_PROBABILITY + level.FOUNTAIN_PROBABILITY + level.DANGER_FIELDS_PROBABILITY)
                {
                    newWorld.coordinates[x][y] = DANGER_FIELD;
                    newWorld.dangerCount++;
                }
                else newWorld.coordinates[x][y] = EMPTY_FIELD;
            }
            else newWorld.coordinates[x][y] = EMPTY_FIELD;
        }
    }
    return newWorld;
}

void draw_world(char world[DIMENSION_X][DIMENSION_Y], Player* player, Enemy* enemies[])
{
    char tempMatrix[DIMENSION_X][DIMENSION_Y];

    // Set all fields
    for (int y = 0; y < DIMENSION_Y; y++)
    {
        for (int x = 0; x < DIMENSION_X; x++)
            tempMatrix[x][y] = world[x][y];
    }

    //Set player
    tempMatrix[player->get_x_position()][player->get_y_position()] = PLAYER_FIELD;

    //Set enemies
    for (int e = 0; e < level.ENEMIES; e++)
        tempMatrix[enemies[e]->get_x_position()][enemies[e]->get_y_position()] = ENEMY_FIELD;

    // Draw World
    for (int y = 0; y < DIMENSION_Y; y++)
    {
        for (int x = 0; x < DIMENSION_X; x++)
            cout <<  tempMatrix[x][y] << "  ";
        cout << '\n';
    }
    cout << endl;
}

bool enemy_collision(GameObject* object, Enemy* enemies[])
{
    // Check if an object is colliding with an enemy
    for(int e = 0; e < level.ENEMIES; e++)
    {
        if(object == enemies[e]) continue;
        if(object->interact(enemies[e]))
            return true;
    }
    return false;
}


bool player_input(Player * player)
{
    char input;
    cout << "w(up) s(down) a(left) d(right): ";
    cin >> input;

    // handle player movement
    switch(tolower(input))
    {
    case UP:
        player->move_up();
        break;
    case DOWN:
        player->move_down();
        break;
    case LEFT:
        player->move_left();
        break;
    case RIGHT:
        player->move_right();
        break;
    default:
        return false; // If input is not a valid direction, return false
    }
    return true; // If a valid direction is inputted and movement is executed, return true
}

void intelligence_challenge(bool &passed, bool has_item, string item)
{
    // Generate two random numbers between 1 and 1000
    int num1 = rand() % 1000 + 1;
    int num2 = rand() % 1000 + 1;

    cout << "Your intelligence is being questioned" << endl;
    cout << num1 << " + " << num2;

    // If the player has an item, use the item
    if(has_item)
    {
        cout << "\n\n" << item << ": I will modify it for you :)!" << endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        num1 = rand() % 10 + 1;
        num2 = rand() % 10 + 1;
        cout << num1 << " + " << num2;
    }
    cout << " = ";

    int result;
    if (!(cin >> result))
    {
        // If the input is not an integer
        cin.clear(); // Clear the error flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard the input buffer
    }
    else passed = num1 + num2 == result;
}

void analytical_challenge( bool &passed, bool has_item, string item)
{
    // Generate a random number between 1 and 10.
    int number = rand() % 10 + 1;
    int attempts_allowed = 3;
    int attempts = 0;
    if(has_item) cout << item << ": I will help you!\n" << endl;
    cout << "Your analytical thinking is being questioned" << endl;
    cout << "Guess the number from 1 to 10, you have " << attempts_allowed << " attempts." << endl;

    // Loop until the player runs out of attempts
    while(attempts < attempts_allowed)
    {
        attempts++;
        cout << "Guess: ";
        int guess;
        if (!(cin >> guess))
        {
            // If the input is not an integer
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard the input buffer
            continue;
        }
        if(guess == number)
        {
            passed = true;
            return;
        }
        // If the player has the item, provide additional guidance
        if(has_item)
        {
            cout << item << ": ";
            if(guess < number) cout << "Higher..." << endl;
            else if(guess > number) cout << "Lower..." << endl;
        }
    }

}

void find_fastest_way(Player* player, int star_x, int star_y, World world, Enemy* enemies[])
{
    // Difference in x and y coordinates between the player and the star
    int dx = player->get_x_position() - star_x;
    int dy = player->get_y_position() - star_y;

    // Update the world grid to show which direction to move
    if (abs(dx) > abs(dy))
    {
        if (dx < 0) world.coordinates[player->get_x_position() + 1][player->get_y_position()] = '>';
        else world.coordinates[player->get_x_position() - 1][player->get_y_position()] = '<';
    }
    else
    {
        if (dy < 0) world.coordinates[player->get_x_position()][player->get_y_position() + 1] = 'v';
        else world.coordinates[player->get_x_position()][player->get_y_position() - 1] = '^';
    }

    // Check if player is next to the star
    dx = abs(player->get_x_position() - star_x);
    dy = abs(player->get_y_position() - star_y);
    if(dx > 1 || dy > 1 || (dx == 1 && dy == 1))
    {
        system("cls");
        draw_world(world.coordinates, player, enemies);
    }
}

void quickness_challenge(bool &passed, World& world, Player* player, Enemy* enemies[], bool has_item, string item)
{
    int attempts_allowed = 10;
    int attempts= 0;

    // Set '*' at a random position on the world grid
    int random_x = rand() % (DIMENSION_X - 1);
    int random_y = rand() % (DIMENSION_Y - 1);
    char previous_field = world.coordinates[random_x][random_y];
    world.coordinates[random_x][random_y] = '*';

    player->set_last_x_pos(), player->set_last_y_pos();

    // Loop until the player finds the '*' or runs out of attempts
    while(attempts < attempts_allowed)
    {
        attempts++;
        system("cls");
        draw_world(world.coordinates, player, enemies);

        // Provide guidance if the player has the item
        if(has_item)
        {
            find_fastest_way(player, random_x, random_y, world, enemies);
            cout << item << ": Follow me!\n" << endl;
        }

        cout << "Your quickness is being questioned" << endl;
        cout << "Find the * within " << attempts_allowed << " moves." << endl;
        player_input(player);

        if(player->is_out_of_bounce())
            continue;

        if(player->get_x_position() == random_x && player->get_y_position() == random_y)
        {
            passed = true;
            break;
        }

    }

    // Restore the previous field type at the position where '*' was placed
    world.coordinates[random_x][random_y] = previous_field;
    player->undo_position();
}

bool challenge_player(Player* player, World &world, Enemy* enemies[])
{
    int randomAttribute = rand()%3;
    bool passed = false;
    system("cls");
    draw_world(world.coordinates, player, enemies);

    // Challenge the player based on a random attribute
    switch(randomAttribute)
    {
    case player->INTELLIGENCE:
    {
        string item = "Number Wizard";
        bool has_item = player->has_item(item);
        intelligence_challenge(passed, has_item, item);
        if(has_item) player->remove_item(item);
        break;
    }
    case player->ANALYTICAL:
    {
        string item = "Hint Helper";
        bool has_item = player->has_item(item);
        analytical_challenge(passed,has_item, item);
        if(has_item) player->remove_item(item);
        break;
    }
    case player->QUICKNESS:
    {
        string item= "Pathfinder";
        bool has_item = player->has_item(item);
        quickness_challenge(passed, world, player, enemies, has_item, item);
        if(has_item) player->remove_item(item);
        break;
    }
    default:
        // Injury the player with a 1/6 probability
        double probabilityInjured = (double)rand() / RAND_MAX;
        if(probabilityInjured < (1.0/6.0) )
            player->set_life(-1);
    }
    return passed;
}


// Randomly give an item to the player
void get_random_item(Player* player)
{
    srand(time(NULL));
    int probability = rand() % 10;
    if (probability < itemProbability.HINT_HELPER_PROBABILITY)
        player->set_item("Hint Helper");
    else if(probability < itemProbability.NUMBER_WIZARD_PROBABILITY + itemProbability.HINT_HELPER_PROBABILITY)
        player->set_item("Number Wizard");
    else player->set_item("Pathfinder");
}

void collision(World &world, Player* player, Enemy* enemies[])
{
    // Collision with enemy
    for(int e= 0; e < level.ENEMIES; e++ )
    {
        Enemy* enemy = enemies[e];
        if(enemy->interact(player))
        {
            player->set_life(-level.ENEMY_DAMAGE);
            enemies[e]->reset();
        }
    }

    int player_currXPos = player->get_x_position(),player_currYPos = player->get_y_position();
    char currentField = world.coordinates[player_currXPos][player_currYPos];

    // Collision with game fields
    switch(currentField)
    {
    case DANGER_FIELD:
    {
        if(!challenge_player(player, world, enemies))
        {
            player->set_life(-1);
            cout << "\nUh-oh! You've lost a life." << endl;
        }
        else cout << "\nCongrats, you passed!" << endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        break;
    }
    case FOUNTAIN_FIELD:
    {
        get_random_item(player);
        player->set_life(1);
        break;
    }
    case RELIC_FIELD:
    {
        get_random_item(player);
        player->set_relics(1);
        break;
    }
    default:
        break;
    }
    world.coordinates[player_currXPos][player_currYPos] = EMPTY_FIELD;
}

void initialize_game_status(World world, Player* player, GAMESTATUS &gameStatus)
{
    if(player->get_life_points() == 0)
        gameStatus = LOST;

    else if(world.relicsCount == player->get_relics_points())
        gameStatus = LEVEL_UP;
}

void enemies_movement(char world[DIMENSION_X][DIMENSION_Y], Player* player, Enemy* enemies[])
{
    for(int e= 0; e < level.ENEMIES; e++ )
    {
        Enemy* enemy = enemies[e];
        // Calculate the difference in x and y coordinates between the player and the enemy
        int dx = player->get_x_position() - enemy->get_x_position();
        int dy = player->get_y_position() - enemy->get_y_position();

        // Move the enemy towards the player while avoiding collisions.
        do
        {
            if (abs(dx) > abs(dy))
                dx > 0 ? enemy->move_right():enemy->move_left();

            else
                dy > 0 ? enemy->move_down():enemy->move_up();

            if(enemy_collision(enemy, enemies))
            {
                enemy->undo_position();
                continue;
            }
        }
        while(enemy->is_out_of_bounce());
    }
}

void print_information(World world, Player* player)
{
    cout << "----LEVEL: " << level.LEVEL << "----" << endl;
    cout << "\nPlayer: " << player->name << endl;
    cout << "\nLife: " << player->get_life_points() << " | Relics: " << player->get_relics_points() << "\n" << endl;
    player->print_items();
}

void create_enemies(Enemy* enemies[])
{
    for(int e = 0; e < level.ENEMIES; e++)
    {
        // Get random coordinates for enemy
        int random_x = level.ENEMIES > 1 ? rand() % (DIMENSION_X - 1):DIMENSION_X - 1;
        int random_y = level.ENEMIES > 1 ? rand() % (DIMENSION_Y - 1): DIMENSION_Y - 1;
        enemies[e] = new Enemy(random_x, random_y);
    }
}

void delete_enemies(Enemy* enemies[])
{
    for(int e = 0; e < level.ENEMIES; e++)
        delete enemies[e];
    delete[] enemies;
}


void next_level(Player* player, World &world, Enemy* enemies[], GAMESTATUS &gameStatus)
{
    delete_enemies(enemies);
    gameStatus = RUNNING;
    level.LEVEL++;
    level.ENEMIES++;
    level.ENEMY_DAMAGE++;
    level.FOUNTAIN_PROBABILITY -= 0.02;
    level.RELICS_PROBABILITY += 0.05;
    player->reset();
    enemies = new Enemy*[level.ENEMIES];
    create_enemies(enemies);
    world = create_world(player, enemies);
    cout << "\nLevel " << level.LEVEL << "..." << endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));

}

void set_last_positions(Player* player, Enemy* enemies[])
{
    player->set_last_x_pos();
    player->set_last_y_pos();
    for(int e = 0; e < level.ENEMIES; e++)
    {
        enemies[e]->set_last_x_pos();
        enemies[e]->set_last_y_pos();
    }

}

void update_game(World &world, Player* player, GAMESTATUS &gameStatus, Enemy* enemies[], bool &enemyMoved)
{
    collision(world, player, enemies);
    initialize_game_status(world, player, gameStatus);
    // Allow enemy to move every two player steps
    enemyMoved = !enemyMoved;
    if(gameStatus == LEVEL_UP)
        next_level(player, world, enemies, gameStatus);
    system("cls");
}


void oasencrawler()
{
    // Initialize random seed
    srand(time(NULL));
    Player* player = new Player(0,0, 10, "Mina");
    Enemy** enemies = new Enemy*[level.ENEMIES];
    create_enemies(enemies);
    World world = create_world(player, enemies);
    GAMESTATUS gameStatus = RUNNING;

    bool enemyMoved = true;

    while(gameStatus == RUNNING || gameStatus == LEVEL_UP)
    {
        system("cls");
        srand(time(NULL));
        set_last_positions(player, enemies);
        print_information(world, player);
        draw_world(world.coordinates, player, enemies);
        bool valid_input = player_input(player);
        // Input validation
        if(!valid_input || player->is_out_of_bounce()) continue;
        // Move enemies if it's their turn.
        if(!enemyMoved) enemies_movement(world.coordinates, player,enemies);
        update_game(world, player, gameStatus, enemies, enemyMoved);
    }

    if(gameStatus == LOST)
    {
        draw_world(world.coordinates, player, enemies);
        cout << "You LOST!" << endl;
        cout << "\nMax Level reached: " << level.LEVEL << endl;
    }

    // Clean up memory
    delete_enemies(enemies);
    delete player;
}

int main()
{
    oasencrawler();
    return 0;
}
