#include "main.hpp"

set<sf::Event::EventType> UIElement::TrackedEvents={sf::Event::KeyPressed,sf::Event::KeyReleased,sf::Event::MouseMoved,sf::Event::MouseButtonPressed,sf::Event::MouseButtonReleased,sf::Event::MouseWheelMoved};

UIElement::UIElement(sf::Vector2f Location,sf::Vector2f Size,bool Visible):Location(Location),Size(Size),Visible(Visible),NeedUpdate(true){}

UIElement::~UIElement(){}

UIElement::State::State(const sf::Input& I):Mouse(),MouseDelta(0),Input(I){}
