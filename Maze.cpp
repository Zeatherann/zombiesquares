#include "main.hpp"

void NewMaze(){
    Maze.clear();
    int Area=3;
    Entity::Clear();
    GameTime=0;
    for(int a=-Area;a<=Area;a++)for(int b=-Area;b<=Area;b++)Maze[pairi(a,b)]=abs(a)==Area||abs(b)==Area?3:0;
    for(int a=-Area*10;a<=Area*10;a++)for(int b=-Area*10;b<=Area*10;b++)GetTile(Maze,a,b);
}

void EvalMaze(maze& Tiles,pairi Tile,int Size,set<char> Blockers){
    int Step=1;
    Tiles.clear();
    set<pairi> Added={Tile},Old;
    Tiles[Tile]=0;
    pairi P[4]={pairi(-1,0),pairi(1,0),pairi(0,-1),pairi(0,1)};
    while(Step<Size+1&&Added.size()){
        Old=Added;
        Added.clear();
        for(const pairi& p:Old){
            for(unsigned int i=0u;i<4u;i++){
                pairi PP=P[i]+p;
                if(!Blockers.count(GetTile(Maze,PP.first,PP.second))){
                    bool Add=false;
                    if(Tiles.count(PP)){
                        Add=Tiles[PP]>Step;
                    }else{
                        Add=true;
                    }
                    if(Add){
                        Tiles[PP]=Step;
                        Added.insert(PP);
                    }
                }
            }
        }
        Step++;
    }
}

char MakeTile(maze& Tiles,int x,int y){
    char& Ret=(Tiles[pairi(x,y)]=0);
    bool X=(x%2==0);
    bool Y=(y%2==0);
    int R=rand()%10;
    if(R==0)Ret=2;
    else if(R<=1+GameTime/50)Ret=99;
    if(X&&Y){
        Ret=1;
        int rnd=rand()%2;
        if(((y%4)+x)%4==0){
            pairi Loc(x-1,y);
            if(rnd){
                Loc.first+=2;
            }
            char& NT=Tiles[Loc];
            if(NT!=4)NT=1;
        }else{
            pairi Loc(x,y-1);
            if(rnd){
                Loc.second+=2;
            }
            char& NT=Tiles[Loc];
            if(NT!=4)NT=1;
        }
    }
    if(Ret==99){
        Ret=0;
        short Power=1+GameTime/50;
        char Speed=20;
        short Life=5;
        int Rnd=rand()%10;
        sf::Color Tint=sf::Color::Red;
        if(Rnd==0){
            Speed=10;
            Power/=2;
            if(Power<=0)Power=1;
            Tint.b=255;
        }else if(Rnd==1){
            Speed=40;
            Power*=2;
            Tint.g=255;
            Tint.r=128;
            Tint.b=255;
            Life=25;
        }
        (new Enemy(x,y,Power,Life,Speed))->Color=Tint;
    }
    return Ret;
}
