#pragma once
#include "stdafx.h"

#include "kb_essentials.h"

class ForwardChain
{
	typedef std::pair<variable, bool> var_instance_pair;

	unordered_map<int, rule> _rules;
	unordered_map<string, vector<int>> _clause_variable_list;
	map<string, var_instance_pair> _variable_list;
	queue<string> _conclusion_var_q;
	vector<variable> _conclusion_list;
public:
	void build_forward_KB(const char*);
	void start_fc(const std::vector<variable>&);

	//Called from the GUI to set the variable from User
	void set_variable(const char* name,const char* value);
};
