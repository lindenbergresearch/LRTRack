#!/bin/bash

PARAM=$1
RACKDIR="../.."


make clean
make

[[ $? != 0 ]] && exit 1

[[ ${PARAM} == "run" ]] || exit


pushd $RACKDIR
  make clean
  make run
popd
