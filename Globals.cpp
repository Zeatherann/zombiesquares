#include "main.hpp"

const sf::Color HighLight(32,32,32);
maze Maze;
int GameTime=0;
const pairi Adj[8]={pairi(-1,0),pairi(1,0),pairi(0,-1),pairi(0,1),pairi(1,1),pairi(-1,1),pairi(-1,-1),pairi(1,-1)};
char MenuMode=2;
float Awake=1.f;
set<Menu*> Menus;
KeyMap Keys;
Settings Config=Settings::LoadSettings("Settings.yml");
float WinWidth=Config.WindowWidth;
float WinHeight=Config.WindowHeight;
string BindKey;
sf::Font Font;
map<ColorType,sf::Image> TileImages;
sf::PostFX BlackAndWhite;

void InitGlobals(){
    srand(time(NULL));
    ColorsInit();
    StructuresLoad();
    for(const pair<ColorType,sf::Color>& Iter:Colors){
        sf::Image& TileImage=TileImages[Iter.first]=sf::Image(TileSize,TileSize,sf::Color(0,0,0,0));
        TileImage.SetSmooth(false);
        sf::Color Edge=Iter.second+HighLight;
        unsigned int End=TileSize-1u;
        for(unsigned int a=0u;a<=End;a++){
            for(unsigned int b=0u;b<=End;b++){
                TileImage.SetPixel(a,b,a==0u||a==End||b==0u||b==End?Edge:Iter.second);
            }
        }
    }
    if(!Font.LoadFromFile("Font.ttf",(unsigned int)TileSize)){
        cout<<"Unable to load font \'Font.ttf\'!"<<endl;
        exit(11);
    }
    ((sf::Image&)Font.GetImage()).SetSmooth(false);
    if(!BlackAndWhite.LoadFromFile("MenuEffect.sfx")){
        cout<<"Unable to load file \'MenuEffect.sfx\'!"<<endl;
        exit(10);
    }
    BlackAndWhite.SetTexture("framebuffer",NULL);
}
