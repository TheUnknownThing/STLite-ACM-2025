include_directories(src)
include_directories(data)

set(files_prefix "${CMAKE_CURRENT_SOURCE_DIR}/data")
file(GLOB_RECURSE CPPs "${files_prefix}/**.cpp")

#string(REGEX REPLACE "[a-zA-Z]:" "" direc "${CMAKE_CURRENT_SOURCE_DIR}")
string(REGEX REPLACE "/([a-zA-Z0-9:_-]*/)*" "" cata "${CMAKE_CURRENT_SOURCE_DIR}")

foreach (cpp_file ${CPPs})
    string(REGEX REPLACE "/[a-zA-Z]*\\.cpp" "" fpath ${cpp_file})
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/data/" "" testname "${fpath}")
    set(testname "${cata}-${testname}")
    add_executable(${testname} ${cpp_file})
    add_test(NAME ${testname}
            COMMAND bash -c "$<TARGET_FILE:${testname}> | diff ${fpath}/answer.txt -")
    #            COMMAND "$<TARGET_FILE:${testname}>")
    set_property(TEST ${testname} PROPERTY TIMEOUT 5)
endforeach ()