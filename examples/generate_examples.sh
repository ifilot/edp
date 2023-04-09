#!/bin/bash

#
# Generate example images that are used in the documentation
#

EXEC="../build/edp"
SCALE=25
TARGET="../docs/_static/img"
TEMPTARGET="./imgtest"
BOUNDS="-3,1"

# create temporary folder for testing if it does not exist
mkdir -pv $TEMPTARGET

# unpack dataset if it does not exists
if [ ! -f CHGCAR_Al_FCC111 ]
then
    tar -xvjf ../src/test/testinput/dataset.tar.bz2
fi

#
# Benzene MO #07
#

SRC="PARCHG_BENZENE_07"

# xy image
$EXEC -i $SRC -p 7.15666,7.49029,5.0 -v 1,0,0 -w 0,1,0 -s $SCALE -o $TARGET/benzene_xy_01.png

# xy image with different bounds and legend
$EXEC -i $SRC -p 7.15666,7.49029,5.0 -v 1,0,0 -w 0,1,0 -s $SCALE -o $TARGET/benzene_xy_02.png -b -5,0 -l

# xz image
$EXEC -i $SRC -v 1,0,0 -w 0,0,1 -s $SCALE -p 1 -o $TARGET/benzene_xz_01.png -b -5,0 -l

# xz image, increased scale
$EXEC -i $SRC -v 1,0,0 -w 0,0,1 -s 30 -p 1 -o $TARGET/benzene_xz_02.png -b -5,0 -l

# remove dataset files
rm -rvf planedata-*.bin

#
# Al FCC111
#

SRC="CHGCAR_Al_FCC111"

# xy image
$EXEC -i $SRC -v 1,0,0 -w 0,1,0 -s $SCALE -p 25 -o $TARGET/al_fcc111_xy.png -b -5,0 -l

# xz image including z-averaging
$EXEC -i $SRC -v 1,0,0 -w 0,0,1 -s $SCALE -p 28 -o $TARGET/al_fcc111_xz.png -b -5,0 -l -z
python al_fcc111_zavg.py

#
# CH4
#

SRC="CHGCAR_CH4"

# xy image
$EXEC -i $SRC -v 1,0,0 -w 0,1,0 -s $SCALE -p 1 -o $TARGET/ch4_xy.png -r 1,2.0
python ch4_radial.py

# xy image
$EXEC -i $SRC -v 1,0,0 -w 0,1,0 -s $SCALE -p 1 -o $TARGET/ch4_xy.png -e 1-2
python ch4_line.py

# DONE
echo "--done--"
