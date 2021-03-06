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

#ifndef _RAY_CASTER_H
#define _RAY_CASTER_H

#include <memory>
#include <array>
#include <glm/glm.hpp>

#include "plotter.h"
#include "scalar_field.h"

class RayCaster {
private:
    ScalarField* sf;
    std::unique_ptr<Plotter> plt;
    ColorScheme* scheme;

    int ix = 512;
    int iy = 512;

    float minval, maxval;

    // whether to sample front to back or back to front
    bool front_to_back = false;

    // implement density scaling
    float density_scaling = 0.5f;
    unsigned int raysamples = 16;

    unsigned int color_scheme_id;

    // store colors
    std::vector<std::array<float, 4>> pixels;

public:
    /**
     * @brief      Constructs the object.
     *
     * @param      _sf               Pointer to scalar field
     * @param[in]  _color_scheme_id  The color scheme identifier
     */
    RayCaster(ScalarField* _sf, unsigned int _color_scheme_id);

    /**
     * @brief      Perform volumetric ray casting
     */
    void cast();

    /**
     * @brief      Write to png file
     *
     * @param[in]  filename  The filename
     */
    void write(const std::string& filename);

private:
    /**
     * @brief      Calculate the normal vector at some point
     *
     * @param[in]  x     x coordinate
     * @param[in]  y     y coordinate
     * @param[in]  z     z coordinate
     *
     * @return     The normal.
     */
    glm::vec3 calculate_normal(float x, float y, float z) const;

};

#endif // _RAY_CASTER_H
