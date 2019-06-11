#ifndef SINGLETON_H
#define SINGLETON_H

template<typename T>
class singleton
{
public: 
    singleton() {}
    virtual ~singleton() {}
    
    static T& instance() 
    {
        static T inst;
        return inst;
    }
    
    singleton(const singleton &) = delete;
    singleton& operator= (const singleton &) = delete;
};

#endif                   
