#include "stdafx.h"
#include "GUIWindow.h"

class WordUpper {
public:
	WordUpper() : m_wasLetter(false) {}
	char operator()(char c)
	{
		if (isalpha(c)) {
			if (!m_wasLetter) c = toupper(c);
			m_wasLetter = true;
		}
		else
			m_wasLetter = false;

		return c;
	}

private:
	bool m_wasLetter;
};

static string StringToUpper(string strToConvert)
{
	std::transform(strToConvert.begin(), strToConvert.end(), strToConvert.begin(), WordUpper());

	return strToConvert;
}

GUIWindow::GUIWindow() :nana::form(nana::API::make_center(W_W, 150), nana::appear::decorate<nana::appear::taskbar>())
{
	_infEng.build_KB();
	this->caption("Poison Inference System");
	_lblFDBName.create(*this, nana::rectangle(W_W / 2 - W_W / 4, row*CTRL_HEIGHT + PAD, W_W / 2, CTRL_HEIGHT), true);
	_lblFDBName.caption("Forward Chaining KB: f_kb.json");

	row++;
	_lblBDBName.create(*this, nana::rectangle(W_W / 2 - W_W / 4, row*CTRL_HEIGHT + PAD, W_W / 2, CTRL_HEIGHT), true);
	_lblBDBName.caption("Backward Chaining KB: b_kb.json");

	row++;
	_btnStarDiag.create(*this, nana::rectangle(W_W / 2 - W_W / 4, row*CTRL_HEIGHT + PAD, W_W / 2, CTRL_HEIGHT), true);
	_btnStarDiag.caption("Start Poison Diagnosis");
	_btnStarDiag.events().click([=]
	{
		row++;
		_lblStartDiag.create(*this, nana::rectangle(PAD, row*CTRL_HEIGHT + PAD * 2, W_W / 2, CTRL_HEIGHT), true);
		_lblStartDiag.caption("Symptoms Include:");
		row++;
		_lbSymptoms.create(*this, nana::rectangle(PAD, row*CTRL_HEIGHT + PAD, W_W / 2, CTRL_HEIGHT), true);
		//nana::threads::pool_push(_workerThread, [=]
//{
		_infEng.get_bc().start_bc("poison");
		//});
		row++;
		_lbPoison.create(*this, nana::rectangle(PAD, _lbSymptoms.pos().y + _lbSymptoms.size().height, int(float(W_W)*.80), CTRL_HEIGHT), true);
		_lbPoison.fgcolor(nana::colors::red);
		nana::size&sz = Get_global_window()->size();
		sz.height += CTRL_HEIGHT;
		Get_global_window()->size(sz);
		if (_infEng.get_bc().is_conclusion_available())
		{
			string poison = _infEng.get_bc().get_conclusion().value();
			string poison_upper = StringToUpper(poison);
			nana::msgbox mb(*this, "Poison Detected!!");
			mb << "Poison Detected for the given symptoms!\nThe poison is:" + poison_upper;
			mb.show();
			//add_symptom(string("Detected poison: " + poison).c_str());
			_lbPoison.caption("Detected poison: " + poison_upper);

			_infEng.get_fc().start_fc({ variable("poison", poison.c_str()) });
			variable& conc = _infEng.get_fc().get_conclusion();
			string antedote = StringToUpper(conc.value());
			mb << "Antidote for the Poison is: " + antedote;
			mb.show();
			row++;
			sz = _lbPoison.size();
			sz.height += CTRL_HEIGHT;
			_lbPoison.size(sz);
			string strz = _lbPoison.caption();
			strz += "\nAntidote for the Poison is: " + antedote;
			_lbPoison.caption(strz);

			sz = Get_global_window()->size();
			sz.height += CTRL_HEIGHT;
			Get_global_window()->size(sz);
		}
		else
		{
			nana::msgbox mb(*this, "No Poison Detected!");
			mb << "No Poison Detected for the given symptoms! Sorry";
			_lbPoison.caption("No Poison Detected for the given symptoms");

			mb.show();
			return;
		}
	});
}

string GUIWindow::ask_symptom(const char* symptom)
{
	nana::msgbox mb(*this, "What is the symptom?", nana::msgbox::yes_no);
	mb.icon(mb.icon_question);
	mb << "Is symptom includes\"" + string(symptom) + "\"?";
	bool is_yes = (mb.show() == nana::msgbox::pick_yes);
	mb.clear();

	if (is_yes)
	{
		add_symptom(symptom);
		return "true";
	}
	return "false";
}

void GUIWindow::add_symptom(const char* symptom)
{
	row++;
	nana::size&sz = this->size();
	sz.height += 15;
	this->size(sz);
	string sss = _lbSymptoms.caption();
	sss += '\n';
	sss += symptom;
	_lbSymptoms.caption(sss);
	nana::size ss = _lbSymptoms.size();
	ss.height += 15;
	_lbSymptoms.size(ss);
	//lbl.close();
}

static std::shared_ptr<GUIWindow> g_global_ptr_window;
std::shared_ptr<GUIWindow> Get_global_window()
{
	GUIWindow* gg = g_global_ptr_window.get();
	if (gg == nullptr)
	{
		g_global_ptr_window.reset(new GUIWindow());
	}
	return g_global_ptr_window;
}

void user_input(variable& var) {
	cout << "Enter Value for Variable \"" << var.name() << "\":";
	string val = Get_global_window()->ask_symptom(var.name().c_str());
	//string readval;
	//cin >> readval;
	var.set_value(val.c_str());
}