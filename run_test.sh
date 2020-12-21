#!/bin/bash
oldFolder=$(pwd)

cd test
make
bin/test01_measure

cd $oldFolder