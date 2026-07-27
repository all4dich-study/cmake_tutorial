cmake_minimum_required(VERSION 3.10)

project(Tutorial VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(USE_MYMATH "Use tutorial provided math implementation" OFF)

# Directory that holds the auto-detected library subdirectories.
set(TARGET_DIR "target")

# Subdirectories to skip during auto-detection, e.g.
#   cmake -S . -B build -DEXCLUDE_SUBDIRS="MathFunctions;experimental"
set(EXCLUDE_SUBDIRS "" CACHE STRING
    "Semicolon-separated list of subdirectories to exclude from auto-detection")

configure_file(TutorialConfig.h.in TutorialConfig.h)

# Add every immediate subdirectory of ${TARGET_DIR} that has its own
# CMakeLists.txt, then link whatever libraries those subdirectories defined.
file(GLOB CHILDREN RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_DIR}/*")
foreach(child ${CHILDREN})
  if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_DIR}/${child}"
     AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_DIR}/${child}/CMakeLists.txt")
    if(child IN_LIST EXCLUDE_SUBDIRS)
      message(STATUS "Skipping excluded subdirectory: ${child}")
      continue()
    endif()

    message(STATUS "Adding subdirectory: ${TARGET_DIR}/${child}")
    add_subdirectory(${TARGET_DIR}/${child})
    list(APPEND BUILT_SUBDIRS ${child})

    get_property(child_targets DIRECTORY "${TARGET_DIR}/${child}" PROPERTY BUILDSYSTEM_TARGETS)
    foreach(target ${child_targets})
      get_target_property(target_type ${target} TYPE)
      if(target_type MATCHES "^(STATIC|SHARED|MODULE|OBJECT|INTERFACE)_LIBRARY$")
        list(APPEND EXTRA_LIBS ${target})
      endif()
    endforeach()
  endif()
endforeach()

# --- Makefile-based module: hello_world (built with make, not CMake) ---
set(HELLO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_DIR}/hello_world")
set(HELLO_LIB "${HELLO_DIR}/libhello_world.a")

add_custom_command(
  OUTPUT "${HELLO_LIB}"
  COMMAND make
  WORKING_DIRECTORY "${HELLO_DIR}"
  DEPENDS "${HELLO_DIR}/hello_world.c" "${HELLO_DIR}/hello_world.h" "${HELLO_DIR}/Makefile"
  COMMENT "Building hello_world via its Makefile"
  VERBATIM
)
add_custom_target(build_hello_world DEPENDS "${HELLO_LIB}")

add_library(hello_world STATIC IMPORTED)
set_target_properties(hello_world PROPERTIES
  IMPORTED_LOCATION "${HELLO_LIB}"
  INTERFACE_INCLUDE_DIRECTORIES "${HELLO_DIR}"
)
list(APPEND EXTRA_LIBS hello_world)

# Write the list of subdirectories that were built to a file, one per line.
set(BUILT_SUBDIRS_TEXT "")
foreach(dir ${BUILT_SUBDIRS})
  string(APPEND BUILT_SUBDIRS_TEXT "${dir}\n")
endforeach()
file(WRITE "${PROJECT_BINARY_DIR}/built_subdirs.txt" "${BUILT_SUBDIRS_TEXT}")
message(STATUS "Wrote built subdirectory list to ${PROJECT_BINARY_DIR}/built_subdirs.txt")

add_executable(${PROJECT_NAME} main.cpp)

# Ensure the Makefile-based library is built before linking the executable.
add_dependencies(${PROJECT_NAME} build_hello_world)

target_link_libraries(${PROJECT_NAME} PUBLIC ${EXTRA_LIBS})

target_include_directories(${PROJECT_NAME} PUBLIC
  "${PROJECT_BINARY_DIR}"
)
