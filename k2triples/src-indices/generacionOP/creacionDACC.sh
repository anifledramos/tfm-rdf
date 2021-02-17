#!/bin/bash

if [ "$#" != "1" ];then 
  echo 'uso:./creacionDACC.sh <entrada>'
  exit 1
fi


echo 'Creacion de índice OP'

./create_indexOP $1 opDAC

./create_dac opDAC opDAC.dacc opDAC.ind opDAC.dicc log.txt opDAC.info

