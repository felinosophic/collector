cmake_minimum_required(VERSION 3.22)
project(collector LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(collector src/main.cpp)
target_compile_features(collector PRIVATE cxx_std_20)
target_include_directories(collector PRIVATE ${PROJECT_SOURCE_DIR}/include)

find_package(pugixml REQUIRED)
find_package(fmt REQUIRED)
find_package(fmtlog REQUIRED)
find_package(cpr REQUIRED)

target_link_libraries(collector PRIVATE
  cpr::cpr
  pugixml::pugixml
  fmt::fmt
  fmtlog::fmtlog
)

