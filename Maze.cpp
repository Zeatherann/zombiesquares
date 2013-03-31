#include "main.hpp"

void NewMaze(){
    Maze.clear();
    int Area=3;
    Entity::Clear();
    GameTime=0;
    for(int a=-Area;a<=Area;a++)for(int b=-Area;b<=Area;b++)Maze[pairi(a,b)]=abs(a)==Area||abs(b)==Area?StartingWall:Floor;
    for(int a=-Area*10;a<=Area*10;a++)for(int b=-Area*10;b<=Area*10;b++)GetTile(Maze,a,b);
    Maze.clear();
    //LoadStructures();
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
        int Rnd=rand()%10;
        if(Rnd==0){
            Enemy::NewFastEnemy(x,y,Power);
        }else if(Rnd==1){
            Enemy::NewSlowEnemy(x,y,Power);
        }else{
            new Enemy(x,y,Power,5,20);
        }
    }
    return Ret;
}

// TODO: Store somewhere.
// TODO: Randomly place.
// TODO: Restrict size.
// TODO? Print error messages instead of just skipping on ahead?
void LoadStructures() {
    path p("Structures");
    if (exists(p) && is_directory(p)) {
        for (directory_iterator iter(p); iter != directory_iterator(); iter++) {
          directory_entry e = *iter;
          string file = e.path().relative_path().string();
          sf::Image Img;
          bool success = Img.LoadFromFile(file);
          if (!success)
            continue;
          unsigned int centerX = Img.GetWidth() / 2u;
          unsigned int centerY = Img.GetHeight() / 2u;
          for (unsigned int i = 0u; i < Img.GetWidth(); i++) {
              for (unsigned int j = 0u; j < Img.GetHeight(); j++) {
                  sf::Color color = Img.GetPixel(i, j);
                  char Tile=GetTile(Maze,i-centerX,j-centerY);
                  if (color == Colors["floor"]) {
                      Maze[pairi(i-centerX,j-centerY)] = Floor;
                  } else if (color == Colors["wall"]) {
                      Tile = Wall;
                  } else if (color == Colors["point"]) {
                      Tile = Point;
                  } else if (color == Colors["zombie"]) {
                      new Enemy(i-centerX, j-centerY);
                  } else if (color == Colors["fast zombie"]) {
                      Enemy::NewFastEnemy(i-centerX, j-centerY, 1);
                  } else if (color == Colors["slow zombie"]) {
                       Enemy::NewSlowEnemy(i-centerX, j-centerY, 1);
                  }
                  Maze[pairi(i-centerX,j-centerY)]=Tile;
              }
           }
        }
    }
}

bool EraseMazeChunk(maze& Tiles,pairi TopLeft,pairi BottomRight){
    function<bool(const Entity*,int,int,int,int)> InRect=[](const Entity* E,int Left,int Top,int Right,int Bottom)->bool{
        if(E->X<Left||E->X>Right||E->Y<Top||E->Y>Bottom)return false;
        return true;
    };
    if(TopLeft.first>=BottomRight.first||TopLeft.second>=BottomRight.second)return false;
    if(Player::Character&&InRect(Player::Character,TopLeft.first,TopLeft.second,BottomRight.first,BottomRight.second))return false;
    for(int x=TopLeft.first;x<=BottomRight.first;x++){
        for(int y=TopLeft.second;y<=BottomRight.second;y++){
            pairi pt(x,y);
            if(Tiles.count(pt))Tiles.erase(pt);
        }
    }
    for(Entity* Iter:Entity::Entities){
        if(Iter->Type!='C'&&InRect(Iter,TopLeft.first,TopLeft.second,BottomRight.first,BottomRight.second))Entity::Delete(Iter);
    }
    return true;
}
