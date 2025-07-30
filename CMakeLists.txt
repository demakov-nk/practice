cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME Practice)
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


set(DB_LIBS Funcs Note NoteArr Header)
set(DB_SRC funcs note notearr header)
foreach(LIB SRC IN ZIP_LISTS DB_LIBS DB_SRC)
    add_library(${LIB} STATIC
        src/${SRC}.h src/${SRC}.c
    )
endforeach()
target_link_libraries(Funcs PRIVATE NoteArr Header)
target_link_libraries(Note PRIVATE Funcs Header)
target_link_libraries(NoteArr PRIVATE Note)
target_link_libraries(Header PRIVATE Note)
foreach(LIB IN LISTS DB_LIBS)
    target_include_directories(${LIB} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
endforeach()


find_package(Curses REQUIRED COMPONENTS Menu Form)
find_library(MENU_LIB NAMES menu PATHS /practice/build)
find_package(ZLIB REQUIRED)
list(APPEND DB_LIBS ${CURSES_LIBRARIES} ${CURSES_FORM_LIBRARIES} ${MENU_LIB} ${ZLIB_LIBRARIES})


add_executable(SpeedTestStatic
	tests/static_speedtest.c
)
add_executable(SpeedTestShared
	tests/shared_speedtest.c
)
add_executable(DBManager
	src/main.c
)
add_executable(Play
    play/play.c
)

target_link_libraries(SpeedTestStatic SelectionSortStatic)
target_link_libraries(SpeedTestShared SelectionSortShared)
target_link_libraries(DBManager ${DB_LIBS})
target_link_libraries(Play ${DB_LIBS})

add_custom_target(speedtest
    COMMAND SpeedTestStatic
    COMMAND SpeedTestShared
    DEPENDS SpeedTestStatic SpeedTestShared
    COMMENT "Тестирование скорости работы функций"
)

add_custom_target(dbmanager
    COMMAND DBManager
    DEPENDS DBManager
    COMMENT "Запуск менеджера базы данных"
)
add_custom_target(play
    COMMAND Play
    DEPENDS Play
    COMMENT "Запуск песочницы"
)
add_custom_target(debug
    COMMAND gdb --args $<TARGET_FILE:DBManager>
    DEPENDS DBManager
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
        COMMENT "Флаг оптимизации: ${flag}"
    )
endforeach()





