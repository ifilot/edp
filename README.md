# EDP – a program for projecting electron densities

[![Build](https://github.com/ifilot/edp/actions/workflows/build.yml/badge.svg)](https://github.com/ifilot/edp/actions/workflows/build.yml)
[![Documentation](https://github.com/ifilot/edp/actions/workflows/docs.yml/badge.svg)](https://edp.imc-tue.nl)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![status](https://joss.theoj.org/papers/5544210e68408b1f00a6fb802b7745e8/status.svg)](https://joss.theoj.org/papers/5544210e68408b1f00a6fb802b7745e8)

* Detailed documentation can be found at https://edp.imc-tue.nl.
* See [the contributing guide](CONTRIBUTING.md) for detailed instructions how to make contributions.
* Bugs and feature requests are ideally submitted via the [gitlab issue tracker](https://github.com/ifilot/edp/issues).
* The development team can also be reached by email via i.a.w.filot@tue.nl.

## Purpose

EDP is a C++ program designed to project the electron density as stored in a
[CHGCAR](https://www.vasp.at/wiki/index.php/CHGCAR) or
[PARCHG](https://www.vasp.at/wiki/index.php/PARCHG) file
(calculated in [VASP](https://www.vasp.at/), onto a plane.

With EDP, users can effortlessly define the projection plane based
on atomic positions while also having the option for detailed customization of
the plane's position and direction. Its versatility and ease of use make EDP
a handy tool for researchers in the field of materials science.

![molecular orbitals of benzene](docs/_static/img/benzene_mos.jpg)

## Features

### Easy command line instructions

By referring to the atomic coordinates, one can easily define the projection
plane and produce nice and clear images.

```bash
edp -i PARCHG_BENZENE_07 -o benzene_xy.png -p 1 -v 1,0,0 -w 0,1,0 -s 25 -b -5,0 -l
```

![molecular orbitals of benzene](docs/_static/img/benzene_xy_02.png)

### Large variety of color schemes

EDP supports 16 different color schemes.

![molecular orbitals of benzene](docs/_static/img/color_schemes.jpg)
