#ifndef MazePathing
#define MazePathing
/// Defines
/// Libraries
#include <iostream>
#include <sstream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <yaml-cpp/yaml.h>
#include "KeyMap.hpp"
#include <boost/filesystem.hpp>

#define Debug cout<<__FUNCTION__<<" @"<<__LINE__<<endl;
#define TileSize 32.f
#define TileNum (5u) // ct_floor, ct_wall, ct_point Square, Starting ct_wall, purple wall
/// Namespaces
using namespace std;
using namespace boost::filesystem;
/// Class Forward Declarations
class Enemy;
class Player;
class Menu;
class Settings;
class Button;
/// Enumerations
enum ColorType{
    ct_invalid=-1, // sf::Color(0,0,0,0)
    ct_floor, // sf::Color(0,0,0,255)
    ct_wall, // sf::Color(0,0,128,255)
    ct_point, // sf::Color(0,128,0,255)
    ct_startwall, // sf::Color(64,192,64,255)
    ct_testtile, // sf::Color(255,128,255,255)
    ct_zombie, // sf::Color(255,0,0,255)
    ct_slowzombie, // sf::Color(128,255,255,255)
    ct_fastzombie, // sf::Color(255,0,255,255)
    ct_lazer, // sf::Color(255,0,255,255)
    ct_player // sf::Color(128,255,0,255)
};
enum MenuType{
    mt_singleplayer,
    mt_multiplayer,
    mt_mainmenu,
    mt_settings,
    mt_newgame,
    mt_loadgame,
    mt_singlepause,
    mt_multipause,
    mt_quittomenu,
    mt_quitgame,
    mt_singlelose
};
/// Typedefs
typedef pair<int,int> pairi;
typedef pair<char,bool> tile;
typedef map<pairi,tile> maze;
/// Global Functions
int main();
//-----
void ShowMenu(Menu* M);
void HideMenus();
void AddKeyBinding(string KeyName,int Side,Menu* MenuToAdd,sf::Vector2f Size);
sf::Matrix3 GetViewMatrix(const sf::View& View);
// Templates
template<typename T> std::string ToString(T number);
template<typename KeyType,typename ValueType>const KeyType* InMap(const map<KeyType*,ValueType>& Map,const KeyType& Element);
template<typename T> T ToNumber(std::string str);
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
// Templated Operators
template<typename Type>void operator+=(vector<Type>& L,const vector<Type>& R);
template<typename Type>set<Type>& operator+=(set<Type>& L,const set<Type>& R);
template<typename Type1,typename Type2>pair<Type1,Type2> operator+(const pair<Type1,Type2>& L,const pair<Type1,Type2>& R);
template<typename Type1,typename Type2>pair<Type1,Type2> operator-(const pair<Type1,Type2>& L,const pair<Type1,Type2>& R);
template<typename Type1,typename Type2>ostream& operator<<(ostream& L,const pair<Type1,Type2>& R);
template<typename Type>sf::Vector2<Type> operator*(const sf::Vector2<Type>& L,const sf::Vector2<Type>& R);
/// Project Files
#include "Globals.hpp"
#include "Colors.hpp"
#include "Settings.hpp"
#include "UIElement.hpp"
#include "Clickable.hpp"
#include "KeyClickable.hpp"
#include "MouseClickable.hpp"
#include "Draggable.hpp"
#include "Scrollable.hpp"
#include "Maze.hpp"
#include "Entity.hpp"
#include "Lazer.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Saving.hpp"
#include "Button.hpp"
Button* ButtonStyle(Button* B,sf::Color Base);
#include "UIGroup.hpp"
#include "Menu.hpp"
// -----
#include "main.inl"
#endif // MazePathing
