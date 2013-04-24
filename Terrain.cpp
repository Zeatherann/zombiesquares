#include "main.hpp"
// Terrain

Terrain::Terrain(const sf::Image& Map):World(),Map(Map),GeneratingStructure(false),Omnisight(false){
    unsigned int Width=Map.GetWidth();
    unsigned int Height=Map.GetHeight();
    int CenterX=int(Width+1u)/2;
    int CenterY=int(Height+1u)/2;
    for(unsigned int X=0u;X<Width;X++){
        for(unsigned int Y=0u;Y<Height;Y++){
            sf::Color Pixel=Map.GetPixel(X,Y);
            pairi Location(X-CenterX,Y-CenterY);
            ColorType Type=ColorsInv[Pixel];
            if(Type==ct_invalid){
                operator[](Location);
            }else{
                World[Location]=new Terrain::Tile(Type,Location);
            }
        }
    }
}

Terrain::Terrain(istream& stream):World(),Map(),GeneratingStructure(false),Omnisight(false){
    // Terrain Header (Map Size/Pixels, World Size)
    unsigned int Width=0u;
    unsigned int Height=0u;
    stream.read((char*)&Width,4u);
    stream.read((char*)&Height,4u);
    unsigned int BufferSize=Width*Height*4u;
    unsigned char* PixelBuffer=new unsigned char[BufferSize];
    stream.read((char*)PixelBuffer,BufferSize);
    Map.LoadFromPixels(Width,Height,PixelBuffer);
    delete PixelBuffer;
    unsigned int WorldSize=0u;
    stream.read((char*)&WorldSize,4u);
    for(unsigned int i=0u;i<WorldSize;i++){
        int x=0;
        int y=0;
        stream.read((char*)&x,4u);
        stream.read((char*)&y,4u);
        World[pairi(x,y)]=new Terrain::Tile(stream);
    }
}

Terrain::~Terrain(){}

void Terrain::GeneratePathing(unordered_map<pairi,int>& Map,int Radius,pairi Start,bitset<16> Flags)const{
    set<pairi> OpenNodes({Start});
    set<pairi> ClosedNodes;
    int Range=0;
    while(OpenNodes.size()>0u&&Range<=Radius){
        set<pairi> LoopNodes(OpenNodes);
        ClosedNodes.insert(OpenNodes.begin(),OpenNodes.end());
        OpenNodes.clear();
        for(const pairi& Iter:LoopNodes){
            Terrain::WorldType::const_iterator TileIter=World.find(Iter);
            if(TileIter!=World.end()){
                // Check Flags
                bitset<8> TileFlags=TileIter->second->Flags;
                bool Works=true;
                for(unsigned int Index=0u;Index<8u;Index++){
                    if(Flags[Index+8u]==true&&Flags[Index]!=TileFlags[Index])Works=false;
                }
                // If it works, add it to Map and add the 4 adjacent too.
                if(Works){
                    Map[Iter]=Range;
                    int X=Iter.first;
                    int Y=Iter.second;
                    OpenNodes.insert(pairi(X+1,Y-1));
                    OpenNodes.insert(pairi(X+1,Y+1));
                    OpenNodes.insert(pairi(X-1,Y+1));
                    OpenNodes.insert(pairi(X-1,Y-1));
                    OpenNodes.erase(ClosedNodes.begin(),ClosedNodes.end());
                }
            }
        }
        Range++;
    }
}

void Terrain::GetAdjacentTiles(set<pairi>& Return,const pairi& Start)const{
    set<pairi> OpenNodes({Start});
    Terrain::WorldType::const_iterator StartIter=World.find(Start);
    Terrain::WorldType::const_iterator TileEnd=World.end();
    ColorType MatchedType=ct_invalid;
    if(StartIter!=TileEnd)MatchedType=StartIter->second->tile;
    else return;
    set<pairi> ClosedNodes;
    while(OpenNodes.size()!=0u){
        set<pairi> LoopNodes(OpenNodes);
        ClosedNodes.insert(OpenNodes.begin(),OpenNodes.end());
        OpenNodes.clear();
        for(const pairi& Iter:LoopNodes){
            Terrain::WorldType::const_iterator TileIter=World.find(Iter);
            if(TileIter!=TileEnd){
                // If it works, add it to Map and add the 4 adjacent too.
                if(TileIter->second->tile==MatchedType){
                    Return.insert(Iter);
                    int X=Iter.first;
                    int Y=Iter.second;
                    OpenNodes.insert(pairi(X+1,Y-1));
                    OpenNodes.insert(pairi(X+1,Y+1));
                    OpenNodes.insert(pairi(X-1,Y+1));
                    OpenNodes.insert(pairi(X-1,Y-1));
                    OpenNodes.erase(ClosedNodes.begin(),ClosedNodes.end());
                }
            }
        }
    }
}

void Terrain::Save(ostream& stream)const{
    unsigned int Width=Map.GetWidth();
    unsigned int Height=Map.GetHeight();
    stream.write((char*)&Width,4u);
    stream.write((char*)&Height,4u);
    stream.write((char*)Map.GetPixelsPtr(),Width*Height*4u);
    unsigned int WorldSize=World.size();
    stream.write((char*)&WorldSize,4u);
    for(const pair<const pairi,Terrain::Tile*>& Iter:World){
        stream.write((char*)&Iter.first.first,4u);
        stream.write((char*)&Iter.first.second,4u);
        Iter.second->Save(stream);
    }
}

Terrain::Tile* Terrain::GenerateRandomTile(pairi Location){
    Debug
    // Get tile pointer from the world map.
    Terrain::Tile* Ret=World[Location];
    // If the tile isn't NULL, return it.
    if(Ret!=NULL){return Ret;}
    // Get ChallengeMap Width/Height/Center
    int Width=Map.GetWidth();
    int Height=Map.GetHeight();
    int CenterX=int(Width+1u)/2;
    int CenterY=int(Height+1u)/2;
    // Convert world coordinates to image coordinates.
    int ImgX=CenterX+Location.first;
    int ImgY=CenterY+Location.second;
    // If image coordinates are invalid (past the edge) move them to the edge(s) they are past.
    if(ImgX<0)ImgX=0;
    else if(ImgX>=Width)ImgX=Width-1;
    if(ImgY<0)ImgY=0;
    else if(ImgY>=Height)ImgY=Height-1;
    // If the image pixel is not invalid (fully opaque) then return a new tile based on that pixel.
    sf::Color Color=Map.GetPixel(ImgX,ImgY);
    if(Color.a==255){
        Ret=new Terrain::Tile(GetColor(Color),Location);
    }else{
        // If the image pixel is invalid (transparent) then generate a random tile based on the location.
        /* Generate random tile!
            1: Get the tile pointer in the world map.
            2: If a tile exists and is not NULL, return it.
        */
        int X=Location.first;
        int Y=Location.second;
        bool X_Node=(X%2==0);
        bool Y_Node=(Y%2==0);
        if(X_Node&&Y_Node){
            if(rand()%300==0){
                cout<<"Made a structure at "<<Location<<endl;
                const sf::Image& Struct=*Structures[rand()%Structures.size()];
                PlaceStructureMap(Struct,Location);
            }else{
                Ret=new Terrain::Tile(ct_wall);
                int rnd=rand()%2;
                if(((Y%4)+X)%4==0){
                    Location.first--;
                    if(rnd)Location.first+=2;
                }else{
                    Location.second--;
                    if(rnd)Location.second+=2;
                }
                Terrain::Tile* WallTile=World[Location];
                if(WallTile==NULL||WallTile->Flags[0]==false){
                    delete WallTile;
                    WallTile=new Terrain::Tile(ct_wall);
                }
            }
        }else{
            int R=rand()%10;
            if(R==0){
                Ret=new Terrain::Tile(ct_point);
            }else if(R<=1+GameTime/50){
                int Rnd=rand()%10;
                ColorType ZombieType=ct_zombie;
                if(Rnd==0){
                    ZombieType=ct_fastzombie;
                }else if(Rnd==1){
                    ZombieType=ct_slowzombie;
                }
                Ret=new Terrain::Tile(ZombieType,Location);
            }
        }
    }
    return Ret;
}

void Terrain::PlaceStructureMap(const sf::Image& Structure,const pairi& Location){
    // If we're already placing a structure, ignore the function call.
    if(GeneratingStructure)return;
    // Signify we're generating a structure to avoid an infinite loop with extremely large structures.
    GeneratingStructure=true;
    // Get the width/height/center of the structure image.
    unsigned int Width=Structure.GetWidth();
    unsigned int Height=Structure.GetHeight();
    unsigned int CenterX=(Width+1u)/2u;
    unsigned int CenterY=(Height+1u)/2u;
    // This boolean value determines if we place the structure or not.
    bool Place=true;
    // Loop over the area that the structure will be placed.
    for(unsigned int X=0u;X<Width;X++){
        for(unsigned int Y=0u;Y<Height;Y++){
            // Get corresponding pixel in the image.
            sf::Color Pixel=Structure.GetPixel(X,Y);
            // Get the colortype of the pixel.
            ColorType Type=GetColor(Pixel);
            // if the pixel contains a valid color,
            if(!(Type==ct_invalid&&Pixel.a==0)){
                pairi Loc=pairi(int(X)-int(CenterX),int(Y)-int(CenterY))+Location;
                Terrain::Tile* Tile=operator[](Loc);
                if(Tile!=NULL&&Tile->Flags[0]==true/*||Player.InSight(Loc)*/)Place=false;
            }
        }
    }
    if(!Place){
        // Invalid location for this structure; abort the function call.
        GeneratingStructure=false;
        return;
    }
    // Place the structure.
    for(unsigned int X=0u;X<Width;X++){
        for(unsigned int Y=0u;Y<Height;Y++){
            sf::Color Pixel=Structure.GetPixel(X,Y);
            ColorType Type=GetColor(Pixel);
            if(!(Type==ct_invalid&&Pixel.a==0)){
                pairi Loc=pairi(int(X)-int(CenterX),int(Y)-int(CenterY))+Location;
                World[Loc]=new Terrain::Tile(Type,Loc);
            }
        }
    }
    GeneratingStructure=false;
}

void Terrain::Draw(pairi ViewCenter){
    int SR=Player::SightRadius+1;
    int X=ViewCenter.first;
    int Y=ViewCenter.second;
    for(int x=X-SR;x<=X+SR;x++){
        for(int y=Y-SR;y<=Y+SR;y++){
            pairi Loc(x,y);
            // Change Player::InSight to take a Terrain& as well
            // Get iterator
            if(Omnisight||Player::Self->InSight(Loc)){
                // Get tile
//                Terrain::WorldType::iterator TileIter=World.find(Loc);
//                if(TileIter==World.end())continue;
//                Terrain::Tile& Tile=TileIter->second;
                Terrain::Tile* Tile=World[Loc];
                if(Tile==NULL)continue;
                // Get alpha (degree of black)
                float Alpha=1.f;
                if(!Omnisight&&Player::Self){
                    Alpha=Player::Self->GetSight(Loc);// returns 0.f to 1.f
                }
                // Get tile square coords.
                float Min_X=x*TileSize;
                float Min_Y=y*TileSize;
                float Max_X=Min_X+TileSize-1;
                float Max_Y=Min_Y+TileSize-1;
                // Bind texture
                if(Tile->Entities.size()!=0u){
                    // Bind Entity Texture
                    Entity* FirstEnt=*(Tile->Entities.begin());
                    if(FirstEnt!=NULL)TileImages[FirstEnt->Type].Bind();
                    else TileImages[Tile->tile].Bind();
                }else{
                    // Bind Terrain Texture
                    TileImages[Tile->tile].Bind();
                }
                //TileImages[(Tile->Entities.size()!=0u)?(*Tile->Entities.begin())->Type:Tile->tile].Bind();
                // Begin quads
                glBegin(GL_QUADS);
                // Draw
                glColor4f(Alpha,Alpha,Alpha,1);
                glTexCoord2f(0,0);glVertex2f(Min_X,Min_Y);
                glTexCoord2f(0,1);glVertex2f(Min_X,Max_Y);
                glTexCoord2f(1,1);glVertex2f(Max_X,Max_Y);
                glTexCoord2f(1,0);glVertex2f(Max_X,Min_Y);
                // Cleanup
                glEnd();
            }
        }
    }
}

Terrain::Tile* Terrain::operator[](const pairi& Location){
    Terrain::WorldType::iterator Iter=World.find(Location);
    if(Iter!=World.end())return Iter->second;
    return GenerateRandomTile(Location);
}

/// Terrain::Tile

Terrain::Tile::Tile(ColorType Type,set<Entity*> Entities):tile(Type),Flags(),Entities(Entities){
    /* Flags for the tile, defined below:
        0: Built. True means placed by a structure, false means randomly placed.
        1: Walkable. True means entities can walk upon this (Enemy/Player), false means they can't move into this tile, and die if placed on it.
        2: Aggroable. True means players' aggro pathing is not blocked by this tile, false means enemies can 'see' a player(s) through this tile.
        3: Seeable. True means player's can see past this tile, false means it blocks sight.
        4: Shootable. True means player's lazers can fire through this tile, false means it blocks lazers.
        5: UNUSED
        6: UNUSED
        7: UNUSED
    */
    string flags;
    ColorType TestType=Type;
    switch(TestType){
        case ct_slowzombie:
        case ct_zombie:
        case ct_fastzombie:
        case ct_lazer:
        case ct_player:
        case ct_floor:
            Type=ct_floor;
            flags="01111000";
            break;
        case ct_point:
        case ct_testtile:
            flags="01111000";
            break;
        case ct_startwall:
            flags="01010000";
            break;
        case ct_wall:
        case ct_invalid:
            flags="00000000";
            break;
    }
    Flags=bitset<8>(flags);
}

Terrain::Tile::Tile(istream& stream):tile(),Flags(),Entities(){
    stream.read((char*)&tile,4u);
    stream.read((char*)&Flags,1u);
    // New Code
    unsigned int NumEntities=0u;
    stream.read((char*)&NumEntities,4u);
    if(NumEntities!=0u)for(unsigned int Index=0u;Index<NumEntities;Index++){
        Entities.insert(Entity::StaticLoad(stream));
    }
}

Terrain::Tile::Tile(ColorType Type,const pairi& Location):tile(),Flags(),Entities(){
    /* Flags for the tile, defined below:
        0: Built. True means placed by a structure, false means randomly placed.
        1: Walkable. True means entities can walk upon this (Enemy/Player), false means they can't move into this tile, and die if placed on it.
        2: Aggroable. True means players' aggro pathing is not blocked by this tile, false means enemies can 'see' a player(s) through this tile.
        3: Seeable. True means player's can see past this tile, false means it blocks sight.
        4: Shootable. True means player's lazers can fire through this tile, false means it blocks lazers.
        5: UNUSED
        6: UNUSED
        7: UNUSED
    */
    int X=Location.first;
    int Y=Location.second;
    short Power=short(GameTime)/50+1;
    // Check Entities
    bool Convert2Floor=true;
    switch(Type){
        case ct_fastzombie:
            Entities.insert(Enemy::NewFastEnemy(X,Y,Power));
            break;
        case ct_lazer:
            Entities.insert(new Lazer(Location,3));
            break;
        case ct_player:
            if(Player::Self==NULL)Entities.insert(Player::Self=new Player(X,Y));
            break;
        case ct_slowzombie:
            Entities.insert(Enemy::NewSlowEnemy(X,Y,Power));
            break;
        case ct_zombie:
            Entities.insert(new Enemy(X,Y,Power));
            break;
        default:Convert2Floor=false;
    }
    if(Convert2Floor){Type=ct_floor;}
    // Check Tiles
    string flags="01111000";
    switch(Type){
        case ct_wall:
        case ct_invalid:
            flags="00000000";
            break;
        case ct_startwall:
            flags="01010000";
            break;
        default:;
    }
    Flags=bitset<8>(flags);
    tile=Type;
}

Terrain::Tile::~Tile(){
    if(Entities.size()!=0u){
        for(set<Entity*>::iterator Iter=Entities.begin();Iter!=Entities.end();){
            delete *(Iter++);
        }
    }
}

void Terrain::Tile::Save(ostream& stream)const{
    stream.write((char*)&tile,4u);
    stream.write((char*)&Flags,1u);
    unsigned int NumEntities=Entities.size();
    stream.write((char*)&NumEntities,4u);
    if(NumEntities!=0u)for(const Entity* Iter:Entities){
        Iter->Save(stream);
    }
}
