#include "main.hpp"

Enemy::Enemy(int x,int y,short p,short hp,char tick):Entity(ct_zombie,x,y,Colors[ct_zombie],hp),Power(p),Tick(20),mTick(tick){}

Enemy* Enemy::NewSlowEnemy(int x, int y, short power) {
    Enemy* e = new Enemy(x, y, power*2, 25, 40);
    e->Color = Colors[ct_slowzombie];
    e->Type=ct_slowzombie;
    return e;
}

Enemy* Enemy::NewFastEnemy(int x, int y, short power) {
    power /= 2;
    if (power <= 0) power = 1;
    Enemy* e = new Enemy(x, y, power, 5, 10);
    e->Color = Colors[ct_fastzombie];
    e->Type=ct_fastzombie;
    return e;
}

Enemy::~Enemy(){}

void Enemy::Update(){
    if(--Tick<=0){
        Tick=mTick;
        if(Player::Pathing.count(pairi(X,Y))){
            char C=Player::Pathing[pairi(X,Y)].first;
            vector<pairi> NewCoords;
            pairi P[4]={pairi(X-1,Y),pairi(X+1,Y),pairi(X,Y-1),pairi(X,Y+1)};
            for(unsigned int i=0u;i<4u;i++){
                if(Player::Pathing.count(P[i]))if(Player::Pathing[P[i]].first<C)NewCoords.push_back(P[i]);
            }
            if(NewCoords.size()){
                pairi NC=NewCoords[rand()%NewCoords.size()];
                X=NC.first;
                Y=NC.second;
            }
        }
    }
}

void Enemy::Save(ofstream& File)const{
    Entity::Save(File);
    File.write((char*)&Power,2u);
    File.write((char*)&Tick,1u);
}

void Enemy::Load(ifstream& File){
    Entity::Load(File);
    File.read((char*)&Power,2u);
    File.read((char*)&Tick,1u);
}

bool Enemy::Remove()const{
    Player* Char=Player::Character;
    char C=GetTile(Maze,X,Y).first;
    if(C==1)return true;
    if(Life<=0){
        if(Char)Char->ModScore(1);
        return true;
    }
    if(Char&&X==Char->X&&Y==Char->Y){
        Char->ModScore(-Power);
        return true;
    }
    return false;
}

bool Enemy::IsEnemy(const ColorType& ct){
    switch(ct){
        case ct_zombie:
        case ct_slowzombie:
        case ct_fastzombie:
            return true;
        default:
            return false;
    }
}
