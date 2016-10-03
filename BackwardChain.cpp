#include "BackwardChain.h"

void BackwardChain::build_backward_KB(const char* kb_file_name)
{
	//Read KB file
	jxx::Object json;
	ifstream kbfile;
	kbfile.open(kb_file_name, ios::in);
#if !defined(__GNUC__)
	assert(kbfile.is_open());
	assert(json.parse(kbfile));
#else
	string str((istreambuf_iterator<char>(kbfile)),
		istreambuf_iterator<char>());
	if (!(kbfile.is_open() && json.parse(str)))
	{
		cout << "!!!!!!!!!!!!";
		return;
	}
#endif
	jxx::Array& rulesets = json.get<jxx::Array>(RULESET_KW);

	//Read Each rule
	int clause_number = 1;
	//cout << rulesets.size();
	for (size_t i = 0; i < rulesets.size(); i++)
	{
		jxx::Object &o = rulesets.get<jxx::Object>(i);
		int id = (int)o.get<jxx::Number>(RULE_ID_KW);
		rule& r = _rules[id];
		r.set_id(id);

		//auto &cond = r.get_and_conditions();
		jxx::Array& ands = o.get<jxx::Array>(RULE_AND_KW);

		//Read each antecedents in rules (IF <condition> part)
		size_t c = 0;
		for (; c < ands.size(); c++)
		{
			variable v;
			jxx::Object& oo = ands.get<jxx::Object>(c);

			v.set_name(oo.get<jxx::String>(VAR_NAME_KW));
			v.set_value(oo.get<jxx::String>(VAR_VALUE_KW));

			r.add_and_condition(v);
			_clause_variable_list[v.name()].push_back(clause_number);
			clause_number++;
		}

		clause_number += EACH_RULE_MAX_CONDTN_COUNT - c; //MAX number of Conditions in each RULE
		jxx::Object& oo = o.get<jxx::Object>(RULE_ACTION_KW);

		//Save the Action variable (THEN <variable = VALUE> part)

		variable& actionvar = r.get_action();
		actionvar.set_name(oo.get<jxx::String>(VAR_NAME_KW));
		actionvar.set_value(oo.get<jxx::String>(VAR_VALUE_KW));
		if (oo.get<jxx::String>(TARGET_KW).compare(VAR_TARGET_ID_KW) == 0)
		{
			//Already set as default
			//actionvar.type = var_type::ANTECEDENT;
		}
		else // Consequence
		{
			actionvar.type = var_type::CONSEQUENCE;
			//add onto the conclusion list
			_conclusion_rule_list[actionvar.name()].push_back(id);
		}
	}

	jxx::Array& var_list = json.get<jxx::Array>(VAR_LIST_ID_KW);
	for (size_t c = 0; c < var_list.size(); c++)
	{
		string& name = var_list.get<jxx::String>(c);
		_variable_list[name].first.set_name(name);
	}


	jxx::Array& var_list2 = json.get<jxx::Array>(CONC_VAR_LIST_KW);
	for (size_t c = 0; c < var_list2.size(); c++)
	{
		string& name = var_list2.get<jxx::String>(c);
		_conclusion_variable_list[name].first.set_name(name);
	}

	//Create the conclusion list
	//Printing out the clause variable list
	/*for(const auto& p : clause_variable_list){
		string con = p.first;
		vector<int> attach_rules = p.second;
		cout << con << endl;
		for (size_t i = 0; i < attach_rules.size(); i++){
			cout << attach_rules[i] << endl;
		}

	}*/

	kbfile.close();
}

void BackwardChain::process_conclusion(const char* cvar)
{
	rule& current_rule = _rules[_conclusion_rule_list[cvar].front()];
	_conclusion_rule_list[cvar].pop_front();
	_conclusion_stack_pointer.push(current_rule);

	process_rule(current_rule);
	_conclusion_stack_pointer.pop();
}

bool BackwardChain::process_rule(rule& rl)
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
void BackwardChain::start_bc(const char* cvar)
{
	if (_conclusion_rule_list.find(cvar) == _conclusion_rule_list.end())
	{
		cout << "Conlcusion Not Found" << endl;
		//Not Found
	}

	_target_conclusion_variable = &_conclusion_variable_list[cvar];
	while (_conclusion_rule_list[cvar].size() != 0 && !_target_conclusion_variable->second)
	{
		process_conclusion(cvar);
	}

	if (!_target_conclusion_variable->second)
	{
		cout << "No Conclusion is reached" << endl;
	}
}