#!/bin/sh

buildDir='.build'

if [ ! -d "./$buildDir" ]; then
	mkdir "./$buildDir"
fi

clear ; clear

cd "./$buildDir" &&
cmake .. -G "Unix Makefiles" &&
cmake --build . -- -j $(nproc) &&
cp libB* ../tests/