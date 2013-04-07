#ifndef KEYCLICKABLE
#define KEYCLICKABLE
class KeyClickable:public Clickable{
public:
    // Variables
    sf::Key::Code Key;
    // Constructor
    KeyClickable(const sf::Key::Code& Key=sf::Key::Count);
    // Destructor
    ~KeyClickable();
    // Functions
    virtual bool IsEvent(const sf::Event& EventToCheck);
    virtual void RunEvent(const sf::Event& EventToRun,const UIElement::State& CurState);
    virtual void KeyPress()=0;
    virtual void KeyRelease()=0;
};
#endif // KEYCLICKABLE

