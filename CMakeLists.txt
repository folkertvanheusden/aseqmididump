project(aseqmididump)
cmake_minimum_required(VERSION 3.2)
add_definitions("-std=gnu++17 -Wall -Werror")

set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fstack-protector-strong")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer")

add_executable(
  aseqmididump
  aseqmididump.cpp
)

include(FindPkgConfig)

pkg_check_modules(ALSA REQUIRED alsa)
target_link_libraries(aseqmididump ${ALSA_LIBRARIES})
target_include_directories(aseqmididump PUBLIC ${ALSA_INCLUDE_DIRS})
target_compile_options(aseqmididump PUBLIC ${ALSA_CFLAGS_OTHER})

set_target_properties(aseqmididump PROPERTIES OUTPUT_NAME aseqmididump)
