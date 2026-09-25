# 工程面试示例的构建入口。
# 每个示例生成独立可执行文件，使用 C++11，并注册带超时的 CTest 检查。
# 并发示例统一链接系统线程库。

find_package(Threads REQUIRED)

function(add_engineering_problem name source category)
  add_executable(${name} ${source})
  set_target_properties(${name} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
  target_link_libraries(${name} PRIVATE Threads::Threads)
  if(MSVC)
    target_compile_options(${name} PRIVATE /W4 /EHs-c-)
    target_compile_definitions(${name} PRIVATE _HAS_EXCEPTIONS=0)
  else()
    target_compile_options(${name} PRIVATE -Wall -Wextra -Wpedantic -fno-exceptions)
  endif()
  if(BUILD_TESTING)
    add_test(NAME ${name} COMMAND $<TARGET_FILE:${name}>)
    set_tests_properties(${name} PROPERTIES LABELS "engineering;${category}" TIMEOUT 30)
  endif()
endfunction()

add_engineering_problem(eng_thread_basics concurrency/01_thread_basics.cpp concurrency)
add_engineering_problem(eng_blocking_queue concurrency/02_blocking_queue.cpp concurrency)
add_engineering_problem(eng_thread_pool concurrency/03_thread_pool.cpp concurrency)
add_engineering_problem(eng_spsc_queue concurrency/04_spsc_queue.cpp concurrency)
add_engineering_problem(eng_index_snapshot concurrency/05_index_snapshot.cpp concurrency)
add_engineering_problem(eng_bitmap index/06_bitmap.cpp index)
add_engineering_problem(eng_roaring_bitmap index/07_roaring_bitmap.cpp index)
add_engineering_problem(eng_bloom_filter index/08_bloom_filter.cpp index)
add_engineering_problem(eng_posting_list index/09_posting_list.cpp index)
add_engineering_problem(eng_posting_compression index/10_posting_compression.cpp index)
add_engineering_problem(eng_inverted_index index/11_inverted_index.cpp index)
add_engineering_problem(eng_topk_pruning retrieval/12_topk_pruning.cpp retrieval)
add_engineering_problem(eng_realtime_segments retrieval/13_realtime_segments.cpp retrieval)
add_engineering_problem(eng_shard_merge retrieval/14_shard_merge.cpp retrieval)
add_engineering_problem(eng_latency_diagnosis retrieval/15_latency_diagnosis.cpp retrieval)
add_engineering_problem(eng_lru_cache cache/16_lru_cache.cpp cache)
