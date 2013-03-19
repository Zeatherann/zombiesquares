#ifndef MazePathing
#define MazePathing
/// Defines
/// Libraries
#include <iostream>
#include <sstream>
#include <fstream>
#include <SFML/Graphics.hpp>

#define TileSize 32.f
#define TileNum (4u) // Ground, Wall, Point Square, Starting Wall
/// Namespaces
using namespace std;
namespace Data{

typedef pair<char*,unsigned int> Chunk;

// to bytes

template<typename Type>inline Chunk In(const Type& Input){
    return Chunk((char*)(&Input),sizeof(Type));
}

inline Chunk In(const std::string& Input){
    return Chunk((char*)Input.c_str(),Input.size());
}

// from bytes

template<typename Type>inline const Type& Out(const Type& Input){
    return *(Type*)(Input);
}

}// namespace Data
/// Class Forward Declarations
typedef pair<int,int> pairi;
typedef map<pair<int,int>,char> maze;
class Enemy;
class Player;
/// Enumerations

/// Structs
extern const sf::Color HighLight;
extern maze Maze;
extern int GameTime;
extern pairi Adj[8];
extern char MenuMode; // 0: Playing, 1: Paused
extern float WinWidth;
extern float WinHeight;
/// Global Functions
int main();
//-----
// Templates
template<typename T> std::string ToString(T number){
    std::stringstream S;
    S<<number;
    return S.str();
}

template<typename T> T ToNumber(std::string str){
    std::stringstream S;
    S<<str;
    T Ret;
    S>>Ret;
    return Ret;
}
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
inline sf::Color operator/(const sf::Color& L,const unsigned char& R){
    return sf::Color(L.r/R,L.g/R,L.b/R);
}
inline sf::Color operator*(const sf::Color& L,const unsigned char& R){
    return sf::Color(L.r*R,L.g*R,L.b*R);
}
inline sf::Color operator-(const sf::Color& L,const sf::Color& R){
    return sf::Color(L.r<R.r?0:L.r-R.r,L.g<R.g?0:L.g-R.g,L.b<R.b?0:L.b-R.b);
}
/// Project Files
#include "Maze.hpp"
#include "Entity.hpp"
#include "Lazer.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Saving.hpp"
#include "UIElement.hpp"
#include "Button.hpp"
Button* ButtonStyle(Button* B,sf::Color Base);
#include "UIGroup.hpp"
#include "Menu.hpp"
#endif // MazePathing
