#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "RBTree.h"
using namespace std;

// Input stream operator overload for node elements
istream& operator>>(istream& in, vector<pair<string, int>>& e);

bool myfunction (pair<string, int> i, pair<string, int> j){
	if(i.second == j.second) return (i.first < j.first);
	else return (i.second > j.second);
}

int main(int argc, char** argv) {
	// Implement your search code here
	ifstream infile;
	int wordNum = argc - 2;
	string phrase[wordNum];
	infile.open(string(argv[1]));
	for(int i=0; i<wordNum; i++){
		phrase[i] = string(argv[2+i]);
	}	
	RBTree<string, vector<pair<string, int>>> wordTree;
	infile >> wordTree;
	vector<vector<pair<string, int>>> wordList;
	for(int i=0; i<wordNum; i++){
		transform (phrase[i].begin(), phrase[i].end(), phrase[i].begin(), ::tolower);
		vector<pair<string, int>> tempList;
		if(!wordTree.search(phrase[i]) == NULL){
			tempList = wordTree.search(phrase[i])->getElement();
			wordList.push_back(tempList);
		}
	}
	if(wordList.empty()||wordList.size()<wordNum){
		cout << "Not found" << endl;
		return 0;
	}
	if(wordList.size() == 1){
		if(wordList[0].size() == 0) cout << "Not found" << endl;
		else{
			sort(wordList[0].begin(), wordList[0].end(), myfunction);
			for(int i=0; i<wordList[0].size(); i++){
				cout << wordList[0][i].first << " " << wordList[0][i].second << endl;
			}
		}
		return 0;	
	}

	vector<vector<pair<string, int>>> interList;
	for(int i=1; i<wordList.size(); i++){
		vector<pair<string, int>> tempi;
		interList.push_back(tempi);
	}

	for(int i=0; i<wordList[0].size(); i++){
		for(int j=0; j<wordList[1].size(); j++){
			if(wordList[0][i].first == wordList[1][j].first){
				wordList[0][i].second = wordList[0][i].second + wordList[1][j].second;
				interList[0].push_back(wordList[0][i]);
			}
		}
	}

	if(wordList.size()>2){
	for(int i=2; i<wordList.size(); i++){
		for(int j=0; j<wordList[i].size(); j++){
			for(int k=0; k<interList[i-2].size(); k++){
				if(interList[i-2][k].first == wordList[i][j].first){
					interList[i-2][k].second = interList[i-2][k].second + wordList[i][j].second;
					interList[i-1].push_back(interList[i-2][k]);				
				}
			}
		}
	}
	}

	
	sort(interList[interList.size()-1].begin(), interList[interList.size()-1].end(), myfunction);
	for(int i=0; i<interList[interList.size()-1].size(); i++){
		cout << interList[interList.size()-1][i].first << " " << interList[interList.size()-1][i].second << endl;
	}
	
	return 0;
}

// This function overloads the input stream operator for Node
// elements. It has been provided to you and does not need to be
// altered.
istream& operator>>(istream& in, vector<pair<string, int>>& e) {
	// Clear any existing contents
	e.clear();

	// Read the number of elements
	size_t n;
	in >> n;
	for (int i = 0; i < n; i++) {
		// Read each member of each pair the vector stores
		string u;
		in >> u;
		int f;
		in >> f;
		// Add to vector
		e.push_back(make_pair(u, f));
	}

	return in;
}
