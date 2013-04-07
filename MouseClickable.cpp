#include "main.hpp"

MouseClickable::MouseClickable(const sf::Mouse::Button& ClickButton):Clickable(),ClickButton(ClickButton){}

MouseClickable::~MouseClickable(){}

bool MouseClickable::IsEvent(const sf::Event& EventToCheck){
    sf::Event::EventType Type=EventToCheck.Type;
    return Type==sf::Event::MouseButtonPressed||Type==sf::Event::MouseButtonReleased;
}

void MouseClickable::RunEvent(const sf::Event& EventToRun,const UIElement::State& State){
    if(EventToRun.MouseButton.Button==ClickButton&&PointHovering(State.Mouse)){
        if(EventToRun.Type==sf::Event::MouseButtonPressed){
            OnClick();
        }else{
            OnUnclick();
        }
    }
}
