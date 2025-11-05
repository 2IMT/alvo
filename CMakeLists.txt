cmake_minimum_required(VERSION 3.27)

include(FetchContent)

set(ALVO_VERSION "0.1.0")

project(alvo VERSION ${ALVO_VERSION} LANGUAGES C CXX)

# Project options

option(ALVO_TEST "Build tests" ON)
option(ALVO_SANITIZE "Enable sanitizers for main target" OFF)
option(ALVO_TEST_SANITIZE "Enable sanitizers for test target" ON)

# Set warning Flags
if (MSVC)
    set(ALVO_WARNING_FLAGS /W4)
else()
    set(ALVO_WARNING_FLAGS -Wall -Wunused -Wpedantic -Wextra)
endif()

# Enable sanitizers for specified target
function(alvo_enable_sanitizers target)
    if (MSVC)
        target_compile_options(${target} PRIVATE /fsanitize=address)
        target_link_options(${target} PRIVATE /fsanitize=address)
    else()
        set(ALVO_SANITIZERS address undefined)

        if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            list(APPEND ALVO_SANITIZERS leak)
        endif()

        string(JOIN "," ALVO_SANITIZER_FLAGS ${ALVO_SANITIZERS})
        target_compile_options(${target} PRIVATE
            -fsanitize=${ALVO_SANITIZER_FLAGS}
            -fno-omit-frame-pointer
        )
        target_link_options(${target} PRIVATE
            -fsanitize=${ALVO_SANITIZER_FLAGS}
        )
    endif()
endfunction()

# Set common settings for specified target
function(alvo_setup_target target)
    set_target_properties(${target} PROPERTIES
        CXX_STANDARD 20
    )
    target_compile_options(${target} PRIVATE ${ALVO_WARNING_FLAGS})
    target_compile_definitions(${target} PRIVATE "ALVO_DEBUG=$<IF:$<CONFIG:Debug>,1,0>")
    target_compile_definitions(${target} PRIVATE "ALVO_VERSION=\"${ALVO_VERSION}\"")
endfunction()

# Create main target

set(ALVO_SOURCES
    "src/lex.cpp"
    "src/utf8.cpp"
    "src/diag.cpp"
    "src/mem.cpp"
    "src/parse.cpp"
    "src/ast.cpp"
    "src/ast/print.cpp"
    "src/args.cpp"
)

add_executable(alvo "src/main.cpp" ${ALVO_SOURCES})
alvo_setup_target(alvo)
if (ALVO_SANITIZE)
    alvo_enable_sanitizers(alvo)
endif()

FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt
    GIT_TAG 12.0.0
)
FetchContent_MakeAvailable(fmt)

FetchContent_Declare(
     expected
     GIT_REPOSITORY https://github.com/TartanLlama/expected
     GIT_TAG v1.3.1
 )
set(EXPECTED_BUILD_TESTS OFF)
FetchContent_MakeAvailable(expected)

target_link_libraries(alvo fmt::fmt tl::expected)

# Create test target

if (ALVO_TEST)
    set(ALVO_TESTS
        "test/mem.cpp"
    )

    add_executable(alvo_test ${ALVO_TESTS} ${ALVO_SOURCES})
    alvo_setup_target(alvo_test)
    if (ALVO_TEST_SANITIZE)
        alvo_enable_sanitizers(alvo_test)
    endif()
    target_include_directories(alvo_test PRIVATE "src")

    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.10.0
    )
    FetchContent_MakeAvailable(Catch2)

    target_link_libraries(alvo_test Catch2::Catch2WithMain tl::expected fmt::fmt)

    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
    include(CTest)
    include(Catch)
    catch_discover_tests(alvo_test)
endif()
