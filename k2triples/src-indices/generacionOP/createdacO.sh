#!/bin/bash

rm dataset.triples
ln -s ../../../../datasetsTriples/dbpedia.hdt.triples dataset.triples
./creacionIndice.sh 65200769
cp spDAC.dacc spDAC.dicc spDAC.ind spDAC.info datasets/dbpedia/

