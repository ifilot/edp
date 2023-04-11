---
title: 'EDP: a program for projecting electron densities from VASP onto planes'
tags:
  - Electron density
  - Charge density
  - VASP
  - Projection
authors:
  - name: I.A.W. Filot
    orcid: 0000-0003-1403-8379
    affiliation: 1
affiliations:
 - name: Inorganic Materials and Catalysis, Department of Chemical Engineering and Chemistry, Eindhoven University of Technology
   index: 1
date: 11 April 2023
bibliography: paper.bib
---

# Summary

The electron density is a fundamental concept in electronic structure calculations, as it provides a detailed description of the distribution of electrons in a material or molecule. In many electronic structure methods, the electron density is used as the basic quantity from which other properties such as the energy, potential, and forces can be derived. The electron density provides insight into chemical bonding and reactivity and understanding the electron density (and its redistribution upon chemical bond breaking or formation) is crucial for predicting and interpreting the properties of materials and molecules.[@martin:2004]

The electron density is a scalar field, which means that it is a function that assigns a scalar value to each point in space. In the case of the electron density, this scalar value represents the probability density of finding an electron at that point in space. Specialized visualization tools and techniques are often required to effectively visualize scalar fields such as the electron density. These tools may include contour plots, isosurface rendering, and volume rendering, among others. Visualizing the electron density is essential for gaining insights into the electronic properties and behavior of materials and molecules.

# Statement of need

The visualization of scalar fields using contour plots or heat maps is ubiquitous in science and engineering and plenty of general purpose programs such as `Open Data Explorer` [@OpenDX] and `ParaView` [@ParaView] readily accomodate this task. There exist also programs that specifically cater to `VASP`, such as `Vesta` [@momma:2011] and `sisl` [@papior:2023]. These programs are typically designed for interactive use , `sisl` being a notable exception, and utilize a graphical user interface of some sort. In contrast, `EDP` is a C++ based command line tool that carries out the projection of the electron density scalar field as stored in `VASP` CHGCAR or PARCHG file onto a plane. This plane is then rendered onto a canvas using a color map and stored as a PNG file. `EDP` uses a minimal set of dependencies, i.e. Boost[@BoostLibrary], Cairo[@CairoLibrary], `TCLAP`[@TclapLibrary], Eigen3[@eigenweb], which are all readily available on modern Linux based operating systems.

`EDP` was designed to be used by researchers and students working in computational materials modelling using the VASP software. It has already been used in a number of scientific publications[@zijlstra:2019; @vogt:2019; @filot:2016]. `EDP` is designed with ease of use in mind. The projection planes can be defined with respect to the atomic coordinates, which are available in the structure block of CHGCAR and PARCHG files. This allows for users to efficiently define projection plane intersecting one or more atoms of interest. This feature is especially relevant to chemists who study the electron density to understand bonding patterns and explain chemical reactivity.

![Schematic overview of the pipeline of the `EDP` program.\label{fig:pipeline}](img/edp_application_pipeline.png)

Besides the projection of the electron density onto a plane, `EDP` also carries a number of additional features corresponding to averaging or sampling operations on the electron density as schematically depicted in \autoref{fig:pipeline}. These correspond to the integration of the electron density over the xy-axes to obtain the averaged electron density as function of the z-coordinate. This feature is of use for researchers studying slab models, relevant to applications in catalysis and material science. There also exist the option to produce one-dimensional projections. The user can either define a line in three-dimensional space on which the electron density is projected. A use case for this is to study the electron density alongside a chemical bond. Alternatively, the electron density can be averaged over a sphere. Herein, the user can indicate a position and spheres with increasing size are generated up to a fixed radius. For each sphere, the average electron density is established by sampling grid points corresponding to the integration points of the 193th order Lebedev quadrature. The result is the averaged electron density as function of the distance to the atom. A use case for this feature is to study the lateral interactions between adsorbates by the through-space electron-electron repulsion.[@zijlstra:2019]

# Acknowledgements

This work was supported by the Netherlands Center for Multiscale Catalytic Energy Conversion, and NWO Gravitation program funded by the Ministry of Education, Culture and Science of the government of the Netherlands. The Netherlands Organization for Scientific Research is acknowledged for providing access to computational resources. The author wishes to thank Dr. Bart Zijlstra and Ellen Sterk MSc. for extensively testing the software and providing valuable feedback.

# References
