# Configure the code and resource files needed to build the application
file(GLOB WX_APPLICATION_DEMO_HEADER_FILES
    LIST_DIRECTORIES false CONFIGURE_DEPENDS
    GlobalConstants.h
    ListViewEx.h
    MainApp.h
    MainFrame.h
    TableEx.hpp
    TableExAdapter.hpp
    )

file(GLOB WX_APPLICATION_DEMO_SOURCE_FILES
    LIST_DIRECTORIES false CONFIGURE_DEPENDS
    GlobalConstants.cpp
    ListViewEx.cpp
    MainApp.cpp
    MainFrame.cpp
    )

file(GLOB WX_APPLICATION_DEMO_RESOURCE_FILES
    LIST_DIRECTORIES false CONFIGURE_DEPENDS
    MainApp.ico
    MainApp.rc
    resource.h
    targetver.h
    )


# Configure build options

## Configure general build options
add_executable            (wxAppDemo
    )
target_link_libraries     (wxAppDemo
    wx::mono
    BasicModule
    )

## Configure platform-specific build options
if(WIN32)
    set_target_properties     (wxAppDemo
        PROPERTIES
        WIN32_EXECUTABLE TRUE
        )
endif()
if(WIN32)
    target_sources            (wxAppDemo
        PRIVATE
        ${WX_APPLICATION_DEMO_HEADER_FILES}
        ${WX_APPLICATION_DEMO_SOURCE_FILES}
        ${WX_APPLICATION_DEMO_RESOURCE_FILES}
        )
else()
    target_sources            (wxAppDemo
        PRIVATE
        ${WX_APPLICATION_DEMO_HEADER_FILES}
        ${WX_APPLICATION_DEMO_SOURCE_FILES}
        )
endif()
