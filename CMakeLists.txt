cmake_minimum_required(VERSION 3.25)

#--- Find vcpkg executable in PATH ---#
find_program(VCPKG_EXECUTABLE NAMES vcpkg)

#--- If vcpkg is not found, stop with an error message ---#
if(NOT VCPKG_EXECUTABLE)
    message(FATAL_ERROR "*** Vcpkg executable not found in PATH. Please install vcpkg and ensure it is added to PATH. ***")
endif()

#--- Extract the root directory from the vcpkg executable path ---#
get_filename_component(VCPKG_ROOT ${VCPKG_EXECUTABLE} DIRECTORY)

#--- Print the paths of the folder and executable for verification ---#
message(STATUS "*** Found vcpkg executable at: ${VCPKG_EXECUTABLE} ***")
message(STATUS "*** Vcpkg root directory is: ${VCPKG_ROOT} ***")

#--- Set the CMake toolchain file ---#
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

#--- Set C++ standard ---#
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON) 

#--- Project must be afer all vcpkg setup ---#
project(GLSL_LIVE VERSION 1.1.0)

#--- Platform setup, you can modify if needed ---#
if(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(PLATFORM "Win64")
        set(VCPKG_TARGET_TRIPLET "x64-windows")
    else()
        set(PLATFORM "Win32")
        set(VCPKG_TARGET_TRIPLET "x32-windows")
    endif()
elseif(UNIX)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(PLATFORM "Linux64")
        set(VCPKG_TARGET_TRIPLET "x64-linux")
    else()
        set(PLATFORM "Linux32")
        set(VCPKG_TARGET_TRIPLET "x32-linux")
    endif()
endif()

#--- Print target triplet to verify it's set correctly ---#
message(STATUS "*** VCPKG_TARGET_TRIPLET = ${VCPKG_TARGET_TRIPLET} ***")

#--- Recursively look for all project source files and resource files to add to the executable ---#
file(GLOB_RECURSE PROJECT_SOURCES        "src/*.cpp")
file(GLOB         PROJECT_RESOURCE_FILES "resources/*.rc")

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)  # Added extra warnings
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/W4)  # You can adjust to /W4 or /Wall for all warnings
endif()

option(BUILD_SHARED_LIBS "Build libraries as shared" ON)

#--- Create and link the executable with other files ---#
add_executable(${PROJECT_NAME} main.cpp ${PROJECT_RESOURCE_FILES} ${PROJECT_SOURCES})

#--- Set up output dir same for different generators ---#
if(CMAKE_CONFIGURATION_TYPES) # CORRECT WAY FOR MULTICONFIG GENERATOR
    set(OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/bin/${PLATFORM}/$<CONFIG>/${PROJECT_NAME}.v.${CMAKE_PROJECT_VERSION})   
else()
    set(OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/bin/${PLATFORM}/${CMAKE_BUILD_TYPE}/${PROJECT_NAME}.v.${CMAKE_PROJECT_VERSION})
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})

#--- Set and Include headers ---#
set(PROJECT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE_DIR})

#--- Set and include precompiled header ---#
set(PROJECT_PCH_HEADER ${CMAKE_SOURCE_DIR}/src/PCH.h)
target_precompile_headers(${PROJECT_NAME} PRIVATE ${PROJECT_PCH_HEADER})

#--- Compile definitions ---#
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:DEBUG>)
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:RELEASE>)

#--- OpenGL connecting ---#
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)

#--- GLFW connecting ---#
find_package(glfw3 3.3.9 REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

#--- Glad connecting ---#
find_package(glad CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad)

#--- GLM connecting ---#
find_package(glm CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm-header-only)

#--- ImGUI connecting ---#
find_package(imgui CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)

#--- Spdlog connecting ---#
find_package(spdlog CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE>)
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO>)

#--- Magic Enum connecting ---#
find_package(magic_enum CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE magic_enum::magic_enum)

#--- Function to copy a directory to the output directory ---#
function(copy_directory_to_output_directory SOURCE_DIR)
    set(target_name copy_dir_${SOURCE_DIR})

    add_custom_target(${target_name} ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_DIR}
                $<TARGET_FILE_DIR:${PROJECT_NAME}>/${SOURCE_DIR}
        COMMENT "Copying ${SOURCE_DIR} to the output directory"
    )

    #--- Ensure that the copy_dir target is run after the project is built ---#
    add_dependencies(${PROJECT_NAME} ${target_name})
endfunction()

#--- Copy the assets and shaders to the output directory ---#
copy_directory_to_output_directory(assets)
copy_directory_to_output_directory(shaders)