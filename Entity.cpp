#include "main.hpp"
sf::Shape Entity::Tile=sf::Shape::Rectangle(1.f,1.f,TileSize-2,TileSize-2,sf::Color::White,1.f);

Entity::Entity(int x,int y,sf::Color color,short life):X(x),Y(y),Color(color),Life(life){}

Entity::~Entity(){}

void Entity::Draw(sf::RenderWindow& Window){
    Tile.SetPosition(X*TileSize,Y*TileSize);
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

void Entity::Save(ofstream& File)const{
    File.write((char*)&X,4u);
    File.write((char*)&Y,4u);
    File.write((char*)&Color.r,1u);
    File.write((char*)&Color.g,1u);
    File.write((char*)&Color.b,1u);
    File.write((char*)&Color.a,1u);
    File.write((char*)&Life,2u);
}

void Entity::Load(ifstream& File){
    File.read((char*)&X,4u);
    File.read((char*)&Y,4u);
    File.read((char*)&Color.r,1u);
    File.read((char*)&Color.g,1u);
    File.read((char*)&Color.b,1u);
    File.read((char*)&Color.a,1u);
    File.read((char*)&Life,2u);
}
