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
#include "raycaster.h"
#include "config.h"

int main(int argc, char *argv[]) {
    // command line grabbing
    try {
        TCLAP::CmdLine cmd("Projects the electrondensity of a CHGCAR file onto a image file.", ' ', PROGRAM_VERSION);

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

        // whether or not to orthogonalize the input vectors
        TCLAP::SwitchArg arg_gram_schmidt("g","gramschmidt","Orthogonalize input vectors", cmd, false);

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

        // whether to perform 1d extraction
        TCLAP::ValueArg<std::string> arg_ext("e","extraction","Line extraction vector",false, "", "two atoms ids");
        cmd.add(arg_ext);

        // whether to perform radial extraction
        TCLAP::ValueArg<std::string> arg_r("r","radius","Extraction at radius on atom",false, "", "Atom and radius");
        cmd.add(arg_r);

        // whether or not to write out a z-average extraction
        TCLAP::SwitchArg arg_z("z","zaverage","Averaging over z", cmd, false);

        // graph value bounds (for coloring purposes)
        TCLAP::ValueArg<std::string> arg_b("b","bounds","Lower and upper bounds",false, "", "-3,2");
        cmd.add(arg_b);

        // whether or not to print a legend
        TCLAP::SwitchArg arg_raytrace("t","raytrace","Whether to perform raytracing", cmd, false);

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

        //***************************************)
        // identify whether this file is a locpot
        //***************************************
        bool is_locpot = false;
        if(input_filename.size() >= 6) {
            if(input_filename.substr(0,6).compare("LOCPOT") == 0) {
                is_locpot = true;
            }
        }
        if(is_locpot) {
            std::cout << input_filename << " is identified as a LOCPOT file. This means that we use scalar field as is and perform *no* volume correction on it." << std::endl;
        } else {
            std::cout << input_filename << " is identified as a CHGCAR/PARCHG file. This means that we perform a volume correction on it as described in the link below:" << std::endl;
            std::cout << "https://cms.mpi.univie.ac.at/vasp/vasp/CHGCAR_file.html#file-chgcar." << std::endl;
        }
        std::cout << std::endl;

        //**************************************
        // read header and atoms
        //**************************************
        ScalarField sf(input_filename.c_str(), is_locpot);
        sf.read_header_and_atoms();

        //**************************************
        // determine vector and positions
        //**************************************
        const boost::regex re_vec3("^([0-9.-]+),([0-9.-]+),([0-9.-]+)$");     // 3-vector
        const boost::regex re_vec2("^([0-9-]+),([0-9-]+)$");                  // 2-vector
        const boost::regex re_scalar("^([0-9]+)$");                           // single atom
        const boost::regex re_scalar_radius("^([0-9]+),([0-9.]+)$");          // single atom and radius
        const boost::regex re_scalar_2("^([0-9]+)-([0-9]+)(:?[xyz]*)$");      // two atoms + xyz directives

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

            // if the user has supplied a "xyz" directives, check which of these
            // vector components should be retained
            std::string str = boost::lexical_cast<std::string>(what[3]);
            if(str.size() > 0 && str[0] == ':') {
                //std::cout << "Extracting components" << boost::lexical_cast<std::string>(what[3]) << std::endl;

                if(str.find('x') == std::string::npos) {
                    v[0] = 0.0;
                }

                if(str.find('y') == std::string::npos) {
                    v[1] = 0.0;
                }

                if(str.find('z') == std::string::npos) {
                    v[2] = 0.0;
                }
            }

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

        // check whether vectors need to be orthogonalized
        if(arg_gram_schmidt.getValue()) {
            v = v - glm::dot(v, w) / glm::dot(w, w) * w;
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
        std::cout << "Minimum value: " << sf.get_min() << std::endl;
        std::cout << "Maximum value: " << sf.get_max() << std::endl;
        std::cout << std::endl;

        if(sf.get_min() < 1e-4 && !arg_negative.getValue()) {
            std::cout << "------------------- NOTE -------------------" << std::endl;
            std::cout << "Significant negative values are encountered." << std::endl;
            std::cout << "If you want to parse these, consider setting" << std::endl;
            std::cout << "the `-n` argument." << std::endl;
            std::cout << "------------------- NOTE -------------------" << std::endl;
        }

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
        static const float interval = 20.0;
        static const float li = -interval;
        static const float hi = interval;
        static const float lj = -interval;
        static const float hj = interval;

        //**************************************
        // construct plane
        //**************************************
        start = std::chrono::system_clock::now();
        PlaneProjector pp(&sf, color_scheme_id);

        //**************************************
        // set bounds for legend
        //**************************************
        int bounds[2];
        const std::string bound_str = arg_b.getValue();
        if(boost::regex_match(bound_str, what, re_vec2)) {
            bounds[0] = boost::lexical_cast<int>(what[1]);
            bounds[1] = boost::lexical_cast<int>(what[2]);
            if(bounds[0] == bounds[1]) {
                throw std::runtime_error("Supplied bounds (-b) should be different.");
            }
            if(bounds[0] > bounds[1]) {
                throw std::runtime_error("Lower bound should be supplied before upper bound (-b).");
            }
            std::cout << "Using specified bounds: [log10(" << bounds[0] << "), log10(" << bounds[1] << ")]." << std::endl;
            pp.set_scaling(negative_values, bounds[0], bounds[1]);
        } else {
            if(sf.is_locpot()) {
                pp.set_scaling(negative_values, -3, 1);
            } else {
                pp.set_scaling(negative_values, -7, 1);
            }
        }

        pp.extract(v, w, p, scale, li, hi, lj, hj);
        pp.plot();
        pp.isolines(10);
        if(print_legend) {
            pp.draw_legend();
        }
        pp.write(output_filename);  // write graph to file
        end = std::chrono::system_clock::now();
        elapsed_seconds = end-start;
        std::cout << "Constructed contour plot in " << elapsed_seconds.count() << " seconds." << std::endl;

        std::cout << "--------------------------------------------------------------" << std::endl;

        //**************************************
        // Performing optional 1D line extraction
        //**************************************
        std::string ex_str = arg_ext.getValue();
        glm::vec3 e;
        if(boost::regex_match(ex_str, what, re_vec3)) {
            e[0] = boost::lexical_cast<float>(what[1]);
            e[1] = boost::lexical_cast<float>(what[2]);
            e[2] = boost::lexical_cast<float>(what[3]);
            pp.extract_line(e, p, scale, li, hi);
        } else if(boost::regex_match(ex_str, what, re_scalar_2)) {
            e = glm::normalize(
                sf.get_atom_position(boost::lexical_cast<unsigned int>(what[2])-1) -
                sf.get_atom_position(boost::lexical_cast<unsigned int>(what[1])-1)
            );
            pp.extract_line(e, p, scale, li, hi);
        }

        //**************************************
        // Performing optional z-axis averaging
        //**************************************
        if(arg_z.getValue()) {
            pp.extract_plane_average();
        }

        //**************************************
        // Performing optional radial extraction
        //**************************************
        std::string re_str = arg_r.getValue();
        if(boost::regex_match(re_str, what, re_scalar_radius)) {
            const unsigned int atid = boost::lexical_cast<unsigned int>(what[1]);
            glm::vec3 pr = sf.get_atom_position(atid-1);
            const float radius = boost::lexical_cast<float>(what[2]);

            std::cout << "Averaging sphere surface for atom " << atid << " at radius: " << radius << std::endl;
            pp.extract_sphere_average(pr, radius);
        }

        //**************************************
        // Ray Tracing
        //**************************************
        if(arg_raytrace.getValue()) {
            std::cout << "Performing ray casting" << std::endl;
            std::string outputrayfile = "raytrace.png";
            start = std::chrono::system_clock::now();
            RayCaster rc(&sf, color_scheme_id);
            rc.cast();
            rc.write(outputrayfile);
            std::cout << "Writing output to " << outputrayfile << std::endl;
            end = std::chrono::system_clock::now();
            elapsed_seconds = end-start;
            std::cout << "Done ray tracing in " << elapsed_seconds.count() << " seconds." << std::endl;
            std::cout << "--------------------------------------------------------------" << std::endl;
        }

        std::cout << "Done" << std::endl << std::endl;

        return 0;

    } catch (TCLAP::ArgException &e) {
        std::cerr << "error: " << e.error() <<
                     " for arg " << e.argId() << std::endl;
        return -1;
    }
}
