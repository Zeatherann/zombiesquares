#include "main.hpp"

Player* Player::Character;
maze Player::Pathing;
maze Player::Sight;
int Player::AggroRadius=25;
int Player::SightRadius=6;

Player::Player(sf::String& T,sf::String& B,sf::String& H):Entity('P',0,0,sf::Color(255,128,0),10),Score(0),HScore(0),oX(X+1),oY(Y+1),Timer(50),Shots(3),Text(T),Bullets(B),High(H){
    Character=this;
    Bullets.SetText(" Bullets: "+ToString(Shots));
    High.SetText("  High Score: "+ToString(HScore));
    Text.SetText("Score: "+ToString(Score));
}

Player::~Player(){
    if(Character==this)Character=NULL;
}

void Player::Update(){
    if(oX!=X||oY!=Y){
        oX=X;
        oY=Y;
        EvalMaze(Pathing,pairi(X,Y),AggroRadius,{1,3});
        EvalMaze(Sight,pairi(X,Y),SightRadius,{1});
    }
    if(Timer)Timer--;
    else{
        Timer=50;
        if(Shots<3){
            Shots++;
            Bullets.SetText(" Bullets: "+ToString(Shots));
        }
    }
}

bool Player::MoveTo(pairi Loc){
    char T=GetTile(Maze,Loc.first,Loc.second);
    if(T==1)return false;
    X=Loc.first;
    Y=Loc.second;
    if(T==2){
        Point(1);
        Maze[Loc]=0;
    }
    if(T==3){
        for(pair<const pairi,char>& Iter:Maze){
            if(Iter.second==3){
                Iter.second=0;
            }
        }
    }
    return true;
}

void Player::Shoot(pairi Direction){
    if(!Shots||Direction==pairi(0,0))return;
    Shots--;
    Bullets.SetText(" Bullets: "+ToString(Shots));
    pairi C(X,Y);
    while(true){
        C=C+Direction;
        if(Maze.count(C)){
            char T=GetTile(Maze,C.first,C.second);
            if(T==1||T==4){
                return;
            }else{
                new Lazer(C,1);
            }
        }else{
            return;
        }
    }
}

void Player::Point(int Change){
    Score+=Change;
    Text.SetText("Score: "+ToString(Score));
    if(Score>HScore){
        HScore=Score;
        High.SetText(" High Score: "+ToString(HScore));
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

char Player::GetSight(pairi Loc){
    if(Sight.count(Loc))return Sight[Loc];
    else{
        char Ret=-1;
        for(unsigned int i=0u;i<8u;i++){
            pairi P(Loc.first+Adj[i].first,Loc.second+Adj[i].second);
            if(Sight.count(P)&&(Ret==-1||Sight[P]<Ret))Ret=Sight[P];
        }
        return Ret;
    }
}

void Player::Save(ofstream& File)const{
    Entity::Save(File);
    File.write((char*)&Score,4u);
    File.write((char*)&HScore,4u);
    File.write((char*)&Timer,4u);
    File.write((char*)&Shots,4u);
}

void Player::Load(ifstream& File){
    Entity::Load(File);
    File.read((char*)&Score,4u);
    File.read((char*)&HScore,4u);
    File.read((char*)&Timer,4u);
    File.read((char*)&Shots,4u);
    oX=X;
    oY=Y;
    EvalMaze(Pathing,pairi(X,Y),AggroRadius,{1,3});
    EvalMaze(Sight,pairi(X,Y),SightRadius,{1});
    Bullets.SetText(" Bullets: "+ToString(Shots));
    High.SetText("  High Score: "+ToString(HScore));
    Text.SetText("Score: "+ToString(Score));
}

bool Player::Remove()const{
    return Score<0;
}
