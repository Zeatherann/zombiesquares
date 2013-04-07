#ifndef BUTTON_HPP
#define BUTTON_HPP
class Button:public UIElement, KeyClickable, MouseClickable {
    void UpdateGraphics();
public:
    // Variables
    std::function<void()> RunOnClick;
    bool OldClick;
    sf::String Text;
    sf::Shape Normal;
    sf::Shape Hovering;
    sf::Shape Pressed;
    sf::Key::Code Hotkey;
    char HotKeyChar;
    sf::String HotKeyString;
    bool NotIgnore;
    bool LeftPressed;
    sf::Shape* ToDraw;
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

    virtual bool IsEvent(const sf::Event& EventToCheck);
    virtual void RunEvent(const sf::Event& EventToRun,const UIElement::State& CurState);
    virtual void KeyPress();
    virtual void KeyRelease();
    virtual void OnClick();
    virtual void OnUnclickInside();
    virtual void OnUnclickOutside();
};
#endif // BUTTON_HPP

