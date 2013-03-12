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

void Save(string FileName,const Player& Character){
    ofstream File(FileName.c_str(),ios::out|ios::binary);
    Data::Chunk Block=Data::In(Maze.size());
    File.write(Block.first,4u);
    for(const pair<const pairi,char>& Iter:Maze){
        Block=Data::In(Iter.first.first);File.write(Block.first,4u);
        Block=Data::In(Iter.first.second);File.write(Block.first,4u);
        File.write(&Iter.second,1u);
    }
    Character.Save(File);
    Block=Data::In(Lazer::Lazers.size());
    File.write(Block.first,4u);
    for(const Lazer* L:Lazer::Lazers){
        L->Save(File);
    }
    Block=Data::In(Enemy::Enemies.size());
    File.write(Block.first,4u);
    for(const Enemy& E:Enemy::Enemies){
        E.Save(File);
    }
}

void Load(string FileName,Player& Character){
    Maze.clear();
    ifstream File(FileName.c_str(),ios::in|ios::binary);
    unsigned int Size=0u;
    File.read((char*)&Size,4u);
    for(unsigned int i=0u;i<Size;i++){
        pair<pairi,char> Tile;
        File.read((char*)&Tile.first.first,4u);
        File.read((char*)&Tile.first.second,4u);
        File.read((char*)&Tile.second,1u);
        Maze.insert(Tile);
    }
    Character.Load(File);
    File.read((char*)&Size,4u);
    for(unsigned int i=0u;i<Size;i++){
        Lazer* L = new Lazer();
        Lazer::Lazers.push_back(L);
        L->Load(File);
    }
    File.read((char*)&Size,4u);
    for(unsigned int i=0u;i<Size;i++){
        Enemy& E=*Enemy::Enemies.insert(Enemy::Enemies.end(),Enemy());
        E.Load(File);
    }
}

namespace Data{

ifstream& LoadChunk(Chunk& Block,ifstream& File){
    char* Buffer=new char[Block.second];
    File.read(Buffer,Block.second);
    delete Buffer;
    return File;
}

} // namespace Data
