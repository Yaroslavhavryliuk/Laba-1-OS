#include <iostream>
#include <thread>
#include <type_traits>
#include "trialfuncs.hpp"
#include <conio.h>

int x;
std::variant<os::lab1::compfuncs::hard_fail, os::lab1::compfuncs::soft_fail, int> fResult, gResult, int_sumResult;
std::condition_variable cv;
std::mutex cv_m;
bool fail;

int main()
{
    std::cout << "Enter the parameter for computation f and g" << std::endl;
    std::cin >> x;
    std::cout << "Press any button of the keyboard to cancel the computation" << std::endl;
    auto fRes = []()
    {
        fResult = os::lab1::compfuncs::trial_f<os::lab1::compfuncs::INT_SUM>(x);
    };

    auto gRes = []()
    {
        gResult = os::lab1::compfuncs::trial_g<os::lab1::compfuncs::INT_SUM>(x);
    };
    
    
    std::unique_lock<std::mutex> lk(cv_m);
    int kbhit(void);
    std::thread f(fRes);
    std::thread g(gRes);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    if (kbhit())
    {
        cv.wait(lk, [] {std::cout << "Computation canceled"; return false; });
    }
    f.join();
    g.join();
    std::cout << "Function f result: " << fResult << std::endl;
    std::cout << "Function g result: " << gResult << std::endl;
    if (std::holds_alternative<os::lab1::compfuncs::hard_fail>(fResult) == false &&
        std::holds_alternative<os::lab1::compfuncs::hard_fail>(gResult) == false &&
        std::holds_alternative<os::lab1::compfuncs::soft_fail>(fResult) == false &&
        std::holds_alternative<os::lab1::compfuncs::soft_fail>(gResult) == false)
    {
        int_sumResult = std::get<int>(fResult) + std::get<int>(gResult);
        std::cout << "Function int_sum result: " << int_sumResult << std::endl;
    }
    else
    {
        if (std::holds_alternative<os::lab1::compfuncs::hard_fail>(fResult) == true &&
            std::holds_alternative<os::lab1::compfuncs::hard_fail>(gResult) == true)
        {
            std::cout << "Function int_sum result: " << "Hard fail of both functions" << std::endl;
        }
        if (std::holds_alternative<os::lab1::compfuncs::hard_fail>(fResult) == true &&
            std::holds_alternative<os::lab1::compfuncs::hard_fail>(gResult) == false)
        {
            std::cout << "Function int_sum result: " << "Hard fail of the f function" << std::endl;
        }
        if (std::holds_alternative<os::lab1::compfuncs::hard_fail>(fResult) == false &&
            std::holds_alternative<os::lab1::compfuncs::hard_fail>(gResult) == true)
        {
            std::cout << "Function int_sum result: " << "Hard fail of the g function" << std::endl;
        }
        if (std::holds_alternative<os::lab1::compfuncs::soft_fail>(fResult) == true &&
            std::holds_alternative<os::lab1::compfuncs::soft_fail>(gResult) == true)
        {
            std::cout << "Function int_sum result: " << "Soft fail of both functions" << std::endl;
        }
        if (std::holds_alternative<os::lab1::compfuncs::soft_fail>(fResult) == true &&
            std::holds_alternative<os::lab1::compfuncs::soft_fail>(gResult) == false)
        {
            std::cout << "Function int_sum result: " << "Soft fail of the f function" << std::endl;
        }
        if (std::holds_alternative<os::lab1::compfuncs::soft_fail>(fResult) == false &&
            std::holds_alternative<os::lab1::compfuncs::soft_fail>(gResult) == true)
        {
            std::cout << "Function int_sum result: " << "Soft fail of the g function" << std::endl;
        }
    }
    return 0;
}
