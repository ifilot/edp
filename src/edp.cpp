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
 * The Electron Density Plotter (EDP) projects the 3D electron density onto
 * a 2D plane (i.e. a surface cut).
 *
 * The a program reads one or several CHGCAR files, performs elementwise
 * some mathematical operations on the content and creates a memory object
 * of the 3D scalar field.
 *
 * From this 3D scalar field, a surface cut is produced using a trilinear
 * interpolation routine.
 *
 */

#include <iostream>
#include <tclap/CmdLine.h> // parsing command line arguments
#include "mathtools.h"
#include "scalar_field.h"
#include "planeprojector.h"

int main(int argc, char *argv[]) {
    // command line grabbing
    try {
        TCLAP::CmdLine cmd("Projects the electrondensity of a CHGCAR file onto a image file.", ' ', "0.9");

        //**************************************
        // declare values to be parsed
        //**************************************
        TCLAP::ValueArg<std::string> arg_output_filename("o","filename","Filename to print to",true,"test.png","string");
        cmd.add(arg_output_filename);
        TCLAP::ValueArg<std::string> arg_sp("p","starting_point","Start point of cutting plane",true,"(0.5,0.5,0.5)","3d-vector");
        cmd.add(arg_sp);
        TCLAP::ValueArg<std::string> arg_v("v","vector1","Plane Vector 1",true,"(1,0,0)","3d-vector");
        cmd.add(arg_v);
        TCLAP::ValueArg<std::string> arg_w("w","vector2","Plane Vector 2",true,"(0,0,1)","3d-vector");
        cmd.add(arg_w);
        TCLAP::ValueArg<unsigned int> arg_s("s","scale","Scaling in px/angstrom",true, 200,"unsigned integer");
        cmd.add(arg_s);
        TCLAP::ValueArg<std::string> arg_input_filename("i","input","Input file (i.e. CHGCAR)",true,"CHGCAR","filename");
        cmd.add(arg_input_filename);
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
        std::cout << "Electron Density Plotter v1.1.0" << std::endl;
        std::cout << "===============================" << std::endl;
        std::cout << "Ivo Filot <i.a.w.filot@tue.nl>" << std::endl;
        std::cout << "===============================" << std::endl;

        std::cout << "Lattice v1: " << v_in[0] << "," << v_in[1] << "," << v_in[2] << std::endl;
        std::cout << "Lattice v2: " << w_in[0] << "," << w_in[1] << "," << w_in[2] << std::endl;
        std::cout << "Start point: " << sp_in[0] << "," << sp_in[1] << "," << sp_in[2] << std::endl;

        // read in field
        std::cout << "Start reading " << input_filename << "; This might take a while." << std::endl;
        ScalarField sf(input_filename.c_str());
        sf.read();
        std::cout << "Done reading " << input_filename << std::endl;

        // define intervals in Angstrom
        const float interval = 20.0;
        const float li = -interval;
        const float hi = interval;
        const float lj = -interval;
        const float hj = interval;

        PlaneProjector pp(&sf, -4, 1);
        pp.extract(v1, v2, s, scale, li, hi, lj, hj, negative_values);
        pp.plot();
        pp.isolines(6, negative_values);
        pp.write(output_filename);

        return 0;
    } catch (TCLAP::ArgException &e) {
        std::cerr << "error: " << e.error() <<
                     " for arg " << e.argId() << std::endl;
        return -1;
    }
}
