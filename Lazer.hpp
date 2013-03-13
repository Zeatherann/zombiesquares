#ifndef LAZER_HPP
#define LAZER_HPP
class Lazer:public Entity{
protected:
    // Destructor
    ~Lazer();
public:
    // Constructor
    Lazer(pairi Loc=pairi(0,0),short P=0);
    // Variables
    set<Enemy*> Hit;
    short Power;
    // Functions
    void Update();
    void Save(ofstream& File)const;
    void Load(ifstream& File);
    bool Remove()const;
};
#endif // LAZER_HPP
