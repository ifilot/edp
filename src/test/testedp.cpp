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

void TestEDP::testConstructor() {
    // build object
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
}
