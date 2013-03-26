#ifndef ENTITY_HPP
#define ENTITY_HPP
class Entity{
protected:
    // Constructor
    Entity(char type='\0',int x=0,int y=0,sf::Color color=sf::Color::White,short life=1,bool lightsource=false);
    Entity(const Entity&);
    // Destructor
    virtual ~Entity();
    // Functions
    Entity& operator=(const Entity&);
public:
    static sf::Shape Tile;
    static set<Entity*> Entities;
    // Variables
    char Type;
    int X;
    int Y;
    sf::Color Color;
    short Life;
    bool LightSource;
    // Functions
    static void Tick(sf::RenderWindow& Window);
    static void Clear();
    // -----
    void Draw(sf::RenderWindow& Window)const;
    virtual void Update()=0;
    virtual void Save(ofstream& File)const;
    virtual void Load(ifstream& File);
    virtual bool Remove()const=0;
};
#endif // ENTITY_HPP
