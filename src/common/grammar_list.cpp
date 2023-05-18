#include "grammar_list.h"
#include <list>

namespace compiler
{
    int grammar_list::cnt = 0;
    std::list<grammar> grammar_list::ls = std::list<grammar>();
    int grammar_list::start_id = 0;
}