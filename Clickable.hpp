#ifndef CLICKABLE_HPP
#define CLICKABLE_HPP
class Clickable{
public:
    static set<Clickable*> Clickables;
    sf::FloatRect Rect;
    // Constructor
    Clickable(const sf::FloatRect& rect=sf::FloatRect(0,0,0,0));
    // Destructor
    ~Clickable();
    // Functions
    virtual bool IsEvent(const sf::Event& EventToCheck)=0;
    virtual void RunEvent(const sf::Event& EventToRun,const UIElement::State& CurState)=0;
    bool inline PointHovering(const sf::Vector2f Point){
        return Rect.Contains(Point.x,Point.y);
    }
};
#endif // CLICKABLE_HPP
