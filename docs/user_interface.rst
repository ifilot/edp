.. _userinterface:
.. index:: User Interface

User Interface
**************

Mandatory arguments
===================

``-i``, ``--input`` ``<filename>``

File containing electron density. The filename should start with ``CHGCAR`` or
``PARCHG`` for automatic recognition.

*Example*: ``-i CHGCAR_calculation``

*****

``-o``, ``-output`` ``<filename>``

A valid path where to write the image file to.

*Example*: ``-o projection.png``

*****

``-v``, ``-vector1`` <vector designation or two atom indices>

The vector spanning the `horizontal` direction of the projection. The user
should either specify a triplet of numbers representing the vector or a
pair of atom indices (counting from 1). Vectors are automatically normalized.

*Example*: ``-v 0,0,1`` or ``-v 1-2``

*****

``-w``, ``-vector2`` <vector designation or two atom indices>

The vector spanning the `vertical` direction of the projection. The user
should either specify a triplet of numbers representing the vector or a
pair of atom indices (counting from 1). Vectors are automatically normalized.

*Example*: ``-w 0,0,1`` or ``-w 1-2``

*****

``-p``, ``-starting_point`` <vector designation or single atom index>

A point in the unit cell through which the projection plane should pass. The
user should either specify a triplet of numbers (that lies in the unit cell)
or a single atom index (counting from one). The units for the vector are in
ångström.

*Example*: ``-p 5,5,5`` or ``-p 1``

*****

``-s``, ``-scaling`` <unsigned integer>

The number of pixels per ångström used to build the plane.

*Example*: ``-s 100``

Optional arguments
==================

