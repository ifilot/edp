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

#include "mathtools.h"
#include "scalar_field.h"
#include "planeprojector.h"

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
        TCLAP::ValueArg<unsigned int> arg_s("s","scale","Scaling in px/angstrom",true, 200,"unsigned integer");
        cmd.add(arg_s);

        // whether or not negative values are allowed
        TCLAP::SwitchArg arg_negative("n","negative_values","CHGCAR can contain negative values", cmd, false);

        cmd.parse(argc, argv);

        //**************************************
        // parsing values
        //**************************************
        std::string input_filename = arg_input_filename.getValue();
        std::string output_filename = arg_output_filename.getValue();

        boost::regex re("^([0-9.-]+),([0-9.-]+),([0-9.-]+)$");
        std::string sp = arg_sp.getValue();
        float sp_in[3];
        boost::smatch what;
        if(boost::regex_match(sp, what, re)) {
            sp_in[0] = boost::lexical_cast<float>(what[1]);
            sp_in[1] = boost::lexical_cast<float>(what[2]);
            sp_in[2] = boost::lexical_cast<float>(what[3]);
        }
        Vector s(sp_in[0],sp_in[1],sp_in[2]);

        std::string v = arg_v.getValue();
        float v_in[3];
        if(boost::regex_match(v, what, re)) {
            v_in[0] = boost::lexical_cast<float>(what[1]);
            v_in[1] = boost::lexical_cast<float>(what[2]);
            v_in[2] = boost::lexical_cast<float>(what[3]);
        }
        Vector v1(v_in[0],v_in[1],v_in[2]);

        std::string w = arg_w.getValue();
        float w_in[3];
        if(boost::regex_match(w, what, re)) {
            w_in[0] = boost::lexical_cast<float>(what[1]);
            w_in[1] = boost::lexical_cast<float>(what[2]);
            w_in[2] = boost::lexical_cast<float>(what[3]);
        }
        Vector v2(w_in[0],w_in[1],w_in[2]);

        float scale = arg_s.getValue();

        bool negative_values = arg_negative.getValue();

        //**************************************
        // start running the program
        //**************************************
        std::cout << "==============================================================" << std::endl;
        std::cout << "Electron Density Plotter v1.1.1" << std::endl;
        std::cout << std::endl;
        std::cout << "Ivo Filot <i.a.w.filot@tue.nl>" << std::endl;
        std::cout << "==============================================================" << std::endl;
        std::cout << std::endl;
        std::cout << boost::format("Lattice vector 1: (%12.6f;%12.6f;%12.6f)") % v_in[0] % v_in[1] % v_in[2] << std::endl;
        std::cout << boost::format("Lattice vector 2: (%12.6f;%12.6f;%12.6f)") % w_in[0] % w_in[1] % w_in[2] << std::endl;
        std::cout << boost::format("Starting point  : (%12.6f;%12.6f;%12.6f)") % sp_in[0] % sp_in[1] % sp_in[2] << std::endl;
        std::cout << std::endl;

        // read in field
        std::cout << "Start reading " << input_filename << "..." << std::endl;
        auto start = std::chrono::system_clock::now();
        ScalarField sf(input_filename.c_str());
        sf.read();
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;
        std::cout << "Done reading " << input_filename << " in " << elapsed_seconds.count() << " seconds." << std::endl;
        std::cout << std::endl;

        // define intervals in Angstrom
        const float interval = 20.0;
        const float li = -interval;
        const float hi = interval;
        const float lj = -interval;
        const float hj = interval;

        // construct plane
        start = std::chrono::system_clock::now();
        PlaneProjector pp(&sf, -4, 1);
        pp.extract(v1, v2, s, scale, li, hi, lj, hj, negative_values);
        pp.plot();
        pp.isolines(6, negative_values);
        pp.write(output_filename);
        end = std::chrono::system_clock::now();
        elapsed_seconds = end-start;
        std::cout << "Constructed contour plot in " << elapsed_seconds.count() << " seconds." << std::endl;

        std::cout << "==============================================================" << std::endl;
        std::cout << "Done" << std::endl;

        return 0;
    } catch (TCLAP::ArgException &e) {
        std::cerr << "error: " << e.error() <<
                     " for arg " << e.argId() << std::endl;
        return -1;
    }
}
