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

void UIGroup::Update(const UIElement::State& CurState,sf::RenderWindow& Window){
    for(UIElement* Iter:Elements){
        if(Iter->Visible)Iter->Update(CurState,Window);
    }
}
