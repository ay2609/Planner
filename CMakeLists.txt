cmake_minimum_required(VERSION 3.0) # was 3.26

#set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")

# need to add this file somewhere: vcpkg_installed/arm64-osx/share/imgui/imgui-config.cmake

#if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{CMAKE_TOOLCHAIN_FILE})
#    set(CMAKE_TOOLCHAIN_FILE $ENV{CMAKE_TOOLCHAIN_FILE})
#endif()

#set(CMAKE_PREFIX_PATH "imgui")

project(Planner)

set(CMAKE_CXX_STANDARD 20)

#set(IMGUI_PATH ${CMAKE_CURRENT_LIST_DIR}/imgui)
#set(IMGUI_SOURCES
#        ${IMGUI_PATH}/imgui.h
#        ${IMGUI_PATH}/imgui.cpp
#        ${IMGUI_PATH}/imconfig.h
#        ${IMGUI_PATH}/imgui_demo.cpp
#        ${IMGUI_PATH}/imgui_draw.cpp
#        ${IMGUI_PATH}/imgui_internal.h
#        ${IMGUI_PATH}/imstb_rectpack.h
#        ${IMGUI_PATH}/imstb_textedit.h
#        ${IMGUI_PATH}/imstb_truetype.h
#        ${IMGUI_PATH}/imgui_tables.cpp
#        ${IMGUI_PATH}/imgui_widgets.cpp
#
#        # specific bindings...
#        ${IMGUI_PATH}/backends/imgui_impl_glfw.h
#        ${IMGUI_PATH}/backends/imgui_impl_glfw.cpp
#        ${IMGUI_PATH}/backends/imgui_impl_opengl3.h
#        ${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp
#        ${IMGUI_PATH}/backends/imgui_impl_opengl3_loader.cpp)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm REQUIRED)

#find_package(fmt REQUIRED)

add_executable(Planner src/planner.cpp src/functions.cpp)

#set(FMT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/fmt/include/fmt/)
#file(GLOB FMT_GLOB
#
#${FMT_PATH}/core.h
#)
#add_library(fmt STATIC ${FMT_GLOB})

set(IMGUI_PATH ${CMAKE_CURRENT_LIST_DIR}/imgui)
file(GLOB IMGUI_GLOB
        ${IMGUI_PATH}/imgui.h
        ${IMGUI_PATH}/imgui.cpp
        ${IMGUI_PATH}/imconfig.h
        ${IMGUI_PATH}/imgui_demo.cpp
        ${IMGUI_PATH}/imgui_draw.cpp
        ${IMGUI_PATH}/imgui_internal.h
        ${IMGUI_PATH}/imstb_rectpack.h
        ${IMGUI_PATH}/imstb_textedit.h
        ${IMGUI_PATH}/imstb_truetype.h
        ${IMGUI_PATH}/imgui_tables.cpp
        ${IMGUI_PATH}/imgui_widgets.cpp
        ${IMGUI_PATH}/imguidatechooser.h
        ${IMGUI_PATH}/imguidatechooser.cpp

        # specific bindings...
        ${IMGUI_PATH}/backends/imgui_impl_glfw.h
        ${IMGUI_PATH}/backends/imgui_impl_glfw.cpp
        ${IMGUI_PATH}/backends/imgui_impl_opengl3.h
        ${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp
        ${IMGUI_PATH}/backends/imgui_impl_opengl3_loader.cpp)
add_library(imgui STATIC ${IMGUI_GLOB})

target_include_directories(imgui PUBLIC ${IMGUI_PATH})
target_link_libraries(imgui PRIVATE glfw)

target_link_libraries(Planner PUBLIC imgui)
target_include_directories(Planner PUBLIC imgui)

target_include_directories(Planner PRIVATE imgui)
target_include_directories(Planner PRIVATE imgui/backends)

target_include_directories(Planner PUBLIC ${OPENGL_INCLUDE_DIR})
target_link_libraries(Planner PUBLIC ${OPENGL_LIBRARIES})
target_link_libraries(Planner PRIVATE glfw)

target_include_directories(Planner PUBLIC /opt/homebrew/Cellar/glm/1.0.1/include/glm)
target_link_directories(Planner PUBLIC /opt/homebrew/Cellar/glm/1.0.1/include/glm)

target_include_directories(Planner PUBLIC /opt/homebrew/Cellar/rapidjson/1.1.0/include/rapidjson)
target_link_directories(Planner PUBLIC /opt/homebrew/Cellar/rapidjson/1.1.0/include/rapidjson)
#target_link_libraries(Planner PRIVATE glm)


#target_include_directories(Planner PUBLIC
##        ./vcpkg_installed/arm64-osx/include/
#        ./imgui
#        ./imgui/backends
#)
#
#target_link_directories(Planner BEFORE PUBLIC
##        ./vcpkg_installed/arm64-osx/include/
#        ./imgui
#        ./imgui/backends
#)

