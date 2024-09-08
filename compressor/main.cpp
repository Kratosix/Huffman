#include <string>
#include <fstream>
#include <iostream>
#include <cassert>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <print>
#include <queue>
struct Node{
	int weight;
	char character;
	std::string code;
	Node* left;
	Node* right;
};
struct compare{
	bool operator()(Node* l, Node* r){
		if(l->weight==r->weight){
			return l->character>r->character;
		}
		return l->weight>r->weight;
	}
};
Node* createNode(char character, int weight){
	Node* node=new Node;
	node->character=character;
	node->weight=weight;
	node->left=nullptr;
	node->right=nullptr;
	return node;
}
namespace Logger{
	void printNode(Node* node){
		char character=node->character;
		int weight=node->weight;
		std::cout<<"Character: "<<character<<"."<<"Weight: "<<weight<<"\n";
	}
	void printVectorNode(std::vector<Node*> node){
		for(const auto& vec_node:node){
			std::cout<<"Character: "<<vec_node->character<<". "<<vec_node->weight<<"\n";
		}
	}
}
struct NodeIterator{
	private:
	std::size_t currentIndex;
	std::vector<Node*> node;
	public:
	NodeIterator(std::size_t currentIndex, std::vector<Node*> node):currentIndex(currentIndex), node(node){}
	Node* currentNode(){
		if(currentIndex>node.size())	throw "Out of range";
		return node[currentIndex++];
	}
	Node* nextNode(){
		return currentNode();
	}
};
void traversePreOrder(Node* temp){
	if(temp!=NULL){
		std::cout<<temp->character<<"."<<temp->weight<<'\n';
		traversePreOrder(temp->left);
		traversePreOrder(temp->right);
	}
}
bool isLeaf(Node* node){
	return !node->left && !node->right;
}
void encode(Node* root_tree, std::string binCodes, std::unordered_map<char, std::string>& codeTable){
	if(root_tree==nullptr){return;}
	if(isLeaf(root_tree)){
		codeTable[root_tree->character]=binCodes;
	}
	encode(root_tree->left, binCodes+"0", codeTable);
	encode(root_tree->right, binCodes+"1", codeTable);
}
void decode(Node* root_tree, int& index, std::string binCodes){
	if(root_tree==nullptr){
		return;
	}
	if(isLeaf(root_tree)){
		std::cout<<root_tree->character;
		return;
	}
	index++;
	if(binCodes[index]=='0'){
		decode(root_tree->left, index, binCodes);
	}
	else{
		decode(root_tree->right, index, binCodes);
	}
}
class Huffman{
	private:
	std::string path;
	public:
	Huffman(std::string path):path{path}{}
	Huffman(Huffman&)=delete;
	std::unordered_map<char,int> scan(){
		static std::ifstream file(path);
		if(!file)	std::cerr<<"failed to load file\n";
		static std::string input;
		static std::string source;
		while(file){
			std::getline(file, input);
			source+=input;
		}
		static std::unordered_map<char, int> occurences;
		auto bit_encodings_table=[](){
			static std::unordered_map<char, int> freqmap;
			for(char c:source){
				freqmap[c]++;
			}
			std::vector<std::pair<char, int>> freqvec(freqmap.begin(), freqmap.end());
			std::priority_queue<Node*, std::vector<Node*>, compare> node;
			for(auto& pair:freqvec){
				node.push(createNode(pair.first, pair.second));
			}
			while(node.size()!=1){
				Node* small_1=node.top();
				node.pop();
				Node* small_2=node.top();
				node.pop();
				Node* bigNode=createNode('$', small_1->weight+small_2->weight);
				bigNode->left=small_1;
				bigNode->right=small_2;
				node.push(bigNode);
			}
			Node* root=node.top();
			std::unordered_map<char, std::string> encodedBins;
			encode(root, "", encodedBins);
			std::string encodedString="";
			for(char c:source){
				encodedString+=encodedBins[c];
			}
			std::print("{}\n", encodedString);
			int index=-1;
			while(index < (int)encodedString.size()-2){
				decode(root, index, encodedString);
			}
		};
		bit_encodings_table();
		return occurences;
	}
};
int main(){
	Huffman("file.txt").scan();
}
