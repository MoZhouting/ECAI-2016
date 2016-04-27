#This script will remove HTML tags from the WIkipedia pages. Some of the tags could not be removed.
#!/bin/bash
#PBS -P PR294
#PBS -q serial
#PBS -l select=1:ncpus=1
#PBS -l walltime=70:00:00
#PBS -o progress.txt
#PBS -e error.txt

WDPATH=/scratch/scmsj4/ijcai_2016/knowledge_graph_embedding/temp/testing_data/experimental_dataset/collection/wikipedia_small_docs
cd ${WDPATH}

file_counter=1
num_files=`find . -type f -name "*.txt" | wc -l`
while [ "$file_counter" -le "$num_files" ];
do
echo "$file_counter".txt
sed -i '/doc id\=/d' $file_counter.txt
file_counter=$(($file_counter + 1))
done

