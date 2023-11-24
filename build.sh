#!/bin/bash
premake5 gmake2
rm -r bin
gmake
./bin/mac/debug/Renderer