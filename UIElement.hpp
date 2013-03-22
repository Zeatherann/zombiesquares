#ifndef UIELEMENT_HPP
#define UIELEMENT_HPP
class UIElement{
public:
    struct State{
        sf::Vector2f Mouse;
        int MouseDelta;
        const sf::Input& Input;
        State(const sf::Input& I);
    };
    static set<sf::Event::EventType> TrackedEvents;
    // Variables
    sf::Vector2f Location;
    sf::Vector2f Size;
    bool Visible;
    bool NeedUpdate;
    // Constructor
    UIElement(sf::Vector2f Location,sf::Vector2f Size,bool Visible=true);
    // Destructor
    virtual ~UIElement();
    // Functions
    inline bool IsHovering(sf::Vector2f Mouse)const{
        return Mouse.x>Location.x&&Mouse.x<Location.x+Size.x&&Mouse.y>Location.y&&Mouse.y<Location.y+Size.y;
    }
    virtual void Update(const State& CurState,sf::RenderWindow& Window)=0;
};
#endif // UIELEMENT_HPP
