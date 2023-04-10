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

#ifndef _SCALAR_FIELD_H
#define _SCALAR_FIELD_H

#include <string>
#include <vector>
#include <iostream>
#include <ios>
#include <sstream>
#include <fstream>
#include <math.h>

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/filesystem.hpp>

#include "math.h"
#include "float_parser.h"
#include "periodic_table.h"

class ScalarField{
private:
    std::string filename;
    fpt scalar;

    MatrixUnitcell mat;
    MatrixUnitcell imat;

    fpt volume;

    std::array<unsigned int, 3> grid_dimensions;
    std::vector<unsigned int> nrat;

    std::vector<Vec3> atom_pos;
    std::vector<unsigned int> atom_charges;

    std::vector<Vec3> atom_pos_exp;
    std::vector<unsigned int> atom_charges_exp;

    std::string gridline;
    std::vector<fpt> gridptr;
    std::vector<fpt> gridptr2;
    unsigned int gridsize;
    bool vasp5_input;
    bool has_read;
    bool header_read;
    std::ifstream infile;
    bool flag_is_locpot;

public:

    /**
     * @brief      constructor
     *
     * @param[in]  _filename   url to filename
     * @param[in]  _flag_is_locpot  whether this file is a locpot
     */
    ScalarField(const std::string &_filename, bool _flag_is_locpot);

    /*
     * @brief output()
     *
     * Outputs a summary of the ScalarField to std::cout.
     * Mainly used for debugging purposes.
     *
     */
    void output() const;

    inline const auto& get_grid_dimensions() const {
        return this->grid_dimensions;
    }

    /**
     * @brief Get the volume of the unit cell
     *
     * @return volume of the unit cell
     */
    inline fpt get_volume() const {
        return this->volume;
    }

    MatrixUnitcell get_unitcell_matrix() const {
        return this->mat;
    }

    /**
     * @brief      determines if this Scalarfield is in LOCPOT-style
     *
     * @return     True if locpot, False otherwise.
     */
    inline bool is_locpot() const {
        return this->flag_is_locpot;
    }

    /*
     * void read()
     *
     * Wrapper function that reads in the OUTCAR file
     *
     * Usage: sf.read(true);
     *
     */
    void read();

    void read_header_and_atoms();

    /*
     * fpt get_value_interp(x,y,z)
     *
     * Grabs a value from the 3D scalar field. Calculate the value
     * by using a trilinear interpolation.
     *
     * The trilinear interpolation algorithm has been extracted from:
     * http://paulbourke.net/miscellaneous/interpolation/
     *
     * Future algorithm can make use of a cubic interpolation.
     *
     */
    fpt get_value_interp(fpt x, fpt y, fpt z) const;

    /**
     * @brief      test whether point is inside unit cell
     *
     * @param[in]  x     x position
     * @param[in]  y     y position
     * @param[in]  z     z position
     *
     * @return     True if inside, False otherwise.
     */
    bool is_inside(fpt x, fpt y, fpt z) const;

    fpt get_value(unsigned int i, unsigned int j, unsigned int k) const;

    Vec3 grid_to_realspace(fpt i, fpt j, fpt k) const;

    Vec3 realspace_to_grid(fpt i, fpt j, fpt k) const;

    Vec3 realspace_to_direct(fpt i, fpt j, fpt k) const;

    fpt get_max() const;

    fpt get_min() const;

    Vec3 get_atom_position(unsigned int atid) const;

    inline const MatrixUnitcell& get_mat_unitcell() const {
        return this->mat;
    }

    inline const MatrixUnitcell& get_mat_unitcell_inverse() const {
        return this->imat;
    }

    inline const fpt* get_grid_ptr() const {
        return &this->gridptr[0];
    }

    unsigned int get_size() const {
        return this->gridptr.size();
    }

    inline const std::string& get_filename() const {
        return this->filename;
    }

private:
    /*
     * void test_vasp5()
     *
     * Test if the input file is a VASP5 output file
     *
     */
    void test_vasp5();

    /*
     * void read_scalar()
     *
     * Read the scalar value from the 2nd line of the
     * CHGCAR file. Note that all read_* functions can
     * be used seperately, although they may depend
     * on each other and have to be used in some
     * consecutive order as is done in the read()
     * wrapper function.
     *
     */
    void read_scalar();

    /*
     * void read_matrix()
     *
     * Reads the matrix that defines the unit cell
     * in the CHGCAR file. The inverse of that matrix
     * is automatically constructed.
     *
     * Note that all read_* functions can
     * be used seperately, although they may depend
     * on each other and have to be used in some
     * consecutive order as is done in the read()
     * wrapper function.
     *
     */
    void read_matrix();

    /*
     * void read_grid_dimensions()
     *
     * Read the number of gridpoints in each
     * direction.
     *
     * Note that all read_* functions can
     * be used seperately, although they may depend
     * on each other and have to be used in some
     * consecutive order as is done in the read()
     * wrapper function.
     *
     */
    void read_grid_dimensions();

    /*
     * void read_nr_atoms()
     *
     * Read the number of atoms of each element. These
     * numbers are used to skip the required amount of
     * lines.
     *
     * Note that all read_* functions can
     * be used seperately, although they may depend
     * on each other and have to be used in some
     * consecutive order as is done in the read()
     * wrapper function.
     *
     */
    void read_nr_atoms();

    void read_atom_positions();

    /*
     * void read_grid()
     *
     * Read all the grid points. This function depends
     * on the the gridsize being set via the
     * read_grid_dimensions() function.
     *
     * Note that all read_* functions can
     * be used seperately, although they may depend
     * on each other and have to be used in some
     * consecutive order as is done in the read()
     * wrapper function.
     *
     */
    void read_grid();

    /*
     * fpt get_max_direction(dim)
     *
     * Get the maximum value in a particular dimension. This is a convenience
     * function for the get_value_interp() function.
     *
     */
    fpt get_max_direction(unsigned int dim);
};

#endif //_SCALAR_FIELD_H
