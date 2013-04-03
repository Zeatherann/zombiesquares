#ifndef MAZE_HPP
#define MAZE_HPP
void NewMaze();
void EvalMaze(maze& Tiles,pairi Tile,int Size,set<char> Blockers);
tile MakeTile(maze& Tiles,int x,int y);
inline tile GetTile(maze& Tiles,int x,int y){
    return Tiles.count(pairi(x,y))?Tiles[pairi(x,y)]:MakeTile(Tiles,x,y);
}
void PasteBuilding(const sf::Image& Struct,maze& Tiles,const pairi& Coord);
void StructuresLoad();
void StructurePlace(sf::Image structure, pairi offset);
void StructurePlaceRandom(pairi offset);
bool EraseMazeChunk(maze& Tiles,pairi TopLeft,pairi BottomRight);
TileType Color2Tile(const sf::Color& Color);
EntityType Color2Entity(const sf::Color& Color);
#endif // MAZE_HPP
