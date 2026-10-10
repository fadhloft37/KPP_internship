cmake_minimum_required(VERSION 3.28)
project(antreanBank)

include_directories(include)

add_library(bank_lib src/people.cpp)

add_executable(bank_app src/main.cpp)

target_link_libraries(bank_app bank_lib)

install(TARGETS bank_app DESTINATION bin)

install(TARGETS bank_lib DESTINATION lib)

install(FILES include/people.h DESTINATION include/antrean_bank)