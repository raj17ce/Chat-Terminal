find_package(Threads REQUIRED)

add_executable(Client
    main.cpp
    src/Client.cpp
)

target_include_directories(Client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(Client PRIVATE -Wall -Wextra)

target_link_libraries(Client PRIVATE Threads::Threads)
