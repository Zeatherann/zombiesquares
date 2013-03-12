#ifndef PLAYER_HPP
#define PLAYER_HPP
class Player:public Entity{
    int Score;
public:
    static maze Pathing;
    static maze Sight;
    // Variables
    int HScore;
    int oX;
    int oY;
    int Timer;
    int Shots;
    sf::String& Text;
    sf::String& Bullets;
    sf::String& High;
    // Constructor
    Player(sf::String& T,sf::String& B,sf::String& H);
    // Destructor
    ~Player();
    // Functions
    void Update();
    bool MoveTo(pairi Loc);
    inline bool Move(pairi Offset){
        return MoveTo(pairi(X,Y)+Offset);
    }
    void Shoot(pairi Direction);
    void Point(int Change);
    bool InSight(pairi Loc);
};
#endif // PLAYER_HPP
