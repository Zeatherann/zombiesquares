#ifndef ENEMY_HPP
#define ENEMY_HPP
class Enemy:public Entity{
protected:
    // Destructor
    ~Enemy();
public:
    // Variables
    short Power;
    char Tick;
    char mTick;
    // Constructor
    Enemy(int x=0,int y=0,short p=1,short hp=5,char tick=20);
    // Functions
    void Update();
    void Save(ofstream& File)const;
    void Load(ifstream& File);
    bool Remove()const;
};
#endif // ENEMY_HPP
