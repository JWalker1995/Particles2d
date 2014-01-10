#ifndef OFFSETVECTOR_H
#define OFFSETVECTOR_H

#include <deque>

template <typename Type>
class OffsetVector
{
public:
    OffsetVector()
    {}

    Type &operator[](signed int i)
    {
        if (items.empty())
        {
            items.emplace_back();
            offset = i;
            return items.front();
        }

        if (i < offset)
        {
            items.insert(items.begin(), offset - i, Type());
            offset = i;
            return items.front();
        }

        i -= offset;
        if (i >= items.size())
        {
            items.resize(i + 1);
        }

        return items[i];
    }

protected:
    signed int offset;
    std::deque<Type> items;
};

#endif // OFFSETVECTOR_H
