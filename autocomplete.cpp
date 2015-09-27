
#include <stdlib.h>
#include <string>
#include <map>
#include <iostream>
#include <istream>
#include <fstream>
#include <deque>
#include <vector>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <algorithm> 


using namespace std;

const double PROGRESS_BAR_MAX_SIZE = 50;

//Node of ternary tree
class Node
{
friend class TernaryTree;
private:
	char sym; //symbol in the node
	Node * left;
	Node * center;
	Node * right;
	bool wordEnd; //if this is last node for some word
	int freq; //if wordEnd == true ==> freq - frequency of the word read from file, otherwise - 0
	int highest_freq_mark; //this mark tells what highest frequency we can reach in this node subtree  
	int highest_freq; //highest word frequency among all words with corresponding prefix (prefix that ends with this node)

public:
	Node(char ch, bool wordEnd)
	{
		this->sym = ch;
		this->wordEnd = wordEnd;
		
		left = NULL;
		right = NULL;
		center = NULL;
		
		this->freq = 0;
		this->highest_freq_mark = 0;
		this->highest_freq = 0;
	
	}
	~Node(){
	}
};

//ternary tree for storing dictionary
class TernaryTree
{
private :
	Node * root;
	
	int size; //number of symbols in the tree, used only for destrustor progress bar
	
	//add word's "s" tail (starting with "pos" position) to the tree starting with node "node", "freq" -frequency of "s" 
	void add(char* s, int pos, Node *& node, int freq)
	{
		if (node == NULL){
				node = new Node(s[pos], false);
				if (s[pos + 1] == '\0'){
					node->wordEnd = true;
					node->freq = freq;
				}
				size++;
		}
		if (s[pos] < node->sym) 
			add(s, pos, node->left, freq); 
		
		else if (s[pos] > node->sym) 
			add(s, pos, node->right, freq); 
		else if (s[pos + 1] != '\0')
			add(s, pos + 1, node->center, freq); 
		
		//recalculate node parameters
		node->highest_freq = calculateHighestFreq(node);
		node->highest_freq_mark = calculateHighestFreqMark(node);
	}

	//this mark tells what highest frequency we can reach in this node subtree  
	//helps to traverse the tree in order to find the most frequent tail for certain prefix
	int calculateHighestFreqMark(Node * node){
		if (node == NULL)
			return 0;
		
		return max(
					max(
						max	(node->freq, node->highest_freq_mark),
			    		(node->center != NULL) ? node->center->highest_freq_mark : 0),
					max(
						(node->left != NULL) ? node->left->highest_freq_mark : 0,
						(node->right != NULL) ? node->right->highest_freq_mark : 0)
			   );

	}

    //highest_freq - highest word frequency among all words with corresponding prefix (prefix that ends with this node)
    //helps to traverse the tree in order to find the most frequent tail for certain prefix
	int calculateHighestFreq(Node * node){
		
		if (node == NULL)
			return 0;

		if (node->center != NULL){
			return max(
					   max(	node->freq, 
							node->highest_freq),
				       max(	(node->center != NULL) ? node->center->highest_freq : 0, 
					   		(node->center != NULL) ? node->center->highest_freq_mark : 0)
				   );
		}

		return node->freq;
	}


	//put into buffer the most frequent tail starting with the node
	void appendMostFrequentTail(Node * node, char *buffer){
		int highest_freq = node->highest_freq;

		bool first_iteration = true;
		
		//search end when we see end of the word with the frequency equal to node->highest_freq
		//highest_freq_mark tells where to go in order to find the most frequent tail in corresponding subtree
		while (node->freq != highest_freq) {
			if ((node->left != NULL) && (highest_freq == node->left->highest_freq_mark))
				node = node->left;
			if ((node->right != NULL) && (highest_freq == node->right->highest_freq_mark))
				node = node->right;
			if ((node->center != NULL) && (highest_freq == node->center->highest_freq_mark)){
				if (!first_iteration) {
					*buffer = node->sym;
					buffer++;
				}
				node = node->center;
			}
			first_iteration = false;
		}

		if (!first_iteration) {
			*buffer = node->sym;
			buffer++;
		}
		*buffer = '\0';
		return;
	}

	//print tree starting with some node (for testing)
	void print(Node *n, int i = 0){

		for (int j = 1; j <= i; j++){
			if (j == i) cout << "|  ";
			else cout << "   ";
		}

		if (n == NULL) {
			cout << "NULL" << endl;
			return;
		}

		cout << n->sym << " " << n->freq << " " << n->highest_freq << " " << n->highest_freq_mark << endl;
		print(n->left, i + 1);
		print(n->center, i + 1);
		print(n->right, i + 1);
	}


public :

	TernaryTree(){
		root = NULL;
		size == 0;
	}
	
	//destructor performs breadth tree traversal and deletes all nodes 
	~TernaryTree(){
		cout << "delete dictionary tree..." << endl;
		deque<Node*> * deleteList = new deque < Node * >;
		int deleted = 0;
		int progressBarCurSize = 0;
		int progressBarPrevSize = 0;

		deleteList->push_back(root);
		while (deleteList->size() > 0){
			Node * toDelete = deleteList->front();
			deleted++;
			progressBarCurSize = round(((double)deleted / (double)size) * PROGRESS_BAR_MAX_SIZE);

			for (int i = 0; i < progressBarCurSize - progressBarPrevSize; i++){
				cout << "#";
				cout.flush();
			}
			progressBarPrevSize = progressBarCurSize;
			if (toDelete->left != NULL) deleteList->push_back(toDelete->left);
			if (toDelete->center != NULL) deleteList->push_back(toDelete->center);
			if (toDelete->right != NULL) deleteList->push_back(toDelete->right);
			deleteList->pop_front();
			delete toDelete;
		}
		cout << endl;
		delete deleteList;
		cout << "dictionary tree deleted" << endl;
	}

	//add word "s" with frequency "freq" to the tree  
	void add(char* s, int freq){
		if (*s == '\0') 
			return;

		add(s, 0, root, freq);
	}

	//put into "dest" the most frequent word with prefix equal to "prefix"
	void getMostFrequentWordByPrefix(char * prefix, char *dest){
		if (*prefix == '\0') {
			*dest = '\0';
			return;
		}

	    int pos = 0;
		Node * node = root;
		while (node != NULL)
		{
			//find the prefix in the tree, go to the node that corresponds to the prefix end
			if (prefix[pos] < node->sym) { node = node->left; }
			else if (prefix[pos] > node->sym) { node = node->right; }
			else
			{
				
				if (prefix[++pos] == '\0') {//end of the prefix found in the tree
					int i;
					//copy prefix to "dest"
					for (i = 0; i < pos; i++){
						dest[i] = prefix[i];
                    }
                    //append to "dest" the most frequent tail in corresponding subtree 
					appendMostFrequentTail(node, dest + i);
					//and return
					return;
				}
				node = node->center;
			}
		}
		
		
		//if prefix search hasn't reach it's end ==> tree doesn't contain words with such prefix
		if (prefix[pos] != '\0') {
			*dest = '\0';
			return;
		}
		/*else{
			int i;
			for (i = 0; prefix[i] != '\0'; i++)
				dest[i++] = prefix[i];
			
			dest[i] = '\0';
		}*/

		return;
	}

	
	//print tree the whole tree (for testing)
	void print(){
		print(root, 0);
	}
};

//dictionary file record
struct FileRecord{
	char * word; //word
	int number;  //word frequency

	FileRecord(char * w, int n){
		word = w;
		number = n;
	}
};

/*
//random word generator for testing
char alf[26] =  { 'a', 'b', 'c', 'd', 'i', 'f', 'j', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
void randWord(int maxLen, char *dest){
	int len = rand() % maxLen;
	
	int ind;
	int i;
	for (i = 0; i < len; i++){
		ind = rand() % 26;
		dest[i] = alf[ind];
	}
	dest[i] = '\0';
	return;
}
*/

int main(int argc, char* argv[])
{
	const clock_t begin_time = clock();

	TernaryTree *tree = new TernaryTree();

	FILE * pFile;
	long lSize;
	char * buffer;
	size_t result;

	pFile = fopen("wiki.dic", "rb");
	if (pFile == NULL) { 
		fputs("File error", stdout); 
		exit(1); 
		cout << "NULL" << endl;
	}
	cout << "read dictionary file..." << endl;
	
	// obtain file size:
	fseek(pFile, 0, SEEK_END);
	lSize = ftell(pFile);
	rewind(pFile);

	// allocate memory to contain the whole file:
	buffer = (char*)malloc(sizeof(char)*lSize);
	if (buffer == NULL) { 
		fputs("Memory error", stderr); exit(2); 
	}

	// copy the file into the buffer:
	result = fread(buffer, 1, lSize, pFile);
	if (result != lSize) { 
		fputs("Reading error", stderr); exit(3); }
	
	int i = 0;
	char *c = buffer;
	int j = 0;

	//vector to store dictionary records
	vector<FileRecord> *dictionaryVector = new vector<FileRecord>;

	cout << endl;
	cout << "parsing..." << endl;

	//parse dictionary, put records into dictionaryVector
	while (i != lSize){
		char * ptr = c;
		while (*ptr != '\t' &&  *ptr != ' '){
			ptr++;
		}
		*ptr = '\0';
		int wordLength = ptr - c + 1;
		while (!isdigit(*ptr)){
		 	ptr++;
		}

		char *numberStart = ptr;
		while (*ptr != '\n'){ 
		 	ptr++; 
		}
		*ptr = '\0';

		dictionaryVector->push_back(FileRecord(c, atoi(numberStart)));
		i += ptr - c + 1;
		c = ++ptr;
	}
	cout << "dictionary size : " << dictionaryVector->size() << " words" << endl;

    //shuffling the dictionary in order to optimize ternary tree operations
	random_shuffle(dictionaryVector->begin(), dictionaryVector->end());
	
	//progress bar parameters
	int barPrevSize = 0;
	int barCurSize = 0;
	
	cout << endl;
	for (int j = 0; j < PROGRESS_BAR_MAX_SIZE; j++)
		cout << "#";
	cout << endl;

	vector<FileRecord>::iterator it;
	//adding records to ternary search tree, drawing progress bar
	for (it = dictionaryVector->begin(), i = 0; it != dictionaryVector->end(); it++, i++){
		tree->add(it->word, it->number);
		barCurSize = round((double)i / (double)dictionaryVector->size() * (double)PROGRESS_BAR_MAX_SIZE);
		if (barCurSize > barPrevSize)
			for (int j = 0; j < barCurSize - barPrevSize; j++)
				cout << "#";

		barPrevSize = barCurSize;
		cout.flush();
	}
	cout << endl;
	cout << "time for start :" << float(clock() - begin_time) / CLOCKS_PER_SEC << "sec " << endl;
	
	char str[1024]; //string entered by user
	char dest[1024]; //search result
	
	//query ternary tree
	while (scanf("%s\0", str) != EOF)
	{
		tree->getMostFrequentWordByPrefix(str, dest);
		cout << dest << endl;
	}

	/*
	const clock_t begin_time1 = clock();

	char test[1024];
	for (int j = 0; j < 300; j++){
		randWord(5, test);
		tree.getMostFrequentWordByPrefix(test, dest);
		cout << "test : "<< test << endl;
		cout << "dest : " << dest << endl;

	}
	cout << float(clock() - begin_time1) / CLOCKS_PER_SEC;
	*/

	delete dictionaryVector;
	delete tree;
	getchar();
	fclose(pFile);
	free(buffer);
	return 0;
}

