#include "taskflow.hpp"
#include "executor.hpp"

int main()
{
    tf::Taskflow taskflow;
    auto [A, B, C, D, E, F] = taskflow.emplace( 
         [] () { std::cout << "TaskA\n"; }, 
         [] () { std::cout << "TaskB\n"; },
         [] () { std::cout << "TaskC\n"; }, 
         [] () { std::cout << "TaskD\n"; }, 
         [] () { std::cout << "TaskE\n"; },
         [] () { std::cout << "TaskF\n"; }
      );
    
    A.name("A"); B.name("B"); C.name("C"); D.name("D"); E.name("E"); F.name("F");
    
    A.precede(B);  // A runs before B
    A.precede(C);  // A runs before C 
    B.precede(D);  // B runs before D
    C.precede(D);  // C runs before D 
    C.precede(E);  // C runs before E
    E.precede(F);  // E runs before F
    F.precede(D);  // F runs before D

    //taskflow.printNodes();
    tf::Executor executor; 
    executor.run(taskflow);
    
    std::cout << "---------------------------------\n";
    
    tf::Taskflow taskflow2;
    
    tf::TaskRef A1 = taskflow2.emplace([] () { std::cout << "TaskA1\n"; });
    tf::TaskRef B1 = taskflow2.emplace([] () { std::cout << "TaskB1\n"; });
    tf::TaskRef C1 = taskflow2.emplace([] () { std::cout << "TaskC1\n"; });
    tf::TaskRef D1 = taskflow2.emplace([] () { std::cout << "TaskD1\n"; });
    A1.precede(B1);  // A1 runs before B1                      
    A1.precede(C1);  // A1 runs before C1
    C1.precede(B1);  // C1 runs before B1
    B1.precede(D1);  // B1 runs before D1                     
    C1.precede(D1);  // C1 runs before D1                     
    executor.run(taskflow2);                                                  
    
    return 0;
}
