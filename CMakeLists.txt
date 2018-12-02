cmake_minimum_required(VERSION 3.12.4)
project(sjtu_oj_ans)

add_executable(1221 src/1221.cpp)
target_include_directories(1221 PUBLIC include)
target_compile_features(1221 PRIVATE cxx_std_17)
