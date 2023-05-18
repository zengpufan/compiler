#include "expection.h"
#include <iostream>
#include <string>
namespace compiler
{

    Expection::Expection(const std::string &_file, const std::string &_description)
        : _file(_file), _description(_description) {}
    void Expection::print()
    {
        std::cout << _file << " " << _description << std::endl;
    }

}