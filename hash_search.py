with open("/home/msjameel/ijcai_2016/knowledge_graph_embedding/temp/testing_data/experimental_dataset/intermediate_files/trigrams/trigrams.txt", "r") as f:
    dic = dict((" ".join(line.split()[:-1]), line.split()[-1]) for line in f)
    #print dic.values()
f.close()

with open('/home/msjameel/ijcai_2016/knowledge_graph_embedding/temp/testing_data/titles/common_entities/truncated_parts/hyphenated_entities.txt') as entity_file:
    for line in entity_file:
        line = line.rstrip()
        output = str(dic.get(line)) + ' ' + line
        print output
entity_file.close()

