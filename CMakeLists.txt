## Lualvm CMake ##
cmake_minimum_required (VERSION 2.8)
project (lualvm)

# have `make install` install to `/usr/share` instead of `/usr/local/share`
# lua `require` doesn't search `/usr/local`
IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  SET(CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "lua compat" FORCE)
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

# Include extra cmake modules
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# First, find required packages
find_package (Inclua REQUIRED)
include (${INCLUA_USE_FILE})
find_package (Lua REQUIRED)
find_package (LLVM REQUIRED)

# then run the src/ CMake script
add_subdirectory (src)
