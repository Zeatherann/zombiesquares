#include "main.hpp"

Player* Player::Self;
unordered_map<pairi,int> Player::Pathing;
unordered_map<pairi,int> Player::Sight;
int Player::AggroRadius=25;
int Player::SightRadius=5;

Player::Player(int x,int y):Entity(ct_player,x,y,Colors[ct_player],0),HScore(0),oX(x+1),oY(y+1),Timer(50),Shots(3){}

Player::~Player(){
    if(Self==this)Self=NULL;
}

void Player::Update(const Terrain& Land){
    if(Life>HScore)HScore=Life;
    if(oX!=X||oY!=Y){
        oX=X;
        oY=Y;
        Pathing.clear();
        Sight.clear();
        Land.GeneratePathing(Pathing,AggroRadius,pairi(X,Y),bitset<16>("0010000000100000"));
        Land.GeneratePathing(Sight,SightRadius,pairi(X,Y),bitset<16>("0001000000010000"));
        //EvalMaze(Pathing,pairi(X,Y),AggroRadius,{ct_wall,ct_startwall});
        //EvalMaze(Sight,pairi(X,Y),SightRadius,{ct_wall});
    }
    if(Timer)Timer--;
    else{
        Timer=50;
        if(Shots<3){
            Shots++;
        }
    }
}

bool Player::MoveTo(Terrain& Land,pairi Loc){
    if(!Move(Land,Loc))return false;
    Terrain::Tile* Tile=Land[Loc];
    if(Tile->tile==ct_point){
        Life++;
        Tile->tile=ct_floor;
    }
    if(Tile->tile==ct_startwall){
        set<pairi> Walls;
        Land.GetAdjacentTiles(Walls,Loc);
        for(const pairi& Iter:Walls){
            Land[Iter]->tile=ct_floor;
        }
    }
    return true;
}

void Player::Shoot(Terrain& Land,pairi Direction){
    if(!Shots||Direction==pairi(0,0))return;
    Shots--;
    pairi C(X,Y);
    while(true){
        C=C+Direction;
        if(Maze.count(C)){
            ColorType T=Land[C]->tile;
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

float Player::GetSight(pairi Loc)const{
    unordered_map<pairi,int>::const_iterator Iter=Sight.find(Loc);
    unordered_map<pairi,int>::const_iterator SightEnd=Sight.cend();
    if(Iter!=SightEnd){
        return Iter->second;
    }else{
        int SightValue=-1;
        for(unsigned int i=0u;i<8u;i++){
            pairi P(Loc.first+Adj[i].first,Loc.second+Adj[i].second);
            unordered_map<pairi,int>::const_iterator SightIter=Sight.find(P);
            if(SightIter!=SightEnd&&(SightValue==-1||SightIter->second<SightValue))SightValue=SightIter->second;
        }
        return 1.f-float(SightValue)/float(SightRadius+1);
    }
}

void Player::Save(ostream& File)const{
    Entity::Save(File);
    File.write((char*)&HScore,2u);
    File.write((char*)&Timer,4u);
    File.write((char*)&Shots,4u);
    bool IsSelf=this==Self;
    File.write((char*)&IsSelf,1u);
}

void Player::Load(istream& File){
    Entity::Load(File);
    File.read((char*)&HScore,2u);
    File.read((char*)&Timer,4u);
    File.read((char*)&Shots,4u);
    bool IsSelf=false;
    File.read((char*)&IsSelf,1u);
    if(IsSelf)Self=this;
    oX=!X;
    oY=!Y;
}

bool Player::Remove()const{
    return Life<0;
}
