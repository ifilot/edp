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

#ifndef _COLOR_SCHEME_H
#define _COLOR_SCHEME_H

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <math.h>

namespace colorscheme {

class Color {
private:
    unsigned int r,g,b;
public:
    Color(unsigned int _r, unsigned int _g, unsigned int _b);
    float get_r() const;
    float get_g() const;
    float get_b() const;
};

class ColorScheme {
private:
    std::vector<std::string> scheme;
    std::vector<Color> colors;
    double low, high;

public:
    ColorScheme(const double &_low, const double &_high);
    Color get_color(const double &_value);

private:
    void construct_scheme();
    void convert_scheme();
    Color rgb2color(const std::string &_hex);
    unsigned int hex2int(const std::string &_hex);
};

}

#endif //_COLOR_SCHEME_H
