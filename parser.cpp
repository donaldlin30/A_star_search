/**
 * 8-Puzzle Solver Data Parser
 * 
 * This program parses the results of the 8-puzzle solver, specifically the data related to the effective branching factor (b*)
 * and the number of nodes (N) generated during the search. The results are read from a file named "h_2.txt".
 * 
 * The parsed data is then summarized to calculate the average effective branching factor and average number of nodes for each depth.
 * The summary is written to a file named "h_2_summary.txt".
 * 
 * Data Structures:
 * 1. ma1: Map that associates a depth (d) with a vector of effective branching factors (b*).
 * 2. ma2: Map that associates a depth (d) with a vector of node counts (N).
 * 3. bav: Map that associates a depth (d) with the average effective branching factor for that depth.
 * 4. nav: Map that associates a depth (d) with the average node count for that depth.
 * 
 * The program reads each line of the "h_2.txt" file, extracts the relevant data, and populates the ma1 and ma2 maps.
 * After reading all the data, it calculates the average values and populates the bav and nav maps.
 * Finally, the summary is written to the "h_2_summary.txt" file.
 * 
 */

#include <bits/stdc++.h>
#define chesb pair<vector<vector<int>>,pair<int,int>>
using namespace std;

// Global variables to store parsed data and results
map<int,vector<double>>ma1;
map<int,vector<int>>ma2;
map<int,double>bav,nav;
ifstream fin; // File input stream
ofstream fout; // File output stream

int main(){
// Open the input and output files
	fin.open("h_2.txt");
	fout.open("h_2_summary.txt");
// Parse each line of the input file and populate the ma1 and ma2 maps
	for(int i=0;i<181439;i++){
		string s;
		int n,d;
		double b;
		fin>>s>>s>>n>>s>>d>>s>>b;
		if(ma1.find(d)==ma1.end()){
			vector<double>ve;
			vector<int>ve1;
			ma1.insert(make_pair(d,ve));
			ma2.insert(make_pair(d,ve1));
		}
		ma1.at(d).push_back(b);
		ma2.at(d).push_back(n);
	}
// Calculate the average values and populate the bav and nav maps
	for(pair<int,vector<double>>pa:ma1){
		double avb=0;
		for(double b:pa.second)avb+=b;
		avb/=pa.second.size();
		bav.insert(make_pair(pa.first,avb));
		double avn=0;
		for(int n:ma2.at(pa.first))avn+=n;
		avn/=pa.second.size();
		nav.insert(make_pair(pa.first,avn));
	}
// Write the summary to the output file
	for(pair<int,double>pa:bav){
		int d=pa.first;
		fout<<"depth: "<<d<<" number of problems: "<<ma1.at(d).size()<<" average of N: "<<nav.at(d)<<" average of bstar: "<<bav.at(d)<<endl;
	}	
// Close the input and output files
	fout.close();
	fin.close();
	return 0;
}