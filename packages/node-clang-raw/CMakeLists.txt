cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0091 NEW)
cmake_policy(SET CMP0042 NEW)

project(node-clang-raw)

add_definitions(-DNAPI_VERSION=9)

include_directories(${CMAKE_JS_INC})

# Additional include directories
include_directories(/opt/homebrew/Cellar/llvm/20.1.8/include)

file(GLOB SOURCE_FILES "src/*.cpp" "src/*.cc")

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${CMAKE_JS_SRC})

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

# Link libraries
link_directories(/opt/homebrew/Cellar/llvm/20.1.8/lib)
find_library(CLANG_LIB clang PATHS /opt/homebrew/Cellar/llvm/20.1.8/lib REQUIRED)
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB} ${CLANG_LIB})

if(MSVC AND CMAKE_JS_NODELIB_DEF AND CMAKE_JS_NODELIB_TARGET)
  execute_process(COMMAND ${CMAKE_AR} /def:${CMAKE_JS_NODELIB_DEF} /out:${CMAKE_JS_NODELIB_TARGET} ${CMAKE_STATIC_LINKER_FLAGS})
endif()

# Include Node-API wrappers
execute_process(COMMAND node -p "require('node-addon-api').include"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE NODE_ADDON_API_DIR
        )
string(REPLACE "\n" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
string(REPLACE "\"" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${NODE_ADDON_API_DIR})

# define NAPI_CPP_EXCEPTIONS to disable exceptions
add_definitions(-DNAPI_DISABLE_CPP_EXCEPTIONS)