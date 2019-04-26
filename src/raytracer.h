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

#ifndef _RAYTRACER_H
#define _RAYTRACER_H

#include <memory>

#include "plotter.h"
#include "scalar_field.h"

class RayTracer {
private:
    ScalarField* sf;
    std::unique_ptr<Plotter> plt;
    ColorScheme* scheme;

    int ix = 1024;
    int iy = 1024;

    float minval, maxval;

    unsigned int color_scheme_id;

public:
    RayTracer(ScalarField* _sf, unsigned int _color_scheme_id);

    void trace();

private:

};

#endif // _RAYTRACER_H
