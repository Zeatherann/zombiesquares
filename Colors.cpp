#include "main.hpp"

map<string,sf::Color> Colors;

void ColorsInit() {
    Colors["player"] = sf::Color(255, 128, 0);
    Colors["zombie"] = sf::Color::Red;
    Colors["slow zombie"] = sf::Color(128, 255, 255);
    Colors["fast zombie"] = sf::Color::Magenta;
    Colors["floor"] = sf::Color::Black;
    Colors["wall"] = sf::Color(0, 0, 128);
    Colors["points"] = sf::Color(0, 128, 0);
    Colors["starting wall"] = sf::Color(64, 64, 128);
    Colors["lazer"] = sf::Color::Cyan;
}
