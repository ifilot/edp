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

#include "periodic_table.h"

PeriodicTable::Atom::Atom() {
    this->elnr = 0;
    this->mass = 0;
    this->label = "X";
}

PeriodicTable::PeriodicTable() {
    // row 1
    this->add_atom(0,  "X",   0.000, 0.10);
    this->add_atom(1,  "H",   1.008, 0.25);
    this->add_atom(2,  "He",  4.003, 0.40);

    // row 2
    this->add_atom(3,  "Li",  6.940, 0.40);
    this->add_atom(4,  "Be",  9.012, 0.40);
    this->add_atom(5,  "B",   10.81, 0.40);
    this->add_atom(6,  "C",  12.010, 0.40);
    this->add_atom(7,  "N",  14.010, 0.40);
    this->add_atom(8,  "O",  16.000, 0.40);
    this->add_atom(9,  "F",  19.000, 0.40);
    this->add_atom(10, "Ne", 20.180, 0.40);

    // row 3
    this->add_atom(11, "Na", 22.990, 0.40);
    this->add_atom(12, "Mg", 24.310, 0.40);
    this->add_atom(13, "Al", 26.980, 0.40);
    this->add_atom(14, "Si", 28.090, 0.40);
    this->add_atom(15, "P",  30.970, 0.40);
    this->add_atom(16, "S",  16.000, 0.60);
    this->add_atom(17, "Cl", 35.450, 0.60);
    this->add_atom(18, "Ar", 39.950, 0.60);

    // row 4
    this->add_atom(19, "K",  39.100, 1.20);
    this->add_atom(20, "Ca", 40.080, 1.20);
    this->add_atom(21, "Sc", 44.960, 1.20);
    this->add_atom(22, "Ti", 47.880, 1.20);
    this->add_atom(23, "V",  50.940, 1.20);
    this->add_atom(24, "Cr", 52.000, 1.20);
    this->add_atom(25, "Mn", 54.940, 1.20);
    this->add_atom(26, "Fe", 55.850, 1.20);
    this->add_atom(27, "Co", 58.930, 1.20);
    this->add_atom(28, "Ni", 58.690, 1.20);
    this->add_atom(29, "Cu", 63.550, 1.20);
    this->add_atom(30, "Zn", 65.390, 1.20);
    this->add_atom(31, "Ga", 69.720, 1.20);
    this->add_atom(32, "Ge", 72.640, 1.20);
    this->add_atom(33, "As", 74.920, 1.20);
    this->add_atom(34, "Se", 78.960, 1.20);
    this->add_atom(35, "Br", 79.900, 1.20);
    this->add_atom(36, "Kr", 83.790, 1.20);

    // row 5
    this->add_atom(37, "Rb",  0.000, 1.20);
    this->add_atom(38, "Sr",  0.000, 1.20);
    this->add_atom(39, "Y",   0.000, 1.20);
    this->add_atom(40, "Zr",  0.000, 1.20);
    this->add_atom(41, "Nb",  0.000, 1.20);
    this->add_atom(42, "Mo",  0.000, 1.20);
    this->add_atom(43, "Tc",  0.000, 1.20);
    this->add_atom(44, "Ru",  0.000, 1.20);
    this->add_atom(45, "Rh",  0.000, 1.20);
    this->add_atom(46, "Pd",  0.000, 1.20);
    this->add_atom(47, "Ag",  0.000, 1.20);
    this->add_atom(48, "Cd",  0.000, 1.20);
    this->add_atom(49, "In",  0.000, 1.20);
    this->add_atom(50, "Sn",  0.000, 1.20);
    this->add_atom(51, "Sb",  0.000, 1.20);
    this->add_atom(52, "Te",  0.000, 1.20);
    this->add_atom(53, "I",   0.000, 1.20);
    this->add_atom(54, "Xe",  0.000, 1.20);

    // row 6
    this->add_atom(55, "Cs",  0.000, 1.20);
    this->add_atom(56, "Ba",  0.000, 1.20);
    this->add_atom(57, "La",  0.000, 1.20);
    this->add_atom(58, "Ce",  0.000, 1.20);
    this->add_atom(59, "Pr",  0.000, 1.20);
    this->add_atom(60, "Nd",  0.000, 1.20);
    this->add_atom(61, "Pm",  0.000, 1.20);
    this->add_atom(62, "Sm",  0.000, 1.20);
    this->add_atom(63, "Eu",  0.000, 1.20);
    this->add_atom(64, "Gd",  0.000, 1.20);
    this->add_atom(65, "Tb",  0.000, 1.20);
    this->add_atom(66, "Dy",  0.000, 1.20);
    this->add_atom(67, "Ho",  0.000, 1.20);
    this->add_atom(68, "Er",  0.000, 1.20);
    this->add_atom(69, "Tm",  0.000, 1.20);
    this->add_atom(70, "Yb",  0.000, 1.20);
    this->add_atom(71, "Lu",  0.000, 1.20);
    this->add_atom(72, "Hf",  0.000, 1.20);
    this->add_atom(73, "Ta",  0.000, 1.20);
    this->add_atom(74, "W",   0.000, 1.20);
    this->add_atom(75, "Re",  0.000, 1.20);
    this->add_atom(76, "Os",  0.000, 1.20);
    this->add_atom(77, "Ir",  0.000, 1.20);
    this->add_atom(78, "Pt",  0.000, 1.20);
    this->add_atom(79, "Au",  0.000, 1.20);
    this->add_atom(80, "Hg",  0.000, 1.20);
    this->add_atom(81, "Tl",  0.000, 1.20);
    this->add_atom(82, "Pb",  0.000, 1.20);
    this->add_atom(83, "Bi",  0.000, 1.20);
    this->add_atom(84, "Po",  0.000, 1.20);
    this->add_atom(85, "At",  0.000, 1.20);
    this->add_atom(86, "Rn",  0.000, 1.20);

    // row 7
    this->add_atom(87, "Fr",  0.000, 1.20);
    this->add_atom(88, "Ra",  0.000, 1.20);
    this->add_atom(89, "Ac",  0.000, 1.20);
    this->add_atom(90, "Th",  0.000, 1.20);
    this->add_atom(91, "Pa",  0.000, 1.20);
    this->add_atom(92, "U",  0.000, 1.20);
    this->add_atom(93, "Np",  0.000, 1.20);
    this->add_atom(94, "Pu",  0.000, 1.20);
    this->add_atom(95, "Am",  0.000, 1.20);
    this->add_atom(96, "Cm",  0.000, 1.20);
    this->add_atom(97, "Bk",  0.000, 1.20);
    this->add_atom(98, "Cf",  0.000, 1.20);
    this->add_atom(99, "Es",  0.000, 1.20);
    this->add_atom(100, "Fm",  0.000, 1.20);
    this->add_atom(101, "Md",  0.000, 1.20);
    this->add_atom(102, "No",  0.000, 1.20);
    this->add_atom(103, "Lr",  0.000, 1.20);
    this->add_atom(104, "Rf",  0.000, 1.20);
    this->add_atom(105, "Db",  0.000, 1.20);
    this->add_atom(106, "Sg",   0.000, 1.20);
    this->add_atom(107, "Bh",  0.000, 1.20);
    this->add_atom(108, "Hs",  0.000, 1.20);
    this->add_atom(109, "Mt",  0.000, 1.20);
    this->add_atom(110, "Ds",  0.000, 1.20);
    this->add_atom(111, "Rg",  0.000, 1.20);
    this->add_atom(112, "Cn",  0.000, 1.20);
    this->add_atom(113, "Uut",  0.000, 1.20);
    this->add_atom(114, "Fl",  0.000, 1.20);
    this->add_atom(115, "Uup",  0.000, 1.20);
    this->add_atom(116, "Lv",  0.000, 1.20);
    this->add_atom(117, "Uus",  0.000, 1.20);
    this->add_atom(118, "Uuo",  0.000, 1.20);


    this->atoms.resize(120);
    for(auto it = this->map_atoms.begin(); it != this->map_atoms.end(); ++it) {
        this->atoms[it->second.elnr] = it->second;
    }

    this->bond_distances.resize(121);
    for(unsigned int i=0; i<121; i++) {
        this->bond_distances[i].resize(121, 3.0);
    }

    for(unsigned int i=0; i<121; i++) {
        this->bond_distances[i][1] = 1.2;
        this->bond_distances[1][i] = 1.2;

        if(i > 20) {
            for(unsigned int j=2; j<=20; j++) {
                this->bond_distances[i][j] = 2.5;
                this->bond_distances[j][i] = 2.5;
            }
        } else {
            for(unsigned int j=2; j<=20; j++) {
                this->bond_distances[i][j] = 2.0;
                this->bond_distances[j][i] = 2.0;
            }
        }
    }
}

unsigned int PeriodicTable::get_elnr(const std::string& name) const {
    std::unordered_map<std::string, PeriodicTable::Atom>::const_iterator got = this->map_atoms.find(name);

    if(got == this->map_atoms.end()) {
        return 0;
    } else {
        return got->second.elnr;
    }
}

void PeriodicTable::add_atom(unsigned int elnr,
                             const std::string& label,
                             double mass,
                             double radius) {
    PeriodicTable::Atom at;
    at.elnr = elnr;
    at.label = label;
    at.mass = mass;
    at.radius = radius;

    this->add_atom(label, at);
}

void PeriodicTable::add_atom(const std::string& symbol, const PeriodicTable::Atom& atom) {
    this->map_atoms.insert(std::pair<std::string,PeriodicTable::Atom>(symbol, atom));
}

std::string PeriodicTable::get_label_elnr_string(unsigned int elnr) const {
    std::string el = this->get_label_elnr(elnr);
    std::string temp(el.length(), ' ');
    std::copy(el.begin(), el.end(), temp.begin());
    return temp;
}
