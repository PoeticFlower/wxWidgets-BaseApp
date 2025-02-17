# Configure the code and resource files needed to build the library
file(GLOB WX_APPLICATION_BASIC_MODULE_HEADER_FILES
    LIST_DIRECTORIES false CONFIGURE_DEPENDS
    ListViewEx.h
    TableEx.hpp
    TableExAdapter.hpp
    )
file(GLOB WX_APPLICATION_BASIC_MODULE_SOURCE_FILES
    LIST_DIRECTORIES false CONFIGURE_DEPENDS
    ListViewEx.cpp
    )


# Configure build options

## Configure general build options
add_library               (BasicModule
    STATIC
    )
target_sources            (BasicModule
    PRIVATE
    ${WX_APPLICATION_BASIC_MODULE_HEADER_FILES}
    ${WX_APPLICATION_BASIC_MODULE_SOURCE_FILES}
    )
target_link_libraries     (BasicModule
    wx::mono
    )
target_include_directories(BasicModule
    PUBLIC ${CMAKE_SOURCE_DIR}/BasicModule
    )
