#ifndef KEYMAP_HPP
#define KEYMAP_HPP
class KeyMap{
public:
    // Variables
    std::map<sf::Key::Code,std::string> Keys;
    std::map<std::string,sf::Key::Code> Names;
    // Constructor
    KeyMap();
    // Destructor
    ~KeyMap();
    // Functions
    inline sf::Key::Code Find(const std::string& Name)const{
        return Names.count(Name)==1?Names.find(Name)->second:sf::Key::Count;
    }
    inline std::string Find(const sf::Key::Code& Key)const{
        return Keys.count(Key)==1?Keys.find(Key)->second:"";
    }
    // Function Pointers
    // Operators
};
#endif // KEYMAP_HPP
