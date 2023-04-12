.. _userinterface:
.. index:: User Interface

User Interface
**************

.. note::
   There are two ways to specify vectors, either as "raw" vectors or by using atom pairs.
   Raw vectors are always written as ``x,y,z``, *without* spaces and are automatically
   normalized. Atom pairs are written as ``id1-id2`` wherein one-based indexing
   is used. In other words, the first atom has an id of 1, not 0.

.. warning::
   Note that some of the output files have generic names
   (e.g. ``planedata-real.bin``). If files with the same name already exist
   in the current working directory, these files will be overwritten. To avoid
   this, you either need to rename these files or execute :program:`EDP` in
   separate directories.

Mandatory arguments
===================

``-i``, ``--input`` ``<filename>``

File containing electron density. The filename should start with ``CHGCAR`` or
``PARCHG`` for automatic recognition.

*Example*: ``-i CHGCAR_calculation``

*****

``-o``, ``--output`` ``<filename>``

A valid path where to write the image file to.

*Example*: ``-o projection.png``

*****

``-v``, ``--vector1`` <vector designation or two atom indices>

The vector :math:`\vec{v}` spanning the `horizontal` direction of the projection. The user
should either specify a triplet of numbers representing the vector or a
pair of atom indices (counting from 1). Vectors are automatically normalized.

*Example*: ``-v 0,0,1`` or ``-v 1-2``

*****

``-w``, ``--vector2`` <vector designation or two atom indices>

The vector :math:`\vec{w}` spanning the `vertical` direction of the projection. The user
should either specify a triplet of numbers representing the vector or a
pair of atom indices (counting from 1). Vectors are automatically normalized.

*Example*: ``-w 0,0,1`` or ``-w 1-2``

*****

``-p``, ``--starting_point`` <vector designation or single atom index>

The point :math:`\vec{p}` in the unit cell through which the projection plane should pass. The
user should either specify a triplet of numbers (that lies in the unit cell)
or a single atom index (counting from one). The units for the vector are in
ångström.

*Example*: ``-p 5,5,5`` or ``-p 1``

*****

``-s``, ``--scaling`` <unsigned integer>

The number of pixels per ångström used to build the plane.

*Example*: ``-s 100``

Optional arguments
==================

``-b``, ``--bounds`` <double,double>

Bounds for displaying the electron density. Note that the electron density
is plotted on a logarithmic scale (with a base of 10) and the lower and upper
bounds as supplied correspond to the exponent.

*Example*: ``-b <-3,2>``

*****

``-g``, ``--gramschmidt``

Orthogonalize the vector pair :math:`\vec{v}` and :math:`\vec{w}` via the
Gram-Schmidt process.

*Example*: ``-g``

*****

``-l``, ``--legend``

Whether to display a legend.

*Example*: ``-l``

*****

``-n``, ``--negative``

Whether to allow for negative values. By convention, the electron density
should not be negative and any negative densities are set to zero. However if
one is projecting electron density *differences*, negative electron densities
are of course possible and will be plotted accordingly.

*Example*: ``-n``

*****

``-c``, ``--color-scheme-id`` <unsigned int>

Which color scheme to use. :ref:`See the overview of possible color schemes<color schemes>`.

*Example*: ``-c 1``

.. note::
   For the color schemes, zero-based indexing is used. In other words, the first
   color scheme is color scheme #0.

*****

``-c``, ``--color-scheme-id`` <unsigned int>

Which color scheme to use. :ref:`See the overview of possible color schemes<color schemes>`.

*Example*: ``-c 1``

Additional features
===================

Some additional command line arguments are available to execute specific jobs
on the electron density. These correspond to niche features.

*****

``-z``, ``--zaverage``

Calculate the total electron density per plane for the set of planes whose
normal vector lie in the :math:`z`-direction. The output is written in a
two-column text file `z_extraction.txt`.

*Example*: ``-z``

*****

``-e``, ``--extraction`` <vector or pair of two atom indices>

Calculate the electron density through a line defined by a normal vector
:math:`\vec{e}` going through point :math:`\vec{p}`. The output is written
to `line_extraction.txt`.

*Example*: ``-e 1-2`` or ``-e 1,1,1``

*****

``-r``, ``--radius`` <atom id,radius>

Calculate the average electron density (or electrostatic potential) at a
radius :math:`r \in 0,R` from an atom with 0.01 Å increments. The sampling
points are based on the coordinates of the 23<sup>rd</sup> order Lebedev
quadrature. The result is written to `spherical_average.txt`.

*Example*: ``-r 1,1.5``
