#pragma once
#include "stdafx.h"

#include "ForwardChain.h"
#include "BackwardChain.h"

class InferenceEngine
{
private:
	ForwardChain _fc;
	BackwardChain _bc;
	nana::threads::pool _workerThread;
public:
	void build_KB(const char* forward_kb = "f_kb.json", const char* backward_kb = "b_kb.json")
	{
		_fc.build_forward_KB(forward_kb);
		_bc.build_backward_KB(backward_kb);
	}

	void user_input(variable& var) {
		cout << "Enter Value for Variable \"" << var.name() << "\":";
		string readval;
		cin >> readval;
		var.set_value(readval);
	}

	void start_inference(const char* conclusion_name = "poison")
	{
		string valu = conclusion_name;
		nana::threads::pool_push(_workerThread, [=]
		{
			_bc.start_bc(valu.c_str());
		});
	}
};
