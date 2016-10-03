
#include "stdafx.h"


#include <nana/gui/widgets/picture.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/combox.hpp>
#include <nana/gui/widgets/listbox.hpp>

#include <nana/gui/widgets/textbox.hpp>

using namespace std;
using namespace nana;

void ask(string sym)
{
	form fm;

	fm.caption("Poison Diagnosis");


	label title(fm, nana::rectangle(0, 0, 250, 60));
	label lab(fm, nana::rectangle(0, 0, 100, 20));
	label diagnosis(fm, nana::rectangle(0, 0, 250, 60));
	combox symptom{ fm };
	symptom.push_back("yes");
	symptom.push_back("no");

	button butt{ fm, "OK" };

	title.caption("Is the patient suffering from the following?");
	diagnosis.caption(sym);

	butt.events().click([&]
	{
		lab.caption(symptom.caption());
		fm.get_place().field_display("label", true);
		fm.collocate();
	});

	fm.div("vert<title weight=25><symptom weight=25><butt weight=25><<><>label weight=25>");
	fm["symptom"] << diagnosis << symptom;

	fm["label"] << lab;
	fm.get_place().field_display("label", false); //don't display the label field now.

	fm["diagnosis"] << diagnosis;
	fm.get_place().field_display("diagnosis", false); //don't display the label field now.
	fm["butt"] << butt;
	fm.collocate();
	fm.show();
	exec();

}

#include "InferenceEngine.h"
static InferenceEngine g_inferenceEngn;

#if defined(__GNUC__)
int main()
{
#else
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
#endif
	start_klog();
	g_inferenceEngn.build_KB();
	ask("Nauseous");
	return 0;
}