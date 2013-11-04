#ifndef WEAKSET_H
#define WEAKSET_H

#include <vector>

template <typename Type>
class WeakSet
{
public:
    typedef typename std::vector<Type>::iterator iterator;

    void insert(Type &item)
    {
        arr.push_back(item);
    }

    iterator insert(Type &item, iterator i)
    {
        int n = i - arr.begin();
        arr.push_back(item);
        return arr.begin() + n;
    }

    iterator erase(iterator i)
    {
        *i = arr.back();
        arr.pop_back();
        return i;
    }

    void shrink() {arr.shrink_to_fit();}

    iterator begin() {return arr.begin();}
    iterator end() {return arr.end();}

    typename std::vector<Type>::size_type empty() {return arr.empty();}
    bool size() {return arr.size();}

protected:
    std::vector<Type> arr;
};

#endif // WEAKSET_H
