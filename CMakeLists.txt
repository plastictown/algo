project(algo LANGUAGES C )

cmake_minimum_required(VERSION 2.6)

add_executable(algo app/main.c
		src/array.c
		src/list.c
		src/listd.c
		src/search.c
		src/sort.c
		src/util.c
		src/stupid-hashmap.c)

find_package(ZLIB REQUIRED)
if(ZLIB_FOUND)
target_include_directories(algo PUBLIC ${ZLIB_INCLUDE_DIRS})
target_link_libraries(algo PUBLIC ${ZLIB_LIBRARIES})
endif(ZLIB_FOUND)

target_include_directories(algo PUBLIC ./ hdr)
target_link_libraries(algo PUBLIC m)
target_compile_options(algo PUBLIC -Wall -pedantic -O3 -DSTDIO_OUT)

