#include "main.hpp"

Scrollable::Scrollable():Clickable(){}

Scrollable::~Scrollable(){}

bool Scrollable::IsEvent(const sf::Event& EventToCheck){
    return EventToCheck.Type==sf::Event::MouseWheelMoved;
}

void Scrollable::RunEvent(const sf::Event& EventToRun,const UIElement::State& State){
    if(PointHovering(State.Mouse))OnScroll(EventToRun.MouseWheel.Delta);
}
