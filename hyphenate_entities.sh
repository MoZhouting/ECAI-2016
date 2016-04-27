cd bigrams
num_files=`find . -type f -name "*.txt" | wc -l`
file_counter=1
while [ "$file_counter" -le "$num_files" ];
do
echo $file_counter.txt
#extract the first column
awk '{print $1}' $file_counter.txt > first_col

#remove first column and extra one character from every line
awk '{$1=""}1' OFS=" " $file_counter.txt | sed 's .\{1\}  '  > words

#now hyphenate the words.
tr ' ' '-' <words > hyphenated

sed -i "s/^/sed -i \'s\/\</" words #add text beginning of each line
sed -i "s/$/\>\//" words #add text end of each line

sed -i "s/$/\/g\'/" hyphenated #add text end of each line

paste -d'\0' words hyphenated > $file_counter.dat
paste -d' ' $file_counter.dat first_col >tmp
mv tmp ./$file_counter.dat
unlink $file_counter.txt
file_counter=$(($file_counter + 1))
done
