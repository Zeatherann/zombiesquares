#ifndef MOUSECLICKABLE_HPP
#define MOUSECLICKABLE_HPP
class MouseClickable:public Clickable{
public:
    // Variables
    sf::Mouse::Button ClickButton;
    // Constructor
    MouseClickable(const sf::Mouse::Button& ClickButton=sf::Mouse::ButtonCount);
    // Destructor
    ~MouseClickable();
    // Functions
    virtual bool IsEvent(const sf::Event& EventToCheck);
    virtual void RunEvent(const sf::Event& EventToRun,const UIElement::State& CurState);
    virtual void OnClick()=0;
    virtual void OnUnclickInside()=0;
    virtual void OnUnclickOutside()=0;
    // Function Pointers
    // Operators
};
#endif // MOUSECLICKABLE_HPP
