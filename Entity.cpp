#include "main.hpp"

set<Entity*> Entity::Entities;
sf::Shape Entity::Tile=sf::Shape::Rectangle(1.f,1.f,TileSize-2,TileSize-2,sf::Color::White,1.f);

Entity::Entity(char type,int x,int y,sf::Color color,short life,bool lightsource):Type(type),X(x),Y(y),Color(color),Life(life),LightSource(lightsource){
    Entities.insert(this);
}

Entity::~Entity(){
    Entities.erase(this);
}

void Entity::Clear(){
    for(Entity* Iter:Entities){
        delete Iter;
    }
    Entities.clear();
}

void Entity::Tick(sf::RenderWindow& Window){
    vector<Entity*> ToDraw,Lazers;
    for(Entity* Iter:Entities){
        if(Iter->Remove()){
            delete Iter;
        }else{
            if(MenuMode==0)Iter->Update();
            if(Iter->Type=='L')Lazers.push_back(Iter);
            else ToDraw.push_back(Iter);
        }
    }
    ToDraw.insert(ToDraw.end(),Lazers.begin(),Lazers.end());
    if(Player::Character){
        int X=Player::Character->X;
        int Y=Player::Character->Y;
        for(Entity* Iter:ToDraw){
            if(Iter->Type!='P'){
                pairi Loc(Iter->X,Iter->Y);
                if(abs(Iter->X-X)<=(Player::SightRadius+1)&&abs(Iter->Y-Y)<=(Player::SightRadius+1)&&Player::Character->InSight(Loc)){
                    char Sight=Player::Character->GetSight(Loc);
                    float Alpha=255.f*(1.f-(float(Sight)/float(Player::SightRadius+1)));
                    if(Iter->LightSource||Alpha>255.f)Alpha=255.f;
                    if(Alpha<0.f)Alpha=0.f;
                    Entity::Tile.SetColor(sf::Color(255,255,255,Alpha));
                    Iter->Draw(Window);
                }
            }else{
                Entity::Tile.SetColor(sf::Color::White);
                Iter->Draw(Window);
            }
        }
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
    File.write((char*)&LightSource,1u);
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
    File.read((char*)&LightSource,1u);
}

void Entity::Delete(Entity*& Ent){
    delete Ent;
    Ent=NULL;
}
