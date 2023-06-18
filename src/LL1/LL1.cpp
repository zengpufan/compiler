#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <set>
#include <stack>
#include <cstdlib>
#include <streambuf>

using namespace std;

string trans = "->";
struct node
{
    string left;
    vector<string> right;
    set<string> node_first;
};
struct flag_node
{
    bool flag = false;
    node node_node;
};
struct LL1
{
    // using Key = pair<string, string>;
    bool flag = true;
    string Start;
    set<string> NT;
    vector<string> sortedNT;
    set<string> T;
    vector<node> Grammar;
    map<string, set<string>> First;
    map<string, set<string>> Follow;
    map<pair<string, string>, flag_node> parsing_table;
};

void output(LL1 &grammar);                                // 输出文法
vector<node> process(vector<node> &grammar);              // 消去"|"
void left_recursion(LL1 &grammar);                        // 消除左递归
void left_factoring(LL1 &grammar);                        // 消除左因子
void build_first(LL1 &grammar);                           // first set
void build_follow(LL1 &grammar);                          // follow set
void build_parsingtable(LL1 &grammar);                    // parsing table
bool analysis_stack(LL1 &grammar, vector<string> &input); // analysis stack

// 读取文法
int main()
{
    LL1 grammar1;
    fstream f("grammar.txt");
    string line;
    bool flag = true;
    while (getline(f, line))
    {
        string word;
        istringstream record(line);
        node temp;
        int count = 0;
        while (record >> word)
        {
            if (count == 0)
            {
                temp.left = word;
                if (flag)
                {
                    grammar1.Start = word;
                    grammar1.NT.emplace(word);
                    grammar1.sortedNT.emplace_back(word);
                    flag = false;
                }
                if (!flag)
                {
                    if (grammar1.NT.find(word) == grammar1.NT.end())
                    {
                        grammar1.sortedNT.emplace_back(word);
                    }
                    grammar1.NT.emplace(word);
                }
                count = 1;
            }
            else if (word != trans)
            {
                temp.right.emplace_back(word);
            }
        }
        grammar1.Grammar.emplace_back(temp);
    }
    f.close();
    for (auto iter : grammar1.Grammar)
    {
        for (auto it : iter.right)
        {
            if (grammar1.NT.find(it) == grammar1.NT.end() && it != "|" && it != "epsilon")
            {
                grammar1.T.emplace(it);
                grammar1.First[it].emplace(it); // terminals的first集合
            }
        }
    }
    grammar1.Grammar = process(grammar1.Grammar);
    output(grammar1); // 初步结果

    left_factoring(grammar1);
    output(grammar1); // 消除左因子结果

    left_recursion(grammar1);
    output(grammar1); // 消除左递归结果

    build_first(grammar1);
    cout << "first sets: " << endl;
    for (auto it : grammar1.First) // first sets
    {
        cout << it.first << ": { ";
        for (auto itt : it.second)
        {
            cout << itt << " ";
        }
        cout << "}" << endl;
    }
    cout << endl;
    cout << "node_first sets: " << endl;
    for (auto it : grammar1.Grammar) // node_first sets
    {
        for (auto i : it.right)
        {
            cout << i << " ";
        }
        cout << ": { ";
        for (auto itt : it.node_first)
        {
            cout << itt << " ";
        }
        cout << "}" << endl;
    }
    cout << endl;

    build_follow(grammar1);
    cout << "follow sets: " << endl;
    for (auto it : grammar1.Follow) // follow sets
    {
        cout << it.first << ": { ";
        for (auto itt : it.second)
        {
            cout << itt << " ";
        }
        cout << "}" << endl;
    }
    cout << endl;

    build_parsingtable(grammar1);
    if (grammar1.flag) // 当读取的文法为LL1文法时
    {
        set<string> tt = grammar1.T;
        tt.emplace("$");
        ofstream opt;
        opt.open("parsing_table.csv", ios::out | ios::trunc); // 将分析表输出成文件
        opt << "NT T"
            << ",";
        for (auto ii : tt)
        {
            opt << ii << ",";
        }
        opt << endl;
        for (auto x : grammar1.sortedNT)
        {
            opt << x << ",";
            for (auto y : tt)
            {
                pair<string, string> temp;
                temp.first = x;
                temp.second = y;
                if (grammar1.parsing_table[temp].flag)
                {
                    string tempstring;
                    tempstring = grammar1.parsing_table[temp].node_node.left + " " + trans + " ";
                    for (auto it : grammar1.parsing_table[temp].node_node.right)
                    {
                        tempstring += it + " ";
                    }
                    opt << tempstring << ",";
                }
                else
                {
                    opt << "#"
                        << ",";
                }
            }
            opt << endl;
        }
        opt.close();

        cout << "parsing table: " << endl; // 输出分析表
        for (auto x : grammar1.sortedNT)
        {
            for (auto y : tt)
            {
                pair<string, string> temp;
                temp.first = x;
                temp.second = y;
                if (grammar1.parsing_table[temp].flag)
                {
                    cout << "(" << x << "," << y << "): ";
                    cout << grammar1.parsing_table[temp].node_node.left << " " << trans << " ";
                    for (auto it : grammar1.parsing_table[temp].node_node.right)
                    {
                        cout << it << " ";
                    }
                    cout << endl;
                }
                else
                {
                    cout << "(" << x << "," << y << "): ";
                    cout << "#" << endl;
                }
            }
            cout << endl;
        }
        cout << endl;

        cout << "读取的是LL1文法" << endl
             << endl;
        vector<string> input;
        fstream ff("input.txt");
        getline(ff, line);
        istringstream record(line);
        string word;
        while (record >> word)
        {
            input.emplace_back(word);
        }

        if (analysis_stack(grammar1, input)) // 判断是否符合文法
        {
            cout << "输入符合文法" << endl;
        }
        else
        {
            cout << "输入不符合文法" << endl;
        }
    }
}

// output
void output(LL1 &grammar)
{
    cout << "begin: " << grammar.Start << endl;
    cout << "none terminals: ";
    for (auto k : grammar.sortedNT)
    {
        cout << k << " ";
    }
    cout << endl;
    cout << "terminals: ";
    for (auto k : grammar.T)
    {
        cout << k << " ";
    }
    cout << endl;
    cout << "grammar: " << endl;
    for (auto i : grammar.Grammar)
    {
        cout << i.left << " " << trans << " ";
        for (auto j : i.right)
        {
            cout << j << " ";
        }
        cout << endl;
    }
    cout << endl;
}

// 去除分隔符"|"
vector<node> process(vector<node> &grammar)
{
    vector<node> tempgrammar;
    for (auto it : grammar)
    {
        string templeft = it.left;
        node temp;
        temp.left = templeft;
        for (int i = 0; i < it.right.size(); i++)
        {
            if (it.right[i] == "|")
            {
                tempgrammar.emplace_back(temp);
                temp.right.clear();
            }
            else if (i == it.right.size() - 1)
            {
                temp.right.emplace_back(it.right[i]);
                tempgrammar.emplace_back(temp);
            }
            else
            {
                temp.right.emplace_back(it.right[i]);
            }
        }
    }
    return tempgrammar;
}

// 消除左递归
void left_recursion(LL1 &grammar)
{

    // 间接左递归+直接左递归
    vector<string> tempnt;
    tempnt = grammar.sortedNT;
    for (int a = 0; a < tempnt.size(); a++)
    {
        // 间接
        for (int b = 0; b < a; b++)
        {
            int count = 0;
            for (auto &i : grammar.Grammar)
            {
                if (i.left == tempnt[a] && i.right[0] == tempnt[b])
                {
                    node temp;
                    temp.left = i.left;
                    for (auto &j : grammar.Grammar)
                    {
                        if (j.left == tempnt[b])
                        {
                            for (int k = 0; k < j.right.size(); k++)
                            {
                                temp.right.emplace_back(j.right[k]);
                            }
                            for (int kk = 1; kk < i.right.size(); kk++)
                            {
                                temp.right.emplace_back(i.right[kk]);
                            }
                            grammar.Grammar.emplace_back(temp);
                            temp.right.clear();
                        }
                    }
                    auto iter = grammar.Grammar.erase(grammar.Grammar.begin() + count);
                }
                else
                {
                    count++;
                }
            }
        }
        // 直接
        for (auto &i : grammar.Grammar)
        {
            if (i.left == tempnt[a] && i.left == i.right[0])
            {
                string flag = i.left;
                for (auto &j : grammar.Grammar)
                {
                    if (j.left == flag && j.left != j.right[0])
                    {
                        j.right.emplace_back(flag + "`");
                    }
                    else if (j.left == flag && j.left == j.right[0])
                    {
                        j.left = i.left + "`";
                        vector<string> temp;
                        for (int k = 1; k < i.right.size(); k++)
                        {
                            temp.emplace_back(i.right[k]);
                        }
                        temp.emplace_back(j.left);
                        temp.emplace_back("|");
                        temp.emplace_back("epsilon");
                        j.right = temp;
                    }
                }
                if (grammar.NT.find(flag + "`") == grammar.NT.end())
                {
                    grammar.sortedNT.emplace_back(flag + "`");
                }
                grammar.NT.emplace(flag + "`");
            }
        }
        grammar.Grammar = process(grammar.Grammar);
    }
}

// 消除左因子
void left_factoring(LL1 &grammar)
{
    bool flag = true;
    while (flag)
    {
        bool flag1 = true;
        // vector<string> prefix[grammar.sortedNT.size()];
        for (int i = 0; i < grammar.sortedNT.size(); i++) // 每一个non-terminal
        {
            // 找最长左因子
            bool flag2 = false;
            vector<string> temp1; // 最长匹配相等前缀
            for (int j = 0; j < grammar.Grammar.size() - 1; j++)
            {
                if (grammar.Grammar[j].left != grammar.sortedNT[i])
                {
                    continue;
                }
                else
                {
                    vector<string> temp2;
                    for (int k = j + 1; k < grammar.Grammar.size(); k++)
                    {
                        if (grammar.Grammar[k].left != grammar.sortedNT[i])
                        {
                            continue;
                        }
                        else
                        {
                            int length = grammar.Grammar[j].right.size();
                            if (length > grammar.Grammar[k].right.size())
                                length = grammar.Grammar[k].right.size();
                            for (int a = 0; a < length; a++)
                            {
                                if (grammar.Grammar[j].right[a] != grammar.Grammar[k].right[a])
                                    break;
                                else
                                {
                                    temp2.emplace_back(grammar.Grammar[j].right[a]);
                                    flag2 = true;
                                }
                            }
                            if (temp1.size() < temp2.size())
                                temp1 = temp2;
                        }
                    }
                }
            }
            // prefix[i] = temp1;
            if (flag2)
            {
                // 根据共有最长左因子修改原始产生式
                for (auto &it : grammar.Grammar)
                {
                    if (it.left == grammar.sortedNT[i] && temp1.size() <= it.right.size())
                    {
                        int cnt = 0;
                        for (cnt; cnt < temp1.size(); cnt++)
                        {
                            if (temp1[cnt] != it.right[cnt])
                            {
                                break;
                            }
                        }
                        if (cnt == temp1.size())
                        {
                            if (it.right.size() > temp1.size())
                            {
                                it.left += "`";
                                vector<string> temp4;
                                for (int itt = cnt; itt < it.right.size(); itt++)
                                {
                                    temp4.emplace_back(it.right[itt]);
                                }
                                it.right = temp4;
                            }
                            else
                            {
                                it.left += "`";
                                vector<string> temp4;
                                temp4.emplace_back("epsilon");
                                it.right = temp4;
                            }
                        }
                    }
                }
                // 再新增提取左因子的产生式
                node temp3;
                temp3.left = grammar.sortedNT[i];
                temp3.right = temp1;
                temp3.right.emplace_back(temp3.left + "`");
                grammar.Grammar.emplace_back(temp3);
                if (grammar.NT.find(grammar.sortedNT[i] + "`") == grammar.NT.end())
                {
                    grammar.sortedNT.emplace_back(grammar.sortedNT[i] + "`");
                }
                grammar.NT.emplace(grammar.sortedNT[i] + "`");
            }
            flag1 = flag2;
        }
        flag = flag1;
    }
}

// 构建first
void build_first(LL1 &grammar)
{

    bool flag = true;
    while (flag)
    {
        bool flag1 = false;
        for (auto j : grammar.Grammar)
        {

            // 循环所有表达式，直至first集合没有变化
            // 先判断右侧第一个元素是否为epsilon，是则直接并入左侧first集合；
            // 再判断第一个元素是否为terminal，是则直接并入左侧first集合；
            // 反之则遍历整个串，先判断是否为空，空则跳出；再判断是否包含epsilon，是则继续遍历，反之跳出。
            if (j.right[0] == "epsilon")
            {
                int count = grammar.First[j.left].size();
                grammar.First[j.left].emplace("epsilon");
                if (count != grammar.First[j.left].size())
                {
                    flag1 = true;
                }
            }
            else if (grammar.T.find(j.right[0]) != grammar.T.end())
            {
                int count = grammar.First[j.left].size();
                grammar.First[j.left].insert(grammar.First[j.right[0]].begin(), grammar.First[j.right[0]].end());
                if (count != grammar.First[j.left].size())
                {
                    flag1 = true;
                }
            }
            else if (grammar.First[j.right[0]].empty())
            {
                continue;
            }
            else
            {
                bool epsilonflag = true;
                for (int k = 0; k < j.right.size(); k++)
                {
                    int count = grammar.First[j.left].size();
                    for (auto iter : grammar.First[j.right[k]])
                    {
                        if (iter != "epsilon")
                        {
                            grammar.First[j.left].emplace(iter);
                        }
                    }
                    if (count != grammar.First[j.left].size())
                    {
                        flag1 = true;
                    }
                    if (grammar.First[j.right[k]].find("epsilon") == grammar.First[j.right[k]].end())
                    {
                        epsilonflag = false;
                        break;
                    }
                }
                if (epsilonflag)
                {
                    grammar.First[j.left].emplace("epsilon");
                }
            }
        }
        flag = flag1;
    }
    for (auto &iter : grammar.Grammar) // 构建每一条产生式右侧的串的first集合
    {
        int count = 0;
        for (auto it : iter.right)
        {
            if (it == "epsilon")
            {
                iter.node_first.emplace("epsilon");
            }
            else
            {
                for (auto itt : grammar.First[it])
                {
                    if (itt != "epsilon")
                    {
                        iter.node_first.emplace(itt);
                    }
                }
                if (grammar.First[it].find("epsilon") == grammar.First[it].end())
                {
                    break;
                }
                else if (count == grammar.First[it].size() - 1)
                {
                    iter.node_first.emplace("epsilon");
                }
            }
            count++;
        }
    }
}

// 构建follow集合
void build_follow(LL1 &grammar)
{
    grammar.Follow[grammar.Start].emplace("$");
    bool flag = true;
    while (flag)
    {
        bool flag1 = false;
        for (auto i : grammar.Grammar)
        {
            for (int j = 0; j < i.right.size(); j++)
            {
                if (grammar.NT.find(i.right[j]) != grammar.NT.end()) // 对于产生式右侧每一个非终结符
                {
                    if (j == i.right.size() - 1) // 当他是最后一位时
                    {
                        int count = grammar.Follow[i.right[j]].size();
                        grammar.Follow[i.right[j]].insert(grammar.Follow[i.left].begin(), grammar.Follow[i.left].end());
                        if (count != grammar.Follow[i.right[j]].size())
                        {
                            flag1 = true;
                        }
                    }
                    else
                    {
                        for (int k = j + 1; k < i.right.size(); k++)
                        {
                            int count = grammar.Follow[i.right[j]].size();
                            for (auto iter : grammar.First[i.right[k]])
                            {
                                if (iter != "epsilon")
                                {
                                    grammar.Follow[i.right[j]].emplace(iter);
                                }
                            }
                            if (count != grammar.Follow[i.right[j]].size())
                            {
                                flag1 = true;
                            }
                            if (grammar.First[i.right[k]].find("epsilon") == grammar.First[i.right[k]].end()) // first doesn`t include epsilon
                            {
                                break;
                            }
                            else if (k == i.right.size() - 1)
                            {
                                int count = grammar.Follow[i.right[j]].size();
                                grammar.Follow[i.right[j]].insert(grammar.Follow[i.left].begin(), grammar.Follow[i.left].end());
                                if (count != grammar.Follow[i.right[j]].size())
                                {
                                    flag1 = true;
                                }
                            }
                        }
                    }
                }
            }
        }
        flag = flag1;
    }
}

// 构建LL1分析表、判断是否符合LL1文法
void build_parsingtable(LL1 &grammar)
{
    for (auto i : grammar.Grammar)
    {
        for (auto j : i.node_first)
        {
            if (j != "epsilon")
            {
                pair<string, string> temp;
                temp.first = i.left;
                temp.second = j;
                if (grammar.parsing_table[temp].flag)
                {
                    cout << "不为LL1文法" << endl;
                    grammar.flag = false;
                    return;
                }
                else
                {
                    grammar.parsing_table[temp].flag = true;
                    grammar.parsing_table[temp].node_node = i;
                }
            }
        }
        if (i.node_first.find("epsilon") != i.node_first.end())
        {
            for (auto k : grammar.Follow[i.left])
            {
                pair<string, string> temp;
                temp.first = i.left;
                temp.second = k;
                if (grammar.parsing_table[temp].flag)
                {
                    cout << "不为LL1文法" << endl;
                    grammar.flag = false;
                    return;
                }
                else
                {
                    grammar.parsing_table[temp].flag = true;
                    grammar.parsing_table[temp].node_node = i;
                }
            }
        }
    }
}

// 分析栈，分析输入是否符合文法
void read_stack(stack<string> &sta)
{
    stack<string> temp = sta;
    while (temp.size() != 0)
    {
        cout << temp.top() << " ";
        temp.pop();
    }
    cout << "  |  ";
}
bool analysis_stack(LL1 &grammar, vector<string> &input)
{

    stack<string> ana;
    ana.push("$");
    ana.push(grammar.Start);
    stack<string> inpt;
    inpt.push("$");
    for (vector<string>::iterator iter = input.end() - 1; iter >= input.begin(); iter--)
    {
        inpt.push(*iter);
    }

    ofstream fff;
    fff.open("result.csv", ios::out | ios::trunc); // 将分析过程输出成文件
    fff << "analysis"
        << ","
        << "input"
        << ","
        << "action" << endl;
    bool result = false;
    while (true)
    {
        stack<string> temps = ana;
        while (temps.size() != 0)
        {
            fff << temps.top() << " ";
            temps.pop();
        }
        fff << ",";
        read_stack(ana);
        temps = inpt;
        while (temps.size() != 0)
        {
            fff << temps.top() << " ";
            temps.pop();
        }
        fff << ",";
        read_stack(inpt);
        if (ana.top() == inpt.top())
        {
            if (ana.top() == "$")
            {
                result = true;
                fff << "accept" << endl;
                cout << "accept" << endl;
                break;
            }
            else
            {
                ana.pop();
                inpt.pop();
                fff << "matched" << endl;
                cout << "matched" << endl;
            }
        }
        else
        {
            if (ana.top() == "$" || grammar.T.find(ana.top()) != grammar.T.end())
            {
                fff << "error" << endl;
                cout << "error" << endl;
                break;
            }
            else
            {
                string temp_top = ana.top();
                ana.pop();
                pair<string, string> temp;
                temp.first = temp_top;
                temp.second = inpt.top();
                if (grammar.parsing_table[temp].flag)
                {
                    for (int i = grammar.parsing_table[temp].node_node.right.size() - 1; i >= 0; i--)
                    {
                        if (grammar.parsing_table[temp].node_node.right[i] != "epsilon")
                        {
                            ana.push(grammar.parsing_table[temp].node_node.right[i]);
                        }
                    }
                    fff << grammar.parsing_table[temp].node_node.left << " -> ";
                    cout << grammar.parsing_table[temp].node_node.left << " -> ";
                    for (auto out : grammar.parsing_table[temp].node_node.right)
                    {
                        fff << out << " ";
                        cout << out << " ";
                    }
                    fff << endl;
                    cout << endl;
                }
                else
                {
                    fff << "error" << endl;
                    cout << "error" << endl;
                    break;
                }
            }
        }
    }
    fff.close();
    return result;
}