#include <iostream>
#include<conio.h>
#include <fstream>
#include<string>
#include <vector>
#include<stack>

using namespace std;

class Route
{
public:
	string POP;
	vector<string> PushList;
	char Symbol;
	int ToID;
};

class State
{
public:
	vector<Route> Routes;
};

int statesCount;
int routesCount;
int finalStatesCount;
int currentState;
int remainingSymboles;
State *states;
int *finalStates;
stack<string> pdaStack;

//void ProccessLine(string line)
//{
//	string item;
//	int counter = 0;
//	int stateIndex;
//	Route route;
//	for (int i = 0; i < line.length(); i++)
//	{
//		if (line[i] == ',' || line[i] == ' ')
//		{
//			if (counter == 0)
//			{
//				stateIndex = stoi(item);
//				counter++;
//			}
//			else if (counter == 1)
//			{
//				route.Symbol = item[0];
//				counter++;
//			}
//			else if (counter == 2)
//			{
//				route.POP = item;
//				counter++;
//			}
//			else if (counter == 3)
//			{
//				route.ToID = stoi(item);
//				counter++;
//			}
//			else
//				route.PushList.push_back(item);
//			item = "";
//		}
//		else
//			item += line[i];
//	}
//	route.PushList.push_back(item);
//	states[stateIndex].Routes.push_back(route);
//}

void ProccessLineSingleSymbol(string line)
{
	string item;
	int counter = 0;
	int stateIndex;
	Route route;
	for (int i = 0; i < line.length(); i++)
	{
		if (line[i] == ',' || line[i] == ' ')
		{
			if (counter == 0)
			{
				stateIndex = stoi(item);
				counter++;
			}
			else if (counter == 1)
			{
				route.Symbol = item[0];
				counter++;
			}
			else if (counter == 2)
			{
				route.POP = item;
				counter++;
			}
			else if (counter == 3)
			{
				route.ToID = stoi(item);
				for (int k = i + 1; k < line.length(); k++)
					route.PushList.push_back(string(1,line[k]));
				states[stateIndex].Routes.push_back(route);
				return;
			}
			item = "";
		}
		else
			item += line[i];
	}
}

void PrintStates()
{
	for (int i = 0; i < statesCount; i++)
	{
		for (int j = 0; j < states[i].Routes.size(); j++)
		{
			cout << i << ',' << states[i].Routes[j].Symbol << ',' << states[i].Routes[j].POP
				<< ' ' << states[i].Routes[j].ToID;
			for (int k = 0; k < states[i].Routes[j].PushList.size(); k++)
				cout << ',' << states[i].Routes[j].PushList[k];
			cout << '\n';
		}
	}
}

void PrintSingleStates()
{
	for (int i = 0; i < statesCount; i++)
	{
		for (int j = 0; j < states[i].Routes.size(); j++)
		{
			cout << i << ',' << states[i].Routes[j].Symbol << ',' << states[i].Routes[j].POP
				<< ' ' << states[i].Routes[j].ToID<<',';
			for (int k = 0; k < states[i].Routes[j].PushList.size(); k++)
				cout << states[i].Routes[j].PushList[k];
			cout << '\n';
		}
	}
}


bool IsAccepted()
{
	bool isInFinalState = false;
	for (int i = 0; i < finalStatesCount; i++)
	{
		if (currentState == finalStates[i])
			isInFinalState = true;
	}
	if (isInFinalState && pdaStack.empty() && remainingSymboles == 0)
		return true;
	return false;
}

void PushToStack(vector<string> items)
{
	for (int i = 0; i < items.size(); i++)
	{
		if (items[i] != "@")
			pdaStack.push(items[i]);
	}
}

bool PopFromStack(string item)
{
	if (item == "@")
		return true;
	if (pdaStack.empty() || pdaStack.top() != item)
		return false;
	pdaStack.pop();
	return true;
}

bool PDA(string str)
{
	currentState = 0;
	remainingSymboles = str.length();
	int strIndex = 0;
	char symbol;
	pdaStack = stack<string>();

	while (true)
	{
		if (IsAccepted())
			return true;

		symbol = str[strIndex];
		bool noWay = true;
		for (int i = 0; i < states[currentState].Routes.size(); i++)
		{
			if (states[currentState].Routes[i].Symbol == symbol)
			{
				if (PopFromStack(states[currentState].Routes[i].POP))
				{
					strIndex++;
					remainingSymboles--;
					PushToStack(states[currentState].Routes[i].PushList);
					currentState = states[currentState].Routes[i].ToID;
					noWay = false;
					break;
				}
			}
			else if (states[currentState].Routes[i].Symbol == '@')
			{
				if (PopFromStack(states[currentState].Routes[i].POP))
				{
					PushToStack(states[currentState].Routes[i].PushList);
					currentState = states[currentState].Routes[i].ToID;
					noWay = false;
					break;
				}
			}
		}

		if (noWay)
			return false;
	}


}

void main()
{
	char ans;
	do
	{
		string name;
		cout << "file name?";
		cin >> name;
		ifstream inputFile = ifstream(name);

		int symbolCount;
		string symboles;
		string line;
		string temp;

		if (inputFile.is_open())
		{
			inputFile >> symbolCount;
			for (int w = 0; w < symbolCount; w++)
			{
				inputFile >> temp;
				symboles += temp+' ';
			}
			inputFile >> statesCount;
			inputFile >> routesCount;
			states = new State[statesCount];

			for (int i = 0; i < routesCount; i++)
			{
				inputFile >> line;
				inputFile >> temp;
				line += ' ' + temp;
				ProccessLineSingleSymbol(line);
			}
			inputFile >> finalStatesCount;
			finalStates = new int[finalStatesCount];
			for (int i = 0; i < finalStatesCount; i++)
				inputFile >> finalStates[i];

			int testCaseIndex = 0;
			while (true)
			{
				inputFile >> line;
				if (line == "$")
					break;
				if (PDA(line))
					cout << testCaseIndex << ' ' << "Accept\n";
				else
					cout << testCaseIndex << ' ' << "Reject\n";
				testCaseIndex++;
			}
			delete[] states;
			inputFile.close();
		}
		else
			cout << "File not found\n";

		cout << "\n\nContinue(y or n)?";
		cin >> ans;
	} while (ans == 'y' || ans == 'Y');
}