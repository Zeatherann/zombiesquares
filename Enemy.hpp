#ifndef ENEMY_HPP
#define ENEMY_HPP
class Enemy:public Entity{
public:
    // Variables
    short Power;
    char Tick;
    char mTick;
    // Constructor
    Enemy(int x=0,int y=0,short p=1,short hp=5,char tick=20);
    ~Enemy();
    static Enemy* NewSlowEnemy(int x, int y, short power);
    static Enemy* NewFastEnemy(int x, int y, short power);
    // Functions
    void Update(const Terrain& Land);
    void Save(ostream& File)const;
    void Load(istream& File);
    bool Remove()const;
    static bool IsEnemy(const ColorType& ct);
};
#endif // ENEMY_HPP
