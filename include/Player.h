#ifndef PLAYER_H
#define PLAYER_H
#include "constants.h"
#include "GameObject.h"
#include <string>
#include "Attribute.h"

class Player : public GameObject
{
public:

    Player(int x = 0, int y = 0, int life_points = 5, std::string name = "Unknown");
    ~Player();
    int get_life_points() const;
    int get_relics_points() const;
    std::string name;
    void set_life(int points);
    void set_relics(int points);
    void reset();
    enum Attribute
    {
        INTELLIGENCE,
        ANALYTICAL,
        QUICKNESS
    };
    std::string inventory[3];
    bool has_item(std::string item) const;
    void set_item(std::string item);
    void remove_item(std::string item);
    void print_items() const;
private:
    int life_points;
    int relics_points;
    int inventory_storage;
    int inventory_capacity = 3;
};

#endif // PLAYER_H
