#ifndef STATE_HPP
#define STATE_HPP
class State{
public:
    // Variables
    int X;
    int Y;
    Player* Character;
    maze Maze;
    set<Entity*> Entities;
    // Constructor
    State();
    // Destructor
    ~State();
    // Function
    void SaveState(const string& StateFileName);
    void LoadState(const string& StateFileName);
    // Operators
    State& operator=(const State& Copy);
};
#endif // STATE_HPP
