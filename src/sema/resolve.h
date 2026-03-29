#pragma once

#include <iterator>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <string_view>
#include <variant>
#include <stdexcept>

#include "../ast.h"
#include "../diag.h"

namespace alvo::sema::resolve {

    template<typename StoredType>
    class Storage {
    public:
        template<bool IsConst>
        class BaseIterator {
        public:
            struct Entry {
                using ElementType =
                    std::conditional_t<IsConst, const StoredType&, StoredType>;

                ast::Id id;
                std::string_view name;
                ElementType element;

                Entry(ast::Id id, std::string_view name, ElementType element) :
                    id(id),
                    name(name),
                    element(element) { }
            };

            using StoragePtr =
                std::conditional_t<IsConst, const Storage*, Storage*>;

            using InnerIter = typename std::unordered_map<std::string_view,
                ast::Id>::iterator;

            using iterator_category = std::forward_iterator_tag;

            using value_type = Entry;

            BaseIterator() :
                m_storage(nullptr),
                m_inner_iter() { }

            BaseIterator(Storage* storage, InnerIter it) :
                m_storage(storage),
                m_inner_iter(it) { }

            value_type operator*() const {
                auto& [name, id] = *m_inner_iter;
                return Entry(id, name, m_storage->m_elements.at(id));
            }

            BaseIterator operator++() {
                ++m_inner_iter;
                return *this;
            }

            BaseIterator operator++(int) {
                BaseIterator tmp = *this;
                ++(*this);
                return tmp;
            }

            friend bool operator==(
                const BaseIterator& lhs, const BaseIterator& rhs) {
                return lhs.m_inner_iter == rhs.m_inner_iter;
            }

            friend bool operator!=(
                const BaseIterator& lhs, const BaseIterator& rhs) {
                return lhs.m_inner_iter != rhs.m_inner_iter;
            }

        private:
            StoragePtr m_storage;
            InnerIter m_inner_iter;
        };

        using Iterator = BaseIterator<false>;
        using ConstIterator = BaseIterator<true>;

        Storage() :
            m_elements(),
            m_element_ids(),
            m_last_id(0) { }

        bool put(std::string_view name, const StoredType& element) {
            if (m_element_ids.contains(name))
                return false;

            ast::Id id = new_id();
            m_elements.insert({ id, element });
            m_element_ids.insert({ name, id });
            return true;
        }

        bool has(std::string_view name) const {
            return m_element_ids.contains(name);
        }

        bool has_id(ast::Id id) const { return m_elements.contains(id); }

        StoredType& get(std::string_view name) {
            return m_elements.at(m_element_ids.at(name));
        }

        const StoredType& get(std::string_view name) const {
            return m_elements.at(m_element_ids.at(name));
        }

        StoredType& get_by_id(ast::Id id) { return m_elements.at(id); }

        const StoredType& get_by_id(ast::Id id) const {
            return m_elements.at(id);
        }

        ast::Id get_id(std::string_view name) const {
            return m_element_ids.at(name);
        }

        Iterator begin() { return Iterator(this, m_element_ids.begin()); }

        Iterator end() { return Iterator(this, m_element_ids.end()); }

        ConstIterator begin() const {
            return ConstIterator(this, m_element_ids.begin());
        }

        ConstIterator end() const {
            return ConstIterator(this, m_element_ids.end());
        }

    private:
        std::unordered_map<ast::Id, StoredType> m_elements;
        std::unordered_map<std::string_view, ast::Id> m_element_ids;
        ast::Id m_last_id;

        ast::Id new_id() {
            ast::Id result = m_last_id;
            m_last_id.id++;
            return result;
        }
    };

    using Bounds = std::unordered_set<ast::Type>;

    struct GenericParam {
        Bounds bounds;
        std::optional<ast::Id> assigned_id;

        GenericParam(const Bounds& bounds,
            std::optional<ast::Id> assigned_id = std::nullopt);
    };

    using GenericParams = std::unordered_map<std::string_view, GenericParam>;

    class ScopedIdStack {
    public:
        ScopedIdStack();

        void push();

        ast::Id new_id();

        void pop();

    private:
        std::vector<ast::Id> m_frames;
    };

    template<typename StoredType, bool AllowShadowing>
    class ScopeStack {
    public:
        struct Entry {
            ast::Id id;
            StoredType& element;

            Entry(ast::Id id, StoredType& element) :
                id(id),
                element(element) { }
        };

        ScopeStack() :
            m_scopes(),
            m_id_stack() {
            m_scopes.reserve(20);
        }

        void push() {
            m_scopes.emplace_back();
            m_id_stack.push();
        }

        bool put(std::string_view name, const StoredType& element) {
            if constexpr (AllowShadowing) {
                if (m_scopes.back().has(name)) {
                    return false;
                }
            } else {
                if (has(name)) {
                    return false;
                }
            }
            ast::Id id = m_id_stack.new_id();
            m_scopes.back().put(name, element, id);
            return true;
        }

        bool has(std::string_view name) {
            for (auto& scope : m_scopes) {
                if (scope.has(name)) {
                    return true;
                }
            }
            return false;
        }

        Entry get(std::string_view name) {
            for (auto it = m_scopes.rbegin(); it != m_scopes.rend(); ++it) {
                auto& scope = *it;
                if (scope.has(name)) {
                    return scope.get(name);
                }
            }
            throw std::out_of_range("name not found in scope stack");
        }

        bool has_id(ast::Id id) {
            for (auto& scope : m_scopes) {
                if (scope.has_id(id)) {
                    return true;
                }
            }
            return false;
        }

        Entry get_by_id(ast::Id id) {
            for (auto it = m_scopes.rbegin(); it != m_scopes.rend(); ++it) {
                auto& scope = *it;
                if (scope.has_id(id)) {
                    return scope.get_by_id(id);
                }
            }
            throw std::out_of_range("id not found in scope stack");
        }

        void pop() {
            m_scopes.pop_back();
            m_id_stack.pop();
        }

    private:
        class Scope {
        public:
            Scope() :
                m_element_ids(),
                m_elements() { }

            void put(
                std::string_view name, const StoredType& element, ast::Id id) {
                m_element_ids.insert({ name, id });
                m_elements.insert({ id, element });
            }

            bool has(std::string_view name) {
                return m_element_ids.contains(name);
            }

            Entry get(std::string_view name) {
                ast::Id id = m_element_ids.at(name);
                return get_by_id(id);
            }

            bool has_id(ast::Id id) { return m_elements.contains(id); }

            Entry get_by_id(ast::Id id) { return Entry(id, m_elements.at(id)); }

        private:
            std::unordered_map<std::string_view, ast::Id> m_element_ids;
            std::unordered_map<ast::Id, StoredType> m_elements;
        };

        std::vector<Scope> m_scopes;
        ScopedIdStack m_id_stack;
        bool m_allow_shadowing;
    };

    struct Decl {
        struct Func {
            GenericParams generic_params;
            ast::Func func;
        };

        using Val = std::variant<Func>;

        bool is_export;
        Val val;
    };

    struct UserDefinedType {
        struct MemberFunc {
            GenericParams decls_block_bounds;
            GenericParams generic_params;
            ast::Func func;
        };

        struct InterfaceImplementation {
            Storage<MemberFunc> members;
        };

        struct Struct {
            struct Member {
                struct Field {
                    ast::Type type;
                };

                using Val = std::variant<Field, MemberFunc>;

                Val val;
                bool is_export;
            };

            Storage<Member> members;
        };

        struct Enum {
            struct Member {
                using Val = std::variant<std::string_view, MemberFunc>;

                Val val;
                bool is_export;
            };

            Storage<Member> members;
        };

        struct Interface {
            struct Member {
                GenericParams generic_params;
                ast::Func::Signature signature;
            };

            Storage<Member> member_functions;
        };

        std::optional<ast::Id> lookup_member_func(std::string_view name);

        using Val = std::variant<Struct, Enum, Interface>;
        GenericParams generic_params;
        Val val;
        bool is_export;
        std::unordered_map<ast::Type, InterfaceImplementation>
            unresolved_interface_implementations;
        std::unordered_map<ast::Type::ResolvedUserDefinedType,
            InterfaceImplementation>
            interface_implementations;
    };

    struct NameIndex {
        Storage<UserDefinedType> user_defined_types;
        Storage<Decl> decls;
    };

    class NameResolver {
    public:
        NameResolver(NameIndex& name_index);

        void set_diag_sink(diag::DiagSink& sink);

        void resolve(ast::Module& module);

    private:
        struct InterfaceMemberHandle {
            ast::Id interface_type_id;
            ast::Id member_id;
        };

        struct DeclsBlockElement {
            std::string_view name;
            bool is_export;
            UserDefinedType::MemberFunc func;
        };

        NameIndex* m_name_index;
        diag::DiagEmitter m_diag_emitter;
        ScopeStack<std::string_view, true> m_scope_stack;
        ScopeStack<GenericParam, false> m_generic_scope_stack;

        void collect_declarations(ast::Module& module);

        void collect_decl(ast::Decl& decl);

        GenericParams create_generic_params(
            ast::util::List<ast::Decl::GenericParam> ast_params);

        void collect_func(std::string_view name, bool is_export,
            ast::util::List<ast::Decl::GenericParam> ast_generic_params,
            ast::Func& func);

        // bool check_decls_block_bounds(const GenericParams& type_params,
        //     const GenericParams& block_bounds);

        void collect_struct(std::string_view name, bool is_export,
            ast::util::List<ast::Decl::GenericParam> ast_generic_params,
            ast::Decl::Struct& struct_);

        void collect_enum(
            std::string_view name, bool is_export, ast::Decl::Enum& enum_);

        void collect_interface(std::string_view name, bool is_export,
            ast::util::List<ast::Decl::GenericParam> ast_generic_params,
            ast::Decl::Interface& interface);

        void resolve_interfaces_in_interface_implementations();

        void resolve_declarations();

        void resolve_func(Decl::Func& func);

        void resolve_ast_func(ast::Func& func);

        void resolve_ast_func_signature(
            ast::Func::Signature& signature, bool put_params_to_stack);

        void resolve_ast_block(ast::Block& block);

        void resolve_ast_stmt(ast::Stmt& stmt);

        void resolve_ast_expr(ast::Expr& expr);

        void resolve_ast_expr_literal(ast::Expr::Literal& literal);

        void resolve_ast_type(ast::Type& type);

        void resolve_generic_params(GenericParams& generic_params);

        void resolve_user_defined_type(UserDefinedType& user_defined_type);

        void resolve_struct(UserDefinedType::Struct& struct_);

        void resolve_member_func(UserDefinedType::MemberFunc& member_func);

        void resolve_enum(UserDefinedType::Enum& enum_);

        void resolve_interface(UserDefinedType::Interface& interface);

        void resolve_interface_implementation(
            UserDefinedType::InterfaceImplementation& impl);

        std::optional<std::vector<DeclsBlockElement>> get_decls_block_elements(
            const ast::Decl::DeclsBlock& block,
            const GenericParams& generic_params);

        std::vector<ast::Id> extract_type_ids_from_bounds(const Bounds& bounds);

        std::optional<InterfaceMemberHandle> search_interface_members(
            std::string_view name, const std::vector<ast::Id>& interface_ids);

        UserDefinedType::InterfaceImplementation
        create_interface_implementation(
            const std::vector<DeclsBlockElement>& members);

        void err(const diag::Err& err);
    };

}
