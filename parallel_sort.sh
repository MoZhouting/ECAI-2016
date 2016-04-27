#!/bin/bash
#PBS -P PR294
#PBS -q serial
#PBS -l select=1:ncpus=12:mem=40GB
#PBS -l walltime=70:00:00
#PBS -o sorted_output.txt
#PBS -e error.txt

WDPATH=/scratch/scmsj4/ijcai_2016/knowledge_graph_embedding/temp/testing_data/experimental_dataset/intermediate_files/trigrams
cd ${WDPATH}
cat trigram_text.txt | parallel --pipe --files sort | parallel -Xj16 sort -k2 -m {} ';' rm {}

