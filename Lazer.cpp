#include "main.hpp"

vector<Lazer*> Lazer::Lazers;

void Lazer::New(pairi Loc,short P){
    Lazers.push_back(new Lazer(Loc,P));
}

Lazer::Lazer(pairi Loc,short P):Entity(Loc.first,Loc.second,sf::Color::Cyan),Hit(),Power(P){}

Lazer::~Lazer(){}

void Lazer::Update(){
    unsigned char& A=Color.a;
    if(A>0u){
        A-=5u;
        for(Enemy* E:Enemy::Enemies){
            if(E->X==X&&E->Y==Y&&Hit.count(E)==0){
                E->Life-=Power;
                Hit.insert(E);
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
