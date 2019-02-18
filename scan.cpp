#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include "RBTree.h"
#include "SimpleHTMLParser.h"
#include "Node.h"
using namespace std;

// Output stream operator overload for node elements
ostream& operator<<(ostream& out, const vector<pair<string, int>>& e);

int main(int argc, char** argv) {
	// Implement your scan code here
	string urls;
	RBTree<string, vector<pair<string, int>>>* rbt;
	urls = argv[1];
	
	ifstream infile (urls);
	SimpleHTMLParser *parser = new SimpleHTMLParser();
	stringstream ss(urls);
	string url;
	string texts;
	getline(infile, url);

	while (getline(infile,url)) {
		parser->parse(url);
		texts = parser->getText();
		istringstream iss(texts);
		string text;
		for(text;iss >> text;){
			if(rbt->search(text)==NULL){
				pair<string, int> p = make_pair(url,1);
				vector<pair<string, int>> v;
				v.push_back(p);
				rbt->insert(text,v);
			}
			else{
				int flag = 0;
				Node<string, vector<pair<string, int>>>* temp = rbt->search(text);
				vector<pair<string, int>> vt = temp->getElement();
				for(int i=0; i<vt.size(); i++){
					if(vt[i].first == url){
						vt[i].second++;
						flag = 1;
						temp->setElement(vt);
						break;
					}
				}
				if(flag == 0){
					pair<string, int> pt = make_pair(url,1);
					vt.push_back(pt);
					temp->setElement(vt);
				}

			}
		}
	}
	ofstream outfile;
	outfile.open(argv[2]);
	outfile << *rbt << endl;
	outfile.close();
	return 0;
}


// This function overloads the output stream operator for Node
// elements. It has been provided to you and does not need to be
// altered.
ostream& operator<<(ostream& out, const vector<pair<string, int>>& e) {
	// Write the number of elements in the vector
	out << e.size() << endl;
	for (auto& uf : e)
		// Write each member of each pair the vector stores
		out << uf.first << " " << uf.second << endl;
	return out;
}
