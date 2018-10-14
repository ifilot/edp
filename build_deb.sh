#!/bin/bash

# set version number
PACKAGE_NAME="edp"
VERSION_MAJOR=`grep "VERSION_MAJOR" ./src/CMakeLists.txt | awk '{print $2}' | grep -Eo "[0-9]+"`
VERSION_MINOR=`grep "VERSION_MINOR" ./src/CMakeLists.txt | awk '{print $2}' | grep -Eo "[0-9]+"`
VERSION_MICRO=`grep "VERSION_MICRO" ./src/CMakeLists.txt | awk '{print $2}' | grep -Eo "[0-9]+"`

# build directories
DISTPATH=${PACKAGE_NAME}-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_MICRO}
mkdir -pv deb/${DISTPATH}
mkdir -pv deb/${DISTPATH}/{DEBIAN,usr/bin,usr/share/man/man1,usr/share/doc/${PACKAGE_NAME}}

# copy files
sed -e "1 s/$/ ${PACKAGE_NAME}-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_MICRO} stretch; urgency=low/" package/changelog > changelog
gzip -9 -n -c changelog > deb/${DISTPATH}/usr/share/doc/${PACKAGE_NAME}/changelog.gz
sed -e "2 s/$/ ${PACKAGE_NAME}-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_MICRO}/" package/copyright > deb/${DISTPATH}/usr/share/doc/${PACKAGE_NAME}/copyright
sed -e "1 s/$/ ${PACKAGE_NAME}-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_MICRO}/" package/control > deb/${DISTPATH}/DEBIAN/control
sed -i "9 s/$/ ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_MICRO}/" deb/${DISTPATH}/DEBIAN/control
objcopy --strip-debug --strip-unneeded ./build/${PACKAGE_NAME} deb/${DISTPATH}/usr/bin/${PACKAGE_NAME}
gzip -9 -n -c package/man/${PACKAGE_NAME} > deb/${DISTPATH}/usr/share/man/man1/${PACKAGE_NAME}.1.gz

# build package
pushd deb
fakeroot dpkg-deb --build ${DISTPATH}
cp ${DISTPATH}.deb ../
popd

# clean-up folder
rm -rvf deb
rm -vf changelog
