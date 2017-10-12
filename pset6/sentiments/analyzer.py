import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        # import positive and negative words into a list
        #create positives list
        self.positives=[]
        #open positives file
        file = open("positive-words.txt", "r")
        #for each line in the file
        for line in file:
            #exclude comment lines
            if line.startswith(";") == False:
                #strip whitespace before and afterwards
                term = line.strip("\n")
                #add line (now word) to list
                self.positives.append(term)
        #close positives file
        file.close()
        #create negatives list and open negatives file
        self.negatives=[]
        file2 = open("negative-words.txt")
        #for each line in the file
        for line in file2:
            #exclude comment lines
            if line.startswith(";") == False:
                #strip whitespace before and afterwards
                term = line.strip("\n")
                #add line (now word) to list
                self.negatives.append(term)
        #close negatives file
        file2.close()
        #printout of text file with positive words for test and check purposes
        """rightfile = open("positivetest.txt", "w")
        for word in self.positives:
            rightfile.write(word + "" + "\n")


        rightfile.close()"""







    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        #implement tweet tokenizer to split tweets into individual words
        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)
        #iterate over words in tweet
        wordsum = 0
        for word in tokens:
            #make the word lowercase
            lcword = word.lower()
            #if positive, increase the sum by 1
            if lcword in self.positives:
                wordsum += 1
            #if negative, decrease word by -1
            if lcword in self.negatives:
                wordsum += -1
            #otherwise word is neutral
            else:
                wordsum += 0
        return wordsum

