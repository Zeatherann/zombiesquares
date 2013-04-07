#include "main.hpp"

KeyClickable::KeyClickable(const sf::Key::Code& Key):Clickable(),Key(Key){}

KeyClickable::~KeyClickable(){}

bool KeyClickable::IsEvent(const sf::Event& EventToCheck){
    sf::Event::EventType Type=EventToCheck.Type;
    return Type==sf::Event::KeyPressed||Type==sf::Event::KeyReleased;
}

void KeyClickable::RunEvent(const sf::Event& EventToRun,const UIElement::State& State){
    if(EventToRun.Key.Code==Key){
        if(EventToRun.Type==sf::Event::KeyPressed){
            KeyPress();
        }else{
            KeyRelease();
        }
    }
}
