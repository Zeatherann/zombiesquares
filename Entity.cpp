#include "main.hpp"

set<Entity*> Entity::Entities;
sf::Shape Entity::Tile=sf::Shape::Rectangle(1.f,1.f,TileSize-2,TileSize-2,sf::Color::White,1.f);

Entity::Entity(char type,int x,int y,sf::Color color,short life):Type(type),X(x),Y(y),Color(color),Life(life){
    Entities.insert(this);
}

Entity::~Entity(){}

void Entity::Tick(sf::RenderWindow& Window){
    set<Entity*> Remove;
    set<Entity*> Lazers;
    for(Entity* Iter:Entities){
        if(Iter->Remove()){
            Remove.insert(Iter);
        }else{
            Iter->Update();
            if(Iter->Type=='L')Lazers.insert(Iter);
        }
    }
    if(Remove.size())for(Entity* Iter:Remove){
        Entities.erase(Iter);
        delete Iter;
    }
    if(Player::Character){
        for(Entity* Iter:Entities)if(Lazers.count(Iter)==0&&(Iter->Type=='P'||(Player::Character->InSight(pairi(Iter->X,Iter->Y)))))Iter->Draw(Window);
        for(Entity* Iter:Lazers)if(Iter->Type=='P'||(Player::Character->InSight(pairi(Iter->X,Iter->Y))))Iter->Draw(Window);
    }
}

void Entity::Draw(sf::RenderWindow& Window)const{
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
    File.write(&Type,1u);
    File.write((char*)&X,4u);
    File.write((char*)&Y,4u);
    File.write((char*)&Color.r,1u);
    File.write((char*)&Color.g,1u);
    File.write((char*)&Color.b,1u);
    File.write((char*)&Color.a,1u);
    File.write((char*)&Life,2u);
}

void Entity::Load(ifstream& File){
    File.read(&Type,1u);
    File.read((char*)&X,4u);
    File.read((char*)&Y,4u);
    File.read((char*)&Color.r,1u);
    File.read((char*)&Color.g,1u);
    File.read((char*)&Color.b,1u);
    File.read((char*)&Color.a,1u);
    File.read((char*)&Life,2u);
}
