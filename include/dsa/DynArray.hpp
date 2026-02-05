#ifndef DYNARRAY_H
#define DYNARRAY_H

#include <algorithm>
#include <iostream>

template<typename T>
class DynArray
{
public:
    DynArray(int init_size = 0)
        : m_size {init_size}
        , m_capacity {init_size + k_spare_capacity}
        , m_elements(new T[m_capacity])
    {
    }

    DynArray(const DynArray &rhs)
        : m_size {rhs.m_size}
        , m_capacity {rhs.m_capacity}
        , m_elements(new T[rhs.m_capacity])
    {
        for (size_t i = 0; i < m_size; ++i) {
            m_elements[i] = rhs.m_elements[i];
        }
    }

    DynArray &operator=(const DynArray &rhs)
    {
        DynArray copy = rhs;
        std::swap(*this, copy);
        return *this;
    }

    DynArray(DynArray &&rhs) noexcept
        : m_size {rhs.m_size}
        , m_capacity {rhs.m_capacity}
        , m_elements {rhs.m_elements}
    {
        rhs.m_size     = 0;
        rhs.m_capacity = 0;
        rhs.m_elements = nullptr;
    }

    DynArray &operator=(DynArray &&rhs)
    {
        std::swap(m_size, rhs.m_size);
        std::swap(m_capacity, rhs.m_capacity);
        std::swap(m_elements, rhs.m_elements);

        rhs.m_size     = 0;
        rhs.m_capacity = 0;
        rhs.m_elements = nullptr;

        return *this;
    }

    // set()
    T &operator[](int index) { return m_elements[index]; }

    // get()
    const T &operator[](int index) const { return m_elements[index]; }

    T &front() { return m_elements[0]; }

    const T &front() const { return m_elements[0]; }

    T &back() { return m_elements[m_size - 1]; }

    const T &back() const { return m_elements[m_size - 1]; }

    typedef T       *iterator;
    typedef const T *const_iterator;

    iterator begin() { return &m_elements[0]; }

    iterator end() { return &m_elements[m_size]; }

    const_iterator begin() const { return &m_elements[0]; }

    const_iterator end() const { return &m_elements[m_size]; }

    int size() { return m_size; }

    int capacity() { return m_capacity; }

    bool empty() { return size() == 0; }

    void resize(int new_size)
    {
        if (new_size > m_capacity) {
            reserve(m_size * 2);
        }
        m_size = new_size;
    }

    void reserve(int new_capacity)
    {
        T *new_arr = new T[new_capacity];
        for (int k = 0; k < m_size; ++k) {
            new_arr[k] = std::move(m_elements[k]);
        }

        m_capacity = new_capacity;
        std::swap(m_elements, new_arr);

        delete[] new_arr;
    }

    void push_back(const T &x)
    {
        if (m_size == m_capacity) {
            reserve((2 * m_capacity) + 1);  // +1 is there in case m_size is 0
        }
        m_elements[m_size++] = x;
    }

    void push_back(T &&x)
    {
        if (m_size == m_capacity) {
            reserve((2 * m_capacity) + 1);  // +1 is there in case m_size is 0
        }
        m_elements[m_size++] = std::move(x);
    }

    void pop_back()
    {
        if (!empty()) {
            m_elements[m_size--];
        }
    }

    void clear() { resize(0); }

private:
    int m_size;
    int m_capacity;
    T  *m_elements = nullptr;

    static const int k_spare_capacity = 16;
};
#endif  // DYNARRAY_H
