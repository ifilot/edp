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
#include "quadrature.h"

class PlaneProjector {
private:
    ColorScheme* scheme;
    ScalarField* sf;
    Plotter* plt;

    float* planegrid_log;
    float* planegrid_real;
    bool* planegrid_box;
    float log_min, log_max;

    int ix, iy;
    float scale;
    unsigned int color_scheme_id;

    bool flag_negative;

public:

    /**
     * @brief      constructor
     *
     * @param      _sf              pointer to ScalarField
     * @param[in]  _min             minimum value
     * @param[in]  _max             maximum value
     * @param[in]  color_scheme_id  The color scheme identifier
     */
    PlaneProjector(ScalarField* _sf, unsigned int _color_scheme_id);

    /**
     * Get a pointer to the planegrid container
     */
    const float* get_planegrid_real() const {
        return this->planegrid_real;
    }

    /**
     * Get the dimensions of the plane
     */
    std::pair<uint,uint> get_dimensions() const {
        return std::make_pair<uint,uint>(this->ix, this->iy);
    }

    /**
     * @brief      set the scaling for the graph
     *
     * @param[in]  allow_negative  whether to allow negative values
     * @param[in]  _min            minimum values
     * @param[in]  _max            maximum values
     */
    void set_scaling(bool allow_negative, float _min, float _max);

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
    void extract(glm::vec3 _v1, glm::vec3 _v2, const glm::vec3& _p, float _scale, float li, float hi, float lj, float hj);

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
     * @brief      calculate the average density (electron or potential) and store it as function of z-height
     */
    void extract_plane_average();

    /**
     * @brief      calculate the average density projected on a sphere of a
     *             specified radius
     *
     * @param[in]  p       position of the sphere
     * @param[in]  radius  radius of the sphere
     */
    void extract_sphere_average(const glm::vec3& p, float radius);

    /**
     * @brief      draw isolines
     *
     * @param[in]  bins             number of bins
     * @param[in]  negative_values  whether there are negative values in the plot
     */
    void isolines(unsigned int bins);

    /**
     * @brief      Draws a legend.
     *
     * @param[in]  negative_values  whether there are negative values in the plot
     */
    void draw_legend();

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

    /**
     * @brief      Calculates the scaled value using a logarithmic scale.
     *
     * @param[in]  input  input value
     *
     * @return     The scaled value.
     */
    float calculate_scaled_value_log(float input);

    void store_field(const std::string& filename, float* field, uint32_t nx, uint32_t ny);

    void store_field_uin8t(const std::string& filename, uint8_t* field, uint32_t nx, uint32_t ny);
};

/**
 * @brief      get sign
 *
 * @param[in]  val   input value
 *
 * @tparam     T     sign type
 *
 * @return     sign of value
 */
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

#endif
