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

#include "mathtools.h"

/*
 * Default vector constructor
 */
Vector::Vector() {
    this->r[0] = 0;
    this->r[1] = 0;
    this->r[2] = 0;
}

/*
 * Specific vector constructor
 */
Vector::Vector(float x, float y, float z) {
    this->r[0] = x;
    this->r[1] = y;
    this->r[2] = z;
}

/*
 * Allocator method to place a value in the Vector
 */
float Vector::operator[](const unsigned int &i) {
    return this->r[i];
}

/*
 * Allocator method to grab a value from the Vector
 */
const float& Vector::operator[](const unsigned int &i) const {
    return this->r[i];
}

float Vector::length() {
    return sqrt(this->r[0] * this->r[0] +
                this->r[1] * this->r[1] +
                this->r[2] * this->r[2]);
}

void Vector::normalize() {
    float length = this->length();
    this->r[0] /= length;
    this->r[1] /= length;
    this->r[2] /= length;
}

/*
 * Default matrix constructor
 */
Matrix::Matrix() {
    for(unsigned int i=0; i<3; i++) {
        this->r[i] = new float[3];
        for(unsigned int j=0; j<3; j++) {
            this->r[i][j] = 0.0f;
        }
    }
}

/*
 * Matrix destructor
 */
Matrix::~Matrix() {
    for(unsigned int i=0; i<3; i++) {
        delete[] this->r[i];
    }
}

/*
 * Allocator method to place a value in the matrix
 */
float* Matrix::operator[](const unsigned int &i) {
    return this->r[i];
}

/*
 * Allocator method to grab a value from the matrix
 */
const float* Matrix::operator[](const unsigned int &i) const {
    return this->r[i];
}

Plane::Plane(const Vector &r, const Vector &n) {
    this->origin = r;
    this->normal = n;
}

void Plane::parametrize() {

}
