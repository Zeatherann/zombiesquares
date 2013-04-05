#include "main.hpp"

Lazer::Lazer(pairi Loc,short P):Entity(ct_lazer,Loc.first,Loc.second,Colors[ct_lazer],1,true),Hit(),Power(P){}

Lazer::~Lazer(){}

void Lazer::Update(){
    if(Color.a==255u)for(Entity* E:Entities)if(Enemy::IsEnemy(E->Type)&&E->X==X&&E->Y==Y)E->Life-=Power;
    Color.a-=5u;
}

void Lazer::Save(ofstream& File)const{
    Entity::Save(File);
    File.write((char*)&Power,2u);
}

void Lazer::Load(ifstream& File){
    Entity::Load(File);
    File.read((char*)&Power,2u);
}

bool Lazer::Remove()const{
    return !Color.a;
}
