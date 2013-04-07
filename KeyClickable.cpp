#include "main.hpp"

KeyClickable::KeyClickable(const sf::Key::Code& Key):Clickable(),Key(Key){}

KeyClickable::~KeyClickable(){}

bool KeyClickable::IsEvent(const sf::Event& EventToCheck){
    sf::Event::EventType Type=EventToCheck.Type;
    if (Type==sf::Event::KeyPressed||Type==sf::Event::KeyReleased) {
        return EventToCheck.Key.Code == Key;
    } else {
        return false;
    }
}

void KeyClickable::RunEvent(const sf::Event& EventToRun,const UIElement::State& State){
    if(EventToRun.Type==sf::Event::KeyPressed){
        KeyPress();
    }else{
        KeyRelease();
    }
}
