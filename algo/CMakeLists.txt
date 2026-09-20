# SOURCE paths are relative to this directory. Register only standalone problems.

add_leetcode_problem(
    NAME lc_704_binary_search
    SOURCE binary_search/704_binary_search.cpp
    CATEGORY binary_search
)

add_leetcode_problem(
    NAME lc_121_best_time_to_buy_and_sell_stock
    SOURCE DP/121_best_time_to_buy_and_sell_stock.cpp
    CATEGORY dynamic_programming
)

add_leetcode_problem(
    NAME lc_122_best_time_to_buy_and_sell_stock_ii
    SOURCE DP/122_best_time_to_buy_and_sell_stock_ii.cpp
    CATEGORY greedy
)

add_leetcode_problem(
    NAME bm_74_restore_ip_addresses
    SOURCE DP/bm74_restore_ip_addresses.cpp
    CATEGORY backtracking
)

add_leetcode_problem(
    NAME lc_27_remove_element
    SOURCE string_and_array/27_remove_element.cpp
    CATEGORY two_pointers
)

add_leetcode_problem(
    NAME lc_15_three_sum
    SOURCE string_and_array/3sum.cpp
    CATEGORY two_pointers
)

add_leetcode_problem(
    NAME lc_384_shuffle_an_array
    SOURCE others/shuffle.cpp
    CATEGORY array
)
