#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <iomanip>
#include <fstream>
#include <bitset>
using namespace std;

#define EMPTY_STRING ""

// A Tree node
struct Node
{
	char ch;
	int freq;
	Node *left, *right;
};

// Function to allocate a new tree node
Node* getNode(char ch, int freq, Node* left, Node* right)
{
	Node* node = new Node();

	node->ch = ch;
	node->freq = freq;
	node->left = left;
	node->right = right;

	return node;
}

// Comparison object to be used to order the heap
struct comp
{
	bool operator()(Node* l, Node* r)
	{
		// highest priority item has lowest frequency
		return l->freq > r->freq;
	}
};

// Utility function to check if Huffman Tree contains only a single node
bool isLeaf(Node* root) {
	return root->left == nullptr && root->right == nullptr;
}

// Traverse the Huffman Tree and store Huffman Codes in a map.
void encode(Node* root, string str, unordered_map<char, string> &huffmanCode)
{
	if (root == nullptr) {
		return;
	}

	// found a leaf node
	if (isLeaf(root)) {
		huffmanCode[root->ch] = (str != EMPTY_STRING) ? str : "1";
	}

	encode(root->left, str + "0", huffmanCode);
	encode(root->right, str + "1", huffmanCode);
}

// Traverse the Huffman Tree and decode the encoded string
void decode(Node* root, int &index, string str)
{
	if (root == nullptr) {
		return;
	}

	// found a leaf node
	if (isLeaf(root)) {
		cout << root->ch;
		return;
	}

	index++;

	if (str[index] == '0') {
		decode(root->left, index, str);
	}
	else {
		decode(root->right, index, str);
	}
}

// Builds Huffman Tree and decode given input text
string buildHuffmanTree(string text)
{
	// base case: empty string
	if (text == EMPTY_STRING) {
		return text;
	}

	// count frequency of appearance of each character
	// and store it in a map
	unordered_map<char, int> freq;
	for (char ch : text) {
		freq[ch]++;
	}

	// Create a priority queue to store live nodes of
	// Huffman tree;
	priority_queue<Node*, vector<Node*>, comp> pq;

	// Create a leaf node for each character and add it
	// to the priority queue.
	for (auto pair : freq) {
		pq.push(getNode(pair.first, pair.second, nullptr, nullptr));
	}

	// do till there is more than one node in the queue
	while (pq.size() != 1)
	{
		// Remove the two nodes of highest priority
		// (lowest frequency) from the queue

		Node *left = pq.top(); pq.pop();
		Node *right = pq.top();    pq.pop();

		// Create a new internal node with these two nodes
		// as children and with frequency equal to the sum
		// of the two nodes' frequencies. Add the new node
		// to the priority queue.

		int sum = left->freq + right->freq;
		pq.push(getNode('\0', sum, left, right));
	}

	// root stores pointer to root of Huffman Tree
	Node* root = pq.top();

	// Traverse the Huffman Tree and store Huffman Codes
	// in a map. Also print them
	unordered_map<char, string> huffmanCode;
	encode(root, EMPTY_STRING, huffmanCode);

	cout << "Huffman Codes are:\n" << '\n';
	cout << left << setw(15) << "Character" << setw(10) << "Code" << setw(10) << "Frequency" << endl;
	cout << setfill('-') << setw(40) << "-" << endl;
	cout << setfill(' ');
	for (auto pair : huffmanCode) {
		cout << setw(15) << pair.first << setw(10) << pair.second << setw(10) << freq[pair.first] << '\n';
	}

	cout << "\nOriginal string is:\n" << text << '\n';

	// Print encoded string
	string str;
	for (char ch : text) {
		str += huffmanCode[ch];
	}

	cout << "\nEncoded string is:\n" << str << '\n';
	cout << "\nDecoded string is:\n";

	if (isLeaf(root))
	{
		// Special case: For input like a, aa, aaa, etc
		while (root->freq--) {
			cout << root->ch;
		}
	}
	else
	{
		// Traverse the Huffman Tree again and this time
		// decode the encoded string
		int index = -1;
		while (index < (int)str.size() - 1) {
			decode(root, index, str);
		}
	}
	return str;
}

// Converts string of 8 huffman codes into char
char byteToChar(string byte) {
	bitset<8> temp(byte);
	return temp.to_ulong();
}

// Compresses the encoded string
string compress(string str) {
	string text = "";
	for (int i = 0; i < str.length(); i += 8) {
		if (i + 8 >= str.length()) 
			break;
		char character = byteToChar(str.substr(i, i + 8));
		text += character;
	}
	return text;
}

// Reads a file and writes the encoded string into an output file
void encodeFile(string fileName) {
	//Read a file
	string text = "";
	ifstream file(fileName);
	if (file.is_open()) {
		string line = "";
		while (getline(file, line)) {
			text += line;
		}
		file.close();
	}
	//Encode the file into a new binary file
	ofstream outputFile("encoded" + fileName, ios::out | ios::binary);
	if (!outputFile) {
		cout << "Cannot open file" << endl;
		return;
	}
	string compressedStr = compress(buildHuffmanTree(text));
	for (int i = 0; i < compressedStr.length(); i++) {
		outputFile << compressedStr[i];
	}
	outputFile.close();
}


// Huffman coding algorithm
int main()
{
	/*
	string text = "";
	ifstream file("input1.txt");
	if (file.is_open()) {
		string line = "";
		while (getline(file, line)) {
			text += line;
		}
		file.close();
	}
	*/
	//encodeFile("1", text);
	//buildHuffmanTree(text); //build the encoded string
	//compress(buildHuffmanTree(text)); //compress the encoded string

	encodeFile("input1.txt");



	system("PAUSE");
	return 0;
}