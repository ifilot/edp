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

#include "test_projection.h"

CPPUNIT_TEST_SUITE_REGISTRATION( TestProjection );

void TestProjection::setUp() {
}

void TestProjection::tearDown() {
}

void TestProjection::testProjection() {
    // create scalar field
    ScalarField sf("CHGCAR_CH4", false);
    sf.read_header_and_atoms();
    sf.read();

    const float ref = 42350.35546875;

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

void TestProjection::test_plane(ScalarField* sf, glm::vec3 v, glm::vec3 w, glm::vec3 p, float ref) {
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
