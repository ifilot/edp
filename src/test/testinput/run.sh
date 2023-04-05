#!/bin/bash

#
# Generate example images that are used in the documentation
#

EXEC="../../../build/edp"
SRC="CHGCAR_CH4"
SCALE=25
TARGET="../../../docs/_static/img"
BOUNDS="-3,1"

#
# CH4 in cubic unit cell
#

# xz image
$EXEC -i $SRC -v 1,0,0 -w 0,0,1 -s $SCALE -p 5,5,5 -o $TARGET/xz.png -b $BOUNDS

# yz image
$EXEC -i $SRC -v 0,1,0 -w 0,0,1 -s $SCALE -p 5,5,5 -o $TARGET/yz.png -b $BOUNDS

# yz image
$EXEC -i $SRC -v 1,0,0 -w 0,1,0 -s $SCALE -p 5,5,5 -o $TARGET/xy.png -b $BOUNDS

# diagonal 1
$EXEC -i $SRC -v 1,1,0 -w 0,0,1 -s $SCALE -p 5,5,5 -o $TARGET/diag1.png -b $BOUNDS

# diagonal 2
$EXEC -i $SRC -v -1,1,0 -w 0,0,1 -s $SCALE -p 5,5,5 -o $TARGET/diag2.png -b $BOUNDS

# diagonal 1
$EXEC -i $SRC -v 2-3 -w 0,0,1 -s $SCALE -p 5,5,5 -o $TARGET/diag1_equiv.png -b $BOUNDS

# diagonal 2
$EXEC -i $SRC -v 4-5 -w 0,0,1 -s $SCALE -p 5,5,5 -o $TARGET/diag2_equiv.png -b $BOUNDS

#
# CH4 in "skewed" triclinic unit cell
#

SRC="CHGCAR_CH4_SU"

# xy image
$EXEC -i $SRC -v 1,0,0 -w 0,1,0 -s $SCALE -p 1 -o $TARGET/xy_su.png -b $BOUNDS

# diagonal 1 SU
$EXEC -i $SRC -v 2-3 -w 0,0,1 -s $SCALE -p 1 -o $TARGET/diag1_su.png -b $BOUNDS

# diagonal 2 SU
$EXEC -i $SRC -v 4-5 -w 0,0,1 -s $SCALE -p 1 -o $TARGET/diag2_su.png -b $BOUNDS
