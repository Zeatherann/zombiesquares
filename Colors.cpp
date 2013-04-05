#include "main.hpp"

map<ColorType,sf::Color> Colors;
map<sf::Color,ColorType> ColorsInv;

void ColorsInit() {
#define AddColor(color,name) Colors[name]=color;ColorsInv[color]=name
    AddColor(sf::Color(0  ,0  ,0  ,0  ),ct_invalid);
    AddColor(sf::Color(255,128,0  ,255),ct_player);
    AddColor(sf::Color(255,0  ,0  ,255),ct_zombie);
    AddColor(sf::Color(128,255,255,255),ct_slowzombie);
    AddColor(sf::Color(255,0  ,255,255),ct_fastzombie);
    AddColor(sf::Color(0  ,255,255,255),ct_lazer);
    AddColor(sf::Color(0  ,0  ,128,255),ct_wall);
    AddColor(sf::Color(0  ,0  ,0  ,255),ct_floor);
    AddColor(sf::Color(255,128,255,255),ct_testtile);
    AddColor(sf::Color(0  ,128,0  ,255),ct_point);
    AddColor(sf::Color(64 ,64 ,128,255),ct_startwall);
#undef AddColor
}

void glColor(const sf::Color& Color){
    glColor4f(float(Color.r)/255.f,float(Color.g)/255.f,float(Color.b)/255.f,float(Color.a)/255.f);
}
