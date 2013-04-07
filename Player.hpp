#ifndef PLAYER_HPP
#define PLAYER_HPP
class Player:public Entity{
protected:
    // Destructor
    ~Player();
public:
    static Player* Self;
    static maze Pathing;
    static maze Sight;
    static int AggroRadius;
    static int SightRadius;
    // Variables
    short HScore;
    int oX;
    int oY;
    int Timer;
    int Shots;
    // Constructor
    Player(int x=0,int y=0);
    // Functions
    void Update();
    bool MoveTo(pairi Loc);
    inline bool Move(pairi Offset){
        return MoveTo(pairi(X,Y)+Offset);
    }
    void Shoot(pairi Direction);
    bool InSight(pairi Loc);
    char GetSight(pairi Loc)const;
    void Save(ofstream& File)const;
    void Load(ifstream& File);
    bool Remove()const;
};
#endif // PLAYER_HPP
