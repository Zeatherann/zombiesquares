#ifndef ENEMY_HPP
#define ENEMY_HPP
class Enemy:public Entity{
public:
    static vector<Enemy> Enemies;
    // Variables
    short Power;
    char Tick;
    // Constructor
    static void New(int x=0,int y=0,short p=1);
    Enemy(int x=0,int y=0,short p=1);
    // Destructor
    ~Enemy();
    // Functions
    void Update();
};
#endif // ENEMY_HPP
