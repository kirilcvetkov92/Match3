cmake_minimum_required(VERSION 3.1)

project("worktest" LANGUAGES C CXX)

file(GLOB kinglib_files "external/king/king/*.*")
file(GLOB candidate_files "src/*.*")

set(project_name "worktest")

add_executable(${project_name} ${kinglib_files} ${candidate_files})

set_target_properties(${project_name} PROPERTIES CXX_STANDARD 14)
set_target_properties(${project_name} PROPERTIES CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/scripts")

if (APPLE)
	set(SDL2_PATH "${CMAKE_CURRENT_LIST_DIR}/external/osx")
	set(SDL2_IMAGE_PATH "${CMAKE_CURRENT_LIST_DIR}/external/osx")
	set(SDL2_TTF_PATH "${CMAKE_CURRENT_LIST_DIR}/external/osx")
else()
	set(SDL2_PATH "${CMAKE_CURRENT_LIST_DIR}/external/win/SDL2-2.0.8")
	set(SDL2_IMAGE_PATH "${CMAKE_CURRENT_LIST_DIR}/external/win/SDL2_image-2.0.3")
	set(SDL2_TTF_PATH "${CMAKE_CURRENT_LIST_DIR}/external/win/SDL2_ttf-2.0.14")
endif()

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

target_include_directories(${project_name} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/external/king")
target_include_directories(${project_name} PRIVATE ${SDL2_INCLUDE_DIR})
target_include_directories(${project_name} PRIVATE ${SDL2_IMAGE_INCLUDE_DIR})
target_include_directories(${project_name} PRIVATE ${SDL2_TTF_INCLUDE_DIR})

target_link_libraries(${project_name} PRIVATE ${SDL2_LIBRARY})
target_link_libraries(${project_name} PRIVATE ${SDL2_IMAGE_LIBRARY})
target_link_libraries(${project_name} PRIVATE ${SDL2_TTF_LIBRARY})

if (WIN32)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(arch "x64")
	else(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(arch "x86")
	endif(CMAKE_SIZEOF_VOID_P EQUAL 8)

	configure_file("${CMAKE_CURRENT_LIST_DIR}/external/win/SDL2-2.0.8/lib/${arch}/SDL2.dll" "${CMAKE_CURRENT_LIST_DIR}/bin" COPYONLY)
	configure_file("${CMAKE_CURRENT_LIST_DIR}/external/win/SDL2_image-2.0.3/lib/${arch}/libjpeg-9.dll" "${CMAKE_CURRENT_LIST_DIR}/bin" COPYONLY)
	configure_file("${CMAKE_CURRENT_LIST_DIR}/external/win/SDL2_image-2.0.3/lib/${arch}/libpng16-16.dll" "${CMAKE_CURRENT_LIST_DIR}/bin" COPYONLY)
	configure_file("${CMAKE_CURRENT_LIST_DIR}/external/win/SDL2_image-2.0.3/lib/${arch}/libtiff-5.dll" "${CMAKE_CURRENT_LIST_DIR}/bin" COPYONLY)
	configure_file("${CMAKE_CURRENT_LIST_DIR}/external/win/SDL2_image-2.0.3/lib/${arch}/libwebp-7.dll" "${CMAKE_CURRENT_LIST_DIR}/bin" COPYONLY)
	configure_file("${CMAKE_CURRENT_LIST_DIR}/external/win/SDL2_image-2.0.3/lib/${arch}/SDL2_image.dll" "${CMAKE_CURRENT_LIST_DIR}/bin" COPYONLY)
	configure_file("${CMAKE_CURRENT_LIST_DIR}/external/win/SDL2_image-2.0.3/lib/${arch}/zlib1.dll" "${CMAKE_CURRENT_LIST_DIR}/bin" COPYONLY)
	configure_file("${CMAKE_CURRENT_LIST_DIR}/external/win/SDL2_ttf-2.0.14/lib/${arch}/libfreetype-6.dll" "${CMAKE_CURRENT_LIST_DIR}/bin" COPYONLY)
	configure_file("${CMAKE_CURRENT_LIST_DIR}/external/win/SDL2_ttf-2.0.14/lib/${arch}/SDL2_ttf.dll" "${CMAKE_CURRENT_LIST_DIR}/bin" COPYONLY)

	if(${CMAKE_VERSION} VERSION_GREATER_EQUAL 3.8.0)
		set_target_properties(${project_name} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/bin")
	endif(${CMAKE_VERSION} VERSION_GREATER_EQUAL 3.8.0)
endif(WIN32)

source_group("Source\\kinglib" FILES ${kinglib_files})
source_group("Source\\worktest" FILES ${candidate_files})
