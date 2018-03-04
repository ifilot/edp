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

/*
 * PURPOSE
 * =======
 *
 * The Electron Density Plotter (EDP) projects the 3D electron density onto
 * a 2D plane (i.e. a surface cut).
 *
 * The a program reads one or several CHGCAR files, performs elementwise
 * some mathematical operations on the content and creates a memory object
 * of the 3D scalar field.
 *
 * From this 3D scalar field, a surface cut is produced using a trilinear
 * interpolation routine.
 */

#include <iostream>
#include <chrono>
#include <tclap/CmdLine.h>
#include <boost/format.hpp>

#include "scalar_field.h"
#include "planeprojector.h"
#include "config.h"

int main(int argc, char *argv[]) {
    // command line grabbing
    try {
        TCLAP::CmdLine cmd("Projects the electrondensity of a CHGCAR file onto a image file.", ' ', "1.1.1");

        //**************************************
        // declare values to be parsed
        //**************************************

        // input filename
        TCLAP::ValueArg<std::string> arg_input_filename("i","input","Input file (i.e. CHGCAR)",true,"CHGCAR","filename");
        cmd.add(arg_input_filename);

        // output filename
        TCLAP::ValueArg<std::string> arg_output_filename("o","filename","Filename to print to",true,"test.png","string");
        cmd.add(arg_output_filename);

        // starting point
        TCLAP::ValueArg<std::string> arg_sp("p","starting_point","Start point of cutting plane",true,"(0.5,0.5,0.5)","3d-vector");
        cmd.add(arg_sp);

        // plane vector 1
        TCLAP::ValueArg<std::string> arg_v("v","vector1","Plane Vector 1",true,"(1,0,0)","3d-vector");
        cmd.add(arg_v);

        // plane vector 2
        TCLAP::ValueArg<std::string> arg_w("w","vector2","Plane Vector 2",true,"(0,0,1)","3d-vector");
        cmd.add(arg_w);

        // scaling constant
        TCLAP::ValueArg<unsigned int> arg_s("s","scale","Scaling in px/angstrom",false, 100,"unsigned integer");
        cmd.add(arg_s);

        // colorscheme id
        TCLAP::ValueArg<unsigned int> arg_c("c","color-scheme-id","Color scheme ID",false, 0,"unsigned integer");
        cmd.add(arg_c);

        // whether or not negative values are allowed
        TCLAP::SwitchArg arg_negative("n","negative_values","CHGCAR can contain negative values", cmd, false);

        // whether or not to print a legend
        TCLAP::SwitchArg arg_legend("l","legend","Print legend", cmd, false);

        cmd.parse(argc, argv);

        //**************************************
        // Inform user about execution
        //**************************************
        std::cout << "--------------------------------------------------------------" << std::endl;
        std::cout << "Executing EDP v." << PROGRAM_VERSION << std::endl;
        std::cout << "Author: Ivo Filot <i.a.w.filot@tue.nl>" << std::endl;
        std::cout << "--------------------------------------------------------------" << std::endl;

        //**************************************
        // parsing values
        //**************************************
        std::string input_filename = arg_input_filename.getValue();
        std::string output_filename = arg_output_filename.getValue();

        //**************************************
        // read header and atoms
        //**************************************
        ScalarField sf(input_filename.c_str());
        sf.read_header_and_atoms();

        //**************************************
        // determine vector and positions
        //**************************************
        boost::regex re_vec3("^([0-9.-]+),([0-9.-]+),([0-9.-]+)$");     // 3-vector
        boost::regex re_scalar("^([0-9]+)$");                           // single atom
        boost::regex re_scalar_2("^([0-9]+)-([0-9]+)$");                // two atoms

        // get position for the point
        std::string sp = arg_sp.getValue();
        boost::smatch what;
        glm::vec3 p;
        if(boost::regex_match(sp, what, re_vec3)) {
            p[0] = boost::lexical_cast<float>(what[1]);
            p[1] = boost::lexical_cast<float>(what[2]);
            p[2] = boost::lexical_cast<float>(what[3]);
        } else if(boost::regex_match(sp, what, re_scalar)) {
            p = sf.get_atom_position(boost::lexical_cast<unsigned int>(what[1])-1);
        } else {
            std::runtime_error("Could not obtain a vector p");
        }

        // obtain first vector
        std::string v_str = arg_v.getValue();
        glm::vec3 v;
        if(boost::regex_match(v_str, what, re_vec3)) {
            v[0] = boost::lexical_cast<float>(what[1]);
            v[1] = boost::lexical_cast<float>(what[2]);
            v[2] = boost::lexical_cast<float>(what[3]);
        } else if(boost::regex_match(v_str, what, re_scalar_2)) {
            v = glm::normalize(
                    sf.get_atom_position(boost::lexical_cast<unsigned int>(what[2])-1) -
                    sf.get_atom_position(boost::lexical_cast<unsigned int>(what[1])-1)
                );
        } else {
            std::runtime_error("Could not obtain a vector v");
        }

        // obtain second vector
        std::string w_str = arg_w.getValue();
        glm::vec3 w;
        if(boost::regex_match(w_str, what, re_vec3)) {
            w[0] = boost::lexical_cast<float>(what[1]);
            w[1] = boost::lexical_cast<float>(what[2]);
            w[2] = boost::lexical_cast<float>(what[3]);
        } else if(boost::regex_match(w_str, what, re_scalar_2)) {
            w = glm::normalize(
                    sf.get_atom_position(boost::lexical_cast<unsigned int>(what[2])-1) -
                    sf.get_atom_position(boost::lexical_cast<unsigned int>(what[1])-1)
                );
        } else {
            std::runtime_error("Could not obtain a vector w");
        }

        //**************************************
        // read grid
        //**************************************
        std::cout << "Start reading " << input_filename << "..." << std::endl;
        auto start = std::chrono::system_clock::now();
        sf.read();
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;
        std::cout << "Done reading " << input_filename << " in " << elapsed_seconds.count() << " seconds." << std::endl;
        std::cout << std::endl;

        //**************************************
        // determine size and colors
        //**************************************

        const float scale = arg_s.getValue();
        const unsigned int color_scheme_id = arg_c.getValue();
        const bool negative_values = arg_negative.getValue();
        const bool print_legend = arg_legend.getValue();

        //**************************************
        // execute construction
        //**************************************
        std::cout << std::endl;
        std::cout << boost::format("Lattice vector 1: (%12.6f;%12.6f;%12.6f)") % v[0] % v[1] % v[2] << std::endl;
        std::cout << boost::format("Lattice vector 2: (%12.6f;%12.6f;%12.6f)") % w[0] % w[1] % w[2] << std::endl;
        std::cout << boost::format("Starting point  : (%12.6f;%12.6f;%12.6f)") % p[0] % p[1] % p[2] << std::endl;
        std::cout << std::endl;

        // define intervals in Angstrom
        const float interval = 20.0;
        const float li = -interval;
        const float hi = interval;
        const float lj = -interval;
        const float hj = interval;

        //**************************************
        // construct plane
        //**************************************
        start = std::chrono::system_clock::now();
        PlaneProjector pp(&sf, negative_values ? -1 : -7, negative_values ? 1 : 1, color_scheme_id);
        pp.extract(v, w, p, scale, li, hi, lj, hj, negative_values);
        pp.plot();
        pp.isolines(6, negative_values);
        if(print_legend) {
            pp.draw_legend(negative_values);
        }
        pp.write(output_filename);
        end = std::chrono::system_clock::now();
        elapsed_seconds = end-start;
        std::cout << "Constructed contour plot in " << elapsed_seconds.count() << " seconds." << std::endl;

        std::cout << "--------------------------------------------------------------" << std::endl;
        std::cout << "Done" << std::endl << std::endl;

        return 0;

    } catch (TCLAP::ArgException &e) {
        std::cerr << "error: " << e.error() <<
                     " for arg " << e.argId() << std::endl;
        return -1;
    }
}
