#ifndef HETEROGENEOUS_CONTAINER_HPP
#define HETEROGENEOUS_CONTAINER_HPP

#include <unordered_map>
#include <vector>
#include <type_traits>

namespace Container
{
    //异构容器
    struct heterogeneous_container;

    template<typename... Args> struct type_list{};

    template<typename T>
    std::unordered_map<const heterogeneous_container *, std::vector<T>> items;

    struct heterogeneous_container
    {
        template<typename T>
        void push_back(const T &t)
        {
            items<T>[this].push_back(t);
        }

        template<typename T>
        void visit(T &&visitor)
        {
            visit_impl(visitor, typename std::decay_t<T>::types{});
        }

        template<typename... Args, typename F>
        void visit1(F &&visitor)
        {
            visit_impl(visitor, type_list<Args...>{});
        }

    private:
        template<typename T, template<typename...> class TLIST, typename... TYPES>
        void visit_impl(T &&visitor, TLIST<TYPES...>)
        {
            (..., visit_impl_help<std::decay_t<T>, TYPES>(visitor));
        }

        template<typename T, typename U>
        void visit_impl_help(T& visitor)
        {
            for(auto &&element : items<U>[this])
            {
                visitor(element);
            }
        }
    };
}



#endif // HETEROGENEOUS_CONTAINER_HPP

