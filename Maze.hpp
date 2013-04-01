#ifndef MAZE_HPP
#define MAZE_HPP
void NewMaze();
void EvalMaze(maze& Tiles,pairi Tile,int Size,set<char> Blockers);
char MakeTile(maze& Tiles,int x,int y);
inline char GetTile(maze& Tiles,int x,int y){
    return Tiles.count(pairi(x,y))?Tiles[pairi(x,y)]:MakeTile(Tiles,x,y);
}
void StructuresLoad();
bool EraseMazeChunk(maze& Tiles,pairi TopLeft,pairi BottomRight);
#endif // MAZE_HPP
