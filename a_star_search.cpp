/**
 * 8-Puzzle Solver using A* Search Algorithm
 * 
 * This program solves the 8-puzzle problem using the A* search algorithm. The 8-puzzle is a sliding puzzle that consists
 * of a 3x3 board with eight numbered tiles and a blank space. The goal is to rearrange the tiles in ascending order.
 * 
 * Heuristics Used:
 * 1. h1: Number of misplaced tiles.
 * 2. h2: Manhattan distance (sum of the distances of the tiles from their goal positions).
 * 3. h3: Manhattan distance plus, adding an additional penalty to the h2 heuristic.
 * 
 * The program also calculates the effective branching factor (b*) using the equation:
 * N = 1 + b* + b*^2 + ... + b*^d
 * where N is the number of nodes generated and d is the depth of the solution.
 * 
 * The bisection method is used to solve for b*.
 * 
 * The program generates all possible board permutations and filters out the valid ones. A board is considered valid if
 * it can be solved by making an even number of swaps from the solution state.
 * 
 * The results, including the effective branching factor for different depths, are written to a text file.
 * 
 * Author: Daniel Zhou, Donald Lin
 */

#include <bits/stdc++.h>

// Define a chessboard as a pair of a 2D vector of integers and a pair of integers (position of the blank tile)
#define chesb pair<vector<vector<int>>,pair<int,int>>
using namespace std;

// Original placement of tiles in the goal state
int orig_place[3][3]={{1,2,3},{4,5,6},{7,8,0}};

// Global variables
vector<int>allN,alld;
vector<chesb>generated_chesb;
set<chesb>explored_chesb;
map<int,vector<double>>ma1;
ofstream fout;

/**
 * Calculate the number of misplaced tiles for the given board.
 * @param current_b The current board state.
 * @return The number of misplaced tiles.
 */
int h_1(chesb current_b){
	int sum_d=0;
	for(int i=0;i<3;i++)for(int j=0;j<3;j++)if(current_b.first[i][j]!=0&&current_b.first[i][j]!=orig_place[i][j])sum_d++;
	return sum_d;
}

/**
 * Calculate the Manhattan distance for the given board.
 * The Manhattan distance is the sum of the distances of the tiles from their goal positions.
 * @param current_b The current board state.
 * @return The Manhattan distance.
 */
int h_2(chesb current_b){
	int sum_d=0;
	for(int i=0;i<3;i++)for(int j=0;j<3;j++){
		int num=current_b.first[i][j];
		if(num==0)continue;
		int x=(num-1)/3;
		int y=(num-1)%3;
		sum_d+=abs(x-i)+abs(y-j);
	}
	return sum_d;
}


/**
 * Calculate the Manhattan distance plus for the given board.
 * The Manhattan distance plus is calculated as the sum of the distances of the tiles from their goal positions,
 * with an additional penalty for tiles being in the reverse order of their goal positions.
 * Specifically, if two tiles are in each other's goal position in the same row or column, an additional penalty of 2
 * is added to the distance for each such pair of tiles.
 * @param current_b The current board state.
 * @return The Manhattan distance plus with penalties for reverse order tiles.
 */
int h_3(chesb current_b){
	int sum_d=0;
	for(int i=0;i<3;i++)for(int j=0;j<3;j++){
		int num=current_b.first[i][j];
		if(num==0)continue;
		int x=(num-1)/3;
		int y=(num-1)%3;
		sum_d+=abs(x-i)+abs(y-j);
	}
	for(int i=0;i<2;i++)for(int j=0;j<3;j++)if(current_b.first[i][j]==orig_place[i+1][j]&&current_b.first[i+1][j]==orig_place[i][j])sum_d+=2;
	for(int i=0;i<3;i++)for(int j=0;j<2;j++)if(current_b.first[i][j]==orig_place[i][j+1]&&current_b.first[i][j+1]==orig_place[i][j])sum_d+=2;
	return sum_d;
}

// Define the node structure for the A* search
struct node{
	chesb current_b;
	int g; // Cost to reach the current node
	node(chesb current_b,int g):current_b(current_b),g(g){}
};

// Overload the '<' operator for the node structure to be used in the priority queue
bool operator<(node a, node b){
    return a.g+h_3(a.current_b)>b.g+h_3(b.current_b);
}

/**
 * Helper function for the bisection method to calculate the effective branching factor.
 * @param b The branching factor.
 * @param d The depth.
 * @return The calculated value.
 */
double to_d1(double b,int d){
	double to_return=1;
	for(int i=0;i<=d;i++){
		to_return*=b;
	}
	return to_return;
}

/**
 * Calculate the effective branching factor using the bisection method.
 * @param N The number of nodes.
 * @param d The depth.
 * @return The effective branching factor.
 */
double efb(int N,int d){
	double l,m,r;
	l=1;
	r=5;
	if((to_d1(r,d)-r)<=(r-1)*N){cout<<"error in ebf"<<endl;exit(0);}
	for(int i=0;i<12;i++){
		m=(l+r)/2;
		if((to_d1(m,d)-m)<=(m-1)*N)l=m;
		else r=m;
	}
	return l;
}


/**
 * Check if a given board is valid.
 * A board is considered valid if it can be solved by making an even number of swaps from the solution state.
 * @param current_b The board to check.
 * @return True if the board is valid, false otherwise.
 */
bool valid_ches(chesb current_b){//We need to move 0 to the right-down corner first.
	chesb var_b;
	var_b=current_b;
	int x,y;
	x=var_b.second.first;
	y=var_b.second.second;
	for(int i=x;i<2;i++)var_b.first[i][y]=var_b.first[i+1][y];
	for(int i=y;i<2;i++)var_b.first[2][i]=var_b.first[2][i+1];
	var_b.first[2][2]=0;
	int order[8]={3,2,1,6,5,4,8,7};
	int cnt=0;
	for(int i=0;i<8;i++)for(x=0;x<3;x++)for(y=0;y<3;y++){
		if(var_b.first[x][y]!=order[i])continue;
		int to_x,to_y;
		to_x=(order[i]-1)/3;
		to_y=(order[i]-1)%3;
		cnt+=y+x-to_x+to_y;
		for(int j=y;j>0;j--)var_b.first[x][j]=var_b.first[x][j-1];
		var_b.first[x][0]=order[i];
		for(int j=x;j>to_x;j--)var_b.first[j][0]=var_b.first[j-1][0];
		var_b.first[to_x][0]=order[i];
		for(int j=0;j<to_y;j++)var_b.first[to_x][j]=var_b.first[to_x][j+1];
		var_b.first[to_x][to_y]=order[i];
		x=y=3;
	}
	if(cnt%2)return false;
	else return true;
	return true;
}

/**
 * Generate all possible valid board permutations.
 */
void generate_chesb(){
	vector<int>ve={0,1,2,3,4,5,6,7,8};
	do{
		chesb new_b;
		vector<vector<int>>ve1;
		for(int i=0;i<3;i++){
			vector<int>ve2;
			for(int j=0;j<3;j++)ve2.push_back(ve[i*3+j]);
			ve1.push_back(ve2);
		}
		for(int i=0;i<3;i++)for(int j=0;j<3;j++)if(ve1[i][j]==0)new_b=make_pair(ve1,make_pair(i,j));
		
		if(valid_ches(new_b))generated_chesb.push_back(new_b);
	}while(next_permutation(ve.begin(),ve.end()));
}

/**
 * Get the next possible board states from the current state.
 * @param current_b The current board state.
 * @return A vector of possible next board states.
 */
vector<chesb> next_board(chesb current_b){
	int x,y;
	x=current_b.second.first;
	y=current_b.second.second;
	vector<chesb> return_c;
	if(y!=2){
		chesb return_b;
		return_b=current_b;
		return_b.second.second+=1;
		return_b.first[x][y]=return_b.first[x][y+1];
		return_b.first[x][y+1]=0;
		return_c.push_back(return_b);
	}
	if(y!=0){
		chesb return_b;
		return_b=current_b;
		return_b.second.second-=1;
		return_b.first[x][y]=return_b.first[x][y-1];
		return_b.first[x][y-1]=0;
		return_c.push_back(return_b);
	}
	if(x!=0){
		chesb return_b;
		return_b=current_b;
		return_b.second.first-=1;
		return_b.first[x][y]=return_b.first[x-1][y];
		return_b.first[x-1][y]=0;
		return_c.push_back(return_b);
	}
	if(x!=2){
		chesb return_b;
		return_b=current_b;
		return_b.second.first+=1;
		return_b.first[x][y]=return_b.first[x+1][y];
		return_b.first[x+1][y]=0;
		return_c.push_back(return_b);
	}
	return return_c;
}

/**
 * Main function to run the A* search algorithm on all possible valid board permutations.
 * The results, including the effective branching factor for different depths, are written to a text file.
 */
int main(){
	generate_chesb();
	
	random_shuffle(generated_chesb.begin(),generated_chesb.end());
	ios::sync_with_stdio(false);
	fout.open("h_3.txt"); // Here's an instance we have for h_3
	clock_t start,end;
	start=clock();
	for(int k=0;k<181440;k++){
		chesb current_b=generated_chesb[k];
		priority_queue<node>frontier;
		frontier.push(node(current_b,0));
		cout<<frontier.top().current_b.first[0][0]<<endl;
		int N=0;
		explored_chesb.clear();
		while(true){
			if(frontier.empty()){cout<<"Error in h_3"<<endl;exit(0);}
			node current_n=frontier.top();
			frontier.pop();
			
			bool is_goal=true;
			for(int i=0;i<3;i++)for(int j=0;j<3;j++)if(current_n.current_b.first[i][j]!=orig_place[i][j])is_goal=false;
			if(is_goal){
				int d=current_n.g;
				if(d==0)break;
				double bstar=efb(N,d);
				fout<<"k"<<k<<" N: "<<N<<" d "<<d<<" bstar: "<<bstar<<endl;
				cout<<k<<" "<<N<<" "<<d<<" "<<bstar<<endl;
				break;
			}
			if(explored_chesb.find(current_n.current_b)!=explored_chesb.end())continue;
			explored_chesb.insert(current_n.current_b);
			vector<chesb>next_b=next_board(current_n.current_b);
			for(chesb new_b:next_b){
				if(explored_chesb.find(new_b)==explored_chesb.end()){frontier.push(node(new_b,current_n.g+1));N+=1;}
			}
		}
	}
	fout.close();
	end=clock();
	cout<<"time = "<<double(end-start)/CLOCKS_PER_SEC<<"s"<<endl;
	return 0;
}
