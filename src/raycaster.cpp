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

#include "raycaster.h"

/**
 * @brief      constructor
 *
 * @param      _sf              pointer to ScalarField
 * @param[in]  color_scheme_id  The color scheme identifier
 */
RayCaster::RayCaster(ScalarField* _sf, unsigned int _color_scheme_id) {
    this->sf = _sf;
    this->color_scheme_id = _color_scheme_id;

    this->minval = 0;
    this->maxval = std::log10(1.0f + this->sf->get_max());

    this->scheme = new ColorScheme(0, 1, this->color_scheme_id);
}

void RayCaster::cast() {
    this->plt = std::make_unique<Plotter>(this->ix, this->iy);

    // set background
    this->plt->draw_filled_rectangle(0, 0, this->ix, this->iy, this->scheme->get_color(0));

    // obtain unit cell dimensions
    const auto& unitcell = this->sf->get_mat_unitcell();

    // expand pixel canvas
    this->pixels.resize(this->iy * this->ix);

    // settings
    const auto light = glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f));
    const auto eye = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));

    #pragma omp parallel for
    for(int y=0; y<this->iy; y++) {
        float yy = (float)y/(float)this->iy * unitcell[2][2];
        for(int x=0; x<this->ix; x++) {
            float xx = (float)x /(float)this->ix * unitcell[0][0];

            float sum = 0.0f;
            float cum_alpha = 0.0f;
            std::array<float, 4> color = {0.0f, 0.0f, 0.0f, 0.0f};

            for(unsigned int d=0; d<this->raysamples; d++) {
                // get intensity and density
                float dd = (float)d /(float)this->raysamples * unitcell[1][1];
                float val = std::log10(1.0 + std::fabs(this->sf->get_value_interp(xx, dd, yy)));

                // calculate alpha value
                float alpha = (val - this->minval) / (this->maxval - this->minval);
                color[3] = std::pow(alpha, density_scaling);

                // calculate base color
                auto col = this->scheme->get_color(color[3]);

                color[0] += color[3] * col.get_r();
                color[1] += color[3] * col.get_g();
                color[2] += color[3] * col.get_b();

                // accumulate alpha
                cum_alpha += color[3];

                if(this->front_to_back) {
                    if(cum_alpha >= 1.0f) {
                        break;
                    }
                }
            }

            if(!this->front_to_back) {
                color[0] /= cum_alpha;
                color[1] /= cum_alpha;
                color[2] /= cum_alpha;
                color[3] = std::max(color[3], 1.0f);
            }

            for(unsigned int k=0; k<4; k++) {
                this->pixels[y * this->ix + x][k] = color[k];
            }
        }
    }
}

/**
 * @brief      Write to png file
 *
 * @param[in]  filename  The filename
 */
void RayCaster::write(const std::string& filename) {
    for(int y=0; y<this->iy; y++) {
        for(int x=0; x<this->ix; x++) {
            this->plt->draw_filled_rectangle(x, y, 1, 1, Color(this->pixels[y * this->ix + x]));
        }
    }

    this->plt->write(filename.c_str());
    this->plt.release();
}

/**
 * @brief      Calculate the normal vector at some point
 *
 * @param[in]  x     x coordinate
 * @param[in]  y     y coordinate
 * @param[in]  z     z coordinate
 *
 * @return     The normal.
 */
glm::vec3 RayCaster::calculate_normal(float x, float y, float z) const {
    static const float dev = 0.05f;

    double dx0 = sf->get_value_interp(x - dev, y, z);
    double dx1 = sf->get_value_interp(x + dev, y, z);

    double dy0 = sf->get_value_interp(x, y - dev, z);
    double dy1 = sf->get_value_interp(x, y + dev, z);

    double dz0 = sf->get_value_interp(x, y, z - dev);
    double dz1 = sf->get_value_interp(x, y, z + dev);

    glm::vec3 normal((dx1 - dx0) / (2.0 * dev),
                     (dy1 - dy0) / (2.0 * dev),
                     (dz1 - dz0) / (2.0 * dev));

    return glm::normalize(normal);
}
