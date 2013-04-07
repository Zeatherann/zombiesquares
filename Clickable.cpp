#include "main.hpp"

set<Clickable*> Clickable::Clickables;

Clickable::Clickable(const sf::FloatRect& rect):Rect(rect){Clickables.insert(this);}

Clickable::~Clickable(){Clickables.erase(this);}
