#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>
#include <map>
#include <unordered_map>
using namespace std;
/// Knowledge base keyworkds
#define RULESET_KW  "ruleset"
#define VAR_LIST_ID_KW  "variable_list"
#define RULE_ID_KW  "id"
#define RULE_AND_KW  "and"
#define RULE_ACTION_KW  "action"
#define VAR_NAME_KW  "name"
#define VAR_VALUE_KW  "value"
#define TARGET_KW  "target"
#define VAR_TARGET_ID_KW  "variable"
#define CONC_TARGET_ID_KW  "conclusion"
#define CONC_VAR_LIST_KW "conclusion_var_list"
#define EACH_RULE_MAX_CONDTN_COUNT 4

enum class var_type
{
	ANTECEDENT,
	CONSEQUENCE
};

//For use with variable list
class variable
{
	string _name;
	string _value;
public:
	var_type type = var_type::ANTECEDENT;
	//bool instantiated = false;
	variable() : _name(32, 0), _value(32, 0) {}
	variable(const char* name, const char* value) : _name(name),_value(value){}

	void set_name(const string& nm) { _name = nm; }
	const string& name() const { return _name; }
	void reset_value() { this->_value = "\0"; }
	void set_value(const string& val) { this->_value = val; }
	const string& value() const { return _value; }
};

//For using with the clause variable list
// or in Our case clause variable map
class clausevar
{
public:
	string name;
	int clause_id = 0;

	clausevar() :name(32,0){}
};

class rule
{
	int _id = 0;
	std::vector<variable> _condition_and;
	variable _action_variable;
public:
	rule() = default;
	virtual ~rule() = default;
	std::vector<variable>& get_and_conditions() { return _condition_and; }

	void add_and_condition(variable& v) { _condition_and.push_back(v); } //Condition format is: Variable == Value

	const variable& get_variable(const string& name) 
	{
		for (const auto& v : _condition_and)
		{
			if (v.name().compare(name) == 0)
			{
				return v;
			}
		}
		
		//Have to handle this case
		return variable();
	}

	const int &get_id() const { return _id; }
	void set_id(int id) { _id = id; }
	variable& get_action() { return _action_variable; }
	
	void execute_action()
	{

	}
};