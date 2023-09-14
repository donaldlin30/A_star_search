#include <bits/stdc++.h>
#define chesb pair<vector<vector<int>>,pair<int,int>>
using namespace std;
int orig_place[3][3]={{1,2,3},{4,5,6},{7,8,0}};
vector<int>allN,alld;
vector<chesb>generated_chesb;
set<chesb>explored_chesb;
map<int,vector<double>>ma1;
ofstream fout;
int h_1(chesb current_b){
	int sum_d=0;
	for(int i=0;i<3;i++)for(int j=0;j<3;j++)if(current_b.first[i][j]!=0&&current_b.first[i][j]!=orig_place[i][j])sum_d++;
	return sum_d;
}
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
struct node{
	chesb current_b;
	int g;
	node(chesb current_b,int g):current_b(current_b),g(g){}
};
bool operator<(node a, node b){
    return a.g+h_1(a.current_b)>b.g+h_1(b.current_b);
}

double to_d1(double b,int d){
	double to_return=1;
	for(int i=0;i<=d;i++){
		to_return*=b;
	}
	return to_return;
}
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

int main(){
	generate_chesb();
	random_shuffle(generated_chesb.begin(),generated_chesb.end());
	ios::sync_with_stdio(false);
	for(chesb current_b:generated_chesb){
		priority_queue<node>frontier;
		frontier.push(node(current_b,0));
		cout<<frontier.top().current_b.first[0][0]<<endl;
		int N=0;
		explored_chesb.clear();
		while(true){
			if(frontier.empty()){cout<<"Error in h_1"<<endl;exit(0);}
			node current_n=frontier.top();
			frontier.pop();
			N+=1;
			
			bool is_goal=true;
			for(int i=0;i<3;i++)for(int j=0;j<3;j++)if(current_n.current_b.first[i][j]!=orig_place[i][j])is_goal=false;
			if(is_goal){
				allN.push_back(N);
				int d=current_n.g;
				alld.push_back(d);
				double bstar=efb(N,d);
				cout<<N<<" "<<d<<endl;
				cout<<bstar<<endl;
				if(ma1.find(d)==ma1.end()){vector<double>vb;ma1.insert(make_pair(d,vb));}
				ma1.at(d).push_back(bstar);
				break;
			}
			if(explored_chesb.find(current_n.current_b)!=explored_chesb.end())continue;
			explored_chesb.insert(current_n.current_b);
			vector<chesb>next_b=next_board(current_n.current_b);
			for(chesb new_b:next_b){
				if(explored_chesb.find(new_b)==explored_chesb.end())frontier.push(node(new_b,current_n.g+1));
			}
		}
	}
	fout.open("h_1.txt");
	for(pair<int,vector<double>>pa:ma1){
		fout<<"depth: "<<pa.first<<"; number of problems: "<<pa.second.size()<<"; average of effective branching factor:";
		double sum=0;
		for(double d:pa.second)sum+=d;
		fout<<sum/pa.second.size()<<endl;
	}
	fout.close();
	return 0;
}


