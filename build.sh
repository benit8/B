#!/bin/bash
set -e

buildDir="build"

if [ ! -d "$buildDir" ]; then
	mkdir "$buildDir"
fi

clear
pushd "$buildDir"

cmake .. -G "Unix Makefiles"
cmake --build . -- -j $(nproc)

popd