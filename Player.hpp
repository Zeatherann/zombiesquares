#ifndef PLAYER_HPP
#define PLAYER_HPP
class Player:public Entity{
protected:
    // Destructor
    ~Player();
public:
    static Player* Self;
    static unordered_map<pairi,int> Pathing;
    static unordered_map<pairi,int> Sight;
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
    void Update(const Terrain& Land);
    bool MoveTo(Terrain& Land,pairi Loc);
    void Shoot(Terrain& Land,pairi Direction);
    bool InSight(pairi Loc);
    float GetSight(pairi Loc)const;
    void Save(ostream& File)const;
    void Load(istream& File);
    bool Remove()const;
};
#endif // PLAYER_HPP
