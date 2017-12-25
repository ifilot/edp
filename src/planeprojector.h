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

#ifndef _PLANEPROJECTOR_H
#define _PLANEPROJECTOR_H

#include <algorithm>
#include "plotter.h"
#include "mathtools.h"
#include "scalar_field.h"

class PlaneProjector {
private:
    ColorScheme* scheme;
    ScalarField* sf;
    Plotter* plt;

    float* planegrid_log;
    float* planegrid_real;
    float min, max;

    int ix, iy;
public:
    PlaneProjector(ScalarField* _sf, float _min, float _max);
    void extract(Vector _v1, Vector _v2, Vector _s, float _scale, float li, float hi, float lj, float hj, bool negative_values);
    void plot();
    void isolines(unsigned int bins, bool negative_values);
    void write(std::string filename);
    ~PlaneProjector();
private:
    void cut_and_recast_plane();
    void draw_isoline(float val);
    bool is_crossing(const unsigned int &i, const unsigned int &j, const float &val);
};

#endif
