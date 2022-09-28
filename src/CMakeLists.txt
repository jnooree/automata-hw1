file(GLOB PRJ_SRCS *.cpp)
list(FILTER PRJ_SRCS EXCLUDE REGEX "main_.+\.cpp$")
add_library(project_lib STATIC "${PRJ_SRCS}")

function(add_project_executable name)
  add_executable(${name} "main_${name}.cpp")
  target_link_libraries(${name} project_lib)
  set_target_properties(${name} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR})

  install(TARGETS ${name} RUNTIME)
endfunction()

add_project_executable(builder)
add_project_executable(runner)
