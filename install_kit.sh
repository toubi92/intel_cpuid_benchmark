#!/bin/bash
oldFolder=$(pwd)

cd kit
make
sudo make install

sudo rmmod bm_mod
sudo insmod bm_mod.ko

cd $oldFolder