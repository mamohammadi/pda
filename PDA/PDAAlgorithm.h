#include<string>
#pragma once

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

class PDAAlgorithm
{
public:
	PDAAlgorithm();
	~PDAAlgorithm();
};

