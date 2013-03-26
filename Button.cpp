#include "main.hpp"

Button::Button(sf::Vector2f L,sf::Vector2f S,sf::String text,std::function<void()> onclick,sf::Key::Code HK,char HKC):UIElement(L,S),OnClick(onclick),OldClick(false),Text(text),Normal(sf::Shape::Rectangle(L,L+S,sf::Color(128,128,128))),Hovering(Normal),Pressed(Normal),Hotkey(HK),HotKeyChar(HKC),HotKeyString(text),NotIgnore(true),LeftPressed(false){
    UpdateGraphics();
    OnPress = [&](sf::Event event, const UIElement::State& CurState) {
        if (!Visible || (Owner && !Owner->Visible))
            return;
        if (event.MouseButton.Button == sf::Mouse::Left) {
            LeftPressed = true;
            if(NotIgnore){
                if(IsHovering(CurState.Mouse)){
                    if(LeftPressed){
                        OldClick=true;
                        NotIgnore=false;
                    }
                }else{
                    OldClick=false;
                }
            }else{
                NotIgnore=true;
            }
        }
    };
    OnRelease = [&](sf::Event event, const UIElement::State&) {
        if (!Visible || (Owner && !Owner->Visible))
            return;
        if (event.MouseButton.Button == sf::Mouse::Left) {
            LeftPressed = false;
            if(OldClick&&OnClick){
                OnClick();
                OldClick=false;
            }
        }
    };
    EventSubscriptions::subscribe(this, sf::Event::MouseButtonPressed, OnPress);
    EventSubscriptions::subscribe(this, sf::Event::MouseButtonReleased, OnRelease);
}

Button::~Button(){
    EventSubscriptions::unsubscribe(this, sf::Event::MouseButtonPressed);
    EventSubscriptions::unsubscribe(this, sf::Event::MouseButtonReleased);
}

void Button::UpdateGraphics(){
    NeedUpdate=false;
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
    Text.SetSize(TileSize);
    while(Text.GetRect().GetWidth()>Size.x){
        Text.SetSize(Text.GetSize()-1.f);
    }
    Text.SetCenter(Text.GetRect().GetWidth()*0.5f,Text.GetRect().GetHeight()*0.5f);
    unsigned int Index;
    string Txt=Text.GetText();
    HotKeyString.SetText("");
    for(Index=0u;Index<=Txt.size();Index++)if(Index==Txt.size()||Txt[Index]==HotKeyChar)break;
    if(Index!=Txt.size()){
        HotKeyString.SetText(string(1,HotKeyChar));
        HotKeyString.SetColor(sf::Color::Yellow);
        sf::Vector2f HKSLoc=Text.GetCharacterPos(Index)*Text.GetScale()+Text.GetPosition()-Text.GetCenter();
        HotKeyString.SetScale(Text.GetScale());
        HotKeyString.SetStyle(sf::String::Style::Bold);
        HotKeyString.SetPosition(HKSLoc);
    }
}

void Button::Update(const UIElement::State& CurState,sf::RenderWindow& Window){
    if(NeedUpdate)UpdateGraphics();
    sf::Shape* ToDraw=&Normal;
    if(IsHovering(CurState.Mouse)){
        if(LeftPressed){
            ToDraw=&Pressed;
        }else{
            ToDraw=&Hovering;
        }
    }
    if(NotIgnore&&Hotkey!=sf::Key::Count){
        if(CurState.Input.IsKeyDown(Hotkey)){
            OldClick=true;
            ToDraw=&Pressed;
            NotIgnore=false;
        }else if(OldClick&&OnClick){
            OldClick=false;
            OnClick();
        }
    }else{
        NotIgnore=true;
    }
    Window.Draw(*ToDraw);
    Window.Draw(Text);
    Window.Draw(HotKeyString);
}

void Button::SetText(string text){
    Text.SetText(text);
    UpdateGraphics();
}
