#ifndef EVENT_SUBSCRIPTIONS_HPP
#define EVENT_SUBSCRIPTIONS_HPP

typedef function<void(sf::Event, const UIElement::State& CurState)> EventHandler;

class EventSubscriptions {
public:
    static map<sf::Event::EventType, multimap<void*,EventHandler>> subscribers;
    static void subscribe(void* who, sf::Event::EventType, EventHandler func);
    static void unsubscribe(void* who, sf::Event::EventType);
    static void process(sf::Event event, const UIElement::State& CurState);
};

#endif // EVENT_SUBSCRIPTIONS_HPP
