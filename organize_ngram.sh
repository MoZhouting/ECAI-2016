#!/bin/bash
#PBS -P PR294
#PBS -q serial
#PBS -l select=1:ncpus=12:mem=46GB
#PBS -l walltime=70:00:00
#PBS -o organize_progress.txt
#PBS -e organize_error.txt

WDPATH=/scratch/scmsj4/ijcai_2016/knowledge_graph_embedding/temp/testing_data/experimental_dataset/intermediate_files
cd ${WDPATH}

#This script will organize the n-gram file such that the hsearch() program can use it for finding the set of files which contain the words.

#sort -k2 hyphenated_trigrams.txt > hyphenated_trigrams_copy.txt
cat hyphenated_quadgrams.txt | parallel --pipe --files sort | parallel -Xj1 sort -k2 -m {} ';' rm {} > hyphenated_quadgrams_copy.txt

awk 'FNR==NR{A=$1;$1="";array[$0]=array[$0]?array[$0] "_" A:A;next} {$1="";B=$0} (B in array){C=B;sub(/^[[:space:]]+/,X,B);print array[C] OFS B;delete array[C]}' hyphenated_quadgrams_copy.txt hyphenated_quadgrams_copy.txt > organized_quadgrams.txt

