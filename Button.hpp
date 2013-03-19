#ifndef BUTTON_HPP
#define BUTTON_HPP
class Button:public UIElement{
    void UpdateGraphics();
public:
    // Variables
    std::function<void()> OnClick;
    bool OldClick;
    sf::String Text;
    sf::Shape Normal;
    sf::Shape Hovering;
    sf::Shape Pressed;
    // Constructor
    Button(sf::Vector2f L,sf::Vector2f S,sf::String text,std::function<void()> onclick);
    // Destructor
    ~Button();
    // Functions
    void Draw(sf::RenderWindow& Window);
    void SetText(string text);
    void Move(sf::Vector2f NewLoc);
    void Resize(sf::Vector2f NewSize);
};
#endif // BUTTON_HPP

