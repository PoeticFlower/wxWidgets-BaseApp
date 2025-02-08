file(GLOB WX_APPLICATION_DEMO_HEADER_FILES
    LIST_DIRECTORIES false CONFIGURE_DEPENDS
    GlobalConstants.h
    MainApp.h
    MainFrame.h
    )

file(GLOB WX_APPLICATION_DEMO_SOURCE_FILES
    LIST_DIRECTORIES false CONFIGURE_DEPENDS
    GlobalConstants.cpp
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
