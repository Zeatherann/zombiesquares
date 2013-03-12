#ifndef LAZER_HPP
#define LAZER_HPP
class Lazer:public Entity{
public:
    static vector<Lazer> Lazers;
    // Variables
    set<Enemy*> Hit;
    short Power;
    // Constructor
    static void New(pairi Loc=pairi(0,0),short P=0);
    Lazer(pairi Loc=pairi(0,0),short P=0);
    // Destructor
    ~Lazer();
    // Functions
    void Update();
};
#endif // LAZER_HPP
