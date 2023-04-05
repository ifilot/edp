add_library(unittest STATIC unittest.cpp)

#######################################################
# Add executables
#######################################################
add_executable(TestEDP testedp.cpp)

#######################################################
# Link mkmsources and other dependencies
#######################################################
if(CMAKE_BUILD_TYPE MATCHES Debug)
    SET(CPPUNIT_LIB ${CPPUNIT_DEBUG_LIBRARY})
else()
    SET(CPPUNIT_LIB ${CPPUNIT_LIBRARY})
endif()
target_link_libraries(TestEDP unittest edpsources ${Boost_LIBRARIES} ${CAIRO_LIBRARIES} ${CPPUNIT_LIB} -lgcov --coverage)
set_target_properties(TestEDP PROPERTIES COMPILE_FLAGS "--coverage")

#######################################################
# add tests to the set
#######################################################
add_test(NAME "TestEDP" COMMAND TestEDP)

#######################################################
# add testinput files
#######################################################
configure_file(testinput/CHGCAR CHGCAR COPYONLY)