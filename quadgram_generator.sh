#Run this program in a copy of the original files otherwise, disaster.....
#FIRST RUN THE BIGRAM COUNTER, WAIT FOR SOME TIME AND THEN RUN TRI AND QUADGRAM COUNTERS
#!/bin/bash
#PBS -P PR294
#PBS -q serial
#PBS -l select=1:ncpus=1
#PBS -l walltime=70:00:00
#PBS -o quadgrams.txt
#PBS -e error_quadgrams.txt

WDPATH=/scratch/scmsj4/ijcai_2016/knowledge_graph_embedding/temp/testing_data/experimental_dataset/collection/wikipedia_small_docs/
cd ${WDPATH}

#file_counter=1
file_counter=2615411
#num_files=`find . -type f -name "*.txt" | wc -l`
num_files=3019189
while [ "$file_counter" -le "$num_files" ];
do
/scratch/scmsj4/ijcai_2016/knowledge_graph_embedding/temp/testing_data/experimental_dataset/scripts/ngram_tool/text2ngram -n4 $file_counter.txt > $file_counter.qua

awk 'NF{NF-=1};1' $file_counter.qua > $file_counter.qua.tmp #remove last column
mv $file_counter.qua.tmp ./$file_counter.qua
sed -i "s/^/$file_counter.txt /" $file_counter.qua
mv $file_counter.qua ../quadgrams
file_counter=$(($file_counter + 1))
done

