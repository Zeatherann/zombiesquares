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
    UIGroup(bool vis=true);
    // Destructor
    ~UIGroup();
    // Functions
    virtual void GetInput(sf::Event MouseEvent,const sf::Input& State);
    virtual void Draw(sf::RenderWindow& Window);
    inline void AddChild(UIElement* Child){Elements.insert(Child);}
    inline void RemoveChild(UIElement* Child,bool Delete=true){Elements.erase(Child);if(Delete)delete Child;}
    inline void ClearChildren(bool Delete=false){if(Delete)for(UIElement* Iter:Elements)delete Iter;Elements.clear();}
    static inline void GetAllInput(sf::Event MouseEvent,const sf::Input& State){
        for(UIGroup* Iter:UI)Iter->GetInput(MouseEvent,State);
    }
    static inline void DrawAll(sf::RenderWindow& Window){
        for(UIGroup* Iter:UI)Iter->Draw(Window);
    }
};
#endif // UIGROUP_HPP
