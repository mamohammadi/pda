#include<string>
#include<vector>

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

class Pda
{
public:
	Pda();
	~Pda();
};

