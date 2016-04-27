#!/bin/bash
#PBS -P PR294
#PBS -q serial
#PBS -l select=1:ncpus=1
#PBS -l walltime=70:00:00
#PBS -o progress.txt
#PBS -e error.txt

WDPATH=/scratch/scmsj4/ijcai_2016/knowledge_graph_embedding/temp/testing_data
cd ${WDPATH}
sh n_gram_generator.sh 250000 1

