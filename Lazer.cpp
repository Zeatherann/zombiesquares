#include "main.hpp"

Lazer::Lazer(pairi Loc,short P):Entity('L',Loc.first,Loc.second,sf::Color::Cyan),Hit(),Power(P){}

Lazer::~Lazer(){}

void Lazer::Update(){
    unsigned char& A=Color.a;
    if(A>0u){
        A-=5u;
        for(Entity* E:Entities){
            if(E->Type=='E'&&E->X==X&&E->Y==Y&&Hit.count((Enemy*)E)==0){
                E->Life-=Power;
                Hit.insert((Enemy*)E);
            }
        }
    }
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
