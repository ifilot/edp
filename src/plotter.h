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

#ifndef _PLOTTER_H
#define _PLOTTER_H

#include <cairo/cairo.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <math.h>

class Color {
private:
    unsigned int r,g,b,a;
public:
    Color(unsigned int _r, unsigned int _g, unsigned int _b);

    Color(unsigned int _r, unsigned int _g, unsigned int _b, unsigned int _a);
    float get_r() const;
    float get_g() const;
    float get_b() const;
    float get_a() const;
};

class ColorScheme {
private:
    std::vector<std::string> scheme;
    std::vector<Color> colors;
    double low, high;
public:
    ColorScheme(double _low, double _high, unsigned int scheme_id);
    Color get_color(double _value);
private:
    void construct_scheme(unsigned int scheme_id);
    void convert_scheme();
    Color rgb2color(const std::string& _hex);
    unsigned int hex2int(const std::string& _hex);
};


class Plotter {
private:
  cairo_t *cr;
  cairo_surface_t *surface;
  unsigned int width, height;
  ColorScheme *scheme;

public:
  Plotter(const unsigned int _width, const unsigned int _height);
  void set_background(const Color &_color);
  void write(const char* filename);
  void draw_filled_rectangle(float xstart, float ystart, float xstop, float ystop,
                             const Color &_color);
  void draw_empty_rectangle(float xstart, float ystart, float xstop, float ystop,
                            const Color &_color, float line_width);
  void draw_line(float xstart, float ystart, float xstop, float ystop,
                 const Color &_color, float line_width);
  void draw_filled_circle(float cx, float cy, float radius,
                          const Color &_color);
  void draw_empty_circle(float cx, float cy, float radius,
                         const Color &_color, float line_width);

  cairo_text_extents_t get_text_bounds(float fontsize, const std::string& text);

  void type(float x, float y, float fontsize, float rotation, const Color &_color, const std::string &_text);

  inline auto get_cairo_ptr() {
    return this->cr;
  }

  ~Plotter();

private:

};

#endif //_PLOTTER_H
