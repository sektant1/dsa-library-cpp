#ifndef ARRAY_H
#define ARRAY_H

#include <algorithm>
#include <iostream>

int k_spare_capacity = 16;

namespace dsa
{
template<typename T>
class Array
{
    int m_size;
    int m_capacity;
    T  *m_elements;

public:
    Array(int init_size = 0)
        : m_size {init_size}
        , m_capacity {init_size + k_spare_capacity}
        , m_elements(new T[m_capacity])
    {
    }

    Array(const Array &rhs)
        : m_size {rhs.m_size}
        , m_capacity {rhs.m_capacity}
        , m_elements(new T[rhs.m_capacity])
    {
        for (size_t i = 0; i < m_size; ++i) {
            m_elements[i] = rhs.m_elements[i];
        }
    }

    Array &operator=(const Array &rhs)
    {
        Array copy = rhs;
        std::swap(*this, copy);
        return *this;
    }

    Array(Array &&rhs) noexcept
        : m_size {rhs.m_size}
        , m_capacity {rhs.m_capacity}
        , m_elements {rhs.m_elements}
    {
        rhs.m_size     = 0;
        rhs.m_capacity = 0;
        rhs.m_elements = nullptr;
    }
};
}  // namespace dsa
#endif  // ARRAY_H
