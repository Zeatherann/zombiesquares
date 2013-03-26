#include "main.hpp"

map<sf::Event::EventType, multimap<void*,std::function<void(sf::Event)>>> EventSubscriptions::subscribers;

void EventSubscriptions::subscribe(void* who, sf::Event::EventType type, function<void(sf::Event)> func) {
    subscribers[type].insert(std::pair<void*, function<void(sf::Event)>>(who, func));
}

void EventSubscriptions::unsubscribe(void* who, sf::Event::EventType type) {
    subscribers[type].erase(who);
}

void EventSubscriptions::process(sf::Event event) {
    for (std::pair<void*, function<void(sf::Event)>> pair : subscribers[event.Type]) {
        pair.second(event);
    }
}

//EventSubscriptions::init();
