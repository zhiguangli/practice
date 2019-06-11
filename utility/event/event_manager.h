#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include "singleton.h"
#include <any>
#include <unordered_map>
#include <functional>
#include <typeinfo>

namespace event
{
    class event_manager : public singleton<event_manager>
    {
    public:

        template<typename EventType>
        using FunctionType = std::function<void(EventType &)>;
        
        template<typename EventType, typename ClassType>
        void registe_event(void (ClassType::*method)(EventType &), ClassType *obj)
        {
            FunctionType<EventType> f = [method, obj](EventType &event) {
                (obj->*method)(event);
            };
            
            add(typeid(EventType).name(), std::move(f));
        }
        
        template<typename EventType>
        void registe_event(void(*func)(EventType &))
        {
            FunctionType<EventType> f = func;
            add(typeid(EventType).name(), std::move(f));
        }
        
        template<typename EventType>
        void registe_event(FunctionType<EventType> && f)
        {
            add(typeid(EventType).name(), std::move(f));
        }
        
        template<typename EventType>
        void notify(EventType &event)
        {
            using function_type = FunctionType<EventType>;
            for(const auto & e : events)
            {
                const auto& f = std::any_cast<const function_type &>(e.second);
                f(event);
            }
        }
    private:
        template <typename F>
        void add(const char* name, F&& f)
        {
            events.emplace(name, std::move(f));
        }
    private:
        std::unordered_multimap<const char *, std::any> events;
    };
}

#define qRegisterEventForCommonFunction(method)              event::event_manager::instance().registe_event(method)
#define qRegisterEventForNonCommonFunction(method, obj)      event::event_manager::instance().registe_event(method, obj)
#define qNotifyAll(event)                                    event::event_manager::instance().notify(event)


#endif
