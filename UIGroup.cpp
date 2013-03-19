#include "main.hpp"

set<UIGroup*> UIGroup::UI;

UIGroup::UIGroup(bool vis):Elements(),Visible(vis){
    UI.insert(this);
}

UIGroup::~UIGroup(){
    UI.erase(this);
    for(UIElement* Iter:Elements){
        delete Iter;
    }
}

void UIGroup::GetInput(sf::Event Event,const sf::Input& State){
    if(!Visible)return;
    for(UIElement* Iter:Elements){
        Iter->GetInput(Event,State);
    }
}

void UIGroup::Draw(sf::RenderWindow& Window){
    if(!Visible)return;
    for(UIElement* Iter:Elements){
        Iter->Draw(Window);
    }
}
