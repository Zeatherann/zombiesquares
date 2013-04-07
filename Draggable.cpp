#include "main.hpp"

Draggable::Draggable(const sf::Mouse::Button& DragButton):Clickable(),DragButton(DragButton),Dragging(false){}

Draggable::~Draggable(){}

bool Draggable::IsEvent(const sf::Event& EventToCheck){
    sf::Event::EventType Type=EventToCheck.Type;
    return Type==sf::Event::MouseButtonPressed||Type==sf::Event::MouseButtonReleased;
}

void Draggable::RunEvent(const sf::Event& EventToRun,const UIElement::State& State){
    if(EventToRun.MouseButton.Button==DragButton&&PointHovering(State.Mouse)){
        if(EventToRun.Type==sf::Event::MouseButtonPressed){
            Dragging=true;
            OnDrag(sf::Vector2f(Rect.Left,Rect.Top)-State.Mouse);
        }else if(Dragging){
            Dragging=false;
            OnRelease();
        }
    }
}
