/**************************************************************************
 *                                                                        *
 *   Author: Ivo Filot <i.a.w.filot@tue.nl>                               *
 *                                                                        *
 *   DEN2BIN is free software:                                            *
 *   you can redistribute it and/or modify it under the terms of the      *
 *   GNU General Public License as published by the Free Software         *
 *   Foundation, either version 3 of the License, or (at your option)     *
 *   any later version.                                                   *
 *                                                                        *
 *   DEN2BIN is distributed in the hope that it will be useful,           *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty          *
 *   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.              *
 *   See the GNU General Public License for more details.                 *
 *                                                                        *
 *   You should have received a copy of the GNU General Public License    *
 *   along with this program.  If not, see http://www.gnu.org/licenses/.  *
 *                                                                        *
 **************************************************************************/

 #include "planeprojector.h"


PlaneProjector::PlaneProjector(ScalarField* _sf, float _min, float _max) {
    this->min = _min;
    this->max = _max;
    this->scheme = new ColorScheme(_min, _max, 0);
    this->sf = _sf;
}

void PlaneProjector::extract(Vector _v1, Vector _v2, Vector _s, float _scale, float li, float hi, float lj, float hj, bool negative_values) {

    //only use normalized vectors
    _v1.normalize();
    _v2.normalize();

    this->ix = int((hi - li) * _scale);
    this->iy = int((hj - lj) * _scale);

    std::cout << "Creating " << this->ix << "x" << this->iy << "px image..." << std::endl;

    this->planegrid_log =  new float[this->ix * this->iy];
    this->planegrid_real =  new float[this->ix * this->iy];

    #pragma omp parallel for collapse(2)
    for(int i=0; i<this->ix; i++) {
        for(int j=0; j<this->iy; j++) {
            float x = _v1[0] * float(i - this->ix / 2) / _scale + _v2[0] * float(j - this->iy / 2) / _scale + _s[0];
            float y = _v1[1] * float(i - this->ix / 2) / _scale + _v2[1] * float(j - this->iy / 2) / _scale + _s[1];
            float z = _v1[2] * float(i - this->ix / 2) / _scale + _v2[2] * float(j - this->iy / 2) / _scale + _s[2];
            float val = this->sf->get_value_interp(x,y,z);
            if(negative_values) {
                if(val < -10) {
                    this->planegrid_log[j * this->ix + i] = -log10(-val);
                } else if(val > 10) {
                    this->planegrid_log[j * this->ix + i] = log10(val);
                } else {
                    this->planegrid_log[j * this->ix + i] = val / 10.0;
                }
            } else {
                // cast negative values to a very small number
                if(val > 0) {
                    this->planegrid_log[j * this->ix + i] = log10(val);
                } else {
                    this->planegrid_log[j * this->ix + i] = -12;
                }
            }
            this->planegrid_real[j * this->ix + i] = val;
        }
    }

    this->cut_and_recast_plane();
}

void PlaneProjector::isolines(unsigned int bins, bool negative_values) {
    float binsize = (this->max - this->min) / float(bins + 1);
    if(negative_values) {
        for(float val = this->min; val < this->max; val += binsize) {
            if(val < -1) {
                this->draw_isoline(-pow(10,-val));
            }
            if(val > 1) {
                this->draw_isoline(pow(10,val));
            }
        }
        this->draw_isoline(0);
    } else {
        for(float val = this->min; val < this->max; val += binsize) {
            this->draw_isoline(pow(10,val));
        }
        this->draw_isoline(0);
    }
}

void PlaneProjector::draw_isoline(float val) {
    for(unsigned int i=1; i<uint(this->ix-1); i++) {
        for(unsigned int j=1; j<uint(this->iy-1); j++) {
            if(this->is_crossing(i,j,val)) {
                this->plt->draw_filled_rectangle(i,j, 1, 1, Color(0,0,0));
            }
        }
    }
}

bool PlaneProjector::is_crossing(const unsigned int &i, const unsigned int &j, const float &val) {
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

    #pragma omp parallel for collapse(2)
    for(unsigned int i=0; i<nx; i++) {
        for(unsigned int j=0; j<ny; j++) {
            newgrid_log[j * nx + i] = this->planegrid_log[(j + min_y) * this->ix + (i + min_x)];
            newgrid_real[j * nx + i] = this->planegrid_real[(j + min_y) * this->ix + (i + min_x)];
        }
    }

    delete[] this->planegrid_real;
    delete[] this->planegrid_log;

    this->planegrid_real = newgrid_real;
    this->planegrid_log = newgrid_log;
    this->ix = nx;
    this->iy = ny;
}

void PlaneProjector::plot() {
    this->plt = new Plotter(this->ix, this->iy);

    for(unsigned int i=0; i<uint(this->iy); i++) {
        for(unsigned int j=0; j<uint(this->ix); j++) {
            this->plt->draw_filled_rectangle(j,i, 1, 1, this->scheme->get_color(this->planegrid_log[i * this->ix + j]));
        }
    }
}

void PlaneProjector::write(std::string filename) {
    plt->write(filename.c_str());
    std::cout << "Writing " << filename << std::endl;
}

PlaneProjector::~PlaneProjector() {
    delete[] this->planegrid_log;
    delete[] this->planegrid_real;
}
