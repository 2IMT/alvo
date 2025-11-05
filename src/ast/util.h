#pragma once

#include <iterator>
#include <concepts>
#include <cstddef>

#include "../mem.h"

namespace alvo::ast::util {
    template<typename T>
    class Ptr {
    public:
        static Ptr<T> null() { return Ptr<T>(); }

        Ptr() :
            m_ptr(nullptr) { }

        Ptr(T* ptr) :
            m_ptr(ptr) { }

        const T& operator*() const { return *m_ptr; }

        T& operator*() { return *m_ptr; }

        const T* operator->() const { return m_ptr; }

        T* operator->() { return m_ptr; }

        friend bool operator==(const Ptr<T>& lhs, const Ptr<T>& rhs)
            requires std::equality_comparable<T>
        {
            return *lhs.m_ptr == *rhs.m_ptr;
        }

        friend bool operator!=(const Ptr<T>& lhs, const Ptr<T>& rhs)
            requires std::equality_comparable<T>
        {
            return !(lhs == rhs);
        }

        T* get_ptr() const { return m_ptr; }

    private:
        T* m_ptr;
    };

    template<typename T>
    class Array {
    public:
        class Iterator {
        public:
            using iterator_category = std::random_access_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;

            Iterator() :
                m_ptr(nullptr) { }

            Iterator(T* ptr) :
                m_ptr(ptr) { }

            reference operator*() const { return *m_ptr; }

            pointer operator->() const { return m_ptr; }

            Iterator& operator++() {
                ++m_ptr;
                return *this;
            }

            Iterator operator++(int) {
                Iterator tmp = *this;
                ++m_ptr;
                return tmp;
            }

            Iterator& operator--() {
                --m_ptr;
                return *this;
            }

            Iterator operator--(int) {
                Iterator tmp = *this;
                --m_ptr;
                return tmp;
            }

            Iterator operator+(difference_type n) const {
                return Iterator(m_ptr + n);
            }

            friend Iterator operator+(
                typename Iterator::difference_type n, const Iterator& it) {
                return it + n;
            }

            Iterator operator-(difference_type n) const {
                return Iterator(m_ptr - n);
            }

            difference_type operator-(const Iterator& rhs) const {
                return m_ptr - rhs.m_ptr;
            }

            Iterator& operator+=(difference_type n) {
                m_ptr += n;
                return *this;
            }

            Iterator& operator-=(difference_type n) {
                m_ptr -= n;
                return *this;
            }

            reference operator[](difference_type n) const { return m_ptr[n]; }

            auto operator<=>(const Iterator& rhs) const = default;

            bool operator==(const Iterator& rhs) const = default;

        private:
            T* m_ptr;
        };

        static_assert(std::random_access_iterator<Iterator>);

        Array() :
            m_data(nullptr),
            m_size(0) { }

        Array(T* data, std::size_t size) :
            m_data(data),
            m_size(size) { }

        std::size_t size() const { return m_size; }

        bool empty() const { return m_size == 0; }

        Iterator begin() { return Iterator(m_data); }

        Iterator end() { return Iterator(m_data + m_size); }

        T& operator[](std::size_t i) { return m_data[i]; }

        const T& operator[](std::size_t i) const { return m_data[i]; }

        friend bool operator==(const Array<T>& lhs, const Array<T>& rhs)
            requires std::equality_comparable<T>
        {
            auto it1 = lhs.begin();
            auto it2 = rhs.begin();

            for (; it1 != lhs.end() && it2 != rhs.end(); ++it1, ++it2) {
                if (!(*it1 == *it2)) {
                    return false;
                }
            }
            return it1 == lhs.end() && it2 == rhs.end();
        }

        friend bool operator!=(const Array<T>& lhs, const Array<T>& rhs)
            requires std::equality_comparable<T>
        {
            return !(lhs == rhs);
        }

    private:
        T* m_data;
        std::size_t m_size;
    };

    template<typename T>
    class List {
    private:
        struct Node {
            T value;
            Node* next;
        };

    public:
        class Iterator {
        public:
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;

            Iterator() :
                m_node(nullptr) { }

            Iterator(Node* node) :
                m_node(node) { }

            reference operator*() { return m_node->value; }

            reference operator*() const { return m_node->value; }

            pointer operator->() const { return &m_node->value; }

            Iterator& operator++() {
                m_node = m_node->next;
                return *this;
            }

            Iterator operator++(int) {
                Iterator tmp = *this;
                m_node = m_node->next;
                return tmp;
            }

            bool operator==(const Iterator& rhs) const {
                return m_node == rhs.m_node;
            }

            bool operator!=(const Iterator& rhs) const {
                return m_node != rhs.m_node;
            }

        private:
            Node* m_node;
        };

        static_assert(std::forward_iterator<Iterator>);

        List() :
            m_head(nullptr),
            m_tail(nullptr),
            m_size(0) { }

        void push_back(mem::Arena& arena, const T& val) {
            void* mem = arena.alloc(sizeof(Node), alignof(Node));
            Node* node = new (mem) Node { val, nullptr };
            if (m_tail) {
                m_tail->next = node;
            } else {
                m_head = node;
            }
            m_tail = node;
            m_size++;
        }

        void push_back(mem::Arena& arena, T&& val) {
            void* mem = arena.alloc(sizeof(Node), alignof(Node));
            Node* node = new (mem) Node { std::move(val), nullptr };
            if (m_tail) {
                m_tail->next = node;
            } else {
                m_head = node;
            }
            m_tail = node;
            m_size++;
        }

        std::size_t size() const { return m_size; }

        bool empty() const { return m_size == 0; }

        Array<T> flatten(mem::Arena& arena) {
            T* mem =
                static_cast<T*>(arena.alloc(sizeof(T) * m_size, alignof(T)));
            std::size_t offset = 0;
            for (T& v : *this) {
                new (mem + offset) T(std::move(v));
                offset++;
            }
            return Array<T>(mem, m_size);
        }

        Iterator begin() { return Iterator(m_head); }

        Iterator end() { return Iterator(nullptr); }

        Iterator begin() const { return Iterator(m_head); }

        Iterator end() const { return Iterator(nullptr); }

        friend bool operator==(const List<T>& lhs, const List<T>& rhs)
            requires std::equality_comparable<T>
        {
            auto it1 = lhs.begin();
            auto it2 = rhs.begin();

            for (; it1 != lhs.end() && it2 != rhs.end(); ++it1, ++it2) {
                if (!(*it1 == *it2)) {
                    return false;
                }
            }
            return it1 == lhs.end() && it2 == rhs.end();
        }

        friend bool operator!=(const List<T>& lhs, const List<T>& rhs)
            requires std::equality_comparable<T>
        {
            return !(lhs == rhs);
        }

    private:
        Node* m_head;
        Node* m_tail;
        std::size_t m_size;
    };

    class NodeCtx {
    public:
        NodeCtx(mem::Arena& arena) :
            m_arena(&arena) { }

        template<typename T, typename... Args>
        Ptr<T> make_node(Args&&... args) {
            void* ptr = m_arena->alloc(sizeof(T), alignof(T));
            return Ptr<T>(new (ptr) T(std::forward<Args>(args)...));
        }

    private:
        mem::Arena* m_arena;
    };

}
