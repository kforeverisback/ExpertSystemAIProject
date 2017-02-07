#pragma once
#include "stdafx.h"

#include <nana\gui\widgets\button.hpp>
#include <nana\gui\widgets\picture.hpp>
#include <nana\gui\widgets\label.hpp>
#include <nana/gui/filebox.hpp>
#include <nana\gui\widgets\combox.hpp>
#include <nana\gui\widgets\slider.hpp>
#include <nana\gui\widgets\group.hpp>
#include <nana\gui\widgets\categorize.hpp>
#include <nana\gui\widgets\checkbox.hpp>

#include "ForwardChain.h"
#include "BackwardChain.h"

void user_input(variable& var);
class InferenceEngine
{
private:
	ForwardChain _fc;
	BackwardChain _bc;
public:
	ForwardChain& get_fc() { return _fc; }
	BackwardChain& get_bc() { return _bc; }
	InferenceEngine() : _fc(user_input), _bc(user_input) {}
	void build_KB(const char* forward_kb = "f_kb.json", const char* backward_kb = "b_kb.json")
	{
		_fc.build_forward_KB(forward_kb);
		_bc.build_backward_KB(backward_kb);
	}

};

namespace {
	enum { W_H = 468, W_W = 350, PAD = 10, CTRL_HEIGHT = 25 };
}
class GUIWindow : public nana::form
{
private:
	std::vector<fs::path> _fileList;
	std::vector<fs::path>::const_iterator it;
	InferenceEngine _infEng;
	nana::threads::pool _workerThread;
private:
	int row = 0;
	nana::button _btnStarDiag;
	nana::label _lblFDBName, _lblBDBName, _lblStartDiag, _lbSymptoms,_lbPoison,_lbAntidote;

	void increase_window_height() {
		row++;
		nana::size&sz = this->size();
		sz.height += CTRL_HEIGHT + PAD;
		this->size(sz);
	}
public:
	GUIWindow();
	string ask_symptom(const char* symptom);
	void add_symptom(const char* symptom);
};

std::shared_ptr<GUIWindow> Get_global_window();

