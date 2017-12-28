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

#include "scalar_field.h"

/*
 * Default constructor
 *
 * Usage: ScalarField sf("CHGCAR");
 *
 * Constructs a ScalarField by pointing it at a file on the HD.
 * The existence of the file is *not* being checked
 */
ScalarField::ScalarField(const std::string &_filename) {
    this->filename = _filename;
    this->scalar = -1;
    this->vasp5_input = false;
    this->has_read = false;
    this->header_read = false;

    // test existence of file, else throw an error
    if (!boost::filesystem::exists(this->filename)) {
        throw std::runtime_error("Cannot open " + this->filename + "!");
    }
}

/*
 * void output()
 *
 * Outputs a summary of the ScalarField to std::cout.
 * Mainly used for debugging purposes.
 *
 */
void ScalarField::output() const {
    std::cout << "Scalar: "<< this->scalar << std::endl;
    std::cout << std::endl;
    std::cout << "Matrix: ";
    for(unsigned i=0; i<3; i++) {
        for(unsigned j=0; j<3; j++) {
            std::cout << this->mat[i][j] << "\t";
        }
        std::cout << std::endl;
        std:: cout << "\t";
    }
    std::cout << std::endl;
    std::cout << "Inverse: ";
    for(unsigned i=0; i<3; i++) {
        for(unsigned j=0; j<3; j++) {
            std::cout << this->imat[i][j] << "\t";
        }
        std::cout << std::endl;
        std:: cout << "\t";
    }
    std::cout << std::endl;
    std::cout << "ion types: " << this->nrat.size() << " ( ";
    for(unsigned i=0; i<this->nrat.size(); i++) {
        std::cout << this->nrat[i] << " ";
    }
    std::cout << ")" << std::endl;
    std::cout << std::endl;
    std::cout << "Grid dimensions: ";
    for(unsigned i=0; i<3; i++) {
        std::cout << this->grid_dimensions[i] << "\t";
    }
    std::cout << std::endl;
}

/*
 * void read()
 *
 * Wrapper function that reads in the OUTCAR file
 *
 * Usage: sf.read(true);
 *
 */
void ScalarField::read() {
    if(has_read) {
        return;
    }

    if(!this->header_read) {
        this->test_vasp5();
        this->read_scalar();
        this->read_matrix();
        this->read_atoms();
        this->read_grid_dimensions();
    }

    this->read_grid();
}

/*
 * void test_vasp5()
 *
 * Test if the input file is a VASP5 output file
 *
 */
void ScalarField::test_vasp5() {
    std::ifstream infile(this->filename.c_str());
    std::string line;
    for(unsigned int i=0; i<5; i++) { // discard first two lines
        std::getline(infile, line);
    }
    std::getline(infile, line);
    // check if this line contains atomic information (i.e. alpha-characters)
    boost::regex regex_vasp_version("^(.*[A-Za-z]+.*)$");
    boost::smatch what;
    if(boost::regex_match(line, what, regex_vasp_version)) {
        this->vasp5_input = true;
    }
}

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
void ScalarField::read_scalar() {
    std::ifstream infile(this->filename.c_str());
    std::string line;
    std::getline(infile, line); // discard this line

    std::getline(infile, line);
    boost::regex regex_scalar("^\\s*([0-9.-]+)\\s*$");
    boost::smatch what;
    if(boost::regex_match(line, what, regex_scalar)) {
        this->scalar = boost::lexical_cast<float>(what[1]);
    } else {
        this->scalar = -1;
    }
}

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
void ScalarField::read_matrix() {
    std::ifstream infile(this->filename.c_str());
    std::string line;
    for(unsigned int i=0; i<2; i++) { // discard first two lines
        std::getline(infile, line);
    }

    // setup match pattern
    boost::regex regex_vasp_matrix_line("^\\s*([0-9.-]+)\\s+([0-9.-]+)\\s+([0-9.-]+)\\s*$");
    for(unsigned int i=0; i<3; i++) {
        std::getline(infile, line);
        boost::smatch what;
        if(boost::regex_match(line, what, regex_vasp_matrix_line)) {
            for(unsigned int j=0; j<3; j++) {
                mat[i][j] = boost::lexical_cast<float>(what[j+1]);
            }
        }
    }

    for(unsigned int i=0; i<3; i++) {
        for(unsigned int j=0; j<3; j++) {
            this->mat[i][j] *= this->scalar;
        }
    }

    // also construct inverse matrix
    this->calculate_inverse();

    // calculate matrix volume
    this->calculate_volume();
}

/*
 * void read_atoms()
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
void ScalarField::read_atoms() {
    std::ifstream infile(this->filename.c_str());
    std::string line;
    for(unsigned int i=0; i < (this->vasp5_input ? 7 : 6); i++) { // discard first two lines
        std::getline(infile, line);
    }

    std::vector<std::string> pieces;
    boost::trim(line);
    boost::split(pieces, line, boost::is_any_of("\t "), boost::token_compress_on);
    for(unsigned int i=0; i<pieces.size(); i++) {
        boost::trim(pieces[i]);
        this->nrat.push_back(boost::lexical_cast<unsigned int>(pieces[i]));
    }
}

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
void ScalarField::read_grid_dimensions() {
    std::ifstream infile(this->filename.c_str());
    std::string line;
    // skip lines that contain atoms
    for(unsigned int i=0; i<(this->vasp5_input ? 10 : 9); i++) {
        std::getline(infile, line);
    }
    for(unsigned int i=0; i<this->nrat.size(); i++) {
        for(unsigned int j=0; j<this->nrat[i]; j++) {
                std::getline(infile, line);
        }
    }

    boost::trim(line);
    this->gridline = line;

    std::vector<std::string> pieces;
    boost::split(pieces, line, boost::is_any_of("\t "), boost::token_compress_on);
    for(unsigned int i=0; i<pieces.size(); i++) {
        this->grid_dimensions[i] = boost::lexical_cast<unsigned int>(pieces[i]);
    }
}

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
void ScalarField::read_grid() {
    if(!this->header_read) {
        this->header_read = true;
        this->infile.open(this->filename.c_str());
        std::string line;
        // skip lines that contain atoms
        unsigned int skiplines = 0;
        for(unsigned int i=0; i<(this->vasp5_input ? 10 : 9); i++) {
            std::getline(this->infile, line);
            skiplines++;
        }
        for(unsigned int i=0; i<this->nrat.size(); i++) {
            for(unsigned int j=0; j<this->nrat[i]; j++) {
                    std::getline(this->infile, line);
                    skiplines++;
            }
        }

        this->gridsize = this->grid_dimensions[0] * this->grid_dimensions[1] * this->grid_dimensions[2];
    }

    float_parser p;

    /* read spin up */
    unsigned int linecounter=0; // for the counter
    static const boost::regex regex_augmentation("augmentation.*");
    std::string line;

    while(std::getline(this->infile, line)) {
        // stop looping when a second gridline appears (this
        // is where the spin down part starts)
        if(line.compare(this->gridline) == 0) {
            std::cout << "I am breaking the loop" << std::endl;
            break;
        }

        boost::smatch what;
        if(boost::regex_match(line, what, regex_augmentation)) {
            std::cout << "Augmentation break encountered" << std::endl;
            break;
        }

        // set iterators
        std::string::const_iterator b = line.begin();
        std::string::const_iterator e = line.end();

        // parse
        std::vector<float> floats;
        boost::spirit::qi::phrase_parse(b, e, p, boost::spirit::ascii::space, floats);

        // expand gridptr with the new size
        unsigned int cursize = this->gridptr.size();
        this->gridptr.resize(cursize + floats.size());

        // set the number of threads equal to the size of the pieces on the line
        for(unsigned int j=0; j<floats.size(); j++) {
            this->gridptr[cursize + j] = floats[j] / this->volume;
        }

        linecounter++;

        if(this->gridptr.size() >= this->gridsize) {
            this->has_read = true;
        }
    }
}

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
float ScalarField::get_value_interp(float x, float y, float z) const {
    // cast the input to the nearest grid point
    glm::vec3 r = this->realspace_to_grid(x,y,z);
    glm::vec3 d = this->realspace_to_direct(x,y,z);

    // to test whether the point is inside the box, we cast the point back
    // to the direct grid and check if it is for each cartesian coordinate
    // within the domain [0,1]
    if(d[0] < 0 || d[0] > 1.0) {
        // std::cerr << "[Error] Attempt to access point outside the grid" << std::endl;
        // std::cerr << "[" << x << "," << y << "," << z << "]" << std::endl;
        // std::cerr << "[" << d[0] << "," << d[0] << "," << d[0] << "]" << std::endl;
        return 0.0;
    }
    if(d[1] < 0 || d[1] > 1.0) {
        // std::cerr << "[Error] Attempt to access point outside the grid" << std::endl;
        // std::cerr << "[" << x << "," << y << "," << z << "]" << std::endl;
        // std::cerr << "[" << d[0] << "," << d[0] << "," << d[0] << "]" << std::endl;
        return 0.0;
    }
    if(d[2] < 0 || d[2] > 1.0) {
        // std::cerr << "[Error] Attempt to access point outside the grid" << std::endl;
        // std::cerr << "[" << x << "," << y << "," << z << "]" << std::endl;
        // std::cerr << "[" << d[0] << "," << d[0] << "," << d[0] << "]" << std::endl;
        return 0.0;
    }

    // calculate value using trilinear interpolation
    float xd = remainderf(r[0], 1.0);
    float yd = remainderf(r[1], 1.0);
    float zd = remainderf(r[2], 1.0);

    if(xd < 0) xd += 1.0;
    if(yd < 0) yd += 1.0;
    if(zd < 0) zd += 1.0;

    float x0 = floor(r[0]);
    float x1 = ceil(r[0]);
    float y0 = floor(r[1]);
    float y1 = ceil(r[1]);
    float z0 = floor(r[2]);
    float z1 = ceil(r[2]);

    return
    this->get_value(x0, y0, z0) * (1.0 - xd) * (1.0 - yd) * (1.0 - zd) +
    this->get_value(x1, y0, z0) * xd                 * (1.0 - yd) * (1.0 - zd) +
    this->get_value(x0, y1, z0) * (1.0 - xd) * yd                 * (1.0 - zd) +
    this->get_value(x0, y0, z1) * (1.0 - xd) * (1.0 - yd) * zd                 +
    this->get_value(x1, y0, z1) * xd                 * (1.0 - yd) * zd                 +
    this->get_value(x0, y1, z1) * (1.0 - xd) * yd                 * zd                 +
    this->get_value(x1, y1, z0) * xd                 * yd                 * (1.0 - zd) +
    this->get_value(x1, y1, z1) * xd                 * yd                 * zd;
}

/*
 * float get_max_direction(dim)
 *
 * Get the maximum value in a particular dimension. This is a convenience
 * function for the get_value_interp() function.
 *
 */
float ScalarField::get_max_direction(unsigned int dim) {
    float sum = 0;
    for(unsigned int i=0; i<3; i++) {
        sum += this->mat[i][dim];
    }
    return sum;
}

/*
 * void calculate_inverse()
 *
 * Calculates the inverse of a 3x3 matrix. This is a convenience
 * function for the read_matrix() function.
 *
 */
void ScalarField::calculate_inverse() {
    float det = 0;
    for(unsigned int i=0;i<3;i++) {
        det += (this->mat[0][i]*(this->mat[1][(i+1)%3]*this->mat[2][(i+2)%3] - this->mat[1][(i+2)%3]*this->mat[2][(i+1)%3]));
    }

    for(unsigned int i=0;i<3;i++){
            for(unsigned int j=0;j<3;j++) {
                     this->imat[i][j] = ((this->mat[(i+1)%3][(j+1)%3] * this->mat[(i+2)%3][(j+2)%3]) - (this->mat[(i+1)%3][(j+2)%3]*this->mat[(i+2)%3][(j+1)%3]))/ det;
            }
     }
}

/*
 * void calculate_volume()
 *
 * Calculates the inverse of a 3x3 matrix. This is a convenience
 * function for the read_matrix() function.
 *
 */
void ScalarField::calculate_volume() {
    for(unsigned int i=0; i<3; i++) {
        for(unsigned int j=0; j<3; j++) {
            this->mat33[i][j] = this->mat[i][j];
        }
    }

    this->volume = glm::dot(glm::cross(this->mat33[0], this->mat33[1]), this->mat33[2]);
}

/*
 * float get_value(i,j,k)
 *
 * Grabs the value at a particular grid point.
 *
 * This is a convenience function for the get_value_interp() function
 *
 */
float ScalarField::get_value(unsigned int i, unsigned int j, unsigned int k) const {
    if(i >= this->grid_dimensions[0]) {
        std::cerr << "ERROR: Cannot access x=" << i << std::endl;
    }
    if(j >= this->grid_dimensions[1]) {
        std::cerr << "ERROR: Cannot access y=" << j << std::endl;
    }
    if(k >= this->grid_dimensions[2]) {
        std::cerr << "ERROR: Cannot access z=" << k << std::endl;
    }
    unsigned int idx = k * this->grid_dimensions[0] * this->grid_dimensions[1] +
                                         j * this->grid_dimensions[0] +
                                         i;
    if(idx > this->gridsize) {
        std::cerr << "Trying to allocate value outside gridspace: (" << i << "," << j << "," << k << ")" << std::endl;
    }
    return this->gridptr[idx];
}

/*
 * glm::vec3 grid_to_realspace(i,j,k)
 *
 * Converts a grid point to a realspace vector. This function
 * is not being used at the moment.
 *
 */
glm::vec3 ScalarField::grid_to_realspace(float i, float j, float k) const {
    float dx = (float)i / (float)grid_dimensions[0];
    float dy = (float)j / (float)grid_dimensions[1];
    float dz = (float)k / (float)grid_dimensions[2];

    glm::vec3 r;
    r[0] = mat[0][0] * dx + mat[1][0] * dy + mat[2][0] * dz;
    r[1] = mat[0][1] * dx + mat[1][1] * dy + mat[2][1] * dz;
    r[2] = mat[0][2] * dx + mat[1][2] * dy + mat[2][2] * dz;

    return r;
}

/*
 * glm::vec3 realspace_to_grid(i,j,k)
 *
 * Convert 3d realspace vector to a position on the grid. Non-integer
 * values (i.e. floating point) are given as the result.
 *
 * This is a convenience function for the get_value_interp() function
 *
 */
glm::vec3 ScalarField::realspace_to_grid(float i, float j, float k) const {
    glm::vec3 r;
    r[0] = imat[0][0] * i + imat[0][1] * j + imat[0][2] * k;
    r[1] = imat[1][0] * i + imat[1][1] * j + imat[1][2] * k;
    r[2] = imat[2][0] * i + imat[2][1] * j + imat[2][2] * k;

    r[0] *= float(this->grid_dimensions[0]-1);
    r[1] *= float(this->grid_dimensions[1]-1);
    r[2] *= float(this->grid_dimensions[2]-1);

    return r;
}

/*
 * glm::vec3 realspace_to_direct(i,j,k)
 *
 * Convert 3d realspace vector to direct position.
 *
 */
glm::vec3 ScalarField::realspace_to_direct(float i, float j, float k) const {
    glm::vec3 r;
    r[0] = imat[0][0] * i + imat[0][1] * j + imat[0][2] * k;
    r[1] = imat[1][0] * i + imat[1][1] * j + imat[1][2] * k;
    r[2] = imat[2][0] * i + imat[2][1] * j + imat[2][2] * k;

    return r;
}

void ScalarField::copy_grid_dimensions(unsigned int _grid_dimensions[]) const {
    for(unsigned int i=0; i<3; i++) {
        _grid_dimensions[i] = this->grid_dimensions[i];
    }
}

float ScalarField::get_max() {
    return *std::max_element(this->gridptr.begin(), this->gridptr.end());
}

float ScalarField::get_min() {
    return *std::min_element(this->gridptr.begin(), this->gridptr.end());
}
