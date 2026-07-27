cmake_minimum_required(VERSION 3.10)

project(Tutorial VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(USE_MYMATH "Use tutorial provided math implementation" OFF)

# Subdirectories to skip during auto-detection, e.g.
#   cmake -S . -B build -DEXCLUDE_SUBDIRS="MathFunctions;experimental"
set(EXCLUDE_SUBDIRS "" CACHE STRING
    "Semicolon-separated list of subdirectories to exclude from auto-detection")

configure_file(TutorialConfig.h.in TutorialConfig.h)

# Add every immediate subdirectory that has its own CMakeLists.txt,
# then link whatever libraries those subdirectories defined.
file(GLOB CHILDREN RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*")
foreach(child ${CHILDREN})
  if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${child}"
     AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${child}/CMakeLists.txt")
    if(child IN_LIST EXCLUDE_SUBDIRS)
      message(STATUS "Skipping excluded subdirectory: ${child}")
      continue()
    endif()

    message(STATUS "Adding subdirectory: ${child}")
    add_subdirectory(${child})

    get_property(child_targets DIRECTORY "${child}" PROPERTY BUILDSYSTEM_TARGETS)
    foreach(target ${child_targets})
      get_target_property(target_type ${target} TYPE)
      if(target_type MATCHES "^(STATIC|SHARED|MODULE|OBJECT|INTERFACE)_LIBRARY$")
        list(APPEND EXTRA_LIBS ${target})
      endif()
    endforeach()
  endif()
endforeach()

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC ${EXTRA_LIBS})

target_include_directories(${PROJECT_NAME} PUBLIC
  "${PROJECT_BINARY_DIR}"
)
