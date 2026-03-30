#!/bin/bash
#
# script to compile the Powtex application with cmake

cd build

rm -r * 

cmake -DGeant4_DIR=~/g4new/geant4-11.0.2-install/lib/Geant4-11.0.2 ../
echo "   "
echo "compiling Powtex.cc........"
echo "   "
make -j4
echo "  "

if [ -e Powtex ]
then
    echo "compilation ok!"
	echo "now copying the executable to the POWTEX/ directory...."
	cp Powtex ../
	echo "done!....!"
else
    echo "compilation failed!"
fi

cd ..



