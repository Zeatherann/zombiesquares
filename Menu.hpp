#ifndef MENU_HPP
#define MENU_HPP
class Menu:public UIGroup{
    void UpdateGraphics();
public:
    // Variables
    sf::Vector2f Size;
    sf::Vector3f NextPos;
    sf::Vector2f Location;
    float Edge;
    float Buffer;
    sf::String Title;
    sf::Shape Background; // Auto sizes;...
    // Constructor
    Menu(sf::Vector2f L,sf::Vector2f S,float E,sf::String title,sf::Shape Back,bool Vis=true);
    // Destructor
    ~Menu();
    // Functions
    void AddButton(Button* Child,char Style=0,int ExtraSteps=0);
    void Draw(sf::RenderWindow& Window);
    void Move(sf::Vector2f NewLoc);
    float GetHeight()const;
};
#endif // MENU_HPP

