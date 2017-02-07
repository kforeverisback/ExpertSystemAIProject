#include "stdafx.h"
#include "InferenceEngine.h"

void InferenceEngine::build_KB(const char* forward_kb, const char* backward_kb)
{
	_fc.build_forward_KB(forward_kb);
	_bc.build_backward_KB(backward_kb);
}

void InferenceEngine::user_input(variable& var) {
	cout << "Enter Value for Variable \"" << var.name() << "\":";
	string readval;
	cin >> readval;
	var.set_value(readval);
}

void InferenceEngine::start_inference(const char* conclusion_name)
{
	string valu = conclusion_name;
	nana::threads::pool_push(_workerThread, [=]
	{
		_bc.start_bc(valu.c_str());
	});
}