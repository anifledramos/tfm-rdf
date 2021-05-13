#!/bin/bash

if [ "$#" != "1" ];then 
  echo 'uso:./creacionDACC.sh <entrada>'
  exit 1
fi


echo 'Creacion de índice SP'

./create_indexSP $1 spDAC

./create_dac spDAC spDAC.dacc spDAC.ind spDAC.dicc log.txt spDAC.info

