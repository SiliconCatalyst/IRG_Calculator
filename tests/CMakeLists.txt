# Dependency-free for scaffolding purposes. Once the domain logic is fleshed
# out, swap this for Catch2 or GoogleTest via FetchContent.
add_executable(irg_calc_tests
    test_irg_calculator.cpp
)
target_link_libraries(irg_calc_tests PRIVATE irg_core)

add_test(NAME irg_calculator_tests COMMAND irg_calc_tests)
