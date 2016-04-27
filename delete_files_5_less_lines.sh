#!/bin/bash
#PBS -P PR294
#PBS -q serial
#PBS -l select=1:ncpus=1
#PBS -l walltime=70:00:00
#PBS -o file_deleter.txt
#PBS -e error_file_deleter.txt

WDPATH=/scratch/scmsj4/ijcai_2016/knowledge_graph_embedding/temp/testing_data/experimental_dataset/collection/wikipedia_small_docs/
cd ${WDPATH}

find . -name "*.txt" -type f |
while read f; do
	i=0
	while read line; do
		i=$((i+1))
		[ $i -eq 7 ] && continue 2
	done < "$f"
	printf %s\\n "$f"
done |
xargs rm -f
num=1
for file in *.txt;
do
       mv "$file" "$(printf "%u" $num).dat"
       num=$(($num + 1))
done
find . -name '*.dat' | xargs rename '.dat' .txt
