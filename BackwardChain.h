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
	user_input_fn _fn;

	var_instance_pair* _target_conclusion_variable = nullptr;
	
private:
	void process_conclusion(const char* cvar)
	{
		rule& current_rule = _rules[_conclusion_rule_list[cvar].front()];
		_conclusion_rule_list[cvar].pop_front();
		_conclusion_stack_pointer.push(current_rule);

		process_rule(current_rule);
		_conclusion_stack_pointer.pop();
	}
	bool process_rule(rule& rl)
	{
		bool conclusion_reached = true;
		for (int x = 0; x < rl.get_and_conditions().size(); x++)
		{
			variable& v = rl.get_and_conditions().at(x);
			var_type vt = (_conclusion_variable_list.find(v.name()) == _conclusion_variable_list.end()) ? var_type::ANTECEDENT : var_type::CONSEQUENCE;

			var_instance_pair& vip = (vt == var_type::ANTECEDENT) ? _variable_list[v.name()] : _conclusion_variable_list[v.name()];
			if (vt == var_type::ANTECEDENT || (vt == var_type::CONSEQUENCE && vip.second == true)) //Is it a condition
			{
				if (false == vip.second) //Variable is not instantiated
				{
					//////////////////////////////////////////////////////////////
					//user_input(vip.first);;
					_fn(vip.first);
					//////////////////////////////////////////////////////////////

					vip.second = true;
				}

				if (vip.first.value().compare(v.value()) != 0)
				{

					return false;
				}
			}
			else //or conclusion
			{
				process_conclusion(v.name().c_str());
				x--;
			}
		}

		variable& action_var = rl.get_action();
		_conclusion_variable_list[action_var.name()].first.set_value(action_var.value());
		_conclusion_variable_list[action_var.name()].second = true;
		return true;

	}
public:
	BackwardChain(user_input_fn fn) :_fn(fn) {}
	void build_backward_KB(const char* kb_file_name);
		void start_bc(const char* cvar);

	bool is_conclusion_available() const { return _target_conclusion_variable->second; }
	const variable& get_conclusion() const { return _target_conclusion_variable->first; }

};