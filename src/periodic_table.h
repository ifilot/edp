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

#ifndef _PERIODIC_TABLE_H
#define _PERIODIC_TABLE_H

#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

class PeriodicTable {
private:
    class Atom {
    public:
        std::string label;
        unsigned int elnr;
        double mass;
        double radius;

        Atom();
    };

    std::unordered_map<std::string,Atom> map_atoms;
    std::vector<Atom> atoms;
    std::vector<std::vector<double>> bond_distances;

public:
    static PeriodicTable& get() {
        static PeriodicTable periodic_table_instance;
        return periodic_table_instance;
    }

    unsigned int get_elnr(const std::string& name) const;

    inline const std::string& get_label_elnr(unsigned int elnr) const {
        return this->atoms[elnr].label;
    }

    std::string get_label_elnr_string(unsigned int elnr) const;

    inline double get_radius_elnr(unsigned int elnr) const {
        return this->atoms[elnr].radius;
    }

    inline double get_bond_distance(unsigned int at1, unsigned int at2) const {
        return this->bond_distances[at1][at2];
    }

private:
    PeriodicTable();

    void add_atom(const std::string& symbol, const Atom& atom);

    void add_atom(unsigned int elnr,
                  const std::string& label,
                  double mass,
                  double radius);

    PeriodicTable(PeriodicTable const&)   = delete;
    void operator=(PeriodicTable const&)  = delete;
};

#endif // _PERIODIC_TABLE_H
