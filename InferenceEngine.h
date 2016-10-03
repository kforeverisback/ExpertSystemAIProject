#pragma once
#include "stdafx.h"

#include "ForwardChain.h"
#include "BackwardChain.h"

class InferenceEngine
{
private:
	ForwardChain _fc;
	BackwardChain _bc;
public:
	InferenceEngine() {}
	void build_KB(const char* forward_kb = "f_kb.json", const char* backward_kb = "b_kb.json")
	{
		_fc.build_forward_KB(forward_kb);
		_bc.build_backward_KB(backward_kb);
	}
};
