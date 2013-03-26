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
    sf::Key::Code Hotkey;
    char HotKeyChar;
    sf::String HotKeyString;
    bool NotIgnore;
    function<void(sf::Event)> OnPress;
    function<void(sf::Event)> OnRelease;
    bool LeftPressed;
    // Constructor
    Button(sf::Vector2f L,sf::Vector2f S,sf::String text,std::function<void()> onclick,sf::Key::Code HK=sf::Key::Count,char HKC='\0');
    // Destructor
    ~Button();
    // Functions
    virtual void Update(const UIElement::State& CurState,sf::RenderWindow& Window);
    void GetInput(const State& CurState);
    void Draw(sf::RenderWindow& Window);
    void SetText(string text);
    void Resize(sf::Vector2f NewSize);
};
#endif // BUTTON_HPP

