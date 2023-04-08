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

#include "test_scalarfield.h"

CPPUNIT_TEST_SUITE_REGISTRATION( TestScalarField );

void TestScalarField::setUp() {
}

void TestScalarField::tearDown() {
}

void TestScalarField::testReading() {
    // create scalar field
    ScalarField sf("CHGCAR_CH4", false);
    CPPUNIT_ASSERT_EQUAL( (uint)0, sf.get_size() );

    // read atoms and check this
    sf.read_header_and_atoms();
    CPPUNIT_ASSERT_EQUAL( (uint)0, sf.get_size() );
    auto p = sf.get_atom_position(0);
    CPPUNIT_ASSERT_EQUAL( (fpt)5.0, p(0) );
    CPPUNIT_ASSERT_EQUAL( (fpt)5.0, p(1) );
    CPPUNIT_ASSERT_EQUAL( (fpt)5.0, p(2) );

    // read scalar field and test this
    sf.read();
    fpt V = sf.get_volume();
    CPPUNIT_ASSERT_EQUAL( (uint)1000000, sf.get_size() );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( (fpt)-0.276298 / V, sf.get_min(), 1e-8 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( (fpt)2047.064424 / V, sf.get_max(), 1e-4 );

    // read specific values from the scalar field
    CPPUNIT_ASSERT_DOUBLES_EQUAL( sf.get_grid_ptr()[0], sf.get_value_interp(0.0,0.0,0.0), 1e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( sf.get_grid_ptr()[1], sf.get_value_interp(0.1,0.0,0.0), 1e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( sf.get_grid_ptr()[100], sf.get_value_interp(0.0,0.1,0.0), 1e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( sf.get_grid_ptr()[100*100], sf.get_value_interp(0.0,0.0,0.1), 1e-12 );

    // also test some false values
    CPPUNIT_ASSERT( std::abs(sf.get_grid_ptr()[50*100*100 + 50 * 100 + 49] - sf.get_value_interp(5.0,5.0,5.0)) > 1e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( sf.get_grid_ptr()[50*100*100 + 50 * 100 + 50], sf.get_value_interp(5.0,5.0,5.0), 1e-12 );
    CPPUNIT_ASSERT( std::abs(sf.get_grid_ptr()[50*100*100 + 50 * 100 + 51] - sf.get_value_interp(5.0,5.0,5.0)) > 1e-12 );

    // test some averaging
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.5 * sf.get_grid_ptr()[50*100*100 + 50 * 100 + 49] +
                                  0.5 * sf.get_grid_ptr()[50*100*100 + 50 * 100 + 50],
                                  sf.get_value_interp(4.95,5.0,5.0), 1e-5);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.5 * sf.get_grid_ptr()[50*100*100 + 49 * 100 + 50] +
                                  0.5 * sf.get_grid_ptr()[50*100*100 + 50 * 100 + 50],
                                  sf.get_value_interp(5.0,4.95,5.0), 1e-5);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.5 * sf.get_grid_ptr()[49*100*100 + 50 * 100 + 50] +
                                  0.5 * sf.get_grid_ptr()[50*100*100 + 50 * 100 + 50],
                                  sf.get_value_interp(5.0,5.0,4.95), 1e-5);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.125 * sf.get_grid_ptr()[49*100*100 + 49 * 100 + 49] +
                                  0.125 * sf.get_grid_ptr()[49*100*100 + 49 * 100 + 50] +
                                  0.125 * sf.get_grid_ptr()[49*100*100 + 50 * 100 + 49] +
                                  0.125 * sf.get_grid_ptr()[49*100*100 + 50 * 100 + 50] +
                                  0.125 * sf.get_grid_ptr()[50*100*100 + 49 * 100 + 49] +
                                  0.125 * sf.get_grid_ptr()[50*100*100 + 49 * 100 + 50] +
                                  0.125 * sf.get_grid_ptr()[50*100*100 + 50 * 100 + 49] +
                                  0.125 * sf.get_grid_ptr()[50*100*100 + 50 * 100 + 50],
                                  sf.get_value_interp(4.95,4.95,4.95), 1e-5);

    // periodic boundary conditions
    CPPUNIT_ASSERT_DOUBLES_EQUAL( sf.get_value_interp(0.0,0.0,0.0),
                                  sf.get_value_interp(10.0,10.0,10.0),
                                  1e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( sf.get_value_interp(0.0,0.0,0.0),
                                  sf.get_value_interp(0.0,10.0,10.0),
                                  1e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( sf.get_value_interp(0.0,0.0,0.0),
                                  sf.get_value_interp(0.0,0.0,10.0),
                                  1e-12);
}
