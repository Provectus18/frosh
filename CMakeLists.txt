cmake_minimum_required(VERSION 3.14)
project(frosh-game LANGUAGES C)

set(FROSH_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/main.c")

include_directories(include)
link_directories(lib)

add_executable(frosh-game ${FROSH_SRC})
target_link_libraries(frosh-game PRIVATE raylib m)

if(LINUX)
    target_link_libraries(frosh-game PRIVATE GL Xrandr Xinerama Xi Xcursor pthread dl)
elseif(APPLE)
    target_link_libraries(frosh-game PRIVATE "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo" "-framework CoreAudio" "-framework CoreFoundation")
endif()