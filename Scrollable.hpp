#ifndef SCROLLABLE_HPP
#define SCROLLABLE_HPP
class Scrollable:public Clickable{
public:
    // Constructor
    Scrollable();
    // Destructor
    ~Scrollable();
    // Functions
    virtual bool IsEvent(const sf::Event& EventToCheck);
    virtual void RunEvent(const sf::Event& EventToRun,const UIElement::State& CurState);
    virtual void OnScroll(int Delta)=0;
};
#endif // SCROLLABLE_HPP
