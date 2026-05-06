find_package(Threads REQUIRED)

add_executable(Server
    main.cpp
    src/Server.cpp
)

target_include_directories(Server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(Server PRIVATE -Wall -Wextra)

target_link_libraries(Server PRIVATE Threads::Threads)
