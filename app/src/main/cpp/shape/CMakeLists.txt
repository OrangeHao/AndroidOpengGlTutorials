

project(shape)

aux_source_directory(. DIR_SHAPE_SRCS)
add_library(shape SHARED ${DIR_SHAPE_SRCS})


find_library( # Sets the name of the path variable.
        log-lib
        log)

target_link_libraries(
        shape

        ${log-lib}
        GLESv3)