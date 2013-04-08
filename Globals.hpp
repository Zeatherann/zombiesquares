#ifndef GLOBALS_HPP
#define GLOBALS_HPP
extern const sf::Color HighLight;
extern maze Maze;
extern int GameTime;
extern const pairi Adj[8];
extern char MenuMode; // 0: Playing, 1: Paused, 2: Main Menu, 3: Game Over, 4: Settings, -1: Quit to Main Menu, -2: Quit Game
extern float WinWidth;
extern float WinHeight;
extern float Awake;
extern set<Menu*> Menus;
extern Settings Config;
extern KeyMap Keys;
extern string BindKey;
extern sf::Font Font;
extern vector<sf::Image*> Structures;
extern map<ColorType,sf::Image> TileImages;
extern sf::PostFX BlackAndWhite;
// -----
void InitGlobals();
#endif // GLOBALS_HPP

