#ifndef WEAKSET_H
#define WEAKSET_H

#include <set>

template <typename Type>
class WeakSet : public std::set<Type>
{
};

#endif // WEAKSET_H
