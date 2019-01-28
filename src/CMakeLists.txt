# CMakeLists.txt for event package. It creates a library with dictionary and a main program
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(BMNTree)

set(FAIRROOT_DIR "$ENV{FAIRROOTPATH}/share/fairbase/cmake/modules")

# You need to tell CMake where to find the ROOT installation. This can be done in a number of ways:
#   - ROOT built with classic configure/make use the provided $ROOTSYS/etc/cmake/FindROOT.cmake
#   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT
#list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
list(APPEND CMAKE_MODULE_PATH ${FAIRROOT_DIR})
#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS MathCore RIO Hist Tree Net EG
  HINTS "$ENV{ROOTSYS}/share/root/cmake")

#---Include BMNROOT framework
#find_package(BMNROOT REQUIRED
#  COMPONENTS BmnData Zdc
#  HINTS $ENV{FAIRLIBDIR})

#list(APPEND CMAKE_MODULE_PATH "$ENV{FAIRROOTPATH}/share/fairbase/cmake/modules")
#---Include FAIRROOT framework
find_package(FairRoot REQUIRED COMPONENTS Base)

# set(ROOT_USE_FILE "${FAIRROOT_DIR}/ROOTMacros.cmake")
#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})
message("ROOT use file: " ${ROOT_USE_FILE})

set(FAIRROOT_USE_FILE "${FAIRROOT_DIR}/FairMacros.cmake")
include(${FAIRROOT_USE_FILE})
message("FAIRROOT use file: " ${FAIRROOT_USE_FILE})

set(INCLUDE_DIRECTORIES
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/Converters
  ${CMAKE_SOURCE_DIR}/bin
  ${ROOT_INCLUDE_DIRS}
  $ENV{VMCWORKDIR}
  $ENV{VMCWORKDIR}/bmndata
  $ENV{VMCWORKDIR}/zdc
  $ENV{FAIRROOTPATH}/include
)

include_directories(${INCLUDE_DIRECTORIES}
)
# set(ROOT_CXX_FLAGS
#   "-O2 -pipe -m64 -fsigned-char -fPIC -pthread -std=c++11 -DR__HAVE_CONFIG")
add_definitions(${ROOT_CXX_FLAGS})
message("ROOT CXX FLAGS: " ${ROOT_CXX_FLAGS})

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS "-O2")

set(BMNTree_headers_files
  Track.h
  EventHeader.h
  Event.h
)

set(BMNTree_cxx_files
  Track.cxx
  EventHeader.cxx
  Event.cxx
)

#---Generate dictionary for BMNTree
ROOT_GENERATE_DICTIONARY(G__BMNTree
  ${BMNTree_headers_files}
LINKDEF BMNTree.LinkDef.h)

#---Create a shared library with geneated dictionary for BMNTree class
add_library(BMNTree SHARED
  ${BMNTree_cxx_files}
G__BMNTree.cxx)

message("ROOT_LIBRARIES: " ${ROOT_LIBRARIES})
target_link_libraries(BMNTree ${ROOT_LIBRARIES})

add_subdirectory(Converters)
add_subdirectory(bin)
