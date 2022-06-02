
cmake_minimum_required(VERSION 3.4.1)

#告诉编译器做关联：包下的头文件
include_directories(
        course
        main
        third
        third/glm
        shape
        utils
        camera
)

file(GLOB src-files
        ${CMAKE_SOURCE_DIR}/*.cpp
        ${CMAKE_SOURCE_DIR}/course/*.cpp
        ${CMAKE_SOURCE_DIR}/main/*.cpp
        ${CMAKE_SOURCE_DIR}/shape/*.cpp
        ${CMAKE_SOURCE_DIR}/utils/*.cpp
        ${CMAKE_SOURCE_DIR}/camera/*.cpp
        )

add_library(
        opengl_native

        SHARED

        ${src-files}
)


find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)



target_link_libraries( # Specifies the target library.
        opengl_native
        GLESv3
        ${log-lib}
        EGL
        android
        )