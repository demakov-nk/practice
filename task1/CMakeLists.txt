cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME Task1)
project(${PROJECT_NAME} LANGUAGES C)

# LIBRARIES
add_library(SelectionSortStatic STATIC
    src/selection_sort.c
	src/selection_sort.h
)
add_library(SelectionSortShared SHARED
    src/selection_sort.c
	src/selection_sort.h
)


set_target_properties(SelectionSortStatic PROPERTIES OUTPUT_NAME "SelectionSort")
set_target_properties(SelectionSortShared PROPERTIES OUTPUT_NAME "SelectionSort")

target_include_directories(SelectionSortStatic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(SelectionSortShared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


add_executable(SelectionSortTest
	tests/selection_sort_test.c
)
add_executable(SpeedTestStatic
	tests/static_speedtest.c
)
add_executable(SpeedTestShared
	tests/shared_speedtest.c
)
add_executable(SpeedTestDl
	tests/dl_speedtest.c
)

target_link_libraries(SelectionSortTest SelectionSortStatic)
target_link_libraries(SpeedTestStatic SelectionSortStatic)
target_link_libraries(SpeedTestShared SelectionSortShared)
target_link_libraries(SpeedTestDl dl)

add_custom_target(test
    COMMAND SelectionSortTest
    DEPENDS SelectionSortTest
    COMMENT "Тестирование функций сортировки выбором"
)
add_custom_target(speedtest
    COMMAND SpeedTestStatic
    COMMAND SpeedTestShared
    DEPENDS SpeedTestStatic SpeedTestShared
    COMMENT "Тестирование скорости работы функций"
)
add_custom_target(dlspeedtest
    COMMAND SpeedTestDl
    DEPENDS SpeedTestDl
    COMMENT "Тестирование скорости работы функций (dlopen)"
)


set(OPT_FLAGS "-O0" "-O1" "-O2" "-Os")
set(SUFFIXES o0 o1 o2 os)
foreach(FLAG SUFFIX IN ZIP_LISTS OPT_FLAGS SUFFIXES)
    add_executable(SpeedTestStatic_${SUFFIX} tests/static_speedtest.c)
    target_compile_options(SpeedTestStatic_${SUFFIX} PRIVATE ${FLAG})
    target_link_libraries(SpeedTestStatic_${SUFFIX} SelectionSortStatic)

    add_executable(SpeedTestShared_${SUFFIX} tests/shared_speedtest.c)
    target_compile_options(SpeedTestShared_${SUFFIX} PRIVATE ${FLAG})
    target_link_libraries(SpeedTestShared_${SUFFIX} SelectionSortShared)
    
    add_custom_target(speedtest_${SUFFIX}
        COMMAND ./SpeedTestStatic_${SUFFIX}
        COMMAND ./SpeedTestShared_${SUFFIX}
        DEPENDS SpeedTestStatic_${SUFFIX} SpeedTestShared_${SUFFIX}
        COMMENT "Флаг оптимизации: ${FLAG}"
    )
endforeach()





