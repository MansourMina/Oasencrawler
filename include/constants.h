#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED

const int DIMENSION_X = 10;
const int DIMENSION_Y = 10;

enum GAMESTATUS
{
    RUNNING,
    WON,
    LOST,
    LEVEL_UP
};

enum FIELD_TYPE
{
    FOUNTAIN_FIELD = '+',
    DANGER_FIELD = '~',
    RELIC_FIELD = 'R',
    EMPTY_FIELD = '-',
    PLAYER_FIELD = 'X',
    ENEMY_FIELD = '!'
};


#endif // CONSTANTS_H_INCLUDED
