
#include <iostream>
#include "event_manager.h"

struct KillMonster
{
    int a;
    double b;
};


class AchievementSystem

{
public:
    AchievementSystem()
    {
        qRegisterEventForNonCommonFunction(&AchievementSystem::onKillMonsterCallback, this);
    }

    void onKillMonsterCallback(KillMonster& event)
    {
        std::cout << "From AchievementSystem a = " << event.a << " b = " << event.b << std::endl;
    }
};


class MissionSystem
{
public:
    MissionSystem()
    {
        qRegisterEventForNonCommonFunction(&MissionSystem::onKillMonsterCallback, this);
    }

    void onKillMonsterCallback(KillMonster& event)
    {
        std::cout << "From MissionSystem a = " << event.a << " b = " << event.b << std::endl;
    }
};

void onKillMonsterCallback(KillMonster& event)
{
    std::cout << "From OnTest Function a = " << event.a << " b = " << event.b << std::endl;
}


int main()
{
    AchievementSystem as;
    MissionSystem ms;

    //qRegisterEventForCommonFunction(onKillMonsterCallback);

    KillMonster event{1, 2.5};
    
    qNotifyAll(event);
    
   }
