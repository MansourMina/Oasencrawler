#include "GameObject.h"
#include "constants.h"

void GameObject::undo_position()
{
    x = lastXPos;
    y = lastYPos;
}

void GameObject::move_right()
{
    x++;
}
void GameObject::move_left()
{
    x--;
}
void GameObject::move_up()
{
    y--;
}
void GameObject::move_down()
{
    y++;
}

void GameObject::set_last_x_pos()
{
    lastXPos = x;
}
void GameObject::set_last_y_pos()
{
    lastYPos = y;
}

int GameObject::get_x_position() const
{

    return x;
}

int GameObject::get_last_x_pos() const
{
    return lastXPos;
}

int GameObject::get_last_y_pos() const
{
    return lastYPos;
}

int GameObject::get_y_position() const
{
    return y;
}

bool GameObject::is_out_of_bounce()
{
    if(x < 0 || x > DIMENSION_X -1 || y < 0 || y > DIMENSION_Y -1)
    {
        undo_position();
        return true;
    }
    return false;
}

bool GameObject::interact(GameObject* gameObject) const
{
    return (this->x == gameObject->x && this->y == gameObject->y);
}

void GameObject::reset()
{
    x = spawnX;
    y = spawnY;
}

void GameObject::set_position(int x, int y)
{
    this->x = x;
    this->y = y;
}
