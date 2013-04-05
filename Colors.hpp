#ifndef COLORS_HPP_INCLUDED
#define COLORS_HPP_INCLUDED

extern map<ColorType,sf::Color> Colors;
extern map<sf::Color,ColorType> ColorsInv;
void ColorsInit();
void glColor(const sf::Color& Color);

namespace std{
    template<> struct less<sf::Color>{
       bool operator() (const sf::Color& L, const sf::Color& R){
           return L.r+L.g*256+L.b*65536+L.a*16777216<R.r+R.g*256+R.b*65536+R.a*16777216;
       }
    };
}
#endif // COLORS_HPP_INCLUDED
