positives=[]

file = open("positive-words.txt", "r")

for line in file:
    #if line starts with a ; discard it
    if line.startswith(";") == False:
        term = line.strip(" ")
        positives.append(term)
file.close()


rightfile = open("positivetest.txt", "w")
for word in positives:
    rightfile.write(word + "")

rightfile.close()


