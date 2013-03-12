#include "main.hpp"

vector<Enemy> Enemy::Enemies;

void Enemy::New(int x,int y,short p){
    Enemies.push_back(Enemy(x,y,p));
}

Enemy::Enemy(int x,int y,short p):Entity(x,y,sf::Color::Red,5),Power(p),Tick(20){}

Enemy::~Enemy(){}

void Enemy::Update(){
    Tick--;
    if(Tick==0){
        Tick=20;
        if(Player::Pathing.count(pairi(X,Y))){
            char C=Player::Pathing[pairi(X,Y)];
            vector<pairi> NewCoords;
            pairi P[4]={pairi(X-1,Y),pairi(X+1,Y),pairi(X,Y-1),pairi(X,Y+1)};
            for(unsigned int i=0u;i<4u;i++){
                if(Player::Pathing.count(P[i]))if(Player::Pathing[P[i]]<C)NewCoords.push_back(P[i]);
            }
            if(NewCoords.size()){
                pairi NC=NewCoords[rand()%NewCoords.size()];
                X=NC.first;
                Y=NC.second;
            }
        }
    }
}
