#include "main.hpp"

vector<sf::Image*> Structures;

void NewMaze(){
    Maze.clear();
    int Area=3;
    Entity::Clear();
    GameTime=0;
    for(int a=-Area;a<=Area;a++)for(int b=-Area;b<=Area;b++)Maze[pairi(a,b)]=abs(a)==Area||abs(b)==Area?tile(StartingWall,true):tile(Floor,true);
}

/**
Creates pathing maps.
*/
void EvalMaze(maze& Tiles,pairi Tile,int Size,set<char> Blockers){
    int Step=1;
    Tiles.clear();
    set<pairi> Added={Tile},Old;
    Tiles[Tile].first=0;
    pairi P[4]={pairi(-1,0),pairi(1,0),pairi(0,-1),pairi(0,1)};
    while(Step<Size+1&&Added.size()){
        Old=Added;
        Added.clear();
        for(const pairi& p:Old){
            for(unsigned int i=0u;i<4u;i++){
                pairi PP=P[i]+p;
                if(!Blockers.count(GetTile(Maze,PP.first,PP.second).first)){
                    bool Add=false;
                    if(Tiles.count(PP)){
                        Add=Tiles[PP].first>Step;
                    }else{
                        Add=true;
                    }
                    if(Add){
                        Tiles[PP].first=Step;
                        Added.insert(PP);
                    }
                }
            }
        }
        Step++;
    }
}
/**
    Returns a TileType based on the given color.
    This color usually refers to a pixel of a structure image, or a challenge map image.
*/
TileType Color2Tile(const sf::Color& Color){
    if(Color==Colors["floor"])return Floor;
    if(Color==Colors["wall"])return Wall;
    if(Color==Colors["purplewall"])return PurpleWall;
    if(Color==Colors["point"])return Point;
    return Invalid;
}
/**
    Returns an EntityType based on the given color.
    This color usually refers to a pixel of a structure image, or a challenge map image.
*/
EntityType Color2Entity(const sf::Color& Color){
    if(Color==Colors["zombie"])return Zombie;
    if(Color==Colors["fast zombie"])return FastZombie;
    if(Color==Colors["slow zombie"])return SlowZombie;
    return InvalidEntity;
}
/**
    Placed a given structure onto the tilemap.
    @param const sf::Image& Struct: This is the structure image to place onto the tilemap.
    @param maze& Tiles: This is the tilemap to place the structure in.
    @param const pairi& Coord: The location to place the structure.
        @note Coord refers to the top left corner of the image.
*/
void PasteBuilding(const sf::Image& Struct,maze& Tiles,const pairi& Coord){
    pairi Size(Struct.GetWidth(),Struct.GetHeight());
    map<pairi,set<Entity*>> Ents;
    // Pre-generate area
    for(int x=0;x<Size.first;x++){
        for(int y=0;y<Size.second;y++){
            pairi Pt(x,y);
            pairi Loc=Pt+Coord;
            GetTile(Tiles,Loc.first,Loc.second);
        }
    }
    // Populate Entity Cells
    for(Entity* Iter:Entity::Entities){
        if(Iter->X<Coord.first||Iter->X>Coord.first+Size.first||Iter->Y<Coord.second||Iter->Y>Coord.second+Size.second)continue;
        Ents[pairi(Iter->X,Iter->Y)].insert(Iter);
    }
    // Place Structure
    for(int x=0;x<Size.first;x++){
        for(int y=0;y<Size.second;y++){
            pairi Pt(x,y);
            pairi Loc=Pt+Coord;
            sf::Color PixelColor=Struct.GetPixel(x,y);
            TileType TileToPlace=Color2Tile(PixelColor);
            // Check if tile is generated or placed.
            if(TileToPlace!=Invalid&&!Tiles[Loc].second){
                // Place Tile
                Tiles[Loc]=tile(TileToPlace,true);
                // Erase Pre-existing Entities
                if(Ents.count(Loc)){
                    for(Entity* Iter:Ents[Loc]){
                        if(Iter->Type!='P')Entity::Delete(Iter);
                    }
                }
                // Place New Entity
                switch(Color2Entity(PixelColor)){
                    case Zombie:{
                        new Enemy(Loc.first,Loc.second);
                        break;
                    }case FastZombie:{
                        Enemy::NewFastEnemy(Loc.first,Loc.second,1);
                        break;
                    }case SlowZombie:{
                        Enemy::NewSlowEnemy(Loc.first,Loc.second,1);
                        break;
                    }default:break;
                }
            }
        }
    }
}
/**
Generates the random labyrinth, don't question the magic.
Divisible by two is the core of the maze generating algorithm. The nested test for modulo 4 sorts out the vertical and horizontal 'walls'.
    This creates the proper corridors in the maze.
*/
tile MakeTile(maze& Tiles,int x,int y){
    tile& Tile=Tiles[pairi(x,y)]=tile(Floor,false);
    char& Ret=Tile.first;
    bool X=(x%2==0);
    bool Y=(y%2==0);
    if(X&&Y){
        if(rand()%300==0){
            cout << "Made a structure @ (" << x << ", " << y << ")" << endl;
            const sf::Image& Struct=*Structures[rand()%Structures.size()];
            PasteBuilding(Struct,Maze,pairi(x-Struct.GetWidth()/2,y-Struct.GetHeight()/2));
        }else{
            Ret=1;
            int rnd=rand()%2;
            pairi Loc(x,y);
            if(((y%4)+x)%4==0){
                Loc.first--;
                if(rnd)Loc.first+=2;
            }else{
                Loc.second--;
                if(rnd)Loc.second+=2;
            }
            if(!Tiles.count(Loc)||!Tiles[Loc].second){
                Tiles[Loc]=tile(Wall,false);
            }
        }
    }
    if(Ret==0){
        int R=rand()%10;
        if(R==0){
            Ret=2;
        }else if(R<1+GameTime/50){
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
    return Tile;
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
          char Tile=GetTile(Maze,i-centerX,j-centerY).first;
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
          Maze[pairi(i-centerX,j-centerY)].first=Tile;
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
