#ifndef MAZE_HPP
#define MAZE_HPP
void EvalMaze(maze& Tiles,pairi Tile,int Size,set<char> Blockers);
char MakeTile(maze& Tiles,int x,int y);
inline char GetTile(maze& Tiles,int x,int y){
    return Tiles.count(pairi(x,y))?Tiles[pairi(x,y)]:MakeTile(Tiles,x,y);
}
#endif // MAZE_HPP
