#include "main.hpp"

set<Entity*> Entity::Entities;
sf::Shape Entity::Tile=sf::Shape::Rectangle(1.f,1.f,TileSize-2,TileSize-2,sf::Color::White,1.f);

Entity::Entity(ColorType type,int x,int y,sf::Color color,short life,bool lightsource):Type(type),X(x),Y(y),Color(color),Life(life),LightSource(lightsource){
    Entities.insert(this);
}

Entity::~Entity(){
    Entities.erase(this);
}

void Entity::Clear(){
    for(Entity* Iter:Entities){
        Delete(Iter);
    }
    Entities.clear();
}

void Entity::Tick(){
    vector<Entity*> ToDraw,Lazers;
    for(Entity* Iter:Entities){
        if(Iter->Remove()){
            delete Iter;
        }else{
            if(MenuMode==0)Iter->Update();
            if(Iter->Type==ct_lazer)Lazers.push_back(Iter);
            else ToDraw.push_back(Iter);
        }
    }
    ToDraw.insert(ToDraw.end(),Lazers.begin(),Lazers.end());
    if(Player::Character){
        int X=Player::Character->X;
        int Y=Player::Character->Y;
        for(Entity* Iter:ToDraw){
            if(Iter->Type!=ct_player){
                pairi Loc(Iter->X,Iter->Y);
                if(abs(Iter->X-X)<=(Player::SightRadius+1)&&abs(Iter->Y-Y)<=(Player::SightRadius+1)&&Player::Character->InSight(Loc)){
                    char Sight=Player::Character->GetSight(Loc);
                    float Alpha=(1.f-(float(Sight)/float(Player::SightRadius+1)));
                    if(Iter->LightSource||Alpha>1.f)Alpha=1.f;
                    if(Alpha<0.f)Alpha=0.f;
                    Iter->Draw(Alpha);
                }
            }else{
                Iter->Draw(1.f);
            }
        }
    }
}

void Entity::Draw(float Alpha)const{
        float x=X*TileSize;
        float y=Y*TileSize;
        TileImages[Type].Bind();
        glBegin(GL_QUADS);
            glColor4f(1,1,1,Alpha*float(Color.a/255.f));
            glTexCoord2f(0,0);glVertex2f(x,y);
            glTexCoord2f(0,1);glVertex2f(x,y+TileSize-1);
            glTexCoord2f(1,1);glVertex2f(x+TileSize-1,y+TileSize-1);
            glTexCoord2f(1,0);glVertex2f(x+TileSize-1,y);
        glEnd();
}

void Entity::Save(ofstream& File)const{
    File.write((char*)&Type,4u);
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
    File.read((char*)&Type,4u);
    File.read((char*)&X,4u);
    File.read((char*)&Y,4u);
    File.read((char*)&Color.r,1u);
    File.read((char*)&Color.g,1u);
    File.read((char*)&Color.b,1u);
    File.read((char*)&Color.a,1u);
    File.read((char*)&Life,2u);
    File.read((char*)&LightSource,1u);
}

void Entity::Delete(Entity* Ent){
    delete Ent;
}
