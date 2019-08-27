#include <bits/stdc++.h>


using namespace std;


//Calculating Epsilon Closure of set of states
unordered_set<string> epCls(unordered_set<string> states, vector<vector<vector<string>>> transTable, map<string,int> stateMap)
{
	unordered_set<string> ans=states;
	queue <string> q;
	for(auto it:ans)
		q.push(it);							//Push all states in the initial set into a queue
	vector<string> null;
	null.push_back("^");
	while(!q.empty())						//For each state in the queue, check for Epsilon transition
	{
		string tState=q.front();
		q.pop();
		vector <string> nxt=transTable[stateMap[tState]][transTable[0].size()-1];
		if(nxt==null)						//If epsilon transition not allowed, skip the state
			continue;
		//If allowed, find the reachable states and check for each of those states
		for(auto it:nxt)					
		{
			//If state already in Epsilon closure set, skip it
			if(ans.find(it)==ans.end())
			{
				//Otherwise push it into the queue(as later it'll also be checked for epsilon transitions) as well as the final epsilon closure set
				ans.insert(it);
				q.push(it);
			}
		}
	}
	return ans;								//Return the final set
}





//Perform transition on a set of states over an input symbol
unordered_set<string> makeTransition(unordered_set<string> tStates,int tSymbolIndex,vector<vector<vector<string>>> transTable,map<string,int> stateMap)
{
	vector<string> null;
	null.push_back("^");
	unordered_set <string> ans;
	for(auto it:tStates)
	{
		//For each state in the set, find the reachable states
		vector <string> v=transTable[stateMap[it]][tSymbolIndex];
		if(v!=null)				//If reachable states vector is not null, insert all of them in the final set
		{
			for(auto it2:v)
			{
				ans.insert(it2);
			}
		}
	}
	return ans;										//Return the final set
}






//Split the input string over comma into a vector of multiple strings
vector<string> splitComma(string s)
{
	vector<string> ans;
	string t="";
	for(auto i:s)
	{
		if(i!=',')									//If comma not found, insert the char in the current part
			t+=i;
		else
		{
			ans.push_back(t);						//If comma found, push current part into vector
			t="";									//Start calculating new part
		}
	}
	ans.push_back(t);								//Insert the last part in the vector
	return ans;										//Return the final vector of strings
}







int main(int argc, char* argv[])
{
	//Reading command line arguments and converting them to integers
	int numStates=((char)(*argv[1]))-'0';
	int numFinal=((char)(*argv[2]))-'0';
	int numSymbols=((char)(*argv[3]))-'0';




	//Opening the input table.txt file
	ifstream infile("table.txt");

	int i;										//loop counter
	string tmp;									//temp variable to read file elements

	


	//Map transition states with their indices to access transition table accordingly
	map<string, int> stateMap;
	//Reading the transition states
	vector <string> states;
	for(i=0; i<numStates; i++)
	{
		infile>>tmp;
		states.push_back(tmp);
		stateMap[tmp]=i;

	}
	



	//Reading the final transition states
	vector <string> finalStates;
	for(i=0; i<numFinal; i++)
	{
		infile>>tmp;
		finalStates.push_back(tmp);
	}
	



	//Map transition symbols with their indices to access transition table accordingly
	map<char,int> symbolMap;
	char tmp2;							//Symbols will always be a single character
	//Reading the alphabet symbols allowed
	vector<char> symbols;
	for(i=0; i<numSymbols; i++)
	{
		infile>>tmp2;
		symbols.push_back(tmp2);		
		symbolMap[tmp2]=i;
	}



	//Reading the trasition table
	int j;
	vector<vector<vector<string>>> transTable;
	transTable.resize(numStates);
	for(i=0; i<numStates; i++)
	{
		transTable[i].resize(numSymbols+1);
		for(j=0; j<=numSymbols; j++)
		{
			infile>>tmp;
			vector <string> tVec=splitComma(tmp);		//Break the input into comma separated strings
			transTable[i][j]=tVec;						//Store as vector of strings
		}
	}
	infile.close();




	//Creating the corresponding DFA
	vector <unordered_set<string>> DStates;				
	vector <unordered_set<string>> DFinalStates;
	unordered_set<string> t;
	t.insert(states[0]);
	DStates.push_back(epCls(t,transTable,stateMap));
	i=0;
	vector<vector<unordered_set<string>>> DTransTable;
	while(i<DStates.size())
	{
		vector<unordered_set<string>> temp;
		for(char x:symbols)
		{
			t=epCls(makeTransition(DStates[i],symbolMap[x],transTable,stateMap),transTable,stateMap);
			temp.push_back(t);
			if(find(DStates.begin(), DStates.end(),t)==DStates.end() && !t.empty())
			{
				DStates.push_back(t);
				for(auto it:finalStates)
				{
					if(*(t.find(it))==it)
					{
						DFinalStates.push_back(t);
						break;
					}
				}
			}

		}
		DTransTable.push_back(temp);
		i++;
	}
	for(i=0; i<DTransTable.size(); i++)
	{
		cout<<"{";
		for(auto it:DStates[i])
			cout<<it<<",";
		cout<<"} ";
		for(j=0; j<numSymbols; j++)
		{
			if(DTransTable[i][j].empty()) cout<<"^ ";
			else
			{
				cout<<"{";
				for(auto it:DTransTable[i][j])
					cout<<it<<" ,";
				cout<<"} ";
			}
		}
		cout<<endl;
	}


	//Finding Final States
	
	cout<<"Final States : ";
	for(auto it:DFinalStates)
	{
		cout<<"{";
		for(auto it2:it)
			cout<<it2<<",";
		cout<<"} ";
	}
}
