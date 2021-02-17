#!/bin/bash

if [ "$#" != "1" ];then 
  echo 'uso:./comprobacionDACC.sh <entrada>'
  exit 1
fi


echo 'Preparando comprobacion...'

sort -g log.txt > logu.txt

./comprobador $1 comparacion.txt

sort -g comparacion.txt > comparacionOrdenado.txt

uniq comparacionOrdenado.txt > original.txt

echo 'DIFERENCIAS'
diff original.txt logu.txt
echo '-----------'


