#!/bin/bash
#PBS -P PR294
#PBS -q serial
#PBS -l select=1:ncpus=12:mem=46GB
#PBS -l walltime=70:00:00
#PBS -o compress_log.txt
#PBS -e error_compress.txt

WDPATH=/scratch/scmsj4/ijcai_2016/knowledge_graph_embedding/temp/testing_data/experimental_dataset/collection
cd ${WDPATH}
tar -c wikipedia_small_docs | /home/scmsj4/parallel_applications/pbzip2-1.1.13/pbzip2 -vc > /scratch/scmsj4/ijcai_2016/knowledge_graph_embedding/temp/testing_data/experimental_dataset/collection/wikipedia_small_docs.tar.bz2

