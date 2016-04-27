#Run this program in a copy of the original files otherwise, disaster.....
#!/bin/bash
#PBS -P PR294
#PBS -q serial
#PBS -l select=1:ncpus=1
#PBS -l walltime=70:00:00
#PBS -o bigrams.txt
#PBS -e error_bigrams.txt

WDPATH=/scratch/scmsj4/ijcai_2016/knowledge_graph_embedding/temp/testing_data/experimental_dataset/collection/wikipedia_small_docs/
cd ${WDPATH}

#file_counter=1
file_counter=2615411
#num_files=`find . -type f -name "*.tmp" | wc -l`
#while [ "$file_counter" -le "$num_files" ];
while [ "$file_counter" -le 3019189 ];
do
#rename $file_counter.tmp $file_counter.txt $file_counter.tmp
cp $file_counter.txt /scratch/scmsj4/ijcai_2016/knowledge_graph_embedding/temp/testing_data/experimental_dataset/collection/backup/backup_before_generating_ngrams/
sed -i '/doc id\=/d' $file_counter.txt #deleted first line.
sed -i '$d' $file_counter.txt #delete last line.

/scratch/scmsj4/ijcai_2016/knowledge_graph_embedding/temp/testing_data/experimental_dataset/scripts/ngram_tool/text2ngram -n2 $file_counter.txt > $file_counter.bi

awk 'NF{NF-=1};1' $file_counter.bi > $file_counter.bi.tmp #remove last column
mv $file_counter.bi.tmp ./$file_counter.bi
sed -i "s/^/$file_counter.txt /" $file_counter.bi
mv $file_counter.bi ../bigrams
file_counter=$(($file_counter + 1))
done

