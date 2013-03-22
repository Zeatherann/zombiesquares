#include "main.hpp"

Menu::Menu(sf::Vector2f L,sf::Vector2f S,float E,sf::String title,sf::Shape Back,bool Vis):UIGroup(Vis),Size(S),NextPos(),Location(L),Edge(E),Buffer(0.f),Title(title),Background(Back){
    Background.SetPointPosition(0,Location.x       ,Location.y);
    Background.SetPointPosition(1,Location.x+Size.x,Location.y);
    Background.SetPointPosition(2,Location.x+Size.x,Location.y+Size.y);
    Background.SetPointPosition(3,Location.x       ,Location.y+Size.y);
    Title.SetPosition(Location.x+Size.x*0.5f,Location.y);
    Title.SetCenter(Title.GetRect().GetWidth()*0.5f,0.f);
}

Menu::~Menu(){}

void Menu::UpdateGraphics(){
    Background.SetPointPosition(0,Location.x       ,Location.y);
    Background.SetPointPosition(1,Location.x+Size.x,Location.y);
    float* NP[4u]={&NextPos.x,&NextPos.y,&NextPos.z,*NP};
    for(unsigned int i=1u;i<3u;i++)NP[3u]=*NP[i]>*NP[3u]?NP[i]:NP[3u];
    Background.SetPointPosition(2,Location.x+Size.x,Location.y+*NP[3u]+Size.y);
    Background.SetPointPosition(3,Location.x,Location.y+*NP[3u]+Size.y);
    Title.SetPosition(Location.x+Size.x*0.5f,Location.y);
    Title.SetCenter(Title.GetRect().GetWidth()*0.5f,0.f);
}

void Menu::AddButton(Button* Child,char Style,int ExtraSteps){
    // Styles: 0 = Left, 1 = Middle, 2 = Right
    float Extra=Child->Normal.GetOutlineWidth();
    float Change=1.f+ExtraSteps;
    float* NewPos=NULL;
    if(Style==1){
        Child->Location=Location+sf::Vector2f(Extra+Size.x*0.5f-(Child->Size.x*0.5f),Size.y+NextPos.y+Extra+Buffer);
        NewPos=&NextPos.y;
        // Middle
    }else if(Style==2){
        Child->Location=Location+sf::Vector2f(Extra+Size.x-(Edge+Child->Size.x),Size.y+NextPos.z+Extra+Buffer);
        NewPos=&NextPos.z;
        // Right
    }else{
        Child->Location=Location+sf::Vector2f(Edge+Extra,Size.y+NextPos.x+Extra+Buffer);
        NewPos=&NextPos.x;
    }
    Child->NeedUpdate=true;
    Change*=Child->Size.y+Extra*2.f+Buffer;
    (*NewPos)+=Change;
    UpdateGraphics();
    Elements.insert(Child);
}

float Menu::GetHeight()const{
    const float* NP[4u]={&NextPos.x,&NextPos.y,&NextPos.z,*NP};
    for(unsigned int i=1u;i<3u;i++)NP[3u]=*NP[i]>*NP[3u]?NP[i]:NP[3u];
    return *NP[3u];
}

void Menu::Move(sf::Vector2f NewLoc){
    sf::Vector2f Diff=Location-NewLoc;
    for(UIElement* Iter:Elements){
        Iter->Location-=Diff;
        Iter->NeedUpdate=true;
    }
    Location=NewLoc;
    UpdateGraphics();
}

void Menu::Update(const UIElement::State& CurState,sf::RenderWindow& Window){
    Window.Draw(Background);
    Window.Draw(Title);
    UIGroup::Update(CurState,Window);
}
