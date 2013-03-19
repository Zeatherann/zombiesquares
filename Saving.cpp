#include "main.hpp"

maze LoadMaze(string FileName){
    ifstream File(FileName.c_str());
    maze Ret;
    map<char,char> Cypher={{'X',1},{' ',0},{'*',5}};
    string Line;
    pairi Pt(0,0);
    while(File.good()){
        Pt.first=0;
        getline(File,Line);
        if(Line!="")for(char C:Line){
            Ret[Pt]=(Cypher.count(C)?Cypher[C]:C);
            Pt.first++;
        }
        Pt.second++;
    }
    File.close();
    return Ret;
}

void Save(string FileName){
    ofstream File(FileName.c_str(),ios::out|ios::binary);
    File.write((char*)&GameTime,4u);
    Data::Chunk Block=Data::In(Maze.size());
    File.write(Block.first,4u);
    for(const pair<const pairi,char>& Iter:Maze){
        Block=Data::In(Iter.first.first);File.write(Block.first,4u);
        Block=Data::In(Iter.first.second);File.write(Block.first,4u);
        File.write(&Iter.second,1u);
    }
    // Save Entities
    Block=Data::In(Entity::Entities.size());
    File.write(Block.first,4u);
    for(const Entity* Iter:Entity::Entities){
        Iter->Save(File);
    }
}

void Load(string FileName){
    Maze.clear();
    ifstream File(FileName.c_str(),ios::in|ios::binary);
    File.read((char*)&GameTime,4u);
    cout<<"Game Time: "<<GameTime<<endl;
    unsigned int Size=0u;
    File.read((char*)&Size,4u);
    cout<<"Tiles Loaded: "<<Size<<endl;
    for(unsigned int i=0u;i<Size;i++){
        pair<pairi,char> Tile;
        File.read((char*)&Tile.first.first,4u);
        File.read((char*)&Tile.first.second,4u);
        File.read((char*)&Tile.second,1u);
        Maze.insert(Tile);
    }
    File.read((char*)&Size,4u);
    cout<<"Entities Loaded: "<<Size<<endl;
    for(unsigned int i=0u;i<Size;i++){
        char C=File.peek();
        switch(C){
            case 'E':{// Enemy
                (new Enemy)->Load(File);
                break;
            }case 'L':{// Lazer
                (new Lazer)->Load(File);
                break;
            }case 'P':{// Player
                Player::Character->Load(File);
                break;
            }default:{
                exit(4);
                break;
            }
        }
    }
}
