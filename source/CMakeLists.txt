# Configuring Global Options
project(BaseProject)

# Configure wxWidgets default build options
set(wxBUILD_MONOLITHIC                            ON )
set(wxBUILD_SHARED                                OFF)

# Process each subdirectory one by one
add_subdirectory(thirdparty)
add_subdirectory(wxAppDemo)
