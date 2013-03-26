#include "main.hpp"

map<sf::Event::EventType, multimap<void*,EventHandler>> EventSubscriptions::subscribers;

void EventSubscriptions::subscribe(void* who, sf::Event::EventType type, EventHandler func) {
    subscribers[type].insert(std::pair<void*, EventHandler>(who, func));
}

void EventSubscriptions::unsubscribe(void* who, sf::Event::EventType type) {
    subscribers[type].erase(who);
}

void EventSubscriptions::process(sf::Event event, const UIElement::State& CurState) {
    for (std::pair<void*, EventHandler> pair : subscribers[event.Type]) {
        pair.second(event, CurState);
    }
}

//EventSubscriptions::init();
