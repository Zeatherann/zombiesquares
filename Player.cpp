#include "main.hpp"

Player* Player::Self;
maze Player::Pathing;
maze Player::Sight;
int Player::AggroRadius=25;
int Player::SightRadius=5;

Player::Player(int x,int y):Entity(ct_player,x,y,Colors[ct_player],0),HScore(0),oX(x+1),oY(y+1),Timer(50),Shots(3){}

Player::~Player(){
    if(Self==this)Self=NULL;
}

void Player::Update(){
    if(Life>HScore)HScore=Life;
    if(oX!=X||oY!=Y){
        oX=X;
        oY=Y;
        EvalMaze(Pathing,pairi(X,Y),AggroRadius,{ct_wall,ct_startwall});
        EvalMaze(Sight,pairi(X,Y),SightRadius,{ct_wall});
    }
    if(Timer)Timer--;
    else{
        Timer=50;
        if(Shots<3){
            Shots++;
        }
    }
}

bool Player::MoveTo(pairi Loc){
    char T=GetTile(Maze,Loc.first,Loc.second).first;
    if(T==ct_wall)return false;
    X=Loc.first;
    Y=Loc.second;
    if(T==ct_point){
        Life++;
        Maze[Loc]=tile(ct_floor,false);
    }
    if(T==ct_startwall){
        for(pair<const pairi,tile>& Iter:Maze){
            if(Iter.second.first==3){
                Iter.second.first=0;
            }
        }
    }
    return true;
}

void Player::Shoot(pairi Direction){
    if(!Shots||Direction==pairi(0,0))return;
    Shots--;
    pairi C(X,Y);
    while(true){
        C=C+Direction;
        if(Maze.count(C)){
            char T=GetTile(Maze,C.first,C.second).first;
            if(T==ct_wall||T==ct_startwall){
                return;
            }else{
                new Lazer(C,3);
            }
        }else{
            return;
        }
    }
}

bool Player::InSight(pairi Loc){
    if(Sight.count(Loc))return true;
    else{
        for(unsigned int i=0u;i<8u;i++){
            if(Sight.count(Loc+Adj[i]))return true;
        }
    }
    return false;
}

char Player::GetSight(pairi Loc)const{
    if(Sight.count(Loc))return Sight.find(Loc)->second.first;
    else{
        char Ret=-1;
        for(unsigned int i=0u;i<8u;i++){
            pairi P(Loc.first+Adj[i].first,Loc.second+Adj[i].second);
            if(Sight.count(P)&&(Ret==-1||Sight.find(P)->second.first<Ret))Ret=Sight.find(P)->second.first;
        }
        return Ret;
    }
}

void Player::Save(ofstream& File)const{
    Entity::Save(File);
    File.write((char*)&HScore,2u);
    File.write((char*)&Timer,4u);
    File.write((char*)&Shots,4u);
    bool IsSelf=this==Self;
    File.write((char*)&IsSelf,1u);
}

void Player::Load(ifstream& File){
    Entity::Load(File);
    File.read((char*)&HScore,2u);
    File.read((char*)&Timer,4u);
    File.read((char*)&Shots,4u);
    bool IsSelf=false;
    File.read((char*)&IsSelf,1u);
    if(IsSelf)Self=this;
    oX=X;
    oY=Y;
    EvalMaze(Pathing,pairi(X,Y),AggroRadius,{ct_wall,ct_startwall});
    EvalMaze(Sight,pairi(X,Y),SightRadius,{ct_wall});
}

bool Player::Remove()const{
    return Life<0;
}
