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

#ifndef _MATHTOOLS_H
#define _MATHTOOLS_H

#include <cmath>

/*
 * Vector class in 3D space
 */
class Vector {
private:
  float r[3];

public:
    Vector();
    Vector(float x, float y, float z);
    float operator[](const unsigned int &i);
    const float& operator[](const unsigned int &i) const;
    float length();
    void normalize();

private:
};

/*
 * 3x3 Matrix class
 */
class Matrix {
private:
    float* r[3];

public:
    Matrix();
    ~Matrix();
    void inverse();
    float* operator[](const unsigned int &i);
    const float* operator[](const unsigned int &i) const;

private:
};

/*
 * Plane class
 */
class Plane {
private:
    Vector origin; // origin of the plane
    Vector normal; // normal vector

public:
    Plane(const Vector &r, const Vector &n);

private:
    void parametrize();
};

#endif //_MATHTOOLS_H
