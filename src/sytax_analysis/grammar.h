#pragma once
#include<string>
#include<list>
namespace sytax_analysis
{
  
    class grammar
    {
        std::string head;
        std::list<std::list<mark>>generator;
    };

}