Project(cimgui_dgui)
cmake_minimum_required(VERSION 3.11)
if(WIN32)
  set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif(WIN32)

set (CMAKE_CXX_STANDARD 11)

include(CMakePrintHelpers)
cmake_print_variables(${CMAKE_CURRENT_SOURCE_DIR})

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/cimgui/imgui/backends)
	set(BAKENDS_FOLDER "../vendor/cimgui/imgui/backends/")
else()
	set(BAKENDS_FOLDER "../vendor/cimgui/imgui/examples/")
endif()

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/cimgui/imgui/imgui_tables.cpp)
	set(TABLES_SOURCE "../vendor/cimgui/imgui/imgui_tables.cpp")
else()
	set(TABLES_SOURCE "")
endif()

cmake_print_variables(${TABLES_SOURCE})

include_directories(../vendor/cimgui/imgui)
add_definitions("-DIMGUI_USER_CONFIG=\"../cimconfig.h\"")
add_definitions("-DIMGUI_DISABLE_OBSOLETE_FUNCTIONS=1")

include_directories(../vendor/cimgui/)
set(IMGUI_SOURCES
../vendor/cimgui/cimgui.cpp
../vendor/cimgui/imgui/imgui.cpp
../vendor/cimgui/imgui/imgui_draw.cpp
../vendor/cimgui/imgui/imgui_demo.cpp
../vendor/cimgui/imgui/imgui_widgets.cpp
${TABLES_SOURCE}
)

set(IMGUI_LIBRARIES)

add_compile_definitions("IMGUI_IMPL_OPENGL_LOADER_GL3W")

list(APPEND IMGUI_SOURCES ${BAKENDS_FOLDER}imgui_impl_opengl3.cpp)
include_directories(../vendor/cimgui/imgui/examples/libs/gl3w)
if(WIN32)
       list(APPEND IMGUI_LIBRARIES opengl32)
else(WIN32) # Unix
       list(APPEND IMGUI_LIBRARIES GL)
endif(WIN32)

list(APPEND IMGUI_SOURCES ${BAKENDS_FOLDER}imgui_impl_glfw.cpp)

if (NOT STATIC_BUILD)
  add_library(cimgui SHARED ${IMGUI_SOURCES})
else()
  add_library(cimgui STATIC ${IMGUI_SOURCES})
endif()

if (WIN32)
    target_compile_definitions(cimgui PUBLIC "-DIMGUI_IMPL_API=extern \"C\" __declspec\(dllexport\)")
else(WIN32)
    target_compile_definitions(cimgui PUBLIC "-DIMGUI_IMPL_API=extern \"C\" ")
endif(WIN32)

target_link_libraries(cimgui ${IMGUI_LIBRARIES} glfw)
