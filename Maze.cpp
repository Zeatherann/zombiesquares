#include "main.hpp"

vector<sf::Image*> Structures;

void NewMaze(){
    Maze.clear();
    int Area=3;
    Entity::Clear();
    GameTime=0;
    for(int a=-Area*10;a<=Area*10;a++)for(int b=-Area*10;b<=Area*10;b++)GetTile(Maze,a,b);
    for(int a=-Area;a<=Area;a++)for(int b=-Area;b<=Area;b++)Maze[pairi(a,b)]=abs(a)==Area||abs(b)==Area?StartingWall:Floor;
}

/**
Creates pathing maps.
*/
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

/**
Generates the random labyrinth, don't question the magic.
Divisible by two is the core of the maze generating algorithm. The nested test for modulo 4 sorts out the vertical and horizontal 'walls'.
    This creates the proper corridors in the maze.
*/
char MakeTile(maze& Tiles,int x,int y){
    char& Ret=(Tiles[pairi(x,y)]=0);
    bool X=(x%2==0);
    bool Y=(y%2==0);
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

    if(Ret==1){
        int R=rand()%1000;
        if(R<100){
            Ret=2;
        }else if(R<=101) {
            cout << "Made a structure @ (" << x << ", " << y << ")" << endl;
            StructurePlaceRandom(pairi(x,y));
            Ret = Maze[pairi(x,y)];
        }else if(R<=102+GameTime/50){
            Ret=0;
            short Power=1+GameTime/50;
            int Rnd=rand()%10;
            if(Rnd==0){
                //Enemy::NewFastEnemy(x,y,Power);
            }else if(Rnd==1){
                //Enemy::NewSlowEnemy(x,y,Power);
            }else{
                //new Enemy(x,y,Power,5,20);
            }
        }
    }
    return Ret;
}

// TODO: Randomly place.
// TODO: Restrict size.
// TODO? Print error messages instead of just skipping on ahead?
/**
Loads structure images from the Structures folder.
*/
void StructuresLoad() {
    path p("Structures");
    if (exists(p) && is_directory(p)) {
        for (recursive_directory_iterator iter(p); iter != recursive_directory_iterator(); iter++) {
            directory_entry e = *iter;
            if (is_regular_file(e)) {
                string file = e.path().relative_path().string();
                sf::Image* Img = new sf::Image();
                bool success = Img->LoadFromFile(file);
                if (!success)
                    continue;
                Structures.push_back(Img);
            }
        }
    }
}

/**
Clears the area the structure occupies.
*/
void StructureEraseBeforePlace(sf::Image* structure, pairi offset) {
    pairi topLeft = offset - pairi(structure->GetWidth(), structure->GetHeight());
    pairi bottomRight = topLeft + pairi(structure->GetWidth(), structure->GetHeight());
    EraseMazeChunk(Maze, topLeft, bottomRight);
}

/**
Places the given structure into the world at the given offset.
*/
void StructurePlace(sf::Image* structure, pairi offset) {
    StructureEraseBeforePlace(structure, offset);
    unsigned int centerX = structure->GetWidth() / 2u + offset.first;
    unsigned int centerY = structure->GetHeight() / 2u + offset.second;
    for (unsigned int i = 0u; i < structure->GetWidth(); i++) {
        for (unsigned int j = 0u; j < structure->GetHeight(); j++) {
          sf::Color color = structure->GetPixel(i, j);
          char Tile=GetTile(Maze,i-centerX,j-centerY);
          if (color == Colors["floor"]) {
              Tile = Floor;
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

/**
Randomly chooses a structure and places it at the given offset.
*/
void StructurePlaceRandom(pairi offset) {
    sf::Image* structure = Structures[rand()%Structures.size()];
    StructurePlace(structure, offset);
}

bool InRect(const Entity* E, int Left, int Top, int Right, int Bottom) {
    if(E->X<Left||E->X>Right||E->Y<Top||E->Y>Bottom)return false;
        return true;
}

bool EraseMazeChunk(maze& Tiles,pairi TopLeft,pairi BottomRight){
    if(TopLeft.first>=BottomRight.first||TopLeft.second>=BottomRight.second)return false;
    if(Player::Character&&InRect(Player::Character,TopLeft.first,TopLeft.second,BottomRight.first,BottomRight.second))return false;
    for(int x=TopLeft.first;x<=BottomRight.first;x++){
        for(int y=TopLeft.second;y<=BottomRight.second;y++){
            pairi pt(x,y);
            Tiles.erase(pt);
        }
    }
    for(Entity* Iter:Entity::Entities){
        if(Iter->Type!='C'&&InRect(Iter,TopLeft.first,TopLeft.second,BottomRight.first,BottomRight.second))Entity::Delete(Iter);
    }
    return true;
}
