#include <iostream>
//#include<bits/stdc++.h>
#include<fstream>
using namespace std;

class MyClass
{
private:
    int myArray[3] = {1, 2, 3};

public:
    MyClass() {}

    int *begin()
    {
        return myArray;
    }

    int *end()
    {
        return myArray + 3;
    }
};

int main()
{
    std::ifstream ifs;
    ifs.open("code.txt");
    if (ifs.is_open() == false)
    {
        std::cout << "can't open src code" << std::endl;
    }
    std::string line;
    while (std::getline(ifs, line))
    {
        cout<<line<<endl;
        // code.push_back()
    }
}
