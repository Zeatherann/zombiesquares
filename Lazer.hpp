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
    void Update(const Terrain& Land);
    void Save(ostream& File)const;
    void Load(istream& File);
    bool Remove()const;
};
#endif // LAZER_HPP
