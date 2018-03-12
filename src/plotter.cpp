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

#include "plotter.h"

Plotter::Plotter(unsigned int _width, unsigned int _height) {
    this->scheme = new ColorScheme(0, 10, 0);

    this->width = _width;
    this->height = _height;

    this->surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, this->width, this->height);
    this->cr = cairo_create (this->surface);

    this->set_background(Color(255, 252, 213, 255));
}

/*
 * Sets the background color of the image
 */
void Plotter::set_background(const Color &_color) {
    cairo_set_source_rgba(this->cr, _color.get_r(), _color.get_g(), _color.get_b(), _color.get_a());
    cairo_rectangle(this->cr, 0, 0, this->width, this->height);
    cairo_fill(this->cr);
}

/*
 * Draws a line. The position xstop and ystop are the ending positions of the line
 * and *not* the direction of the line.
 */
void Plotter::draw_line(float xstart, float ystart, float xstop, float ystop,
                        const Color &_color, float line_width) {
    cairo_set_source_rgba(this->cr, _color.get_r(), _color.get_g(), _color.get_b(), _color.get_a());
    cairo_move_to(this->cr, xstart, ystart);
    cairo_line_to(this->cr, xstop, ystop);
    cairo_set_line_width(this->cr, line_width);
    cairo_stroke(this->cr);
}

/*
 * Create a filled rectangle. That is a rectangle without a border.
 */
void Plotter::draw_filled_rectangle(float xstart, float ystart, float xstop, float ystop,
                                    const Color &_color) {
    cairo_set_source_rgba(this->cr, _color.get_r(), _color.get_g(), _color.get_b(), _color.get_a());
    cairo_rectangle (this->cr, xstart, ystart, xstop, ystop);
    cairo_fill(this->cr);
}

/*
 * Create an empty rectangle. In other words, just the border of the rectangle.
 */
void Plotter::draw_empty_rectangle(float xstart, float ystart, float xstop, float ystop,
                                   const Color &_color, float line_width) {
    cairo_set_source_rgba(this->cr, _color.get_r(), _color.get_g(), _color.get_b(), _color.get_a());
    cairo_rectangle (this->cr, xstart, ystart, xstop, ystop);
    cairo_set_line_width(this->cr, line_width);
    cairo_stroke(this->cr);
}

/*
 * Create a filled circle. That is a circle without a border.
 */
void Plotter::draw_filled_circle(float cx, float cy, float radius,
                                 const Color &_color) {
    cairo_set_source_rgba(this->cr, _color.get_r(), _color.get_g(), _color.get_b(), _color.get_a());
    cairo_arc(this->cr, cx, cy, radius, 0.0, 2 * M_PI);
    cairo_fill(this->cr);
}

/*
 * Create an empty circle. In other words, just the border of the circle.
 */
void Plotter::draw_empty_circle(float cx, float cy, float radius,
                                const Color &_color, float line_width) {
    cairo_set_source_rgba(this->cr, _color.get_r(), _color.get_g(), _color.get_b(), _color.get_a());
    cairo_arc(this->cr, cx, cy, radius, 0.0, 2 * M_PI);
    cairo_set_line_width(this->cr, line_width);
    cairo_stroke(this->cr);
}

void Plotter::write(const char* filename) {
  cairo_surface_write_to_png(this->surface, filename);
}

void Plotter::type(float x, float y, float fontsize, float rotation, const Color &_color, const std::string &_text) {
    cairo_text_extents_t te;
    cairo_set_source_rgba(this->cr, _color.get_r(), _color.get_g(), _color.get_b(), _color.get_a());
    cairo_set_font_size(this->cr, fontsize);
    cairo_text_extents (this->cr, "a", &te);
    cairo_move_to(this->cr, x, y);
    cairo_rotate(this->cr, rotation * M_PI / 180.);
    cairo_show_text (this->cr, _text.c_str());
    cairo_rotate(this->cr, -rotation * M_PI / 180.); // rotate back
}

cairo_text_extents_t Plotter::get_text_bounds(float fontsize, const std::string& text) {
    cairo_text_extents_t te;
    cairo_set_font_size(this->cr, fontsize);
    cairo_text_extents (this->cr, text.c_str(), &te);
    return te;
}

/**************************************************************************
 *                                                                        *
 *   Color scheme and colors                                              *
 *                                                                        *
 **************************************************************************/

/**
 * Constructor
 *
 * Define lower and upper boundary of the color scheme. The colorscheme will
 * generate only colors by interpolation if the requested value to be colorcoded
 * is within the boundaries set here.
 *
 */
ColorScheme::ColorScheme(double _low, double _high, unsigned int scheme_id) {
    this->low = _low;
    this->high = _high;
    this->construct_scheme(scheme_id);
    this->convert_scheme();
}

/**
 *
 * Construct a colorscheme. The color values are here hardcoded and extracted from:
 * http://colorbrewer2.org/
 *
 */
void ColorScheme::construct_scheme(unsigned int scheme_id) {
    switch(scheme_id) {
        case 0: // http://colorbrewer2.org/#type=diverging&scheme=RdBu&n=11
            this->scheme.push_back("053061FF");
            this->scheme.push_back("2166acFF");
            this->scheme.push_back("4393c3FF");
            this->scheme.push_back("92c5deFF");
            this->scheme.push_back("d1e5f0FF");
            this->scheme.push_back("f7f7f7FF");
            this->scheme.push_back("fddbc7FF");
            this->scheme.push_back("f4a582FF");
            this->scheme.push_back("d6604dFF");
            this->scheme.push_back("b2182bFF");
            this->scheme.push_back("67001fFF");
        break;
        case 1: // http://colorbrewer2.org/#type=sequential&scheme=YlGnBu&n=9
            this->scheme.push_back("ffffd9FF");
            this->scheme.push_back("edf8b1FF");
            this->scheme.push_back("c7e9b4FF");
            this->scheme.push_back("7fcdbbFF");
            this->scheme.push_back("41b6c4FF");
            this->scheme.push_back("1d91c0FF");
            this->scheme.push_back("225ea8FF");
            this->scheme.push_back("253494FF");
            this->scheme.push_back("081d58FF");
        break;
        case 2: // http://colorbrewer2.org/#type=diverging&scheme=BrBG&n=11
            this->scheme.push_back("543005FF");
            this->scheme.push_back("8c510aFF");
            this->scheme.push_back("bf812dFF");
            this->scheme.push_back("dfc27dFF");
            this->scheme.push_back("f6e8c3FF");
            this->scheme.push_back("f5f5f5FF");
            this->scheme.push_back("c7eae5FF");
            this->scheme.push_back("80cdc1FF");
            this->scheme.push_back("35978fFF");
            this->scheme.push_back("01665eFF");
            this->scheme.push_back("003c30FF");
        break;
        case 3: // http://colorbrewer2.org/#type=diverging&scheme=PiYG&n=11
            this->scheme.push_back("8e0152FF");
            this->scheme.push_back("c51b7dFF");
            this->scheme.push_back("de77aeFF");
            this->scheme.push_back("f1b6daFF");
            this->scheme.push_back("fde0efFF");
            this->scheme.push_back("f7f7f7FF");
            this->scheme.push_back("e6f5d0FF");
            this->scheme.push_back("b8e186FF");
            this->scheme.push_back("7fbc41FF");
            this->scheme.push_back("4d9221FF");
            this->scheme.push_back("276419FF");
        break;
        case 4: // http://colorbrewer2.org/#type=diverging&scheme=RdYlGn&n=11
            this->scheme.push_back("a50026FF");
            this->scheme.push_back("d73027FF");
            this->scheme.push_back("f46d43FF");
            this->scheme.push_back("fdae61FF");
            this->scheme.push_back("fdae61FF");
            this->scheme.push_back("ffffbfFF");
            this->scheme.push_back("d9ef8bFF");
            this->scheme.push_back("a6d96aFF");
            this->scheme.push_back("66bd63FF");
            this->scheme.push_back("1a9850FF");
            this->scheme.push_back("006837FF");
        break;
        case 5: // http://colorbrewer2.org/#type=sequential&scheme=PuBuGn&n=9
            this->scheme.push_back("fff7fbFF");
            this->scheme.push_back("ece2f0FF");
            this->scheme.push_back("d0d1e6FF");
            this->scheme.push_back("a6bddbFF");
            this->scheme.push_back("67a9cfFF");
            this->scheme.push_back("3690c0FF");
            this->scheme.push_back("02818aFF");
            this->scheme.push_back("016c59FF");
            this->scheme.push_back("014636FF");
        break;
        case 6: http://colorbrewer2.org/#type=sequential&scheme=RdPu&n=9
            this->scheme.push_back("fff7f3FF");
            this->scheme.push_back("fde0ddFF");
            this->scheme.push_back("fcc5c0FF");
            this->scheme.push_back("fa9fb5FF");
            this->scheme.push_back("f768a1FF");
            this->scheme.push_back("dd3497FF");
            this->scheme.push_back("ae017eFF");
            this->scheme.push_back("7a0177FF");
            this->scheme.push_back("49006aFF");
        break;
        case 7: // http://tristen.ca/hcl-picker/#/hlc/6/1.05/CAF270/453B52
            this->scheme.push_back("CAF270FF");
            this->scheme.push_back("73D487FF");
            this->scheme.push_back("30B097FF");
            this->scheme.push_back("288993FF");
            this->scheme.push_back("41607AFF");
            this->scheme.push_back("453B52FF");
        break;
        case 8: // http://tristen.ca/hcl-picker/#/hlc/6/1.05/80C5F4/411D1E
            this->scheme.push_back("80C5F4FF");
            this->scheme.push_back("929ACBFF");
            this->scheme.push_back("92729CFF");
            this->scheme.push_back("824F6BFF");
            this->scheme.push_back("653340FF");
            this->scheme.push_back("411D1EFF");
        break;
        case 9: // http://tristen.ca/hcl-picker/#/hlc/6/1/21313E/EFEE69
            this->scheme.push_back("21313EFF");
            this->scheme.push_back("20575FFF");
            this->scheme.push_back("268073FF");
            this->scheme.push_back("53A976FF");
            this->scheme.push_back("98CF6FFF");
            this->scheme.push_back("EFEE69FF");
        break;
        case 10: // http://tristen.ca/hcl-picker/#/clh/6/253/134695/D8C8BC
            this->scheme.push_back("D8C8BCFF");
            this->scheme.push_back("B4ABC5FF");
            this->scheme.push_back("8F90C5FF");
            this->scheme.push_back("6876BDFF");
            this->scheme.push_back("415DADFF");
            this->scheme.push_back("134695FF");
        break;
        case 11: // http://tristen.ca/hcl-picker/#/clh/6/253/134695/D8C8BC
            this->scheme.push_back("134695FF");
            this->scheme.push_back("415DADFF");
            this->scheme.push_back("6876BDFF");
            this->scheme.push_back("8F90C5FF");
            this->scheme.push_back("B4ABC5FF");
            this->scheme.push_back("D8C8BCFF");
        break;
        case 12: // http://gka.github.io/palettes/#diverging|c0=darkred,deeppink,lightyellow|c1=lightyellow,lightgreen,teal|steps=13|bez0=1|bez1=1|coL0=1|coL1=1
            this->scheme.push_back("008080FF");
            this->scheme.push_back("399785FF");
            this->scheme.push_back("5aaf8cFF");
            this->scheme.push_back("7ac696FF");
            this->scheme.push_back("9edba4FF");
            this->scheme.push_back("c7f0baFF");
            this->scheme.push_back("ffffe0FF");
            this->scheme.push_back("ffd1c9FF");
            this->scheme.push_back("fea0acFF");
            this->scheme.push_back("ef738bFF");
            this->scheme.push_back("d84765FF");
            this->scheme.push_back("b61d39FF");
            this->scheme.push_back("8b0000FF");
        break;
        case 13: // http://gka.github.io/palettes/#diverging|c0=darkred,lightyellow|c1=lightyellow,lightgreen,teal|steps=13|bez0=1|bez1=1|coL0=1|coL1=1
            this->scheme.push_back("008080FF");
            this->scheme.push_back("399785FF");
            this->scheme.push_back("5aaf8cFF");
            this->scheme.push_back("7ac696FF");
            this->scheme.push_back("9edba4FF");
            this->scheme.push_back("c7f0baFF");
            this->scheme.push_back("ffffe0FF");
            this->scheme.push_back("f1d7b7FF");
            this->scheme.push_back("e2b08fFF");
            this->scheme.push_back("cf8a69FF");
            this->scheme.push_back("bb6345FF");
            this->scheme.push_back("a43c23FF");
            this->scheme.push_back("8b0000FF");
        break;
        case 14: // http://paletton.com/#uid=52T0M0kkJlXa9xzfrrfq5gAvubc
            this->scheme.push_back("96A93CFF");
            this->scheme.push_back("2F8441FF");
            this->scheme.push_back("2F4C73FF");;
        break;
        case 15: // black and white
            this->scheme.push_back("000000FF");
            this->scheme.push_back("FFFFFFFF");
        break;
        default:
            this->scheme.push_back("053061FF");
            this->scheme.push_back("2166acFF");
            this->scheme.push_back("4393c3FF");
            this->scheme.push_back("92c5deFF");
            this->scheme.push_back("d1e5f0FF");
            this->scheme.push_back("f7f7f7FF");
            this->scheme.push_back("fddbc7FF");
            this->scheme.push_back("f4a582FF");
            this->scheme.push_back("d6604dFF");
            this->scheme.push_back("b2182bFF");
            this->scheme.push_back("67001fFF");
        break;
    }
}

/**
 *
 * Convert the colorscheme set in hexidecimal RGB to integer RGB
 *
 */
void ColorScheme::convert_scheme() {
    for(unsigned int i=0; i<this->scheme.size(); i++) {
        this->colors.push_back(this->rgb2color(this->scheme[i]));
    }
}

/**
 *
 * Convert string of hexidecimal RGB values to three integers
 *
 */
Color ColorScheme::rgb2color(const std::string &_hex) {
    if(_hex.size() == 6) {
        return Color(this->hex2int(_hex.substr(0,2)),
                     this->hex2int(_hex.substr(2,2)),
                     this->hex2int(_hex.substr(4,2)),
                     255);
    } else if(_hex.size() == 8) {
        return Color(this->hex2int(_hex.substr(0,2)),
                     this->hex2int(_hex.substr(2,2)),
                     this->hex2int(_hex.substr(4,2)),
                     this->hex2int(_hex.substr(6,2)));
    } else {
        throw std::runtime_error("Invalid hex color code received: " + _hex);
    }
}

/**
 *
 * Return a color by interpolation by supplying a value
 *
 */
Color ColorScheme::get_color(double _value) {

    if(_value > this->high) {
        return this->colors.back();
    }
    if(_value < this->low) {
        return this->colors.front();
    }

    float binsize = ((this->high - this->low)/(double)(this->colors.size()-1));
    unsigned int bin = floor((_value - this->low) / binsize);

    // interpolate between the two colors
    float residual = (_value - this->low - (float)bin * binsize) / binsize;

    float r = residual * this->colors[bin+1].get_r() + (1.0-residual) * this->colors[bin].get_r();
    float g = residual * this->colors[bin+1].get_g() + (1.0-residual) * this->colors[bin].get_g();
    float b = residual * this->colors[bin+1].get_b() + (1.0-residual) * this->colors[bin].get_b();
    float a = residual * this->colors[bin+1].get_a() + (1.0-residual) * this->colors[bin].get_a();

    return Color(r*255, g*255, b*255, a*255);
}

/**
 *
 * Convert a single hexidecimal value to an integer
 *
 */
unsigned int ColorScheme::hex2int(const std::string &_hex) {
    char* offset;
    if(_hex.length() > 2) {
        if(_hex[0] == '0' && _hex[1] == 'x') {
            return strtol(_hex.c_str(), &offset, 0);
        }
    }
    return strtol(_hex.c_str(), &offset, 16);
}

/**
 *
 * Color constructor
 *
 */
Color::Color(unsigned int _r, unsigned int _g, unsigned int _b) :
    r(_r),
    g(_g),
    b(_b),
    a(255) {}

/**
 *
 * Color constructor
 *
 */
Color::Color(unsigned int _r, unsigned int _g, unsigned int _b, unsigned int _a) :
    r(_r),
    g(_g),
    b(_b),
    a(_a) {}

/**
 *
 * Return the integer value for red (divide by 255 because we want on the [0,1] interval)
 *
 */
float Color::get_r() const {
    return this->r / 255.0f;
}

/**
 *
 * Return the integer value for green (divide by 255 because we want on the [0,1] interval)
 *
 */
float Color::get_g() const {
    return this->g / 255.0f;
}

/**
 *
 * Return the integer value for blue (divide by 255 because we want on the [0,1] interval)
 *
 */
float Color::get_b() const {
    return this->b / 255.0f;
}

/**
 *
 * Return the integer value for alpha (divide by 255 because we want on the [0,1] interval)
 *
 */
float Color::get_a() const {
    return this->a / 255.0f;
}
