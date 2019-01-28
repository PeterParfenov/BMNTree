set(BMNROOT_LIBRARIES "-L$ENV{FAIRLIBDIR}" BmnData Zdc)
set(FAIRROOT_LIBRARIES
  "-L$ENV{FAIRROOTPATH}/lib" Base)


#---Make all the executables for data conversion (urqmd ascii)
add_executable(urqmd2tc urqmd2tc.cxx)
target_link_libraries(urqmd2tc BMNTree)

#---Make all the executables for data conversion (BMN data)
add_executable(bmn2tc bmn2tc.cxx)
target_link_libraries(bmn2tc BMNTree ${FAIRROOT_LIBRARIES} ${BMNROOT_LIBRARIES})
#target_link_libraries(bmn2tc ${BmnData} ${Zdc} ${Base})
