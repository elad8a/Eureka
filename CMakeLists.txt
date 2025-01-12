﻿cmake_minimum_required (VERSION 3.21)

if(NOT DEFINED ENV{VCPKG_ROOT})
    message(FATAL_ERROR 
    "VCPKG_ROOT envrionment variable is not defined.\n"
    "please install vcpkg and set the appropriate environment variable."    
    )
endif()

message(STATUS
"\n======================================================================================"
"\n======================================================================================"
"\n========================       PROJECT EUREKA      ==================================="
"\n======================================================================================"
"\n======================================================================================"
)
message(STATUS "CMAKE_C_COMPILER = ${CMAKE_C_COMPILER}")
message(STATUS "CMAKE_CXX_COMPILER = ${CMAKE_CXX_COMPILER}")

include(cmake/code_analysis.cmake)
include(cmake/compiler_flags.cmake)
include(cmake/debugging_utils.cmake)
include(cmake/dynamic_library_link.cmake)
include(cmake/global_setup.cmake)
include(cmake/installation.cmake)
include(cmake/target_helpers.cmake)

project ("Eureka")


add_library(
    Eureka.configuration
    INTERFACE
    cmake/code_analysis.cmake
    cmake/compiler_flags.cmake
    cmake/debugging_utils.cmake
    cmake/dynamic_library_link.cmake
    cmake/global_setup.cmake
    cmake/installation.cmake
    cmake/target_helpers.cmake
    CMakePresetsBase.json
    CMakePresets.json
    vcpkg.json
    .clang-tidy
    .clang-format
)

set_target_properties(Eureka.configuration PROPERTIES FOLDER Config)

set(EUREKA_CLANG_TIDY_CONFIG_PATH "${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy")

#set(CMAKE_CXX_CLANG_TIDY 
#  clang-tidy;
#  -header-filter=.;
#  --config-file=${EUREKA_CLANG_TIDY_CONFIG_PATH};
#)

# https://discourse.llvm.org/t/linux-what-is-the-status-of-libc-in-llvm-15-apt-packages-ranges-format/65348/4
#if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
#    target_compile_options(
#      eureka_strict_compiler_flags 
#      INTERFACE
#      $<$<CXX_COMPILER_ID:Clang>:-fexperimental-library -stdlib=libc++>
#    )
#
#)
#endif()
############################################################################
#                            PACKAGES
############################################################################
message(STATUS
"\n======================================================================================"
"\n======================================================================================"
"\n========================           PACKAGES        ==================================="
"\n======================================================================================"
"\n======================================================================================"
)
#find_program(CCACHE_FOUND ccache) 
#if(CCACHE_FOUND)
#    message("CCACHE is found")
#    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
#    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache) # Less useful to do it for linking, see edit2
#else(CCACHE_FOUND)
#    message("CCACHE is NOT found")
#endif(CCACHE_FOUND)

add_compile_definitions(Boost_NO_WARN_NEW_VERSIONS=1)
set(nlohmann-json_IMPLICIT_CONVERSIONS OFF)
message(DEBUG "start find package")
find_package(Boost REQUIRED COMPONENTS container)
find_package(Catch2 CONFIG REQUIRED)
find_package(concurrencpp CONFIG REQUIRED)
find_package(directx-dxc CONFIG REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)
find_package(function2 CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(implot CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(PalSigslot CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
#find_package(Vulkan REQUIRED COMPONENTS glslc)
find_package(unofficial-shaderc_util CONFIG REQUIRED)  
find_package(unofficial-vulkan-memory-allocator CONFIG REQUIRED)
#find_package(unofficial-spirv-reflect CONFIG REQUIRED)
find_package(asio-grpc CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(volk CONFIG REQUIRED)

find_program(dxc_executable NAMES dxc HINTS directx-dxc::dxc CONFIG REQUIRED)
find_program(glslc_executable NAMES glslc HINTS unofficial-shaderc_util::glslc)
#find_program(glslc_executable NAMES glslc HINTS Vulkan::glslc CONFIG REQUIRED)
message(DEBUG "found glslc_executable = ${glslc_executable}")

#message("start TINYGLTF_INCLUDE_DIRS")
#find_path(TINYGLTF_INCLUDE_DIRS "tiny_gltf.h")
#message("start CUDA")
##find_package(CUDAToolkit)
#if (CUDAToolkit_FOUND)
#    message("found cuda toolkit version ${CUDAToolkit_VERSION_MAJOR}.${CUDAToolkit_VERSION_MINOR}.${CUDAToolkit_VERSION_PATCH}")
#endif()

#find_package(Boost REQUIRED COMPONENTS program_options)
#find_package(glslang CONFIG REQUIRED)
#find_package(SPIRV-Tools CONFIG REQUIRED)
#find_package(SPIRV-Tools-opt CONFIG REQUIRED)
#find_path(VULKAN_HPP_INCLUDE_DIRS "vulkan/vulkan.hpp")
#find_program(dxc_executable NAMES dxc HINTS Vulkan::dxc) points to the dxc shipped with vs

message(STATUS
"\n======================================================================================"
"\n======================================================================================"
"\n========================       EUREKA TARGETS      ==================================="
"\n======================================================================================"
"\n======================================================================================"
)

############################################################################
#                        GLOBAL VARIABLES
############################################################################

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
#set(dxc_executable "$ENV{VULKAN_SDK}/Bin/dxc.exe")
message(STATUS "Eureka: dxc_executable = ${dxc_executable}")
message(STATUS "Eureka: glslc_executable = ${glslc_executable}")


configure_default_output_directories()
enable_testing()
include(CTest)
include(Catch)



if (CMAKE_GENERATOR MATCHES "Visual Studio")
    # Do Visual Studio specific stuff
    message(DEBUG "ZZZZ addinig include path ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")
    # it seems that vcpkg based cmake solutions don't add the vcpkg generated include directories to vcproj files' 
    include_directories("${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")
endif()

set(EUREKA_ALLOWED_BUILD_TYPES Debug RelWithDebInfo)
set_available_project_configurations(Debug RelWithDebInfo)

# HACK
if (MSVC)
    # https://stackoverflow.com/questions/58708772/cmake-project-in-visual-studio-gives-flag-override-warnings-command-line-warnin
    string(REGEX REPLACE "/W[3|4]" "/w" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}") 
endif()

if (WIN32)
    add_compile_definitions(
      WIN32_LEAN_AND_MEAN
      NOMINMAX # WINDOWS: to avoid defining min/max macros
     # _WINSOCKAPI_ # WINDOWS: to avoid re-definition in WinSock2.h
    )
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

############################################################################
#                             SUBDIRECTORIES
############################################################################

message(DEBUG "============== AVAILABLE TARGETS AFTER IMPORT ==============")
get_all_targets(_targets_list)
foreach(_target ${_targets_list})
  message(DEBUG "Target: ${_target}")
endforeach()


add_subdirectory(src) 

########################################################################################################
#
# CTest Integration 
# https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md#automatic-test-registration
#
########################################################################################################




message(STATUS
"\n======================================================================================"
"\n======================================================================================"
"\n========================            END            ==================================="
"\n======================================================================================"
"\n======================================================================================"
)









