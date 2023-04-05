/**************************************************************************
 *                                                                        *
 *   Author: Ivo Filot <i.a.w.filot@tue.nl>                               *
 *                                                                        *
 *   EDP is free software:                                                *
 *   you can redistribute it and/or modify it under the terms of the      *
 *   GNU General Public License as published by the Free Software         *
 *   Foundation, either version 3 of the License, or (at your option)     *
 *   any later version.                                                   *
 *                                                                        *
 *   EDP is distributed in the hope that it will be useful,               *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty          *
 *   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.              *
 *   See the GNU General Public License for more details.                 *
 *                                                                        *
 *   You should have received a copy of the GNU General Public License    *
 *   along with this program.  If not, see http://www.gnu.org/licenses/.  *
 *                                                                        *
 **************************************************************************/

#include "testedp.h"

CPPUNIT_TEST_SUITE_REGISTRATION( TestEDP );

void TestEDP::setUp() {
}

void TestEDP::tearDown() {
}

void TestEDP::testReading() {
    // create scalar field
    ScalarField sf("CHGCAR", false);
    CPPUNIT_ASSERT_EQUAL( (uint)0, sf.get_size() );

    // read atoms and check this
    sf.read_header_and_atoms();
    CPPUNIT_ASSERT_EQUAL( (uint)0, sf.get_size() );
    auto p = sf.get_atom_position(0);
    CPPUNIT_ASSERT_EQUAL( (float)5.0, p[0] );
    CPPUNIT_ASSERT_EQUAL( (float)5.0, p[1] );
    CPPUNIT_ASSERT_EQUAL( (float)5.0, p[2] );

    // read scalar field and test this
    sf.read();
    CPPUNIT_ASSERT_EQUAL( (uint)1000000, sf.get_size() );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.276298f / 1e3f, sf.get_min(), 1e-8 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 2047.064424f / 1e3f, sf.get_max(), 1e-4 );
}

void TestEDP::testProjection() {
    // create scalar field
    ScalarField sf("CHGCAR", false);
    sf.read_header_and_atoms();
    sf.read();

    const float ref = 42478.9375;

    // xz
    this->test_plane(&sf,
                     glm::vec3(1,0,0),
                     glm::vec3(0,0,1),
                     glm::vec3(5,5,5),
                     ref);

    // yz
    this->test_plane(&sf,
                     glm::vec3(0,1,0),
                     glm::vec3(0,0,1),
                     glm::vec3(5,5,5),
                     ref);

    // xy
    this->test_plane(&sf,
                     glm::vec3(1,0,0),
                     glm::vec3(0,1,0),
                     glm::vec3(5,5,5),
                     ref);  
}

void TestEDP::test_plane(ScalarField* sf, glm::vec3 v, glm::vec3 w, glm::vec3 p, float ref) {
    // create plane projector
    PlaneProjector pp(sf, 0);

    // produce xz plane
    float scale = 100;
    float li = -20, lj = -20;
    float hi = 20, hj = 20;
    pp.extract(v, w, p, scale, li, hi, lj, hj);
    pp.set_scaling(false, -3, 2);
    pp.plot();
    pp.isolines(10);

    // // verify that the dimensions are correct
    auto dim = pp.get_dimensions();
    // CPPUNIT_ASSERT_EQUAL( (uint)1000, dim.first );
    // CPPUNIT_ASSERT_EQUAL( (uint)1000, dim.second );

    // check that the sum of elements is correct
    float sum = 0;
    const float* data = pp.get_planegrid_real();
    for(unsigned int i=0; i<dim.first * dim.second; i++) {
        sum += data[i];
    }
    std::string str = (boost::format("Value equal to: %f") % sum).str();
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(str.c_str(), ref, sum, 1e-4);
}