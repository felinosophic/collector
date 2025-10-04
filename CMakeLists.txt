cmake_minimum_required(VERSION 3.22)
project(collector LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)


add_subdirectory(src)

enable_testing()

add_subdirectory(test)

# This line tells CMake to generate the compile_commands.json file

# This block automatically creates a symbolic link in your project's root
# directory that points to the compile_commands.json in your build folder.
if(CMAKE_EXPORT_COMPILE_COMMANDS)
    # The file path for the symlink in your project's root directory
    set(COMPILE_COMMANDS_SYMLINK "${CMAKE_SOURCE_DIR}/compile_commands.json")

    # Use 'execute_process' to run the appropriate command for the OS
    if(UNIX)
        # For Linux and macOS, use 'ln -sf'
        # -s for symbolic, -f to force overwrite if the link already exists
        execute_process(
            COMMAND ln -sf "${CMAKE_BINARY_DIR}/compile_commands.json" "${COMPILE_COMMANDS_SYMLINK}"
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
            RESULT_VARIABLE result
        )
    elseif(WIN32)
        # For Windows, use 'cmd /c mklink'
        execute_process(
            COMMAND cmd /c mklink "${COMPILE_COMMANDS_SYMLINK}" "${CMAKE_BINARY_DIR}/compile_commands.json"
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
            RESULT_VARIABLE result
        )
    endif()

    if(NOT result EQUAL 0)
        message(WARNING "Could not create symlink to compile_commands.json")
    endif()
endif()
