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
    unsigned int Size=0u;
    File.read((char*)&Size,4u);
    for(unsigned int i=0u;i<Size;i++){
        pair<pairi,char> Tile;
        File.read((char*)&Tile.first.first,4u);
        File.read((char*)&Tile.first.second,4u);
        File.read((char*)&Tile.second,1u);
        Maze.insert(Tile);
    }
    File.read((char*)&Size,4u);
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

vector<pair<string,string>> GetFiles(const path& Path,bool Recurse,const set<string>& FileMasks){
    vector<pair<string,string>> Files;
    if(exists(Path)&&is_directory(Path)){
        for(directory_iterator Iter(Path);Iter!=directory_iterator();Iter++){
            directory_entry Entry=*Iter;
            const path& P=Entry.path();
            if(is_directory(P)){
                if(Recurse)Files+=GetFiles(P,true,FileMasks);
            }else if(is_regular_file(P)){
                string Extension=P.extension().string();
                if(FileMasks.size()==0u||FileMasks.count(Extension)){
                    Files.push_back(pair<string,string>(P.string(),Extension));
                }
            }
        }
    }
    return Files;
}
