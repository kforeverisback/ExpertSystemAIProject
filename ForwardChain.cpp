#include "ForwardChain.h"

void ForwardChain::build_forward_KB(const char* kb_file_name)
{
	//Read KB file
	jxx::Object json;
	ifstream kbfile;
	kbfile.open(kb_file_name, ios::in);
#if !defined(__GNUC__)
	assert(kbfile.is_open());
	assert(json.parse(kbfile));
#else
	std::string str((std::istreambuf_iterator<char>(kbfile)),
		std::istreambuf_iterator<char>());
	if (!(kbfile.is_open() && json.parse(str)))
	{
		return;
	}
#endif
	jxx::Array& rulesets = json.get<jxx::Array>(RULESET_KW);
	//cout << "Size: " << rulesets.size() <<endl;

	//Read Each rule
	int clause_number = 1;
	for (size_t i = 0; i < rulesets.size(); i++)
	{
		jxx::Object &o = rulesets.get<jxx::Object>(i);
		int id = (int)o.get<jxx::Number>(RULE_ID_KW);
		rule& r = _rules[id];
		r.set_id(id);
		//auto &cond = r.get_and_conditions();
		jxx::Array& ands = o.get<jxx::Array>(RULE_AND_KW);
		size_t c = 0;
		//Read each antecedents in rules (IF <condition> part)
		for (; c < ands.size(); c++)
		{
			variable v;
			jxx::Object& oo = ands.get<jxx::Object>(c);
			v.set_name(oo.get<jxx::String>(VAR_NAME_KW));
			v.set_value(oo.get<jxx::String>(VAR_VALUE_KW));
			r.add_and_condition(v);
			//cond.push_back(v);
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
		}
	}

	jxx::Array& var_list = json.get<jxx::Array>(VAR_LIST_ID_KW);
	for (size_t c = 0; c < var_list.size(); c++)
	{
		string& name = var_list.get<jxx::String>(c);
		_variable_list[name].first.set_name(name);
	}

	//We don't need Conclusion List for Forward

	kbfile.close();
}

void ForwardChain::set_variable(const char* name, const char* value)
{
	var_instance_pair& vip = _variable_list[name];
	variable& vvl = vip.first;
	vvl.set_value(value);
}

ForwardChain::ForwardChain() {}

void ForwardChain::start_fc(const std::vector<variable>& vars)
{
	//Push Valid variables in the queue
	for (const variable& var : vars)
	{
		if (_variable_list.find(var.name()) != _variable_list.end())
		{
			var_instance_pair &vip = _variable_list[var.name()];
			variable& v = vip.first;
			v.set_value(var.value());
			_conclusion_var_q.push(v.name());
			vip.second = true; //Instantiated
		}
	}
	//Process Conclusion variables
	while (_conclusion_var_q.size() != 0)
	{
		//Get the first variable from queue
		const variable& target_var = _variable_list[_conclusion_var_q.front()].first;


		//Get clause numbers associated with the variable+
		auto& clause_numbers = _clause_variable_list[target_var.name()];

		//Calculate Rule Number from clause number
		for (int cn : clause_numbers)
		{
			int rule_id = ((int)(cn / EACH_RULE_MAX_CONDTN_COUNT) + 1) * 10;
			rule& rl = _rules[rule_id]; //clause 

			std::vector<variable> list_var;
			bool process_other_variables = false;
			for (const variable& vr : rl.get_and_conditions())
			{
				if (vr.name().compare(target_var.name()) != 0)
				{
					list_var.push_back(vr);
				}
				else if (vr.value().compare(target_var.value()) == 0)
				{
					process_other_variables = true;
				}
				else
				{
					break;
				}
			}

			if (!process_other_variables)
			{
				continue;
			}

			bool process_action = true;
			for (size_t x = 0; x < list_var.size() && process_action; x++)
			{
				variable& vr = list_var[x];
				var_instance_pair& vip = _variable_list[vr.name()];
				variable& vvl = vip.first;

				if (false == vip.second) // Not instantiated
				{
					//////////////////////////////////////////////////////////////
					//seek_value_for_variable(vvl);
					//////////////////////////////////////////////////////////////
					vip.second = true; //The value is set and instantiated
					x--; //Recheck the last variable which was read
				}
				else if (vvl.value().compare(vr.value()) != 0)
				{
					process_action = false;
				}
			}

			if (process_action)
			{
				variable& action_var = rl.get_action();
				_conclusion_var_q.push(action_var.name());

				if (action_var.type == var_type::ANTECEDENT)
				{
					auto& vlins = _variable_list[action_var.name()];
					vlins.first.set_value(action_var.value());
					vlins.second = true;
				}
				else
				{
					_conclusion_list.push_back(action_var);
				}
			}
			//If Rule variable value matches with conclusion variable value,
			//then continue with other variables

		}
		//After clauses are processed for the specific variable remove the variable from queue
		_conclusion_var_q.pop();
	}

}