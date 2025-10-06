find_package(pugixml REQUIRED)
find_package(cpr REQUIRED)
find_package(spdlog REQUIRED)
find_package(libpqxx REQUIRED)
find_package(llama-cpp REQUIRED)

add_library(collector_lib 
  feed_reader.cpp
  article.cpp
  article_repository.cpp
)

target_link_libraries(collector_lib PRIVATE
  cpr::cpr
  pugixml::pugixml
  spdlog::spdlog
  libpqxx::pqxx
  llama-cpp::llama-cpp
)

target_include_directories(collector_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_executable(fetcher fetcher_main.cpp)
target_link_libraries(fetcher PRIVATE 
  spdlog::spdlog
  collector_lib)
