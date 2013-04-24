#include "main.hpp"

vector<sf::Image*> Structures;

void NewMaze(){
    Maze.clear();
    int Area=3;
    Entity::Clear();
    GameTime=0;
    for(int a=-Area;a<=Area;a++)for(int b=-Area;b<=Area;b++)Maze[pairi(a,b)]=abs(a)==Area||abs(b)==Area?tile(ct_startwall,true):tile(ct_floor,true);
}

/**
Creates pathing maps.
*/
//void EvalMaze(maze& Tiles,pairi Tile,int Size,set<ColorType> Blockers){
//    int Step=1;
//    Tiles.clear();
//    set<pairi> Added={Tile},Old;
//    Tiles[Tile].first=ct_floor;
//    pairi P[4]={pairi(-1,0),pairi(1,0),pairi(0,-1),pairi(0,1)};
//    while(Step<Size+1&&Added.size()){
//        Old=Added;
//        Added.clear();
//        for(const pairi& p:Old){
//            for(unsigned int i=0u;i<4u;i++){
//                pairi PP=P[i]+p;
//                if(!Blockers.count(GetTile(Maze,PP.first,PP.second).first)){
//                    bool Add=false;
//                    if(Tiles.count(PP)){
//                        Add=Tiles[PP].first>Step;
//                    }else{
//                        Add=true;
//                    }
//                    if(Add){
//                        Tiles[PP].first=(ColorType)Step;
//                        Added.insert(PP);
//                    }
//                }
//            }
//        }
//        Step++;
//    }
//}
/**
    Returns a ColorType based on the given color.
    This color usually refers to a pixel of a structure image, or a challenge map image.
*/
//ColorType Color2Tile(const sf::Color& Color){
//    ColorType Match=ColorsInv.find(Color)!=ColorsInv.end()?ColorsInv[Color]:ct_invalid;
//    switch(Match){
//        case ct_floor:
//        case ct_wall:
//        case ct_testtile:
//        case ct_point:
//        case ct_invalid:
//            return Match;
//        default: return ct_floor;
//    }
//}
/**
    Returns an ColorType based on the given color.
    This color usually refers to a pixel of a structure image, or a challenge map image.
*/
//ColorType Color2Entity(const sf::Color& Color){
//    ColorType Match=ColorsInv.find(Color)!=ColorsInv.end()?ColorsInv[Color]:ct_invalid;
//    switch(Match){
//        case ct_slowzombie:
//        case ct_fastzombie:
//        case ct_zombie:
//        case ct_player:
//        case ct_lazer:
//            return Match;
//        default: return ct_floor;
//    }
//}
// TODO: Randomly place. DONE
// TODO: Restrict size.
// TODO? Print error messages instead of just skipping on ahead?
/**
    Placed a given structure onto the tilemap.
    @param const sf::Image& Struct: This is the structure image to place onto the tilemap.
    @param maze& Tiles: This is the tilemap to place the structure in.
    @param const pairi& Coord: The location to place the structure.
        @note Coord refers to the top left corner of the image.
*/
//void PasteBuilding(const sf::Image& Struct,maze& Tiles,const pairi& Coord){
//    pairi Size(Struct.GetWidth(),Struct.GetHeight());
//    map<pairi,set<Entity*>> Ents;
//    // Pre-generate area
//    for(int x=0;x<Size.first;x++){
//        for(int y=0;y<Size.second;y++){
//            pairi Pt(x,y);
//            pairi Loc=Pt+Coord;
//            GetTile(Tiles,Loc.first,Loc.second);
//        }
//    }
//    // Populate Entity Cells
//    for(Entity* Iter:Entity::Entities){
//        if(Iter->X<Coord.first||Iter->X>Coord.first+Size.first||Iter->Y<Coord.second||Iter->Y>Coord.second+Size.second)continue;
//        Ents[pairi(Iter->X,Iter->Y)].insert(Iter);
//    }
//    // Place Structure
//    for(int x=0;x<Size.first;x++){
//        for(int y=0;y<Size.second;y++){
//            pairi Pt(x,y);
//            pairi Loc=Pt+Coord;
//            sf::Color PixelColor=Struct.GetPixel(x,y);
//            ColorType TileToPlace=Color2Tile(PixelColor);
//            // Check if tile is generated or placed.
//            if(TileToPlace!=ct_invalid&&!Tiles[Loc].second){
//                // Place Tile
//                Tiles[Loc]=tile(TileToPlace,true);
//                // Erase Pre-existing Entities
//                if(Ents.count(Loc)){
//                    for(Entity* Iter:Ents[Loc]){
//                        if(Iter->Type!=ct_player)Entity::Delete(Iter);
//                    }
//                }
//                // Place New Entity
//                switch(Color2Entity(PixelColor)){
//                    case ct_zombie:{
//                        new Enemy(Loc.first,Loc.second);
//                        break;
//                    }case ct_fastzombie:{
//                        Enemy::NewFastEnemy(Loc.first,Loc.second,1);
//                        break;
//                    }case ct_slowzombie:{
//                        Enemy::NewSlowEnemy(Loc.first,Loc.second,1);
//                        break;
//                    }default:break;
//                }
//            }
//        }
//    }
//}
/**
Generates the random labyrinth, don't question the magic.
Divisible by two is the core of the maze generating algorithm. The nested test for modulo 4 sorts out the vertical and horizontal 'walls'.
    This creates the proper corridors in the maze.
*/
//tile MakeTile(maze& Tiles,int x,int y){
//    tile& Tile=Tiles[pairi(x,y)]=tile(ct_floor,false);
//    ColorType& Ret=Tile.first;
//    bool X=(x%2==0);
//    bool Y=(y%2==0);
//    if(X&&Y){
//        if(rand()%300==0){
//            cout << "Made a structure @ (" << x << ", " << y << ")" << endl;
//            const sf::Image& Struct=*Structures[rand()%Structures.size()];
//            PasteBuilding(Struct,Maze,pairi(x-Struct.GetWidth()/2,y-Struct.GetHeight()/2));
//        }else{
//            Ret=ct_wall;
//            int rnd=rand()%2;
//            pairi Loc(x,y);
//            if(((y%4)+x)%4==0){
//                Loc.first--;
//                if(rnd)Loc.first+=2;
//            }else{
//                Loc.second--;
//                if(rnd)Loc.second+=2;
//            }
//            if(!Tiles.count(Loc)||!Tiles[Loc].second){
//                Tiles[Loc]=tile(ct_wall,false);
//            }
//        }
//    }
//    if(Ret==ct_floor){
//        int R=rand()%10;
//        short Power=1+GameTime/50;
//        if(R==0){
//            Ret=ct_point;
//        }else if(R<=Power){
//            int Rnd=rand()%10;
//            if(Rnd==0){
//                Enemy::NewFastEnemy(x,y,Power);
//            }else if(Rnd==1){
//                Enemy::NewSlowEnemy(x,y,Power);
//            }else{
//                new Enemy(x,y,Power,5,20);
//            }
//        }
//    }
//    return Tile;
//}

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
