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
    for(set<Entity*>::iterator Iter=Entities.begin();Iter!=Entities.end();){
        Delete((*Iter++));
    }
    Entities.clear();
}

void Entity::Tick(const Terrain& Land,const pairi& UpdateCenter){
    //vector<Entity*> ToDraw,Lazers;
    vector<Entity*>  DeadEntities;
    int UpdateRange=(Player::AggroRadius>Player::SightRadius?Player::AggroRadius:Player::SightRadius)+5;
    for(Entity* Iter:Entities){
        pairi Location(Iter->X,Iter->Y);
        // Check distance...
        if(abs(UpdateCenter.first-Location.first)+abs(UpdateCenter.second-Location.second)<=UpdateRange){
            // Get tile entity is on.
            Terrain::WorldType::const_iterator TileIter=Land.World.find(Location);
            // If tile doesn't exist, or the tile isn't walkable; kill the entity.
            if(TileIter==Land.World.cend()||TileIter->second->Flags[1]==false){
                // Add to list of dead entities.
                DeadEntities.push_back(Iter);
                // Remove from tile's entities.
                TileIter->second->Entities.erase(Iter);
            }else{
                // Entity is still alive.
                Iter->Update(Land);
            }
        }// else out of range: no update/kill.
    }
    // Remove dead entities.
    for(Entity* Iter:DeadEntities){
        delete Iter;
    }
//    for(set<Entity*>::iterator Iter=Entities.begin();Iter!=Entities.end();){
//        if((*Iter)->Remove()){
//            delete (*Iter++);
//        }else{
//            if(MenuMode==0)(*Iter)->Update();
//            if((*Iter)->Type==ct_lazer)Lazers.push_back((*Iter));
//            else ToDraw.push_back((*Iter));
//            Iter++;
//        }
//    }
//    ToDraw.insert(ToDraw.end(),Lazers.begin(),Lazers.end());
//    if(Player::Self){
//        int X=Player::Self->X;
//        int Y=Player::Self->Y;
//        for(Entity* Iter:ToDraw){
//            if(Iter->Type!=ct_player){
//                pairi Loc(Iter->X,Iter->Y);
//                if(abs(Iter->X-X)<=(Player::SightRadius+1)&&abs(Iter->Y-Y)<=(Player::SightRadius+1)&&Player::Self->InSight(Loc)){
//                    char Sight=Player::Self->GetSight(Loc);
//                    float Alpha=(1.f-(float(Sight)/float(Player::SightRadius+1)));
//                    if(Iter->LightSource||Alpha>1.f)Alpha=1.f;
//                    if(Alpha<0.f)Alpha=0.f;
//                    Iter->Draw(Alpha);
//                }
//            }else{
//                Iter->Draw(1.f);
//            }
//        }
//    }
}

bool Entity::Move(Terrain& Land,const pairi& NewCoord){
    Terrain::WorldType::iterator Iter=Land.World.find(NewCoord);
    if(Iter==Land.World.end())return false;
    if(Iter->second->Flags[1]==false)return false;
    Iter->second->Entities.insert(this);
    Land.World[pairi(X,Y)]->Entities.erase(this);
    X=NewCoord.first;
    Y=NewCoord.second;
    return true;
}

//void Entity::Draw(float Alpha)const{
//    float x=X*TileSize;
//    float y=Y*TileSize;
//    TileImages[Type].Bind();
//    glBegin(GL_QUADS);
//        glColor4f(1,1,1,Alpha*float(Color.a/255.f));
//        glTexCoord2f(0,0);glVertex2f(x,y);
//        glTexCoord2f(0,1);glVertex2f(x,y+TileSize-1);
//        glTexCoord2f(1,1);glVertex2f(x+TileSize-1,y+TileSize-1);
//        glTexCoord2f(1,0);glVertex2f(x+TileSize-1,y);
//    glEnd();
//}

void Entity::Save(ostream& File)const{
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

void Entity::Load(istream& File){
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

Entity* Entity::StaticLoad(istream& stream){
    ColorType C=ct_invalid;
    stream.read((char*)&C,4u);
    stream.seekg(-4,stream.cur);
    Entity* Ret=NULL;
    switch(C){
        case ct_zombie:
        case ct_slowzombie:
        case ct_fastzombie:{// Enemy
            Ret=new Enemy;
        }case ct_lazer:{// Lazer
            Ret=new Lazer;
        }case ct_player:{// Player
            Ret=new Player;
            break;
        }default:{
            cerr<<"Unknown entity type in Entity::StaticLoad()!"<<endl;
            std::terminate();
        }
    }
    Ret->Load(stream);
    return Ret;
}

void Entity::Delete(Entity* Ent){
    delete Ent;
}
