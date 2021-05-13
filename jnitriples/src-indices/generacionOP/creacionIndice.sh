#!/bin/bash

if [ "$#" != "1" ];then 
  echo 'uso:./creacionIndice.sh <entrada>'
  exit 1
fi

./creacionDACC.sh $1
./comprobacionDACC.sh $1
./borradoAuxDACC.sh
