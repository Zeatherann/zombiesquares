#ifndef ENTITY_HPP
#define ENTITY_HPP
class Entity{
protected:
    // Constructor
    Entity(ColorType type=ct_invalid,int x=0,int y=0,sf::Color color=sf::Color::White,short life=1,bool lightsource=false);
    Entity(const Entity&);
    // Functions
    Entity& operator=(const Entity&);
public:
    // Destructor
    virtual ~Entity();
    static sf::Shape Tile;
    static set<Entity*> Entities;
    // Variables
    ColorType Type;
    int X;
    int Y;
    sf::Color Color;
    short Life;
    bool LightSource;
    // Functions
    static void Tick(const Terrain& Land,const pairi& UpdateCenter); // Updates entities.
    static void Clear();
    // -----
    bool Move(Terrain& Land,const pairi& NewCoord);
    //void Draw(float Alpha)const;
    virtual void Update(const Terrain& Land)=0;
    virtual void Save(ostream& File)const;
    virtual void Load(istream& File);
    virtual bool Remove()const=0;
    static void Delete(Entity* Ent);
    static Entity* StaticLoad(istream& stream);
};
#endif // ENTITY_HPP
