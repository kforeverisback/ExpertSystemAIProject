#pragma once
#include "stdafx.h"

#include "kb_essentials.h"

class BackwardChain
{
	typedef std::pair<variable, bool> var_instance_pair;

	unordered_map<int, rule> _rules;
	unordered_map<string, vector<int>> _clause_variable_list;
	map<string, var_instance_pair> _variable_list; //list of variables and if instantiated
	map<string, var_instance_pair> _conclusion_variable_list; //list of variables and if instantiated
	map<string, list<int>> _conclusion_rule_list;
	stack<rule> _conclusion_stack_pointer;

	var_instance_pair* _target_conclusion_variable = nullptr;
	void process_conclusion(const char* cvar);
	bool process_rule(rule& rl);
public:
	void build_backward_KB(const char* kb_file_name);
	void start_bc(const char* var);

};