#include "main.hpp"

void Save(string FileName){
    ofstream File(FileName.c_str(),ios::out|ios::binary);
    File.write((char*)&GameTime,4u);
    Data::Chunk Block=Data::In(Maze.size());
    File.write(Block.first,4u);
    for(const pair<const pairi,tile>& Iter:Maze){
        Block=Data::In(Iter.first.first);File.write(Block.first,4u);
        Block=Data::In(Iter.first.second);File.write(Block.first,4u);
        File.write(&Iter.second.first,1u);
        File.write((char*)&Iter.second.second,1u);
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
        pair<pairi,tile> Tile;
        File.read((char*)&Tile.first.first,4u);
        File.read((char*)&Tile.first.second,4u);
        File.read((char*)&Tile.second.first,1u);
        File.read((char*)&Tile.second.second,1u);
        Maze.insert(Tile);
    }
    File.read((char*)&Size,4u);
    for(unsigned int i=0u;i<Size;i++){
        ColorType C=ct_invalid;
        File.read((char*)&C,4u);
        File.seekg(-4,File.cur);
        switch(C){
            case ct_zombie:
            case ct_slowzombie:
            case ct_fastzombie:{// Enemy
                (new Enemy)->Load(File);
                break;
            }case ct_lazer:{// Lazer
                (new Lazer)->Load(File);
                break;
            }case ct_player:{// Player
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
