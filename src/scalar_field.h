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

class ScalarField{
private:
    std::string filename;
    float scalar;
    float mat[3][3];    //!< matrix dimensions
    float imat[3][3];   //!< inverse of matrix
    glm::mat3 mat33;    //!< glm version of the matrix
    float volume;       //!< unit cell volume

    unsigned int grid_dimensions[3];
    std::vector<unsigned int> nrat;
    std::string gridline;
    std::vector<float> gridptr;  //!< grid to first pos of float array
    std::vector<float> gridptr2; //!< grid to first pos of float array
    unsigned int gridsize;
    bool vasp5_input;
    bool has_read;
    bool header_read;
    std::ifstream infile;

public:
    ScalarField(const std::string &_filename);
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

    void read();

    float get_value_interp(float x, float y, float z) const;

    float get_value(unsigned int i, unsigned int j, unsigned int k) const;

    glm::vec3 grid_to_realspace(float i, float j, float k) const;

    glm::vec3 realspace_to_grid(float i, float j, float k) const;

    glm::vec3 realspace_to_direct(float i, float j, float k) const;

    void copy_grid_dimensions(unsigned int _grid_dimensions[]) const;

    float get_max();

    float get_min();

    inline const glm::mat3& get_mat_unitcell() const {
        return this->mat33;
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
    void test_vasp5();
    void read_scalar();
    void read_matrix();
    void read_grid_dimensions();
    void read_atoms();
    void read_grid();
    float get_max_direction(unsigned int dim);
    void calculate_inverse();
    void calculate_volume();
};

#endif //_SCALAR_FIELD_H
