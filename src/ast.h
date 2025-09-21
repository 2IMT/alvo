#pragma once

#include <variant>
#include <string_view>
#include <optional>
#include <utility>
#include <iterator>
#include <cstddef>

#include "mem.h"

namespace alvo::ast {

    template<typename T>
    class Ptr {
    public:
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

    private:
        T* m_ptr;
    };

    template<typename T>
    struct ListNode {
        T value;
        ListNode* next;
    };

    template<typename T>
    class List {
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

            Iterator(ListNode<T>* node) :
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
            ListNode<T>* m_node;
        };

        static_assert(std::forward_iterator<Iterator>);

        List() :
            m_head(nullptr),
            m_tail(nullptr),
            m_size(0) { }

        void push_back(mem::Arena& arena, const T& val) {
            void* mem = arena.alloc(sizeof(ListNode<T>), alignof(ListNode<T>));
            ListNode<T>* node = new (mem) ListNode<T> { val, nullptr };
            if (m_tail) {
                m_tail->next = node;
            } else {
                m_head = node;
            }
            m_tail = node;
            m_size++;
        }

        void push_back(mem::Arena& arena, T&& val) {
            void* mem = arena.alloc(sizeof(ListNode<T>), alignof(ListNode<T>));
            ListNode<T>* node =
                new (mem) ListNode<T> { std::move(val), nullptr };
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
        ListNode<T>* m_head;
        ListNode<T>* m_tail;
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

    struct Invalid { };

    struct PathSegment {
        struct Root { };

        struct Super { };

        struct Name {
            std::string_view value;
        };

        using Val = std::variant<Invalid, Root, Super, Name>;

        Val val;
    };

    struct Import {
        struct Normal { };

        struct Glob { };

        struct Renamed {
            std::string_view renamed_to;
        };

        using Kind = std::variant<Invalid, Normal, Glob, Renamed>;

        Kind kind;
        List<PathSegment> segments;
    };

    struct Type {
        struct Unit { };

        struct String { };

        struct Char { };

        struct Int { };

        struct Byte { };

        struct Float { };

        struct Bool { };

        struct Array {
            Ptr<Type> type;
        };

        struct Tup {
            List<Type> types;
        };

        struct Func {
            List<Type> params;
            Ptr<Type> return_type;
        };

        struct Path {
            List<PathSegment> segments;
        };

        using Val = std::variant<Invalid, Unit, String, Char, Int, Byte, Float,
            Bool, Array, Tup, Func, Path>;

        Val val;
    };

    struct Func;

    struct Expr {
        struct Literal {
            struct Unit { };

            struct Null { };

            struct String {
                std::string_view value;
            };

            struct Character {
                std::string_view value;
            };

            struct Integer {
                std::string_view value;
            };

            struct Byte {
                std::string_view value;
            };

            struct Floating {
                std::string_view value;
            };

            struct Boolean {
                std::string_view value;
            };

            struct Array {
                struct Regular {
                    List<Expr> elements;
                };

                struct DefaultNTimes {
                    Ptr<Expr> times;
                };

                struct ExprNTimes {
                    Ptr<Expr> expr;
                    Ptr<Expr> times;
                };

                using Val = std::variant<Regular, DefaultNTimes, ExprNTimes>;

                Val val;
            };

            struct Tup {
                List<Expr> exprs;
            };

            using Val = std::variant<Invalid, Unit, Null, String, Character,
                Integer, Byte, Floating, Boolean, Array, Tup, Ptr<Func>>;

            Val val;
        };

        struct Unop {
            enum Op {
                Invalid = -1,
                Plus,
                Minus,
                Not,
                BinaryNot,
            };

            Ptr<Expr> expr;
            Op op;
        };

        struct Binop {
            enum Op {
                Invalid = -1,
                Assign,
                PlusAssign,
                MinusAssign,
                MultiplyAssign,
                DivideAssign,
                BinaryAndAssign,
                BinaryOrAssign,
                BinaryXorAssign,
                ModAssign,
                ShiftLeftAssign,
                ShiftRightAssign,
                Or,
                And,
                BinaryOr,
                BinaryXor,
                BinaryAnd,
                Equal,
                NotEqual,
                Less,
                LessEqual,
                Greater,
                GreaterEqual,
                ShiftLeft,
                ShiftRight,
                Plus,
                Minus,
                Multiply,
                Divide,
                Mod,
                Access,
                StaticAccess,
            };

            Ptr<Expr> lhs;
            Ptr<Expr> rhs;
            Op op;
        };

        struct Index {
            Ptr<Expr> expr;
            Ptr<Expr> index;
        };

        struct Call {
            Ptr<Expr> expr;
            List<Expr> args;
        };

        struct Cast {
            Ptr<Expr> expr;
            Type type;
        };

        struct TryCast {
            Ptr<Expr> expr;
            Type type;
        };

        using Val = std::variant<Invalid, Literal, Unop, Binop, Index, Call,
            Cast, TryCast, PathSegment>;

        Val val;
    };

    struct Stmt;

    struct Block {
        List<Stmt> stmts;
    };

    struct Stmt {
        struct Let {
            std::string_view name;
            std::optional<Type> type;
            std::optional<Expr> expr;
        };

        struct If {
            struct Elif {
                Expr expr;
                Block block;
            };

            Expr expr;
            Block main;
            List<Elif> elifs;
            std::optional<Block> else_;
        };

        struct Switch {
            struct Case {
                std::optional<Expr> expr; // nullopt means the case is `default`
                Block block;
            };

            Expr expr;
            List<Case> cases;
        };

        struct Loop {
            Block block;
        };

        struct For {
            std::string_view name;
            Expr expr;
            Block block;
        };

        struct While {
            Expr expr;
            Block block;
        };

        struct Return {
            std::optional<Expr> expr;
        };

        struct Defer {
            Expr expr;
        };

        struct Continue { };

        struct Break { };

        using Val = std::variant<Invalid, Expr, Let, If, Switch, Loop, For,
            While, Return, Defer, Block, Continue, Break>;

        Val val;
    };

    struct Func {
        struct Param {
            std::string_view name;
            Type type;
        };

        List<Param> params;
        Type ret;
        Block block;
    };

    struct Decl {
        struct Struct {
            struct Item {
                struct Field {
                    std::string_view name;
                    Type type;
                };

                using Val = std::variant<Field, Func>;

                Val val;
                bool is_export;
            };

            List<Item> items;
        };

        struct Enum {
            struct Item {
                struct Field {
                    std::string_view name;
                };

                using Val = std::variant<Field, Func>;

                Val val;
                bool is_export;
            };

            List<Item> items;
        };

        struct TypeAlias {
            Type type;
        };

        struct Const {
            Type type;
            Expr expr;
        };

        using Val = std::variant<Invalid, Func, Struct, Enum, TypeAlias, Const>;

        Val val;
        std::string_view name;
        bool is_export;
    };

    struct TopLevel {
        using Val = std::variant<Import, Decl>;

        Val val;
    };

    struct Module {
        List<TopLevel> top_levels;
    };

    bool operator==(const Invalid& l, const Invalid& r);

    bool operator==(const PathSegment& l, const PathSegment& r);

    bool operator==(const PathSegment::Root& l, const PathSegment::Root& r);

    bool operator==(const PathSegment::Super& l, const PathSegment::Super& r);

    bool operator==(const PathSegment::Name& l, const PathSegment::Name& r);

    bool operator==(const Import& l, const Import& r);

    bool operator==(const Import::Normal& l, const Import::Normal& r);

    bool operator==(const Import::Glob& l, const Import::Glob& r);

    bool operator==(const Import::Renamed& l, const Import::Renamed& r);

    bool operator==(const Type& l, const Type& r);

    bool operator==(const Type::Unit& l, const Type::Unit& r);

    bool operator==(const Type::String& l, const Type::String& r);

    bool operator==(const Type::Char& l, const Type::Char& r);

    bool operator==(const Type::Int& l, const Type::Int& r);

    bool operator==(const Type::Byte& l, const Type::Byte& r);

    bool operator==(const Type::Float& l, const Type::Float& r);

    bool operator==(const Type::Bool& l, const Type::Bool& r);

    bool operator==(const Type::Array& l, const Type::Array& r);

    bool operator==(const Type::Tup& l, const Type::Tup& r);

    bool operator==(const Type::Func& l, const Type::Func& r);

    bool operator==(const Type::Path& l, const Type::Path& r);

    bool operator==(const Expr& l, const Expr& r);

    bool operator==(const Expr::Literal& l, const Expr::Literal& r);

    bool operator==(const Expr::Literal::Unit& l, const Expr::Literal::Unit& r);

    bool operator==(const Expr::Literal::Null& l, const Expr::Literal::Null& r);

    bool operator==(
        const Expr::Literal::String& l, const Expr::Literal::String& r);

    bool operator==(
        const Expr::Literal::Character& l, const Expr::Literal::Character& r);

    bool operator==(
        const Expr::Literal::Integer& l, const Expr::Literal::Integer& r);

    bool operator==(const Expr::Literal::Byte& l, const Expr::Literal::Byte& r);

    bool operator==(
        const Expr::Literal::Floating& l, const Expr::Literal::Floating& r);

    bool operator==(
        const Expr::Literal::Boolean& l, const Expr::Literal::Boolean& r);

    bool operator==(
        const Expr::Literal::Array& l, const Expr::Literal::Array& r);

    bool operator==(const Expr::Literal::Array::Regular& l,
        const Expr::Literal::Array::Regular& r);

    bool operator==(const Expr::Literal::Array::DefaultNTimes& l,
        const Expr::Literal::Array::DefaultNTimes& r);

    bool operator==(const Expr::Literal::Array::ExprNTimes& l,
        const Expr::Literal::Array::ExprNTimes& r);

    bool operator==(const Expr::Literal::Tup& l, const Expr::Literal::Tup& r);

    bool operator==(const Expr::Unop& l, const Expr::Unop& r);

    bool operator==(const Expr::Binop& l, const Expr::Binop& r);

    bool operator==(const Expr::Index& l, const Expr::Index& r);

    bool operator==(const Expr::Call& l, const Expr::Call& r);

    bool operator==(const Expr::Cast& l, const Expr::Cast& r);

    bool operator==(const Expr::TryCast& l, const Expr::TryCast& r);

    bool operator==(const Block& l, const Block& r);

    bool operator==(const Stmt& l, const Stmt& r);

    bool operator==(const Stmt::Let& l, const Stmt::Let& r);

    bool operator==(const Stmt::If& l, const Stmt::If& r);

    bool operator==(const Stmt::If::Elif& l, const Stmt::If::Elif& r);

    bool operator==(const Stmt::Switch& l, const Stmt::Switch& r);

    bool operator==(const Stmt::Switch::Case& l, const Stmt::Switch::Case& r);

    bool operator==(const Stmt::Loop& l, const Stmt::Loop& r);

    bool operator==(const Stmt::For& l, const Stmt::For& r);

    bool operator==(const Stmt::While& l, const Stmt::While& r);

    bool operator==(const Stmt::Return& l, const Stmt::Return& r);

    bool operator==(const Stmt::Defer& l, const Stmt::Defer& r);

    bool operator==(const Stmt::Continue& l, const Stmt::Continue& r);

    bool operator==(const Stmt::Break& l, const Stmt::Break& r);

    bool operator==(const Func& l, const Func& r);

    bool operator==(const Func::Param& l, const Func::Param& r);

    bool operator==(const Decl& l, const Decl& r);

    bool operator==(const Decl::Struct& l, const Decl::Struct& r);

    bool operator==(const Decl::Struct::Item& l, const Decl::Struct::Item& r);

    bool operator==(
        const Decl::Struct::Item::Field& l, const Decl::Struct::Item::Field& r);

    bool operator==(const Decl::Enum& l, const Decl::Enum& r);

    bool operator==(const Decl::Enum::Item& l, const Decl::Enum::Item& r);

    bool operator==(
        const Decl::Enum::Item::Field& l, const Decl::Enum::Item::Field& r);

    bool operator==(const Decl::TypeAlias& l, const Decl::TypeAlias& r);

    bool operator==(const Decl::Const& l, const Decl::Const& r);

    bool operator==(const TopLevel& l, const TopLevel& r);

    bool operator==(const Module& l, const Module& r);

    bool operator!=(const Invalid& l, const Invalid& r);

    bool operator!=(const PathSegment& l, const PathSegment& r);

    bool operator!=(const PathSegment::Root& l, const PathSegment::Root& r);

    bool operator!=(const PathSegment::Super& l, const PathSegment::Super& r);

    bool operator!=(const PathSegment::Name& l, const PathSegment::Name& r);

    bool operator!=(const Import& l, const Import& r);

    bool operator!=(const Import::Normal& l, const Import::Normal& r);

    bool operator!=(const Import::Glob& l, const Import::Glob& r);

    bool operator!=(const Import::Renamed& l, const Import::Renamed& r);

    bool operator!=(const Type& l, const Type& r);

    bool operator!=(const Type::Unit& l, const Type::Unit& r);

    bool operator!=(const Type::String& l, const Type::String& r);

    bool operator!=(const Type::Char& l, const Type::Char& r);

    bool operator!=(const Type::Int& l, const Type::Int& r);

    bool operator!=(const Type::Byte& l, const Type::Byte& r);

    bool operator!=(const Type::Float& l, const Type::Float& r);

    bool operator!=(const Type::Bool& l, const Type::Bool& r);

    bool operator!=(const Type::Array& l, const Type::Array& r);

    bool operator!=(const Type::Tup& l, const Type::Tup& r);

    bool operator!=(const Type::Func& l, const Type::Func& r);

    bool operator!=(const Type::Path& l, const Type::Path& r);

    bool operator!=(const Expr& l, const Expr& r);

    bool operator!=(const Expr::Literal& l, const Expr::Literal& r);

    bool operator!=(const Expr::Literal::Unit& l, const Expr::Literal::Unit& r);

    bool operator!=(const Expr::Literal::Null& l, const Expr::Literal::Null& r);

    bool operator!=(
        const Expr::Literal::String& l, const Expr::Literal::String& r);

    bool operator!=(
        const Expr::Literal::Character& l, const Expr::Literal::Character& r);

    bool operator!=(
        const Expr::Literal::Integer& l, const Expr::Literal::Integer& r);

    bool operator!=(const Expr::Literal::Byte& l, const Expr::Literal::Byte& r);

    bool operator!=(
        const Expr::Literal::Floating& l, const Expr::Literal::Floating& r);

    bool operator!=(
        const Expr::Literal::Boolean& l, const Expr::Literal::Boolean& r);

    bool operator!=(
        const Expr::Literal::Array& l, const Expr::Literal::Array& r);

    bool operator!=(const Expr::Literal::Array::Regular& l,
        const Expr::Literal::Array::Regular& r);

    bool operator!=(const Expr::Literal::Array::DefaultNTimes& l,
        const Expr::Literal::Array::DefaultNTimes& r);

    bool operator!=(const Expr::Literal::Array::ExprNTimes& l,
        const Expr::Literal::Array::ExprNTimes& r);

    bool operator!=(const Expr::Literal::Tup& l, const Expr::Literal::Tup& r);

    bool operator!=(const Expr::Unop& l, const Expr::Unop& r);

    bool operator!=(const Expr::Binop& l, const Expr::Binop& r);

    bool operator!=(const Expr::Index& l, const Expr::Index& r);

    bool operator!=(const Expr::Call& l, const Expr::Call& r);

    bool operator!=(const Expr::Cast& l, const Expr::Cast& r);

    bool operator!=(const Expr::TryCast& l, const Expr::TryCast& r);

    bool operator!=(const Block& l, const Block& r);

    bool operator!=(const Stmt& l, const Stmt& r);

    bool operator!=(const Stmt::Let& l, const Stmt::Let& r);

    bool operator!=(const Stmt::If& l, const Stmt::If& r);

    bool operator!=(const Stmt::If::Elif& l, const Stmt::If::Elif& r);

    bool operator!=(const Stmt::Switch& l, const Stmt::Switch& r);

    bool operator!=(const Stmt::Switch::Case& l, const Stmt::Switch::Case& r);

    bool operator!=(const Stmt::Loop& l, const Stmt::Loop& r);

    bool operator!=(const Stmt::For& l, const Stmt::For& r);

    bool operator!=(const Stmt::While& l, const Stmt::While& r);

    bool operator!=(const Stmt::Return& l, const Stmt::Return& r);

    bool operator!=(const Stmt::Defer& l, const Stmt::Defer& r);

    bool operator!=(const Stmt::Continue& l, const Stmt::Continue& r);

    bool operator!=(const Stmt::Break& l, const Stmt::Break& r);

    bool operator!=(const Func& l, const Func& r);

    bool operator!=(const Func::Param& l, const Func::Param& r);

    bool operator!=(const Decl& l, const Decl& r);

    bool operator!=(const Decl::Struct& l, const Decl::Struct& r);

    bool operator!=(const Decl::Struct::Item& l, const Decl::Struct::Item& r);

    bool operator!=(
        const Decl::Struct::Item::Field& l, const Decl::Struct::Item::Field& r);

    bool operator!=(const Decl::Enum& l, const Decl::Enum& r);

    bool operator!=(const Decl::Enum::Item& l, const Decl::Enum::Item& r);

    bool operator!=(
        const Decl::Enum::Item::Field& l, const Decl::Enum::Item::Field& r);

    bool operator!=(const Decl::TypeAlias& l, const Decl::TypeAlias& r);

    bool operator!=(const Decl::Const& l, const Decl::Const& r);

    bool operator!=(const TopLevel& l, const TopLevel& r);

    bool operator!=(const Module& l, const Module& r);

}
