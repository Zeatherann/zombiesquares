#ifndef Draggable_HPP
#define Draggable_HPP
class Draggable:public Clickable{
public:
    // Variables
    sf::Mouse::Button DragButton;
    bool Dragging;
    // Constructor
    Draggable(const sf::Mouse::Button& DragButton=sf::Mouse::ButtonCount);
    // Destructor
    ~Draggable();
    // Functions
    virtual bool IsEvent(const sf::Event& EventToCheck);
    virtual void RunEvent(const sf::Event& EventToRun,const UIElement::State& CurState);
    virtual void OnDrag(const sf::Vector2f& Offset)=0;
    virtual void OnRelease()=0;
};
#endif // Draggable_HPP
