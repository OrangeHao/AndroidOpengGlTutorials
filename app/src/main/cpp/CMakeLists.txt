


cmake_minimum_required(VERSION 3.4.1)

project(opengl_native)

aux_source_directory(. DIRSRCS)

#告诉编译器做关联：包下的头文件
include_directories(shape)
include_directories(utils)

add_subdirectory(shape)
add_subdirectory(utils)



#add_library(
#        opengl_native
#
#    SHARED
#
#        utils/matrix.cpp
#    shape/simple_cube.cpp
#    shape/simple_triange.cpp
#        shape/texture_cube.cpp
#)
#
#
#find_library( # Sets the name of the path variable.
#        log-lib
#        log)
#
#
#
#target_link_libraries( # Specifies the target library.
#        opengl_native
#        GLESv3
#        ${log-lib}
#        )