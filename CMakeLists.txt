cmake_minimum_required(VERSION 3.27)

project(alvo VERSION 0.1.0 LANGUAGES C CXX)

set(ALVO_SOURCES
    "src/main.cpp"
    "src/lex.cpp"
    "src/utf8.cpp"
    "src/diag.cpp"
    "src/mem.cpp"
    "src/parse.cpp"
    "src/ast.cpp"
    "src/ast/print.cpp"
)

add_executable(alvo ${ALVO_SOURCES})
set_target_properties(alvo PROPERTIES
    CXX_STANDARD 20
)

if (MSVC)
    set(ALVO_WARNING_FLAGS /W4)
else()
    set(ALVO_WARNING_FLAGS -Wall -Wunused -Wpedantic -Wextra)
endif()

target_compile_options(alvo PRIVATE ${ALVO_WARNING_FLAGS})
target_compile_definitions(alvo PRIVATE "ALVO_DEBUG=$<IF:$<CONFIG:Debug>,1,0>")

include(FetchContent)

FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt
    GIT_TAG 12.0.0)
FetchContent_MakeAvailable(fmt)

target_link_libraries(alvo fmt::fmt)
