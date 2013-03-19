#include "main.hpp"

Button::Button(sf::Vector2f L,sf::Vector2f S,sf::String text,std::function<void()> onclick):UIElement(L,S),OnClick(onclick),OldClick(false),Text(text),Normal(sf::Shape::Rectangle(L,L+S,sf::Color(128,128,128))),Hovering(Normal),Pressed(Normal){
    UpdateGraphics();
}

Button::~Button(){}

void Button::UpdateGraphics(){
    Normal.SetPointPosition(0,Location.x       ,Location.y);
    Normal.SetPointPosition(1,Location.x+Size.x,Location.y);
    Normal.SetPointPosition(2,Location.x+Size.x,Location.y+Size.y);
    Normal.SetPointPosition(3,Location.x       ,Location.y+Size.y);
    Pressed.SetPointPosition(0,Location.x       ,Location.y);
    Pressed.SetPointPosition(1,Location.x+Size.x,Location.y);
    Pressed.SetPointPosition(2,Location.x+Size.x,Location.y+Size.y);
    Pressed.SetPointPosition(3,Location.x       ,Location.y+Size.y);
    Hovering.SetPointPosition(0,Location.x       ,Location.y);
    Hovering.SetPointPosition(1,Location.x+Size.x,Location.y);
    Hovering.SetPointPosition(2,Location.x+Size.x,Location.y+Size.y);
    Hovering.SetPointPosition(3,Location.x       ,Location.y+Size.y);
    Text.SetPosition(Location.x+Size.x*0.5f,Location.y+Size.y*0.5f);
    sf::FloatRect TRec=Text.GetRect();
    Text.SetCenter(TRec.GetWidth()*0.5f,TRec.GetHeight()*0.5f);
}

void Button::Draw(sf::RenderWindow& Window){
    if(Hover){
        if(LClick){
            Window.Draw(Pressed);
            OldClick=true;
        }else{
            Window.Draw(Hovering);
            if(OldClick&&OnClick){
                OnClick();
                OldClick=false;
            }
        }
    }else{
        OldClick=false;
        Window.Draw(Normal);
    }
    Window.Draw(Text);
}

void Button::SetText(string text){
    Text.SetText(text);
    Text.SetPosition(Location.x+Size.x*0.5f,Location.y+Size.y*0.5f);
    sf::FloatRect TRec=Text.GetRect();
    Text.SetCenter(TRec.GetWidth()*0.5f,TRec.GetHeight()*0.5f);
}

void Button::Move(sf::Vector2f NewLoc){
    UIElement::Move(NewLoc);
    UpdateGraphics();
}

void Button::Resize(sf::Vector2f NewSize){
    UIElement::Resize(NewSize);
    UpdateGraphics();
}
