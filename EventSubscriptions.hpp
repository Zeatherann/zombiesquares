#ifndef EVENT_SUBSCRIPTIONS_HPP
#define EVENT_SUBSCRIPTIONS_HPP

class EventSubscriptions {
public:
    static map<sf::Event::EventType, multimap<void*,std::function<void(sf::Event)>>> subscribers;
    static void subscribe(void* who, sf::Event::EventType, function<void(sf::Event)> func);
    static void unsubscribe(void* who, sf::Event::EventType);
    static void process(sf::Event event);
};

#endif // EVENT_SUBSCRIPTIONS_HPP
