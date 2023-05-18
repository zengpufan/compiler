#include "config_reader.h"
#include "grammar_list.h"
namespace compiler
{
    void grammar_reader::read()
    {
        using namespace std;
        // grammar_list::add("epsilon");
        //  grammar_list *res = new grammar_list;


        std::string line;
        int flag=0;
        while (getline(ifs, line))
        {
            stringstream ss(line);
            string name;

            // if(name=="") continue;
            ss >> name;
            // cout<<name<<endl;
            grammar_list::add(name);

            // set start symbol
            if(flag++==0)
            grammar_list::start_id = grammar_list::get_id_by_name(name);
        }
        ifs.clear();
        ifs.seekg(0, ios::beg);
        while (getline(ifs, line))
        {
            stringstream ss(line);
            string name;
            ss >> name;
            // cout<<123<<endl;
            grammar *gm = nullptr;
            for (auto &it : grammar_list::ls)
            {
                if (it.name == name)
                    gm = &it;
            }
            // std::list<int> exp;
            string s;
            while (ss >> s)
            {
                // cout<<s<<endl;
                if (s == "|")
                    gm->exps.push_back(vector<int>());
                else
                {
                    gm->exps.back().push_back(grammar_list::get_id_by_name(s));
                }
            }
        }
        return;
    }
}