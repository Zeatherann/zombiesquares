#include "main.hpp"
sf::Shape Entity::Tile=sf::Shape::Rectangle(1.f,1.f,TileSize-1,TileSize-1,sf::Color::White,1.f);

Entity::Entity(int x,int y,sf::Color color,short life):X(x),Y(y),Color(color),Life(life){}

Entity::~Entity(){}

void Entity::Draw(sf::RenderWindow& Window){
    Tile.SetPosition(X*TileSize-1,Y*TileSize-1);
    sf::Color C=Color+HighLight;
    Tile.SetPointOutlineColor(0,C);
    Tile.SetPointOutlineColor(1,C);
    Tile.SetPointOutlineColor(2,C);
    Tile.SetPointOutlineColor(3,C);
    Tile.SetPointColor(0,Color);
    Tile.SetPointColor(1,Color);
    Tile.SetPointColor(2,Color);
    Tile.SetPointColor(3,Color);
    Window.Draw(Tile);
}
