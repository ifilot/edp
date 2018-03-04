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

#ifndef _PLANEPROJECTOR_H
#define _PLANEPROJECTOR_H

#include <algorithm>
#include <boost/format.hpp>

#include "plotter.h"
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
    float scale;
    unsigned int color_scheme_id;

public:

    /**
     * @brief      constructor
     *
     * @param      _sf              pointer to ScalarField
     * @param[in]  _min             minimum value
     * @param[in]  _max             maximum value
     * @param[in]  color_scheme_id  The color scheme identifier
     */
    PlaneProjector(ScalarField* _sf, float _min, float _max, unsigned int _color_scheme_id);

    /**
     * @brief      plot contour plane
     */
    void plot();

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
     * @param[in]  negative_values  whether there are negative values in the plot
     */
    void extract(glm::vec3 _v1, glm::vec3 _v2, const glm::vec3& _p, float _scale, float li, float hi, float lj, float hj, bool negative_values);

    /**
     * @brief      extract line
     *
     * @param[in]  e       vector direction
     * @param[in]  p       position vector
     * @param[in]  _scale  scale
     * @param[in]  li      extend in -e direction
     * @param[in]  hi      extend in +e direction
     */
    void extract_line(glm::vec3 e, const glm::vec3& p, float _scale, float li, float hi);

    /**
     * @brief      draw isolines
     *
     * @param[in]  bins             number of bins
     * @param[in]  negative_values  whether there are negative values in the plot
     */
    void isolines(unsigned int bins, bool negative_values);

    /**
     * @brief      Draws a legend.
     *
     * @param[in]  negative_values  whether there are negative values in the plot
     */
    void draw_legend(bool negative_values);

    /**
     * @brief      write contour plane to file
     *
     * @param[in]  filename  path to png file
     */
    void write(std::string filename);

    /**
     * @brief      Destroys the object.
     */
    ~PlaneProjector();
private:

    /**
     * @brief      reposition plane within the boundaries of the unit cell
     */
    void cut_and_recast_plane();

    /**
     * @brief      draw a single isoline
     *
     * @param[in]  val   value of the isoline
     */
    void draw_isoline(float val);

    /**
     * @brief      determines whether a value on the pixel the isovalue is crossing
     *
     * @param[in]  i     index i on contour plane
     * @param[in]  j     index j on contour plane
     * @param[in]  val   isovalue
     *
     * @return     True if crossing, False otherwise.
     */
    bool is_crossing(unsigned int i, unsigned int j, float val);
};

#endif
