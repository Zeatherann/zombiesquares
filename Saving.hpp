#ifndef SAVING_HPP_INCLUDED
#define SAVING_HPP_INCLUDED

maze LoadMaze(string FileName);
void EvalMaze(maze& Tiles,pairi Tile,int Size,set<char> Blockers);
void Save(string FileName,const Player& Character);
void Load(string FileName,Player& Character);

#endif // SAVING_HPP_INCLUDED
