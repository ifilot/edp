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

# add unpacking of dataset to the test suite
add_test(NAME DatasetSetup COMMAND tar -xvjf dataset.tar.bz2)
add_test(NAME DatasetCleanup COMMAND rm -rvf CHGCAR_* PARCHG_*)
set_tests_properties(DatasetSetup PROPERTIES FIXTURES_SETUP Dataset)
set_tests_properties(DatasetCleanup PROPERTIES FIXTURES_CLEANUP Dataset)

# configure common settings for test executables
foreach(testexec ${EXECUTABLES})
    target_link_libraries(${testexec} unittest edpsources ${Boost_LIBRARIES} ${CAIRO_LIBRARIES} ${CPPUNIT_LIB} -lgcov)
    set_target_properties(${testexec} PROPERTIES COMPILE_FLAGS "--coverage")
    add_test(NAME ${testexec} COMMAND ${testexec})
    set_tests_properties(${testexec} PROPERTIES FIXTURES_REQUIRED Dataset)
endforeach()

#######################################################
# add testinput files
#######################################################
configure_file(testinput/dataset.tar.bz2 dataset.tar.bz2 COPYONLY)
