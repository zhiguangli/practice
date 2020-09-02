#pragma once
#include <string>
#include <list>
#include <tuple>
#include <vector>
#include <functional>
#include <iostream>
#include <queue>

using namespace std;

namespace tf
{
     struct ITask 
    { 
        virtual ~ITask() = default; 
        void name(const string& id)  { this->id = id; }
        
        string  id;
        int     indegree {0};
        list<ITask *> adj;  
    };
    
    struct Taskflow;
    
    template<typename Callable = std::function<void()>>
    struct Task : ITask
    {
        Task(Callable&& f, Taskflow* flow) : f(f), flow(flow) {}

        void precede(auto&& task)
        {
            adj.push_back(&task);
            task.indegree++;
        }
        

        void operator()() const
        {
            f();
        }

    private:
        Callable f;
        Taskflow *flow;
        
    };
    using TaskRef = tf::Task<>&; 
    
    struct Taskflow
    {
        using Function_t = std::function<void()>;
        template<typename... Fs>
        auto emplace(Fs&&... fs)
        { 
            return std::make_tuple( ( taskNodes.emplace_back( Task<Function_t>(std::move(fs), this)) ,  std::ref(taskNodes.back()) )...);
        }
        
        template<typename Fs>
        auto& emplace(Fs fs)
        { 
            taskNodes.emplace_back( Task<Function_t>(std::move(fs), this));
            return taskNodes.back();
        }
        
        list<Task<Function_t>>  sort() const
        {
            queue<Task<Function_t>> q;
            list<Task<Function_t>> sortedTaskNodes;
            
            for(const auto& task : taskNodes)
            {
                if(task.indegree == 0) 
                    q.push(task);
            }
            
            int count = 0;
            while(!q.empty())
            {
                auto task = q.front();
                q.pop();
                sortedTaskNodes.emplace_back(std::move(task));
                ++count;
                
                for(const auto& t : task.adj)
                {
                    if(!(--t->indegree))
                    {
                        q.push(*static_cast<Task<Function_t>*>(t));
                    }
                }
            }
            
            if(count < taskNodes.size())
                return list<Task<Function_t>>();
            else
                return sortedTaskNodes;
        }
        void printNodes()
        {
            for(const auto& task : taskNodes)
            {
                std::cout << "Node: " << task.id << " indegree: " << task.indegree << std::endl;
            }
        }
    private:
        list<Task<Function_t>> taskNodes;
    };
}
