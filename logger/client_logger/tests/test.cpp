#include <set>
#include <iostream>

int main()
{
    std::set<std::pair<int, int>> my_set;
    my_set.insert(std::make_pair(2, 1));
    for (auto &[one, two] : my_set)
    {
        std::cout << one << " " << two << std::endl;
    }
}