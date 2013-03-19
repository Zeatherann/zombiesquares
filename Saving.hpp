#ifndef SAVING_HPP_INCLUDED
#define SAVING_HPP_INCLUDED

maze LoadMaze(string FileName);
void Save(string FileName);
void Load(string FileName);
inline bool FileExists(const std::string FileName){return ifstream(FileName.c_str());}

#endif // SAVING_HPP_INCLUDED
