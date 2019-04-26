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

#include "raytracer.h"

/**
 * @brief      constructor
 *
 * @param      _sf              pointer to ScalarField
 * @param[in]  color_scheme_id  The color scheme identifier
 */
RayTracer::RayTracer(ScalarField* _sf, unsigned int _color_scheme_id) {
    this->sf = _sf;
    this->color_scheme_id = _color_scheme_id;

    this->minval = 0;
    this->maxval = this->sf->get_max();

    this->scheme = new ColorScheme(0, 1, this->color_scheme_id);
}

void RayTracer::trace() {
    this->plt = std::make_unique<Plotter>(this->ix, this->iy);
    unsigned int samples = 10;

    const auto& unitcell = this->sf->get_mat_unitcell();

    for(int y=0; y<this->iy; y++) {
        float yy = (float)y/(float)this->iy * unitcell[2][2];
        for(int x=0; x<this->ix; x++) {
            float xx = (float)x /(float)this->ix * unitcell[0][0];

            float sum = 0.0f;
            float cum_alpha = 0;
            std::array<float, 3> color = {0.0f, 0.0f, 0.0f};

            for(unsigned int d=0; d<samples; d++) {
                float dd = (float)d /(float)samples * unitcell[1][1];
                float alpha = (std::fabs(this->sf->get_value_interp(xx, dd, yy)) - this->minval) / (this->maxval - this->minval);
                auto col = this->scheme->get_color(alpha);
                cum_alpha += alpha;
                color[0] += alpha * col.get_r() * 255.0f;
                color[1] += alpha * col.get_g() * 255.0f;
                color[2] += alpha * col.get_b() * 255.0f;

                if(cum_alpha > 0.95) {
                    break;
                }
            }

            this->plt->draw_filled_rectangle(x, y, 1, 1, Color(color[0], color[1], color[2]));
        }
    }

    this->plt->write("test.png");
}
