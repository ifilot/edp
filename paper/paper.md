---
title: 'EDP: a program for projecting electron densities'
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
date: 10 April 2023
bibliography: paper.bib
---

# Summary

The electron density is a fundamental concept in electronic structure calculations, as it provides a detailed description of the distribution of electrons in a material or molecule. In many electronic structure methods, the electron density is used as the basic quantity from which other properties such as the energy, potential, and forces can be derived. The electron density provides insight into chemical bonding and reactivity and understanding the electron density (and its redistribution upon chemical bond breaking or formation) is crucial for predicting and interpreting the properties of materials and molecules.[@martin:2004]

The electron density is a scalar field, which means that it is a function that assigns a scalar value to each point in space. In the case of the electron density, this scalar value represents the probability density of finding an electron at that point in space. Specialized visualization tools and techniques are often required to effectively visualize scalar fields such as the electron density. These tools may include contour plots, isosurface rendering, and volume rendering, among others. Visualizing the electron density is essential for gaining insights into the electronic properties and behavior of materials and molecules.

# Statement of need

EDP is a C++ based command line tool that carries out the projection of the electron density scalar field as stored in VASP CHGCAR or PARCHG file onto a plane and renders this plane using a color map. The user can efficiently define these planes based on the atom coordinates as provided in the structure block of these files.

`EDP` was designed to be used by researchers working in computational materials modelling using the VASP software. It has already been
used in a number of scientific publications[@zijlstra:2019; @vogt:2019; @filot:2016].

# Acknowledgements

This work was supported by the Netherlands Center for Multiscale Catalytic Energy Conversion, and NWO Gravitation program funded by the Ministry of Education, Culture and Science of the government of the Netherlands. The Netherlands Organization for Scientific Research is acknowledged for providing access to computational resources.

# References
