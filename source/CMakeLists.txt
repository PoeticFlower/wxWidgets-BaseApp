# Configuring Global Options
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(BaseProject)

# Configure wxWidgets default build options
set(wxBUILD_MONOLITHIC                            ON )
set(wxBUILD_SHARED                                OFF)

# Process each subdirectory one by one
add_subdirectory(thirdparty)
add_subdirectory(BasicModule)
add_subdirectory(wxAppDemo)
