cmake_minimum_required(VERSION 3.24)
project(Encryption VERSION 1.1.0 LANGUAGES CXX)
add_library(aes SHARED
    "keys/aes.cpp"
)
add_library(caesar SHARED
    "keys/caesar.cpp"
)
add_library(minikey SHARED
    "keys/minikey.cpp"
)
target_include_directories(aes PUBLIC include)
target_include_directories(caesar PUBLIC include)
target_include_directories(minikey PUBLIC include)
find_package(Boost COMPONENTS program_options REQUIRED)
add_executable(Encrypt
    "src/encryption.cpp"
)
target_link_libraries(Encrypt aes caesar minikey Boost::program_options)