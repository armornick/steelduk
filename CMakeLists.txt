cmake_minimum_required (VERSION 3.0)
project (Dukwin)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# remove 'lib' prefix for dlls 
if(WIN32 AND CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif()

if (MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

#------------------------------------------------------------------------------------

option(DYNAMIC_DUKTAPE "build Duktape as a shared library" OFF)

#------------------------------------------------------------------------------------

set(DUKTAPE_DIR "vendor/duktape-1.5.1")
set(DUKTAPE_INCLUDE_DIR ${DUKTAPE_DIR})

set (DUKTAPE_SRCS
	${DUKTAPE_INCLUDE_DIR}/duktape.c
)

if (NOT DYNAMIC_DUKTAPE)

	add_library (duktape STATIC ${DUKTAPE_SRCS})
	target_include_directories (duktape PUBLIC ${DUKTAPE_INCLUDE_DIR})

else()

	add_library (duktape SHARED ${DUKTAPE_SRCS})
	target_include_directories (duktape PUBLIC ${DUKTAPE_INCLUDE_DIR})
	
	add_definitions(-DDUK_OPT_DLL_BUILD)

endif()

#------------------------------------------------------------------------------------

set(STEELDUK_DIR src/steelduk)

set(STEELDUK_SRCS
	${STEELDUK_DIR}/main.cpp
	${STEELDUK_DIR}/modsearch.cpp
	# Duktape helper functions
	${STEELDUK_DIR}/duk-types.cpp
	${STEELDUK_DIR}/duk-strings.cpp
	${STEELDUK_DIR}/duk-objects.cpp
	# module sources
	${STEELDUK_DIR}/modules/encodings.cpp
	${STEELDUK_DIR}/modules/io-file.cpp
	${STEELDUK_DIR}/modules/console.cpp
	${STEELDUK_DIR}/modules/console-color.cpp
)

add_executable(steelduk ${STEELDUK_SRCS})
target_include_directories(steelduk PUBLIC ${DUKTAPE_INCLUDE_DIR} ${STEELDUK_DIR})
target_link_libraries(steelduk duktape)

#------------------------------------------------------------------------------------