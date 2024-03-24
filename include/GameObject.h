#ifndef GameObject_H
#define GameObject_H


class GameObject
{
public:

    void move_right();
    void move_left();
    void move_up();
    void move_down();
    int get_x_position() const;
    int get_last_x_pos() const;
    int get_y_position() const;
    int get_last_y_pos() const;
    bool is_out_of_bounce();
    void set_last_x_pos();
    void set_last_y_pos();
    bool interact(GameObject* gameObject) const;
    void undo_position();
    void reset();
    void set_position(int x, int y);
protected:
    int x;
    int y;
    int lastXPos;
    int lastYPos;
    int spawnX;
    int spawnY;
};

#endif // GAMEOBJECT_H
