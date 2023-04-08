#!/bin/bash

#
# Generate example images that are used in the documentation
#

EXEC="../../../build/edp"
SCALE=25
TARGET="../../../docs/_static/img"
TEMPTARGET="./imgtest"
BOUNDS="-3,1"

# create temporary folder for testing if it does not exist
mkdir -pv $TEMPTARGET

#
# CH4 in cubic unit cell
#

SRC="CHGCAR_CH4"

# xz image
$EXEC -i $SRC -v 1,0,0 -w 0,0,1 -s $SCALE -p 5,5,5 -o $TEMPTARGET/xz.png -b $BOUNDS

# yz image
$EXEC -i $SRC -v 0,1,0 -w 0,0,1 -s $SCALE -p 5,5,5 -o $TEMPTARGET/yz.png -b $BOUNDS

# yz image
$EXEC -i $SRC -v 1,0,0 -w 0,1,0 -s $SCALE -p 5,5,5 -o $TEMPTARGET/xy.png -b $BOUNDS

# diagonal 1
$EXEC -i $SRC -v 1,1,0 -w 0,0,1 -s $SCALE -p 5,5,5 -o $TEMPTARGET/diag1.png -b $BOUNDS

# diagonal 2
$EXEC -i $SRC -v -1,1,0 -w 0,0,1 -s $SCALE -p 5,5,5 -o $TEMPTARGET/diag2.png -b $BOUNDS

# diagonal 1
$EXEC -i $SRC -v 2-3 -w 0,0,1 -s $SCALE -p 5,5,5 -o $TEMPTARGET/diag1_equiv.png -b $BOUNDS

# diagonal 2
$EXEC -i $SRC -v 4-5 -w 0,0,1 -s $SCALE -p 5,5,5 -o $TEMPTARGET/diag2_equiv.png -b $BOUNDS

#
# CH4 in "skewed" triclinic unit cell
#

SRC="CHGCAR_CH4_SU"

# xy image
$EXEC -i $SRC -v 1,0,0 -w 0,1,0 -s $SCALE -p 1 -o $TEMPTARGET/xy_su.png -b $BOUNDS

# diagonal 1 SU
$EXEC -i $SRC -v 2-3 -w 0,0,1 -s $SCALE -p 1 -o $TEMPTARGET/diag1_su.png -b $BOUNDS

# diagonal 2 SU
$EXEC -i $SRC -v 4-5 -w 0,0,1 -s $SCALE -p 1 -o $TEMPTARGET/diag2_su.png -b $BOUNDS

#
# Al FCC111
#

SRC="CHGCAR_Al_FCC111"

# xy image
$EXEC -i $SRC -v 1,0,0 -w 0,1,0 -s $SCALE -p 25 -o $TARGET/al_fcc111_xy.png -b -5,0 -l

# xz image
$EXEC -i $SRC -v 1,0,0 -w 0,0,1 -s $SCALE -p 28 -o $TARGET/al_fcc111_xz.png -b -5,0 -l

#
# Benzene HOMO
#

SRC="CHGCAR_Al_FCC111"

# xy image
$EXEC -i $SRC -v 1,0,0 -w 0,1,0 -s $SCALE -p 25 -o $TARGET/al_fcc111_xy.png -b -5,0 -l

# xz image
$EXEC -i $SRC -v 1,0,0 -w 0,0,1 -s $SCALE -p 28 -o $TARGET/al_fcc111_xz.png -b -5,0 -l
