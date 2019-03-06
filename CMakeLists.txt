#------------------------------------------------------------------------------------------------------------------------------
# $Id: CMakeLists.txt
# Code for Muon Decay
# Abhijit Bhattacharyya
#-------------------------------------------------------------------------------------------------------------------------------
#Setup
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(muonDecay)

set(CMAKE_BUILD_TYPE RelWithDebInfo)
#set(CMAKE_BUILD_TYPE Release)

option(WITH_GEANT4_UIVIS " Build with UI and Vis driver " ON)
if (WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

include(${Geant4_USE_FILE})

include_directories(${PROJECT_SOURCE_DIR}/inc ${Geant4_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/inc/*.hh)

add_executable(muonLife  muonLife.cc ${sources} ${headers})
target_link_libraries(muonLife ${Geant4_LIBRARIES})

#-------------------------------------------------------------------------------------------------------------------------
# copy scripts to build directory
#--------------------------------------------------------------------------------------------------------------------------
set(muonLife_SCRIPTS
  run.mac
  vis.mac
  GenerateMagFieldTable/magField3D.tab
)
foreach(_script ${muonLife_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
  )
  endforeach()


install(TARGETS muonLife DESTINATION bin)
