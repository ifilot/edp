#!/bin/bash

# set version number
PACKAGE_NAME="edp"
VERSION_MAJOR=`grep "VERSION_MAJOR" ./src/CMakeLists.txt | awk '{print $2}' | grep -Eo "[0-9]+"`
VERSION_MINOR=`grep "VERSION_MINOR" ./src/CMakeLists.txt | awk '{print $2}' | grep -Eo "[0-9]+"`
VERSION_MICRO=`grep "VERSION_MICRO" ./src/CMakeLists.txt | awk '{print $2}' | grep -Eo "[0-9]+"`

# build directories
DISTPATH=${PACKAGE_NAME}-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_MICRO}
mkdir -pv deb/${DISTPATH}
mkdir -pv deb/${DISTPATH}/{DEBIAN,usr/bin,usr/share/doc/edp}

# copy files
sed -e "2 s/$/ ${PACKAGE_NAME}-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_MICRO}/" package/copyright > deb/${DISTPATH}/usr/share/doc/edp/copyright
sed -e "1 s/$/ ${PACKAGE_NAME}-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_MICRO}/" package/control > deb/${DISTPATH}/DEBIAN/control
objcopy --strip-debug --strip-unneeded ./build/edp deb/${DISTPATH}/usr/bin/edp

# build package
pushd deb
fakeroot dpkg-deb --build ${DISTPATH}
cp ${DISTPATH}.deb ../
popd

# clean-up folder
rm -rvf deb

# perform lintian
lintian ${DISTPATH}.deb
