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

 #include "planeprojector.h"

/**
 * @brief      constructor
 *
 * @param      _sf               pointer to ScalarField
 * @param[in]  _min              minimum value
 * @param[in]  _max              maximum value
 * @param[in]  _color_scheme_id  The color scheme identifier
 */
PlaneProjector::PlaneProjector(ScalarField* _sf, unsigned int _color_scheme_id) {
    this->sf = _sf;
    this->color_scheme_id = _color_scheme_id;
}

/**
 * @brief      set the scaling for the graph
 *
 * @param[in]  allow_negative  whether to allow negative values
 * @param[in]  _min            minimum values
 * @param[in]  _max            maximum values
 */
void PlaneProjector::set_scaling(bool allow_negative, float _min, float _max) {
    this->flag_negative = allow_negative;
    this->log_min = _min;
    this->log_max = _max;

    if(this->flag_negative) {
        this->scheme = new ColorScheme(-1, 1, this->color_scheme_id);
    } else {
        this->scheme = new ColorScheme(0, 1, this->color_scheme_id);
    }
}

/**
 * @brief      plot contour plane
 */
void PlaneProjector::plot() {
    this->plt = new Plotter(this->ix, this->iy);

    for(unsigned int i=0; i<uint(this->iy); i++) {
        for(unsigned int j=0; j<uint(this->ix); j++) {
            if(this->planegrid_box[i * this->ix + j]) {
                this->plt->draw_filled_rectangle(j,i, 1, 1, this->scheme->get_color(this->planegrid_log[i * this->ix + j]));
            } else {
                this->plt->draw_filled_rectangle(j,i, 1, 1, Color(0,0,0,0));
            }
        }
    }
}

/**
 * @brief      extract plane from scalar field
 *
 * @param[in]  _v1              direction vector 1
 * @param[in]  _v2              direction vector 2
 * @param[in]  _p               position vector
 * @param[in]  _scale           scaling constant (from Angstrom to pixels)
 * @param[in]  li               extend in -v1 direction in Angstrom
 * @param[in]  hi               extend in +v1 direction in Angstrom
 * @param[in]  lj               extend in -v2 direction in Angstrom
 * @param[in]  hj               extend in +v2 direction in Angstrom
 */
void PlaneProjector::extract(glm::vec3 _v1, glm::vec3 _v2, const glm::vec3& _p, float _scale, float li, float hi, float lj, float hj) {

    // use normalized vectors
    _v1 = glm::normalize(_v1);
    _v2 = glm::normalize(_v2);

    this->scale = _scale;
    this->ix = int((hi - li) * _scale);
    this->iy = int((hj - lj) * _scale);

    std::cout << "Creating " << this->ix << "x" << this->iy << "px image..." << std::endl;

    this->planegrid_log =  new float[this->ix * this->iy];
    this->planegrid_real = new float[this->ix * this->iy];
    this->planegrid_box =  new bool[this->ix * this->iy];

    #pragma omp parallel for collapse(2)
    for(int i=0; i<this->ix; i++) {
        for(int j=0; j<this->iy; j++) {
            const float x = _v1[0] * float(i - this->ix / 2) / _scale + _v2[0] * float(j - this->iy / 2) / _scale + _p[0];
            const float y = _v1[1] * float(i - this->ix / 2) / _scale + _v2[1] * float(j - this->iy / 2) / _scale + _p[1];
            const float z = _v1[2] * float(i - this->ix / 2) / _scale + _v2[2] * float(j - this->iy / 2) / _scale + _p[2];

            const bool is_inside = this->sf->is_inside(x,y,z);
            const float val = this->sf->get_value_interp(x,y,z);

            if(!is_inside) {
                this->planegrid_box[j * this->ix + i] = false;
                this->planegrid_log[j * this->ix + i] = 0.0f;
                this->planegrid_real[j * this->ix + i] = 0.0f;
                continue;
            } else {
                this->planegrid_box[j * this->ix + i] = true;
            }

            if(this->flag_negative) {
                this->planegrid_log[j * this->ix + i] = this->calculate_scaled_value_log(val);
            } else {
                if(val > 0) {
                    this->planegrid_log[j * this->ix + i] = this->calculate_scaled_value_log(val);
                } else {
                    this->planegrid_log[j * this->ix + i] = -12;
                }
            }
            this->planegrid_real[j * this->ix + i] = val;
        }
    }

    this->cut_and_recast_plane();
}

/**
 * @brief      extract line
 *
 * @param[in]  e       vector direction
 * @param[in]  p       position vector
 * @param[in]  _scale  scale
 * @param[in]  li      extend in -e direction
 * @param[in]  hi      extend in +e direction
 */
void PlaneProjector::extract_line(glm::vec3 e, const glm::vec3& p, float _scale, float li, float hi) {
    e = glm::normalize(e);

    this->scale = _scale;
    this->ix = int((hi - li) * _scale);

    std::vector<glm::vec3> pos;
    std::vector<float> vals;

    for(int i=0; i<this->ix; i++) {
        float x = e[0] * float(i - this->ix / 2) / _scale + p[0];
        float y = e[1] * float(i - this->ix / 2) / _scale + p[1];
        float z = e[2] * float(i - this->ix / 2) / _scale + p[2];

        float val = this->sf->get_value_interp(x,y,z);

        if(val != 0.0) {
            pos.push_back(glm::vec3(x,y,z));
            vals.push_back(val);
        }
    }

    // open file and output results
    std::ofstream out("line_extraction.txt");
    for(unsigned int i=0; i<vals.size(); i++) {
        out << boost::format("%12.6f  %12.6f  %12.6f  %12.6e\n") % pos[i][0] % pos[i][1] % pos[i][2] % vals[i];
    }

    out.close();
}

/**
 * @brief      calculate the average density (electron or potential) and store it as function of z-height
 */
void PlaneProjector::extract_plane_average() {
    unsigned int dimensions[3];
    this->sf->copy_grid_dimensions(dimensions);

    const float* grid = this->sf->get_grid_ptr();
    std::vector<float> avg;
    std::vector<float> z;

    const float sz = (float)(dimensions[0] * dimensions[1]);

    for(unsigned int i=0; i<dimensions[2]; i++) {   // loop over z-axis
        float sum = 0.0f;
        #pragma omp parallel for collapse(2) reduction(+:sum)
        for(unsigned int j=0; j<dimensions[1]; j++) {   // loop over y-axis
            for(unsigned int k=0; k<dimensions[0]; k++) {   // loop over x-axis
                sum += grid[i * dimensions[0] * dimensions[1] +   // z
                            j * dimensions[0] +                   // y
                            k];                                   // x
            }
        }
        avg.push_back(sum / sz);
        z.push_back(i / (float)dimensions[2]);
    }

    // write to file
    // open file and output results
    std::ofstream out("z_extraction.txt");
    for(unsigned int i=0; i<avg.size(); i++) {
        out << boost::format("%12.6f  %12.6f\n") % z[i] % avg[i];
    }

    out.close();
}

/**
 * @brief      calculate the average density projected on a sphere of a
 *             specified radius
 *
 * @param[in]  p       position of the sphere
 * @param[in]  radius  radius of the sphere
 */
void PlaneProjector::extract_sphere_average(const glm::vec3& p, float radius) {
    // use Lebedev quadrature points
    unsigned int level = Quadrature::LEBEDEV_194;
    unsigned int start_idx = 0;
    for(unsigned int i=0; i<level-1; i++) {
        start_idx += Quadrature::num_lebedev_points[i];
    }

    // build vectors
    std::vector<float> radii;
    std::vector<float> values;

    // integrate over points
    for(float r = 0.0; r <= radius; r += 0.01f) {
        float sum = 0.0f;
        #pragma omp parallel for reduction(+:sum)
        for(unsigned int i=0; i<Quadrature::num_lebedev_points[level]; i++) {
            glm::vec3 pp = p + glm::vec3(Quadrature::lebedev_coefficients[i][0],
                                         Quadrature::lebedev_coefficients[i][1],
                                         Quadrature::lebedev_coefficients[i][2]) * r;

            // align point to unit cell when crossing periodic boundary conditions
            glm::vec3 pd = this->sf->get_mat_unitcell_inverse() * pp;
            pd = glm::fract(pd);
            pp = this->sf->get_mat_unitcell() * pd;

            const float val = this->sf->get_value_interp(pp[0], pp[1], pp[2]);

            sum += val;
        }
        radii.push_back(r);
        values.push_back(sum / (float)Quadrature::num_lebedev_points[level]);
    }

    // write to file
    // open file and output results
    std::ofstream out("spherical_average.txt");
    for(unsigned int i=0; i<radii.size(); i++) {
        out << boost::format("%12.6f  %12.6f\n") % radii[i] % values[i];
    }

    out.close();
}

/**
 * @brief      draw isolines
 *
 * @param[in]  bins             number of bins
 */
void PlaneProjector::isolines(unsigned int bins) {
    if(this->flag_negative) {
        for(float val = this->log_min; val <= this->log_max; val += 1) {
            this->draw_isoline(-pow(10.0, val));
            this->draw_isoline(pow(10.0, val));
        }
        this->draw_isoline(0);
    } else {
        float binsize = (this->log_max - this->log_min) / float(bins + 1);
        for(float val = this->log_min; val <= this->log_max; val += binsize) {
            this->draw_isoline(pow(10.0,val));
        }
        this->draw_isoline(0);
    }
}

/**
 * @brief      Draws a legend.
 *
 * @param[in]  negative_values  whether there are negative values in the plot
 */
void PlaneProjector::draw_legend() {
    // set sizes
    const float size = this->scale * 1.2;
    const float fontsize = 24.f / 100.f * this->scale;
    const float offset = size / 10;
    const float superscript = 20.f / 32.f * fontsize;
    const float shade_offset = fontsize / 10.f;
    const float shade_offset_sc = superscript / 10.f;

    if(this->sf->is_locpot()) { // draw legend for LOCPOT
        const std::string units = "eV";
        auto bounds = this->plt->get_text_bounds(fontsize, units);
        float txpos = this->ix - 0.6 * size - bounds.height * 1.2;
        float typos = size / 4.0f + bounds.width;
        this->plt->type(txpos + shade_offset, typos + shade_offset, fontsize, -90, Color(0,0,0), units);
        this->plt->type(txpos, typos, fontsize, -90, Color(255, 255, 255), units);
    } else {                    // draw legend for CHGCAR
        const std::string units = "electrons / Å";
        auto bounds = this->plt->get_text_bounds(fontsize, units);
        float txpos = this->ix - 0.6 * size - bounds.height;
        float typos = size / 4.0f + bounds.width;

        // draw black
        this->plt->type(txpos + shade_offset, typos + shade_offset, fontsize, -90, Color(0,0,0), units);
        this->plt->type(txpos - bounds.height + fontsize / 2 + shade_offset_sc, typos - bounds.width + shade_offset_sc, superscript, -90, Color(0, 0, 0), "3");

        // draw white
        this->plt->type(txpos, typos, fontsize, -90, Color(255, 255, 255), units);
        this->plt->type(txpos - bounds.height + fontsize / 2, typos - bounds.width, superscript, -90, Color(255, 255, 255), "3");
    }

    float yy = 0;
    float val = 0;

    const float locpot_scale = (this->log_max - this->log_min) + 1.0f;
    const float mmax = this->flag_negative ? locpot_scale : this->log_max;
    const float mmin = this->flag_negative ? -locpot_scale : this->log_min;

    for(int i=mmax; i >= mmin; i--) {

        if(this->flag_negative) {
            if(i > 0) {
                val = pow(10, (i + this->log_min - 1));
            } else if(i < 0) {
                val = -pow(10, -(i - this->log_min + 1));
            } else { // i == 0
                val = 0;
            }

        } else {
            val = pow(10, i);
        }

        // set legend colors
        const Color lcol = this->scheme->get_color(this->calculate_scaled_value_log(val));

        // calculate xpos and ypos for the rectangles
        const float xpos = this->ix - 3.f / 4.f * size;
        const float ypos = size / 4.0f + yy;
        const float xc = xpos + size / 4.0f;
        const float yc = ypos + size / 4.0f;

        this->plt->draw_filled_rectangle(xpos, ypos, size / 2.0f, size / 2.0f, lcol);
        this->plt->draw_empty_rectangle(xpos, ypos, size / 2.0f, size / 2.0f, Color(0,0,0), 1);

        // determine text for base
        std::string textbase;
        if(this->flag_negative) {
            if(i != 0) {
                textbase = i >= 0 ? "10" : "-10";
            } else {
                textbase = "0";
            }
        } else {
            textbase = "10";
        }

        // determine text for power
        std::string textpower;
        if(i != 0) {
            textpower = (boost::format("%i") % log10(std::abs(val))).str();
        } else {
            textpower = "  ";
        }

        // calculate bounds
        auto bounds1 = this->plt->get_text_bounds(fontsize, textbase);
        auto bounds2 = this->plt->get_text_bounds(superscript, textpower);
        const float tw = bounds1.width + bounds2.width;
        const float th = bounds1.height + bounds2.height;

        // type text
        const float tx = xc - tw / 2;
        const float ty = yc + th / 2;

        // draw black
        this->plt->type(xc - tw / 2 + shade_offset, yc + th / 2 + shade_offset, fontsize, 0, Color(0,0,0), textbase);
        this->plt->type(tx + bounds1.width + shade_offset_sc, ty - bounds1.height + shade_offset_sc, superscript, 0, Color(0,0,0), textpower);

        // draw white
        this->plt->type(xc - tw / 2, yc + th / 2, fontsize, 0, Color(255,255,255), textbase);
        this->plt->type(tx + bounds1.width, ty - bounds1.height, superscript, 0, Color(255,255,255), textpower);

        yy += size / 2.0f;
    }
}

/**
 * @brief      write contour plane to file
 *
 * @param[in]  filename  path to png file
 */
void PlaneProjector::write(std::string filename) {
    plt->write(filename.c_str());
    std::cout << "Writing " << filename << std::endl;
    delete this->plt;
}

/**
 * @brief      Destroys the object.
 */
PlaneProjector::~PlaneProjector() {
    delete[] this->planegrid_log;
    delete[] this->planegrid_real;
    delete[] this->planegrid_box;
}

/**
 * @brief      reposition plane within the boundaries of the unit cell
 */
void PlaneProjector::cut_and_recast_plane() {
    unsigned int min_x = 0;
    unsigned int max_x = this->ix;
    unsigned int min_y = 0;
    unsigned int max_y = this->iy;

    // determine min_x
    for(unsigned int i=0; i<uint(this->ix); i++) {
        bool line = false;

        #pragma omp parallel for
        for(unsigned int j=0; j<uint(this->iy); j++) {
            if(this->planegrid_real[(j) * this->ix + i] != 0.0) {
                line = true;
            }
        }
        if(line) {
            min_x = i;
            break;
        }
    }

    // determine max_x
    for(unsigned int i=uint(this->ix); i>0; i--) {
        bool line = false;

        #pragma omp parallel for
        for(unsigned int j=0; j<uint(this->iy); j++) {
            if(this->planegrid_real[(j) * this->ix + i] != 0.0) {
                line = true;
            }
        }
        if(line) {
            max_x = i;
            break;
        }
    }

    // determine min_y
    for(unsigned int j=0; j<uint(this->iy); j++) {
        bool line = false;

        #pragma omp parallel for
        for(unsigned int i=0; i<uint(this->ix); i++) {
            if(this->planegrid_real[(j) * this->ix + i] != 0.0) {
                line = true;
            }
        }
        if(line) {
            min_y = j;
            break;
        }
    }

    // determine max_y
    for(unsigned int j=uint(this->iy-1); j>0; j--) {
        bool line = false;

        #pragma omp parallel for
        for(unsigned int i=0; i<uint(this->ix); i++) {
            if(this->planegrid_real[(j) * this->ix + i] != 0.0) {
                line = true;
            }
        }
        if(line) {
            max_y = j;
            break;
        }
    }

    unsigned int nx = max_x - min_x;
    unsigned int ny = max_y - min_y;

    std::cout << "Recasting to [" << min_x << ":" << max_x
              << "] x [" << min_y << ":" << max_y << "]" << std::endl;

    // recasting
    float* newgrid_log = new float[nx * ny];
    float* newgrid_real = new float[nx * ny];
    bool* newgrid_box = new bool[nx * ny];

    #pragma omp parallel for collapse(2)
    for(unsigned int i=0; i<nx; i++) {
        for(unsigned int j=0; j<ny; j++) {
            newgrid_log[j * nx + i] = this->planegrid_log[(j + min_y) * this->ix + (i + min_x)];
            newgrid_real[j * nx + i] = this->planegrid_real[(j + min_y) * this->ix + (i + min_x)];
            newgrid_box[j * nx + i] = this->planegrid_box[(j + min_y) * this->ix + (i + min_x)];
        }
    }

    delete[] this->planegrid_real;
    delete[] this->planegrid_log;
    delete[] this->planegrid_box;

    this->planegrid_real = newgrid_real;
    this->planegrid_log = newgrid_log;
    this->planegrid_box = newgrid_box;

    this->ix = nx;
    this->iy = ny;
}

/**
 * @brief      draw a single isoline
 *
 * @param[in]  val   value of the isoline
 */
void PlaneProjector::draw_isoline(float val) {
    for(unsigned int i=1; i<uint(this->ix-1); i++) {
        for(unsigned int j=1; j<uint(this->iy-1); j++) {
            if(this->is_crossing(i,j,val)) {
                this->plt->draw_filled_rectangle(i,j, 1, 1, Color(0,0,0));
            }
        }
    }
}

/**
 * @brief      determines whether a value on the pixel the isovalue is crossing
 *
 * @param[in]  i     index i on contour plane
 * @param[in]  j     index j on contour plane
 * @param[in]  val   isovalue
 *
 * @return     True if crossing, False otherwise.
 */
bool PlaneProjector::is_crossing(unsigned int i, unsigned int j, float val) {
    if(this->planegrid_real[(j-1) * this->ix + (i)] < val && this->planegrid_real[(j+1) * this->ix + (i)] > val) {
        return true;
    }
    if(this->planegrid_real[(j-1) * this->ix + (i)] > val && this->planegrid_real[(j+1) * this->ix + (i)] < val) {
        return true;
    }
    if(this->planegrid_real[(j) * this->ix + (i-1)] > val && this->planegrid_real[(j) * this->ix + (i+1)] < val) {
        return true;
    }
    if(this->planegrid_real[(j) * this->ix + (i-1)] < val && this->planegrid_real[(j) * this->ix + (i+1)] > val) {
        return true;
    }
    return false;
}

/**
 * @brief      Calculates the scaled value using a logarithmic scale.
 *
 * @param[in]  input  input value
 *
 * @return     The scaled value.
 */
float PlaneProjector::calculate_scaled_value_log(float input) {
    const float scale = (this->log_max - this->log_min + 1.0f);
    const float logval = std::min(std::max(log10(std::fabs(input)), this->log_min), this->log_max);
    return sgn(input) * (logval - this->log_min) / scale;
}
