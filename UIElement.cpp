#include "main.hpp"

UIElement::UIElement(sf::Vector2f L,sf::Vector2f S):Location(L),Size(S),Hover(false),LClick(false),RClick(false),MouseWheel(0){}

UIElement::~UIElement(){}

void UIElement::GetInput(sf::Event MouseEvent,const sf::Input& State){
    sf::Vector2f M(State.GetMouseX(),State.GetMouseY());
    if(M.x>Location.x&&M.x<Location.x+Size.x&&M.y>Location.y&&M.y<Location.y+Size.y){
        if(MouseEvent.Type==sf::Event::MouseWheelMoved){
            MouseWheel=MouseEvent.MouseWheel.Delta;
        }
        Hover=true;
        LClick=State.IsMouseButtonDown(sf::Mouse::Left);
        RClick=State.IsMouseButtonDown(sf::Mouse::Right);
    }else{
        Hover=false;
        LClick=false;
        RClick=false;
        MouseWheel=0;
    }
}

void UIElement::Move(sf::Vector2f NewLoc){
    Location=NewLoc;
}

void UIElement::Resize(sf::Vector2f NewSize){
    Size=NewSize;
}
