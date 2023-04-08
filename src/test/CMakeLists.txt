add_library(unittest STATIC unittest.cpp)

# set executables
SET(EXECUTABLES TestProjection TestScalarField)

#######################################################
# Add executables
#######################################################
add_executable(TestProjection test_projection.cpp)
add_executable(TestScalarField test_scalarfield.cpp)

#######################################################
# Link mkmsources and other dependencies
#######################################################
if(CMAKE_BUILD_TYPE MATCHES Debug)
    SET(CPPUNIT_LIB ${CPPUNIT_DEBUG_LIBRARY})
else()
    SET(CPPUNIT_LIB ${CPPUNIT_LIBRARY})
endif()

# configure common settings for test executables
foreach(testexec ${EXECUTABLES})
    target_link_libraries(${testexec} unittest edpsources ${Boost_LIBRARIES} ${CAIRO_LIBRARIES} ${CPPUNIT_LIB} -lgcov --coverage)
    set_target_properties(${testexec} PROPERTIES COMPILE_FLAGS "--coverage")
    add_test(NAME ${testexec} COMMAND ${testexec})
endforeach()

#######################################################
# add testinput files
#######################################################
configure_file(testinput/CHGCAR_CH4 CHGCAR_CH4 COPYONLY)
