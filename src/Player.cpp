#include "Player.h"
#include <cstdlib>
#include "constants.h"
#include <string>
#include <iostream>

Player::Player(int x, int y, int life_points, std::string name)
{
    spawnX = x;
    spawnY = y;
    this->x = x;
    this->y = y;
    this->life_points = life_points;
    this->name = name;
    relics_points = 0;
    inventory_storage = 0;

}

Player::~Player() {}

int Player::get_life_points() const
{
    return life_points;
}

int Player::get_relics_points() const
{
    return relics_points;
}


void Player::set_life(int points)
{
    life_points += points;
}

void Player::set_relics(int points)
{
    relics_points += points;
}

void Player::reset()
{
    GameObject::reset();
    relics_points = 0;
}

bool Player::has_item(std::string item) const
{
    for(int i = 0; i < this->inventory_storage; i++)
        if(this->inventory[i] == item) return true;
    return false;
}

void Player::set_item(std::string item)
{
    if(has_item(item)) return;
    if(inventory_storage < inventory_capacity)
    {
        this->inventory[this->inventory_storage] = item;
        this->inventory_storage++;
    }
}

void Player::remove_item(std::string item)
{
    if(!has_item(item)) return;
    for(int i = 0; i < this->inventory_storage; i++)
    {
        if(this->inventory[i] == item)
        {
            for(int j = i; j < this->inventory_storage - 1; j++)
                inventory[j] = inventory[j+1];
            inventory_storage--;
            return;
        }
    }
}

// Method to print the items of the player
void Player::print_items() const
{
    std::cout << this->inventory_storage << " Item(s)" << (this->inventory_storage > 0 ? ": ": "");
    for(int i = 0; i < this->inventory_storage; i++)
    {
        std::cout << inventory[i];
        if(i + 1 < this->inventory_storage)
            std::cout << " | ";
    }
    std::cout << "\n" << std::endl;
}



