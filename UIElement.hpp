#ifndef UIELEMENT_HPP
#define UIELEMENT_HPP
class UIElement{
public:
    // Variables
    sf::Vector2f Location;
    sf::Vector2f Size;
    bool Hover;
    bool LClick;
    bool RClick;
    int MouseWheel;
    // Constructor
    UIElement(sf::Vector2f L,sf::Vector2f S);
    // Destructor
    virtual ~UIElement();
    // Functions
    virtual void GetInput(sf::Event MouseEvent,const sf::Input& State);
    virtual void Draw(sf::RenderWindow& Window)=0;
    virtual void Move(sf::Vector2f NewLoc);
    virtual void Resize(sf::Vector2f NewSize);
};
#endif // UIELEMENT_HPP
