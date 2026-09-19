include_guard(GLOBAL)

function(add_leetcode_problem)
  set(one_value_args NAME SOURCE CATEGORY)
  cmake_parse_arguments(PROBLEM "" "${one_value_args}" "" ${ARGN})

  foreach(required_arg NAME SOURCE CATEGORY)
    if(NOT PROBLEM_${required_arg})
      message(FATAL_ERROR "add_leetcode_problem requires ${required_arg}")
    endif()
  endforeach()

  if(TARGET "${PROBLEM_NAME}")
    message(FATAL_ERROR "Duplicate LeetCode target: ${PROBLEM_NAME}")
  endif()

  get_filename_component(source_path "${PROBLEM_SOURCE}" ABSOLUTE BASE_DIR "${PROJECT_SOURCE_DIR}")
  if(NOT EXISTS "${source_path}")
    message(FATAL_ERROR "Source file does not exist: ${PROBLEM_SOURCE}")
  endif()

  add_executable("${PROBLEM_NAME}" "${source_path}")
  target_compile_features("${PROBLEM_NAME}" PRIVATE cxx_std_17)

  if(MSVC)
    target_compile_options("${PROBLEM_NAME}" PRIVATE /W4)
  else()
    target_compile_options("${PROBLEM_NAME}" PRIVATE -Wall -Wextra -Wpedantic)
  endif()

  set_target_properties("${PROBLEM_NAME}" PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

  if(BUILD_TESTING)
    add_test(NAME "${PROBLEM_NAME}" COMMAND "$<TARGET_FILE:${PROBLEM_NAME}>")
    set_tests_properties("${PROBLEM_NAME}" PROPERTIES LABELS "${PROBLEM_CATEGORY}")
  endif()
endfunction()
