#ifndef TERRAIN_HPP
#define TERRAIN_HPP
class Terrain{
public:
    class Tile{
    public:
        // Variables
        ColorType tile; // The type of the tile (eg: ct_floor for normal floors)
        bitset<8> Flags;/* Flags for the tile, defined below:
            0: Built. True means placed by a structure, false means randomly placed.
            1: Walkable. True means entities can walk upon this (Enemy/Player), false means they can't move into this tile, and die if placed on it.
            2: Aggroable. True means players' aggro pathing is not blocked by this tile, false means enemies can 'see' a player(s) through this tile.
            3: Seeable. True means player's can see past this tile, false means it blocks sight.
            4: Shootable. True means player's lazers can fire through this tile, false means it blocks lazers.
            5: UNUSED
            6: UNUSED
            7: UNUSED
        */
        mutable set<Entity*> Entities; // The entity that is on this tile.
        // Constructor
        Tile(ColorType Type=ct_invalid,set<Entity*> Entities=set<Entity*>());
        Tile(ColorType Type,const pairi& Location);
        Tile(istream& stream);
        // Destructor
        ~Tile();
        // Functions
        void Save(ostream& stream)const;
    };
    // Variables
    typedef unordered_map<pairi,Tile*> WorldType;
    WorldType World;
    sf::Image Map;
    bool GeneratingStructure;
    bool Omnisight;
    // Constructor
    Terrain(const sf::Image& Map);
    Terrain(istream& stream);
    // Destructor
    ~Terrain();
    // Functions
    void GeneratePathing(unordered_map<pairi,int>& Map,int Radius,pairi Start,bitset<16> Flags)const;
    void GetAdjacentTiles(set<pairi>& Return,const pairi& Start)const;
    void Save(ostream& stream)const;
    Tile* GenerateRandomTile(pairi Location);
    void PlaceStructureMap(const sf::Image& Structure,const pairi& Location);
    void Draw(pairi ViewCenter);
    // Operators
    Tile* operator[](const pairi& Location);
};
#endif // TERRAIN_HPP
