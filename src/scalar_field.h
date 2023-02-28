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
#include <glm/glm.hpp>

#include "float_parser.h"
#include "periodic_table.h"

class ScalarField{
private:
    std::string filename;
    float scalar;
    float mat[3][3];            //!< matrix dimensions
    float imat[3][3];           //!< inverse of matrix

    glm::mat3 mat33;            //!< glm version of the matrix
    glm::mat3 imat33;           //!< glm version of the inverse matrix

    float volume;               //!< unit cell volume

    unsigned int grid_dimensions[3];
    std::vector<unsigned int> nrat;

    std::vector<glm::vec3> atom_pos;
    std::vector<unsigned int> atom_charges;

    std::vector<glm::vec3> atom_pos_exp;
    std::vector<unsigned int> atom_charges_exp;

    std::string gridline;
    std::vector<float> gridptr;  //!< grid to first pos of float array
    std::vector<float> gridptr2; //!< grid to first pos of float array
    unsigned int gridsize;
    bool vasp5_input;
    bool has_read;
    bool header_read;
    std::ifstream infile;
    bool flag_is_locpot;         //!< whether scalar field is in LOCPOT style

public:

    /**
     * @brief      constructor
     *
     * @param[in]  _filename   url to filename
     * @param[in]  _flag_is_locpot  whether this file is a locpot
     */
    ScalarField(const std::string &_filename, bool _flag_is_locpot);

    void output() const;

    glm::mat3 get_unitcell_matrix() {
        glm::mat3 out;
        for(unsigned int i=0; i<3; i++) {
            for(unsigned int j=0; j<3; j++) {
                out[i][j] = mat[i][j];
            }
        }

        return out;
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
     * float get_value_interp(x,y,z)
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
    float get_value_interp(float x, float y, float z) const;

    /**
     * @brief      test whether point is inside unit cell
     *
     * @param[in]  x     x position
     * @param[in]  y     y position
     * @param[in]  z     z position
     *
     * @return     True if inside, False otherwise.
     */
    bool is_inside(float x, float y, float z) const;

    float get_value(unsigned int i, unsigned int j, unsigned int k) const;

    glm::vec3 grid_to_realspace(float i, float j, float k) const;

    glm::vec3 realspace_to_grid(float i, float j, float k) const;

    glm::vec3 realspace_to_direct(float i, float j, float k) const;

    void copy_grid_dimensions(unsigned int _grid_dimensions[]) const;

    float get_max() const;

    float get_min() const;

    glm::vec3 get_atom_position(unsigned int atid) const;

    inline const glm::mat3& get_mat_unitcell() const {
        return this->mat33;
    }

    inline const glm::mat3& get_mat_unitcell_inverse() const {
        return this->imat33;
    }

    inline const float* get_grid_ptr() const {
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
     * float get_max_direction(dim)
     *
     * Get the maximum value in a particular dimension. This is a convenience
     * function for the get_value_interp() function.
     *
     */
    float get_max_direction(unsigned int dim);

    /*
     * void calculate_inverse()
     *
     * Calculates the inverse of a 3x3 matrix. This is a convenience
     * function for the read_matrix() function.
     *
     */
    void calculate_inverse();

    /*
     * void calculate_volume()
     *
     * Calculates the inverse of a 3x3 matrix. This is a convenience
     * function for the read_matrix() function.
     *
     */
    void calculate_volume();
};

#endif //_SCALAR_FIELD_H
