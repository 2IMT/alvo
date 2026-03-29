#include "resolve.h"

#include <utility>
#include <algorithm>

namespace alvo::sema::resolve {

    GenericParam::GenericParam(
        const Bounds& bounds, std::optional<ast::Id> assigned_id) :
        bounds(bounds),
        assigned_id(assigned_id) { }

    ScopedIdStack::ScopedIdStack() :
        m_frames() {
        m_frames.reserve(20);
    }

    void ScopedIdStack::push() {
        if (m_frames.size() == 0) {
            m_frames.push_back(ast::Id(0));
        } else {
            ast::Id id = m_frames.back();
            id.id++;
            m_frames.push_back(id);
        }
    }

    ast::Id ScopedIdStack::new_id() {
        ast::Id result = m_frames.back();
        m_frames.back().id++;
        return result;
    }

    void ScopedIdStack::pop() { m_frames.pop_back(); }

    std::optional<ast::Id> UserDefinedType::lookup_member_func(
        std::string_view name) {
        std::optional<ast::Id> res = std::nullopt;
        std::visit(
            util::overload {
                [&](Struct& struct_) {
                    if (!struct_.members.has(name)) {
                        return;
                    }
                    auto& member = struct_.members.get(name);
                    ast::Id id = struct_.members.get_id(name);
                    if (!std::holds_alternative<MemberFunc>(member.val)) {
                        return;
                    }
                    res = id;
                },
                [&](Enum& enum_) {
                    if (!enum_.members.has(name)) {
                        return;
                    }
                    auto& member = enum_.members.get(name);
                    ast::Id id = enum_.members.get_id(name);
                    if (!std::holds_alternative<MemberFunc>(member.val)) {
                        return;
                    }
                    res = id;
                },
                [&](Interface& interface) {
                    if (!interface.member_functions.has(name)) {
                        return;
                    }
                    ast::Id id = interface.member_functions.get_id(name);
                    res = id;
                },
            },
            val);
        return res;
    }

    NameResolver::NameResolver(NameIndex& name_index) :
        m_name_index(&name_index),
        m_diag_emitter(),
        m_scope_stack(),
        m_generic_scope_stack() { }

    void NameResolver::set_diag_sink(diag::DiagSink& sink) {
        m_diag_emitter.set_sink(sink);
    }

    void NameResolver::resolve(ast::Module& module) {
        collect_declarations(module);
        resolve_interfaces_in_interface_implementations();
        resolve_declarations();
    }

    void NameResolver::collect_declarations(ast::Module& module) {
        for (auto decl : module.decls) {
            collect_decl(decl);
        }
    }

    void NameResolver::collect_decl(ast::Decl& decl) {
        std::visit(util::overload {
                       [&](ast::Invalid&) { },
                       [&](ast::Func& func) {
                           collect_func(decl.name, decl.is_export,
                               decl.generic_params, func);
                       },
                       [&](ast::Decl::Struct struct_) {
                           collect_struct(decl.name, decl.is_export,
                               decl.generic_params, struct_);
                       },
                       [&](ast::Decl::Enum& enum_) {
                           collect_enum(decl.name, decl.is_export, enum_);
                       },
                       [&](ast::Decl::TypeAlias&) {
                           // TODO: handle type aliases
                       },
                       [&](ast::Decl::Const&) {
                           // TODO: handle consts
                       },
                       [&](ast::Decl::Interface& interface) {
                           collect_interface(decl.name, decl.is_export,
                               decl.generic_params, interface);
                       },
                   },
            decl.val);
    }

    GenericParams NameResolver::create_generic_params(
        ast::util::List<ast::Decl::GenericParam> ast_params) {
        GenericParams params;
        for (const auto& ast_param : ast_params) {
            if (ast_param.is_invalid) {
                continue;
            }
            if (params.contains(ast_param.name)) {
                err(diag::Err(diag::Err::DuplicateGenericParams {}));
                continue;
            }
            Bounds bounds;
            for (const auto& interface : ast_param.interfaces) {
                if (bounds.contains(interface)) {
                    err(diag::Err(diag::Err::DuplicateGenericBounds {}));
                    continue;
                }
                bounds.insert(interface);
            }
            params.insert({ ast_param.name, GenericParam(bounds) });
        }
        return params;
    }

    void NameResolver::collect_func(std::string_view name, bool is_export,
        ast::util::List<ast::Decl::GenericParam> ast_generic_params,
        ast::Func& func) {
        if (m_name_index->decls.has(name)) {
            err(diag::Err(diag::Err::DuplicateFunctionName {}));
            return;
        }

        if (func.is_invalid) {
            return;
        }
        if (func.signature.is_invalid) {
            return;
        }

        GenericParams generic_params =
            create_generic_params(ast_generic_params);

        // clang-format off
        m_name_index->decls.put(
            name,
            Decl {
                .is_export = is_export,
                .val = Decl::Func {
                    .generic_params = generic_params,
                    .func = func
                }
            }
        );
        // clang-format on
    }

    // bool NameResolver::check_decls_block_bounds(
    //     const GenericParams& type_params, const GenericParams& block_bounds)
    //     { if (type_params.size() != block_bounds.size()) {
    //         return false;
    //     }
    //     for (const auto& type_param : type_params) {
    //         if (!block_bounds.contains(type_param.first)) {
    //             return false;
    //         }
    //
    //         const auto& type_param_bounds = type_param.second;
    //         const auto& block_bounds_bounds =
    //         block_bounds.at(type_param.first); for (const auto&
    //         type_param_bound : type_param_bounds) {
    //             if (!block_bounds_bounds.contains(type_param_bound)) {
    //                 return false;
    //             }
    //         }
    //     }
    //     return true;
    // }

    void NameResolver::collect_struct(std::string_view name, bool is_export,
        ast::util::List<ast::Decl::GenericParam> ast_generic_params,
        ast::Decl::Struct& struct_) {
        if (struct_.is_invalid) {
            return;
        }

        if (m_name_index->user_defined_types.has(name)) {
            err(diag::Err(diag::Err::TypeRedefinition { .name = name }));
            return;
        }

        GenericParams generic_params =
            create_generic_params(ast_generic_params);
        UserDefinedType::Struct res_struct;
        for (auto& field : struct_.fields) {
            if (res_struct.members.has(field.name)) {
                err(diag::Err(diag::Err::DuplicateStructMemberName {}));
                continue;
            }
            // clang-format off
            res_struct.members.put(
                field.name, 
                UserDefinedType::Struct::Member {
                    .val = UserDefinedType::Struct::Member::Field {
                        .type = field.type 
                    },
                    .is_export = field.is_export,
                } 
            );
            // clang-format on
        }
        std::unordered_map<ast::Type, UserDefinedType::InterfaceImplementation>
            unresolved_interface_implementations;
        for (auto& decls_block : struct_.decls_blocks) {
            std::optional<std::vector<DeclsBlockElement>> decls_block_elements =
                get_decls_block_elements(decls_block, generic_params);

            if (!decls_block_elements) {
                continue;
            }

            if (decls_block.interface) { // block is interface implementation
                // clang-format off
                unresolved_interface_implementations.insert({ 
                    *decls_block.interface, 
                    create_interface_implementation(*decls_block_elements) 
                });
                // clang-format on
            } else { // block is regular decls block
                for (auto& elem : *decls_block_elements) {
                    if (res_struct.members.has(elem.name)) {
                        err(diag::Err(diag::Err::DuplicateStructMemberName {}));
                        continue;
                    }
                    // clang-format off
                    res_struct.members.put(
                        elem.name,
                        UserDefinedType::Struct::Member {
                            .val = elem.func, 
                            .is_export = elem.is_export,
                        }
                    );
                    // clang-format on
                }
            }
        }
        UserDefinedType res = {
            .generic_params = generic_params,
            .val = res_struct,
            .is_export = is_export,
            .unresolved_interface_implementations =
                unresolved_interface_implementations,
            .interface_implementations = {},
        };
        m_name_index->user_defined_types.put(name, res);
    }

    void NameResolver::collect_enum(
        std::string_view name, bool is_export, ast::Decl::Enum& enum_) {
        if (enum_.is_invalid) {
            return;
        }

        if (m_name_index->user_defined_types.has(name)) {
            err(diag::Err(diag::Err::TypeRedefinition { .name = name }));
            return;
        }

        UserDefinedType::Enum res_enum;
        for (const auto& element : enum_.elements) {
            if (element.is_invalid) {
                continue;
            }
            if (res_enum.members.has(element.name)) {
                err(diag::Err(diag::Err::DuplicateEnumMemberName {}));
                continue;
            }
            res_enum.members.put(element.name, UserDefinedType::Enum::Member {
                                                   .val = element.name,
                                                   .is_export = true,
                                               });
        }
        std::unordered_map<ast::Type, UserDefinedType::InterfaceImplementation>
            unresolved_interface_implementations;
        for (auto& decls_block : enum_.decls_blocks) {
            std::optional<std::vector<DeclsBlockElement>> decls_block_elements =
                get_decls_block_elements(decls_block, {});

            if (!decls_block_elements) {
                continue;
            }

            if (decls_block.interface) { // block is interface implementation
                // clang-format off
                unresolved_interface_implementations.insert({ 
                    *decls_block.interface, 
                    create_interface_implementation(*decls_block_elements) 
                });
                // clang-format on
            } else {
                for (auto& elem : *decls_block_elements) {
                    if (res_enum.members.has(elem.name)) {
                        err(diag::Err(diag::Err::DuplicateEnumMemberName {}));
                        continue;
                    }
                    // clang-format off
                    res_enum.members.put(
                        elem.name,
                        UserDefinedType::Enum::Member {
                            .val = elem.func,
                            .is_export = elem.is_export,
                        }
                    );
                    // clang-format on
                }
            }
        }
        UserDefinedType res = {
            .generic_params = {},
            .val = res_enum,
            .is_export = is_export,
            .unresolved_interface_implementations =
                unresolved_interface_implementations,
            .interface_implementations = {},
        };
        m_name_index->user_defined_types.put(name, res);
    }

    void NameResolver::collect_interface(std::string_view name, bool is_export,
        ast::util::List<ast::Decl::GenericParam> ast_generic_params,
        ast::Decl::Interface& interface) {
        if (interface.is_invalid) {
            return;
        }

        if (m_name_index->user_defined_types.has(name)) {
            err(diag::Err(diag::Err::TypeRedefinition { .name = name }));
            return;
        }

        GenericParams generic_params =
            create_generic_params(ast_generic_params);
        UserDefinedType::Interface res_interface;
        for (const auto& member : interface.members) {
            if (member.is_invalid) {
                continue;
            }
            if (member.signature.is_invalid) {
                continue;
            }

            if (res_interface.member_functions.has(member.name)) {
                err(diag::Err(diag::Err::DuplicateInterfaceFunctionName {}));
                continue;
            }

            GenericParams member_generic_params =
                create_generic_params(member.generic_params);

            // clang-format off
            res_interface.member_functions.put(
                member.name,
                UserDefinedType::Interface::Member {
                    .generic_params = member_generic_params,
                    .signature = member.signature,
                }
            );
            // clang-format on
        }
        UserDefinedType res = {
            .generic_params = generic_params,
            .val = res_interface,
            .is_export = is_export,
            .unresolved_interface_implementations = {},
            .interface_implementations = {},
        };
        m_name_index->user_defined_types.put(name, res);
    }

    void NameResolver::resolve_interfaces_in_interface_implementations() {
        for (auto entry : m_name_index->user_defined_types) {
            for (auto& [t, impl] :
                entry.element.unresolved_interface_implementations) {
                std::optional<ast::Type::ResolvedUserDefinedType> interface =
                    std::nullopt;
                std::visit(
                    util::overload {
                        [&](const ast::Invalid&) { },
                        [&](const ast::Type::Unit&) { },
                        [&](const ast::Type::String&) { },
                        [&](const ast::Type::Char&) { },
                        [&](const ast::Type::Int&) { },
                        [&](const ast::Type::Byte&) { },
                        [&](const ast::Type::Float&) { },
                        [&](const ast::Type::Bool&) { },
                        [&](const ast::Type::Array&) { },
                        [&](const ast::Type::Tup&) { },
                        [&](const ast::Type::Func&) { },
                        [&](const ast::Type::Name& name) {
                            if (name.is_invalid)
                                return;
                            m_name_index->user_defined_types.has(name.name);
                            UserDefinedType& found =
                                m_name_index->user_defined_types.get(name.name);
                            bool is_interface = std::holds_alternative<
                                UserDefinedType::Interface>(found.val);
                            if (!is_interface) {
                                err(diag::Err(diag::Err::NotAnInterface {}));
                                return;
                            }
                            ast::Id type_id =
                                m_name_index->user_defined_types.get_id(
                                    name.name);
                            interface = ast::Type::ResolvedUserDefinedType(
                                type_id, name.generic_params);
                        },
                        [&](const ast::Type::Ref&) { },
                        [&](const ast::Type::LocalGeneric&) {
                            ALVO_UNREACHABLE();
                        },
                        [&](const ast::Type::ResolvedUserDefinedType&) {
                            ALVO_UNREACHABLE();
                        },
                    },
                    t.val);

                if (!interface) {
                    err(diag::Err(diag::Err::NotAnInterface {}));
                    continue;
                }

                entry.element.interface_implementations.insert(
                    { *interface, impl });
            }
        }
    }

    void NameResolver::resolve_declarations() {
        for (auto entry : m_name_index->decls) {
            std::visit(util::overload { [&](Decl::Func& func) {
                resolve_func(func);
            } },
                entry.element.val);
        }

        for (auto entry : m_name_index->user_defined_types) {
            resolve_user_defined_type(entry.element);
        }
    }

    void NameResolver::resolve_func(Decl::Func& func) {
        m_generic_scope_stack.push();
        resolve_generic_params(func.generic_params);

        resolve_ast_func(func.func);

        m_generic_scope_stack.pop();
    }

    void NameResolver::resolve_ast_func(ast::Func& func) {
        m_scope_stack.push();

        resolve_ast_func_signature(func.signature, true);

        resolve_ast_block(func.block);

        m_scope_stack.pop();
    }

    void NameResolver::resolve_ast_func_signature(
        ast::Func::Signature& signature, bool put_params_to_stack) {
        if (signature.is_invalid)
            return;

        if (signature.is_self_func && put_params_to_stack) {
            m_scope_stack.put("self", "self");
        }

        std::vector<std::string_view> used_param_names;
        used_param_names.reserve(10);
        for (auto& param : signature.params) {
            if (param.is_invalid) {
                continue;
            }

            bool duplicate_param_found =
                std::find(used_param_names.begin(), used_param_names.end(),
                    param.name) != used_param_names.end();
            if (duplicate_param_found) {
                err(diag::Err(diag::Err::DuplicateFuncParamName {}));
                continue;
            }
            used_param_names.push_back(param.name);

            resolve_ast_type(param.type);

            if (put_params_to_stack) {
                // no duplicate names guaranteed
                m_scope_stack.put(param.name, param.name);
            }
        }

        resolve_ast_type(signature.ret);
    }

    void NameResolver::resolve_ast_block(ast::Block& block) {
        if (block.is_invalid)
            return;

        m_scope_stack.push();

        for (auto& stmt : block.stmts) {
            resolve_ast_stmt(stmt);
        }

        m_scope_stack.pop();
    }

    void NameResolver::resolve_ast_stmt(ast::Stmt& stmt) {
        std::visit(util::overload {
                       [&](ast::Invalid&) { },
                       [&](ast::Expr& expr) { resolve_ast_expr(expr); },
                       [&](ast::Stmt::Let& let) {
                           if (let.is_invalid)
                               return;
                           if (let.type) {
                               resolve_ast_type(*let.type);
                           }
                           resolve_ast_expr(*let.expr);
                           if (!m_scope_stack.put(let.name, let.name)) {
                               err(diag::Err(diag::Err::VariableRedefinition {
                                   .name = let.name }));
                           }
                       },
                       [&](ast::Stmt::If& if_) {
                           if (if_.is_invalid)
                               return;
                           resolve_ast_expr(if_.expr);
                           for (auto& elif : if_.elifs) {
                               resolve_ast_expr(elif.expr);
                               resolve_ast_block(elif.block);
                           }
                           if (if_.else_) {
                               resolve_ast_block(*if_.else_);
                           }
                       },
                       [&](ast::Stmt::Switch& switch_) {
                           if (switch_.is_invalid)
                               return;
                           resolve_ast_expr(switch_.expr);
                           for (auto& case_ : switch_.cases) {
                               if (case_.is_invalid)
                                   continue;
                               if (case_.expr) {
                                   resolve_ast_expr(*case_.expr);
                               }
                               resolve_ast_block(case_.block);
                           }
                       },
                       [&](ast::Stmt::Loop& loop) {
                           if (loop.is_invalid)
                               return;
                           resolve_ast_block(loop.block);
                       },
                       [&](ast::Stmt::For& for_) {
                           if (for_.is_invalid)
                               return;
                           m_scope_stack.push();
                           resolve_ast_expr(for_.expr);
                           m_scope_stack.put(for_.name, for_.name);
                           resolve_ast_block(for_.block);
                           m_scope_stack.pop();
                       },
                       [&](ast::Stmt::While& while_) {
                           if (while_.is_invalid)
                               return;
                           resolve_ast_expr(while_.expr);
                           resolve_ast_block(while_.block);
                       },
                       [&](ast::Stmt::Return& return_) {
                           if (return_.is_invalid)
                               return;
                           if (return_.expr) {
                               resolve_ast_expr(*return_.expr);
                           }
                       },
                       [&](ast::Stmt::Defer& defer) {
                           if (defer.is_invalid)
                               return;
                           resolve_ast_expr(defer.expr);
                       },
                       [&](ast::Block& block) { resolve_ast_block(block); },
                       [&](ast::Stmt::Continue&) { },
                       [&](ast::Stmt::Break&) { },
                   },
            stmt.val);
    }

    void NameResolver::resolve_ast_expr(ast::Expr& expr) {
        std::visit(
            util::overload { [&](ast::Invalid&) { },
                [&](ast::Expr::Literal& literal) {
                    resolve_ast_expr_literal(literal);
                },
                [&](ast::Expr::Unop& unop) { resolve_ast_expr(*unop.expr); },
                [&](ast::Expr::Binop& binop) {
                    resolve_ast_expr(*binop.lhs);
                    resolve_ast_expr(*binop.rhs);
                },
                [&](ast::Expr::Index& index) {
                    resolve_ast_expr(*index.expr);
                    resolve_ast_expr(*index.index);
                },
                [&](ast::Expr::Call& call) {
                    resolve_ast_expr(*call.expr);
                    for (auto& arg : call.args) {
                        resolve_ast_expr(arg);
                    }
                },
                [&](ast::Expr::Cast& cast) {
                    resolve_ast_expr(*cast.expr);
                    resolve_ast_type(cast.type);
                },
                [&](ast::Expr::TryCast& try_cast) {
                    resolve_ast_expr(*try_cast.expr);
                    resolve_ast_type(try_cast.type);
                },
                [&](ast::Expr::Ref& ref) {
                    if (ref.is_invalid)
                        return;
                    resolve_ast_expr(*ref.expr);
                },
                [&](ast::Expr::Builtin& builtin) {
                    if (builtin.is_invalid)
                        return;
                    for (auto& generic_param : builtin.generic_params) {
                        resolve_ast_type(generic_param);
                    }
                    for (auto& arg : builtin.args) {
                        resolve_ast_expr(arg);
                    }
                },
                [&](ast::Expr::Name& name) {
                    if (name.is_invalid)
                        return;
                    for (auto& generic_param : name.generic_params) {
                        resolve_ast_type(generic_param);
                    }
                    if (m_scope_stack.has(name.name)) {
                        auto entry = m_scope_stack.get(name.name);
                        expr.val = ast::Expr::LocalVar(entry.id);
                    } else if (m_name_index->decls.has(name.name)) {
                        ast::Id id = m_name_index->decls.get_id(name.name);
                        expr.val =
                            ast::Expr::ResolvedDecl(id, name.generic_params);
                    }
                },
                [&](ast::Expr::TypeMemberAccess& type_member_access) {
                    if (type_member_access.is_invalid)
                        return;
                    if (type_member_access.name.is_invalid)
                        return;
                    ast::Type type = type_member_access.type;
                    resolve_ast_type(type);
                    for (auto& generic_param :
                        type_member_access.name.generic_params) {
                        resolve_ast_type(generic_param);
                    }
                    std::visit(
                        util::overload {
                            [&](ast::Invalid&) { ALVO_UNREACHABLE(); },
                            [&](ast::Type::Unit&) { ALVO_NOT_IMPLEMENTED(); },
                            [&](ast::Type::String&) { ALVO_NOT_IMPLEMENTED(); },
                            [&](ast::Type::Char&) { ALVO_NOT_IMPLEMENTED(); },
                            [&](ast::Type::Int&) { ALVO_NOT_IMPLEMENTED(); },
                            [&](ast::Type::Byte&) { ALVO_NOT_IMPLEMENTED(); },
                            [&](ast::Type::Float&) { ALVO_NOT_IMPLEMENTED(); },
                            [&](ast::Type::Bool&) { ALVO_NOT_IMPLEMENTED(); },
                            [&](ast::Type::Array&) { ALVO_NOT_IMPLEMENTED(); },
                            [&](ast::Type::Tup&) { ALVO_NOT_IMPLEMENTED(); },
                            [&](ast::Type::Func&) { ALVO_NOT_IMPLEMENTED(); },
                            [&](ast::Type::Name&) { ALVO_UNREACHABLE(); },
                            [&](ast::Type::Ref&) { ALVO_NOT_IMPLEMENTED(); },
                            [&](ast::Type::LocalGeneric& local_generic) {
                                auto entry = m_generic_scope_stack.get_by_id(
                                    local_generic.id);

                                if (entry.element.bounds.empty()) {
                                    err(diag::Err(diag::Err::
                                            MemberAccessOnGenericWithNoBounds {}));
                                    return;
                                }

                                std::vector<ast::Id> interface_ids =
                                    extract_type_ids_from_bounds(
                                        entry.element.bounds);

                                auto handle = search_interface_members(
                                    type_member_access.name.name,
                                    interface_ids);
                                if (!handle) {
                                    // exact member not found
                                    return;
                                }

                                expr.val = ast::Expr::ResolvedTypeMemberAccess(
                                    ast::Type(
                                        ast::Type::ResolvedUserDefinedType(
                                            handle->interface_type_id, {}),
                                        type_member_access.type.nullable),
                                    handle->member_id,
                                    type_member_access.name.generic_params);
                            },
                            [&](ast::Type::ResolvedUserDefinedType&
                                    user_defined_type) {
                                auto& type =
                                    m_name_index->user_defined_types.get_by_id(
                                        user_defined_type.id);

                                auto member_id = type.lookup_member_func(
                                    type_member_access.name.name);
                                if (!member_id) {
                                    err(diag::Err(diag::Err::NoMemberFound {}));
                                    return;
                                }
                                expr.val = ast::Expr::ResolvedTypeMemberAccess(
                                    ast::Type(user_defined_type,
                                        type_member_access.type.nullable),
                                    *member_id,
                                    type_member_access.name.generic_params);
                            },
                        },
                        type.val);
                },
                [&](ast::Expr::MemberAccess& member_access) {
                    resolve_ast_expr(*member_access.expr);
                    for (auto& generic_param :
                        member_access.name.generic_params) {
                        resolve_ast_type(generic_param);
                    }
                },
                [&](ast::Expr::LocalVar&) { ALVO_UNREACHABLE(); },
                [&](ast::Expr::ResolvedDecl&) { ALVO_UNREACHABLE(); },
                [&](ast::Expr::ResolvedMemberAccess&) { ALVO_UNREACHABLE(); },
                [&](ast::Expr::ResolvedTypeMemberAccess&) {
                    ALVO_UNREACHABLE();
                },
                [&](ast::Expr::ResolvedGenericMemberAccess&) {
                    ALVO_UNREACHABLE();
                } },
            expr.val);
    }

    void NameResolver::resolve_ast_expr_literal(ast::Expr::Literal& literal) {
        std::visit(
            util::overload { [&](ast::Invalid&) { },
                [&](ast::Expr::Literal::Unit&) { },
                [&](ast::Expr::Literal::Null&) { },
                [&](ast::Expr::Literal::String&) { },
                [&](ast::Expr::Literal::Character&) { },
                [&](ast::Expr::Literal::Integer&) { },
                [&](ast::Expr::Literal::Byte&) { },
                [&](ast::Expr::Literal::Floating&) { },
                [&](ast::Expr::Literal::Boolean&) { },
                [&](ast::Expr::Literal::Array& array) {
                    std::visit(
                        util::overload {
                            [&](ast::Invalid&) { },
                            [&](ast::Expr::Literal::Array::Regular& regular) {
                                for (auto& element : regular.elements) {
                                    resolve_ast_expr(element);
                                }
                            },
                            [&](ast::Expr::Literal::Array::DefaultNTimes&
                                    default_n_times) {
                                resolve_ast_expr(*default_n_times.times);
                            },
                            [&](ast::Expr::Literal::Array::ExprNTimes&
                                    expr_n_times) {
                                resolve_ast_expr(*expr_n_times.expr);
                                resolve_ast_expr(*expr_n_times.times);
                            },
                        },
                        array.val);
                },
                [&](ast::Expr::Literal::Tup& tup) {
                    if (tup.is_invalid)
                        return;
                    for (auto& expr : tup.exprs) {
                        resolve_ast_expr(expr);
                    }
                },
                [&](ast::Expr::Literal::Struct& struct_) {
                    if (struct_.is_invalid)
                        return;
                    resolve_ast_type(struct_.type);
                    for (auto& field : struct_.fields) {
                        if (field.is_invalid)
                            continue;
                        resolve_ast_expr(*field.expr);
                    }
                },
                [&](ast::util::Ptr<ast::Func> func) {
                    resolve_ast_func(*func);
                } },
            literal.val);
    }

    void NameResolver::resolve_ast_type(ast::Type& type) {
        std::visit(
            util::overload { [&](ast::Invalid&) { }, [&](ast::Type::Unit&) { },
                [&](ast::Type::String&) { }, [&](ast::Type::Char&) { },
                [&](ast::Type::Int&) { }, [&](ast::Type::Byte&) { },
                [&](ast::Type::Float&) { }, [&](ast::Type::Bool&) { },
                [&](ast::Type::Array& array) {
                    if (array.is_invalid)
                        return;
                    resolve_ast_type(*array.type);
                },
                [&](ast::Type::Tup& tup) {
                    if (tup.is_invalid)
                        return;
                    for (auto& type : tup.types) {
                        resolve_ast_type(type);
                    }
                },
                [&](ast::Type::Func& func) {
                    if (func.is_invalid)
                        return;
                    for (auto& param : func.params) {
                        resolve_ast_type(param);
                    }
                    resolve_ast_type(*func.return_type);
                },
                [&](ast::Type::Name& name) {
                    if (name.is_invalid)
                        return;

                    for (auto& generic_param : name.generic_params) {
                        resolve_ast_type(generic_param);
                    }

                    if (m_generic_scope_stack.has(name.name)) {
                        auto entry = m_generic_scope_stack.get(name.name);
                        type.val = ast::Type::LocalGeneric(entry.id);
                        return;
                    }

                    if (m_name_index->user_defined_types.has(name.name)) {
                        type.val = ast::Type::ResolvedUserDefinedType(
                            m_name_index->user_defined_types.get_id(name.name),
                            name.generic_params);
                        return;
                    }

                    err(diag::Err(diag::Err::UndeclaredType(name.name)));
                    type.val = ast::Invalid {};
                },
                [&](ast::Type::Ref& ref) {
                    if (ref.is_invalid)
                        return;
                    resolve_ast_type(*ref.type);
                },
                [&](ast::Type::LocalGeneric&) { ALVO_UNREACHABLE(); },
                [&](ast::Type::ResolvedUserDefinedType&) {
                    ALVO_UNREACHABLE();
                } },
            type.val);
    }

    void NameResolver::resolve_generic_params(GenericParams& generic_params) {
        for (auto param : generic_params) {
            Bounds resolved_bounds;
            for (auto& bound : param.second.bounds) {
                ast::Type tmp = bound;
                resolve_ast_type(tmp);
                resolved_bounds.insert(tmp);
            }
            param.second.bounds = std::move(resolved_bounds);
            m_generic_scope_stack.put(param.first, param.second);
            // TODO: somehow put assigned id right away instead of looking it up
            // and setting it
            auto entry = m_generic_scope_stack.get(param.first);
            entry.element.assigned_id = entry.id;
        }
    }

    void NameResolver::resolve_user_defined_type(
        UserDefinedType& user_defined_type) {
        m_generic_scope_stack.push();
        resolve_generic_params(user_defined_type.generic_params);

        std::visit(
            util::overload { [&](UserDefinedType::Struct& struct_) {
                                resolve_struct(struct_);
                            },
                [&](UserDefinedType::Enum& enum_) { resolve_enum(enum_); },
                [&](UserDefinedType::Interface interface) {
                    resolve_interface(interface);
                } },
            user_defined_type.val);

        for (auto& [type, impl] : user_defined_type.interface_implementations) {
            resolve_interface_implementation(impl);
        }

        m_generic_scope_stack.pop();
    }

    void NameResolver::resolve_struct(UserDefinedType::Struct& struct_) {
        for (auto entry : struct_.members) {
            std::visit(util::overload {
                           [&](UserDefinedType::Struct::Member::Field& field) {
                               resolve_ast_type(field.type);
                           },
                           [&](UserDefinedType::MemberFunc& member_func) {
                               resolve_member_func(member_func);
                           } },
                entry.element.val);
        }
    }

    void NameResolver::resolve_member_func(
        UserDefinedType::MemberFunc& member_func) {
        m_generic_scope_stack.push();
        resolve_generic_params(member_func.generic_params);

        resolve_ast_func(member_func.func);

        m_generic_scope_stack.pop();
    }

    void NameResolver::resolve_enum(UserDefinedType::Enum& enum_) {
        for (auto entry : enum_.members) {
            std::visit(util::overload { [&](std::string_view&) { },
                           [&](UserDefinedType::MemberFunc& member_func) {
                               resolve_member_func(member_func);
                           } },
                entry.element.val);
        }
    }

    void NameResolver::resolve_interface(
        UserDefinedType::Interface& interface) {
        for (auto entry : interface.member_functions) {
            resolve_generic_params(entry.element.generic_params);
            resolve_ast_func_signature(entry.element.signature, false);
        }
    }

    void NameResolver::resolve_interface_implementation(
        UserDefinedType::InterfaceImplementation& impl) {
        for (auto entry : impl.members) {
            resolve_member_func(entry.element);
        }
    }

    std::optional<std::vector<NameResolver::DeclsBlockElement>>
    NameResolver::get_decls_block_elements(const ast::Decl::DeclsBlock& block,
        const GenericParams& generic_params) {
        std::vector<NameResolver::DeclsBlockElement> res;
        if (block.is_invalid) {
            return std::nullopt;
        }

        // GenericParams block_bounds =
        //     create_generic_params(block.generic_params);
        // if (!check_decls_block_bounds(generic_params, block_bounds)) {
        //     err(diag::Err(diag::Err::InvalidBounds {}));
        //     return std::nullopt;
        // }

        for (auto& decl : block.decls) {
            using OptFunc = std::optional<ast::Func>;
            OptFunc func = std::visit(
                util::overload {
                    [&](const ast::Invalid&) { return OptFunc(); },
                    [&](const ast::Func& func) { return OptFunc(func); },
                    [&](const ast::Decl::Struct&) { return OptFunc(); },
                    [&](const ast::Decl::Enum&) { return OptFunc(); },
                    [&](const ast::Decl::TypeAlias&) { return OptFunc(); },
                    [&](const ast::Decl::Const&) { return OptFunc(); },
                    [&](const ast::Decl::Interface&) { return OptFunc(); },
                },
                decl.val);

            if (!func) {
                err(diag::Err(diag::Err::NotAFunction {}));
                continue;
            }

            if (func->is_invalid) {
                continue;
            }

            GenericParams func_generic_params =
                create_generic_params(decl.generic_params);

            // clang-format off
            res.push_back(
                DeclsBlockElement {
                    .name = decl.name,
                    .is_export = decl.is_export,
                    .func = UserDefinedType::MemberFunc {
                        .decls_block_bounds = {},
                        .generic_params = func_generic_params,
                        .func = *func,
                    }
                }
            );
            // clang-format on
        }

        return res;
    }

    std::vector<ast::Id> NameResolver::extract_type_ids_from_bounds(
        const Bounds& bounds) {
        std::vector<ast::Id> res;
        for (const auto& type : bounds) {
            if (auto resolved_type =
                    std::get_if<ast::Type::ResolvedUserDefinedType>(
                        &type.val)) {
                auto& user_defined_type =
                    m_name_index->user_defined_types.get_by_id(
                        resolved_type->id);
                if (std::holds_alternative<UserDefinedType::Interface>(
                        user_defined_type.val)) {
                    res.push_back(resolved_type->id);
                }
            }
        }
        return res;
    }

    std::optional<NameResolver::InterfaceMemberHandle>
    NameResolver::search_interface_members(
        std::string_view name, const std::vector<ast::Id>& interface_ids) {
        std::optional<ast::Id> interface_type_id;
        std::optional<ast::Id> member_id;
        for (const auto& id : interface_ids) {
            auto& user_defined_type =
                m_name_index->user_defined_types.get_by_id(id);
            if (auto interface = std::get_if<UserDefinedType::Interface>(
                    &user_defined_type.val)) {
                if (!interface->member_functions.has(name)) {
                    continue;
                }
                if (interface_type_id.has_value()) {
                    err(diag::Err(diag::Err::AmbiguousReference {}));
                    return std::nullopt;
                }

                interface_type_id = id;
                member_id = interface->member_functions.get_id(name);
            } else {
                ALVO_UNREACHABLE();
            }
        }

        if (!interface_type_id.has_value()) {
            return std::nullopt;
        }

        return InterfaceMemberHandle {
            .interface_type_id = *interface_type_id,
            .member_id = *member_id,
        };
    }

    UserDefinedType::InterfaceImplementation
    NameResolver::create_interface_implementation(
        const std::vector<DeclsBlockElement>& members) {
        UserDefinedType::InterfaceImplementation res;
        for (const auto& member : members) {
            res.members.put(member.name, member.func);
        }
        return res;
    }

    void NameResolver::err(const diag::Err& err) {
        m_diag_emitter.err("TODO", tok::Pos(0, 0, 0), err);
    }

}
