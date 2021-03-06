cmake_policy(SET CMP0015 OLD)

# Set project name
project (HexTerrainPrototype)

# Set minimum version
cmake_minimum_required (VERSION 3.14.5)
if (COMMAND cmake_policy)
    cmake_policy (SET CMP0003 NEW)
    if (CMAKE_VERSION VERSION_GREATER 2.8.12 OR CMAKE_VERSION VERSION_EQUAL 2.8.12)
        cmake_policy (SET CMP0022 NEW) # INTERFACE_LINK_LIBRARIES defines the link interface
    endif ()
    if (CMAKE_VERSION VERSION_GREATER 3.0.0 OR CMAKE_VERSION VERSION_EQUAL 3.0.0)
        cmake_policy (SET CMP0026 OLD) # Disallow use of the LOCATION target property - therefore we set to OLD as we still need it
        cmake_policy (SET CMP0042 NEW) # MACOSX_RPATH is enabled by default
    endif ()
endif ()

# Set CMake modules search path
set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)
MESSAGE("${CMAKE_MODULE_PATH}")
# Include Urho3D Cmake common module
include (UrhoCommon)


# Find Urho3D library
find_package (Urho3D REQUIRED)

include_directories (${URHO3D_INCLUDE_DIRS})
include_directories (${INCLUDE_DIRS_ONLY})

# Define target name
set (TARGET_NAME HexTerrainPrototype)


# Define source files
define_source_files (GLOB_CPP_PATTERNS Source/*.c* GLOB_H_PATTERNS Source/*.h* RECURSE GROUP)
# Setup target with resource copying
setup_main_executable ()

ADD_DEFINITIONS(
    -std=c++17 # Or -std=c++0x
    # Other flags
)
