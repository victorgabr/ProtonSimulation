cmake_minimum_required(VERSION 2.8)
project(ProtonSimulation)

# Requires C++11 standard

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#---------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build B1. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.

set(${PROJECT_SOURCE_DIR}_SCRIPTS
  vis.mac
  G4_WATER_GEOMETRY.gdml
  G4_TISSUE_SOFT_ICRP_GEOMETRY.gdml
  Prompt_photon_simulation.gdml
  proton_source.mac
  run.mac
  Geant4Simulation.py
  README.md
  )


foreach(_script ${${PROJECT_SOURCE_DIR}_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()




#------------------------------------------------------------
add_executable(${PROJECT_NAME} "main.cc" ${sources} ${headers})
target_link_libraries(${PROJECT_NAME} ${Geant4_LIBRARIES})
