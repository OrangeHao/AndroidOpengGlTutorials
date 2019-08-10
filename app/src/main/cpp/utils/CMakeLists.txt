



project(utils)

aux_source_directory(. DIR_UTIL_SRCS)


add_library(utils
        SHARED
            ${DIR_UTIL_SRCS})

find_library( # Sets the name of the path variable.
        log-lib
        log)

target_link_libraries(
        utils
        GLESv3

        ${log-lib} )