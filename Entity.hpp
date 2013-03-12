#ifndef ENTITY_HPP
#define ENTITY_HPP
class Entity{
public:
    static sf::Shape Tile;
    // Variables
    int X;
    int Y;
    sf::Color Color;
    short Life;
    // Constructor
    Entity(int x=0,int y=0,sf::Color color=sf::Color::White,short life=1);
    // Destructor
    ~Entity();
    // Functions
    void Draw(sf::RenderWindow& Window);
    virtual void Update()=0;
};
#endif // ENTITY_HPP
