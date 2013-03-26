#ifndef UIGROUP_HPP
#define UIGROUP_HPP
class UIGroup{
protected:
    static set<UIGroup*> UI;
public:
    // Variables
    set<UIElement*> Elements;
    bool Visible;
    // Constructor
    UIGroup(bool vis=1);
    // Destructor
    virtual ~UIGroup();
    // Functions
    virtual void Update(const UIElement::State& CurState,sf::RenderWindow& Window);
    inline void AddChild(UIElement* Child){Elements.insert(Child);Child->Owner = this;}
    inline void RemoveChild(UIElement* Child,bool Delete=true){Elements.erase(Child);if(Delete)delete Child;}
    inline void ClearChildren(bool Delete=false){if(Delete)for(UIElement* Iter:Elements)delete Iter;Elements.clear();}
    static inline void UpdateAll(const UIElement::State& CurState,sf::RenderWindow& Window){
        for(UIGroup* Iter:UI)if(Iter->Visible)Iter->Update(CurState,Window);
    }
};
#endif // UIGROUP_HPP
