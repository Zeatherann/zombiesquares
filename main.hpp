#ifndef MazePathing
#define MazePathing
/// Defines
/// Libraries
#include <iostream>
#include <SFML/Graphics.hpp>
#include <Vanyel/Vanyel.hpp>

#define TileSize (16.f)
#define WinHeight (768)
#define Width (21.f)
#define Height (15.f)
#define SWidth (TileSize*Width)
#define SHeight (TileSize*Height)
#define CWidth (10.f)
#define CHeight (7.f)
#define TileNum (5)
/// Namespaces
using namespace std;
/// Class Forward Declarations
typedef pair<int,int> pairi;
typedef map<pair<int,int>,char> maze;
class Enemy;
/// Enumerations

/// Structs
extern const sf::Color HighLight;
extern maze Maze;
extern int GameTime;
extern pairi Adj[8];
/// Global Functions
int main();
//-----
char MakeTile(maze& Tiles,int x,int y);
inline char GetTile(maze& Tiles,int x,int y){
    return Tiles.count(pairi(x,y))?Tiles[pairi(x,y)]:MakeTile(Tiles,x,y);
}
maze LoadMaze(string FileName);
void EvalMaze(maze& Tiles,pairi Tile,int Size,set<char> Blockers);
// Templates
template<typename Type>inline set<Type>& operator+=(set<Type>& L,const set<Type>& R){
    L.insert(R.begin(),R.end());
    return L;
}
template<typename Type1,typename Type2>inline pair<Type1,Type2> operator+(const pair<Type1,Type2>& L,const pair<Type1,Type2>& R){
    return pair<Type1,Type2>(L.first+R.first,L.second+R.second);
}
template<typename Type1,typename Type2>inline ostream& operator<<(ostream& L,const pair<Type1,Type2>& R){
    return L<<"("<<R.first<<", "<<R.second<<")";
}
// Operators

/// Project Files
#include "Entity.hpp"
#include "Lazer.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#endif // MazePathing
