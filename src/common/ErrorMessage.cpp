#include"ErrorMessage.h"
namespace compiler
{
    std::stack<std::pair<std::string,location>>ErrorMessage::ErrorMessageStack=std::stack<std::pair<std::string,location>>();
}