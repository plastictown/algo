project(algo LANGUAGES C )

cmake_minimum_required(VERSION 2.6)

add_executable(algo app/main.c
		src/array.c
		src/list.c
		src/listd.c
		src/search.c
		src/sort.c
		src/util.c)

#install(TARGETS algo RUNTIME_DESTINATION bin)
target_include_directories(algo PUBLIC ./ hdr)
target_link_libraries(algo PUBLIC m)
target_compile_options(algo PUBLIC -Wall -O3)

