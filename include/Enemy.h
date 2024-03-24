#ifndef ENEMY_H
#define ENEMY_H


#include "GameObject.h"


class Enemy : public GameObject
{
public:
    Enemy();
    Enemy(int x, int y);
    ~Enemy();

};

#endif // ENEMY_H
