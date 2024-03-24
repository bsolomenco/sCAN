cmake_minimum_required(VERSION 3.28)

cmake_path(GET CMAKE_CURRENT_SOURCE_DIR FILENAME PROJECT_NAME)  #use folder name as project name
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED 1)

#set(Qt6_DIR "C:/Qt/6.7.0/msvc2019_64/lib/make/Qt6") #Qt shared
set(Qt6_DIR "C:/Qt/lib/make/Qt6") #Qt static
#set(Qt6_DIR "C:/Qt/6.7.0/msvc2019_64")
set(CMAKE_PREFIX_PATH "C:/Qt")
find_package(Qt6 6.7.0 REQUIRED COMPONENTS Core SerialBus)
qt_standard_project_setup()

file(GLOB_RECURSE Src CONFIGURE_DEPENDS src/*)    #private headers and sources

qt_add_executable(${PROJECT_NAME} ${Src})

target_compile_definitions(${PROJECT_NAME} PRIVATE
    _CRT_SECURE_NO_WARNINGS     #silences a lot of warnings that cannot be fixed
    #_CRT_NONSTDC_NO_WARNINGS   #???
    NOMINMAX                    #eliminates conflict between windows.h & STL regarding min() & max()
    RESVER_DESCRIPTION="...description..."
    RESVER_FILEVERSION=${YYYYMMDDHH}
    RESVER_PRODUCTNAME="...product name..."
    RESVER_PRODUCTVERSION_STR="...product version..."
    RESVER_COPYRIGHT="...copyright..."
)
target_compile_options(${PROJECT_NAME} PRIVATE
    /J                  #char type as unsigned
    /MP                 #multi-process compilation (NOT WORKING with #import)
    /Oi                 #intrinsic functions (helps detecting more bugs)
    #/sdl               #additional security checks (to have security warnings as errors)
    /W4                 #warning level
)


target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::SerialBus)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${Src} ${Hdr})
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
if(WIN32)
    set_target_properties(${PROJECT_NAME}
        PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=C:\\Qt\\6.7.0\\msvc2019_64\\bin;%PATH%"
    )
endif()

install(TARGETS ${PROJECT_NAME}
    RUNTIME         DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}
)