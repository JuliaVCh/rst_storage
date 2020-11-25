#include "cMain.h"

#ifdef MY_TEST
#include "test_runner.h"

void TestIDConversion() {
	std::string expected = "304DB75F196000180001C13A";
	const auto converted = IDToStr(StrToID(expected));

	ASSERT_EQUAL(converted, expected);
}

#endif

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
EVT_MENU(ID_Menu_Edit, cMain::OnMenuEdit)
EVT_MENU(ID_Menu_Exit, cMain::OnMenuExit)
EVT_BUTTON(ID_Button_Clear, cMain::OnButtonClicked)
wxEND_EVENT_TABLE()


cMain::cMain() : wxFrame(nullptr, wxID_ANY, "rst_storage", wxPoint(50, 50))
{
	m_SetMenuBar();

	// main window layout:
	SetBackgroundColour(*wxWHITE);
	wxFont myFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	this->SetFont(myFont);
	wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);

	// 1 - income side
	wxBoxSizer* vbox1 = new wxBoxSizer(wxVERTICAL);

	wxStaticText* st11 = new wxStaticText(this, wxID_ANY, wxT("Поле ввода данных для приема"));
	vbox1->Add(st11, 0, wxALIGN_CENTER | wxALL, 10);

	wxTextCtrl* tc1 = new wxTextCtrl(this, ID_Income_Enter_Form, "", wxDefaultPosition, wxSize(250, 30), wxTE_PROCESS_ENTER);
	wxTextValidator validator1(wxFILTER_EMPTY | wxFILTER_INCLUDE_CHAR_LIST, &id_income_str);
	validator1.SetCharIncludes("0123456789ABCDEFabcdef ");
	tc1->SetValidator(validator1);
	tc1->Bind(wxEVT_TEXT_ENTER, &cMain::OnEnterPressed, this);
	tc1->ForceUpper();
	vbox1->Add(tc1, 0, wxEXPAND | wxALL, 10);

	wxStaticText* st12 = new wxStaticText(this, wxID_ANY, wxT("ПРИЁМ"));
	vbox1->Add(st12, 0, wxALIGN_CENTER | wxALL, 25);

	lc1 = new wxListCtrl(this, ID_Income_List, wxDefaultPosition, wxSize(400, 500), wxLC_REPORT);
	lc1->InsertColumn(Column_idx_id, "ID", wxLIST_FORMAT_CENTRE, 0);
	lc1->InsertColumn(Column_idx_Name, "Наименование", wxLIST_FORMAT_CENTRE, 300);
	lc1->InsertColumn(Column_idx_Count, "Количество", wxLIST_FORMAT_CENTRE, 100);
	vbox1->Add(lc1, 1, wxEXPAND | wxALL, 10);

	hbox->Add(vbox1, 1, wxEXPAND | wxALL, 10);

	// 2 - outcome side
	wxBoxSizer* vbox2 = new wxBoxSizer(wxVERTICAL);

	wxStaticText* st21 = new wxStaticText(this, wxID_ANY, wxT("Поле ввода данных для отгрузки"));
	vbox2->Add(st21, 0, wxALIGN_CENTER | wxALL, 10);

	wxTextCtrl* tc2 = new wxTextCtrl(this, ID_Outcome_Enter_Form, "", wxDefaultPosition, wxSize(250, 30), wxTE_PROCESS_ENTER);
	wxTextValidator validator2(wxFILTER_EMPTY | wxFILTER_INCLUDE_CHAR_LIST, &id_outcome_str);
	validator2.SetCharIncludes("0123456789ABCDEFabcdef ");
	tc2->SetValidator(validator2);
	tc2->Bind(wxEVT_TEXT_ENTER, &cMain::OnEnterPressed, this);
	tc2->ForceUpper();
	vbox2->Add(tc2, 0, wxEXPAND | wxALL, 10);

	wxStaticText* st22 = new wxStaticText(this, wxID_ANY, wxT("ОТГРУЗКА"));
	vbox2->Add(st22, 0, wxALIGN_CENTER | wxALL, 25);

	lc2 = new wxListCtrl(this, ID_Outcome_List, wxDefaultPosition, wxSize(400, 500), wxLC_REPORT);
	lc2->InsertColumn(Column_idx_id, "ID", wxLIST_FORMAT_CENTRE, 0);
	lc2->InsertColumn(Column_idx_Name, ("Наименование"), wxLIST_FORMAT_CENTRE, 300);
	lc2->InsertColumn(Column_idx_Count, ("Количество"), wxLIST_FORMAT_CENTRE, 100);
	vbox2->Add(lc2, 1, wxEXPAND | wxALL, 10);

	hbox->Add(vbox2, 1, wxEXPAND | wxALL, 10);

	// 3 - add both sides and button
	main_sizer->Add(hbox, 1, wxEXPAND);
	wxButton* btn = new wxButton(this, ID_Button_Clear, wxT("Очистить"));
	main_sizer->Add(btn, 0, wxALIGN_LEFT | wxALL, 30);

	// associate sizer with window
	this->SetSizerAndFit(main_sizer);
	main_sizer->Layout();

	// Load DB
	data_holder = std::make_unique<Storage>();

	try
	{
		auto success = data_holder->LoadFromFile();

		if (!success)
		{
			wxMessageBox("Существующего справочника не найдено");
		}

	}
	catch (const std::runtime_error&)
	{
		Close();
	}

#ifdef MY_TEST
	//TestRunner tr;
	//RUN_TEST(tr, TestIDConversion);
#endif
}


void cMain::m_SetMenuBar()
{
	m_MenuBar = new wxMenuBar();
	this->SetMenuBar(m_MenuBar);

	auto* menuFile = new wxMenu();
	menuFile->Append(ID_Menu_Edit, wxT("Edit"));
	menuFile->Append(ID_Menu_Exit, wxT("Exit"));

	m_MenuBar->Append(menuFile, wxT("Справочник"));
}


void cMain::OnMenuEdit(wxCommandEvent& evt)
{
	auto* frame = new cEditFrame(this, &data_holder);
	frame->Show();
	evt.Skip();
}

void cMain::OnMenuExit(wxCommandEvent& evt)
{
	Close();
	evt.Skip();
}


void AddItem(wxListCtrl* lc, const wxString& id_str, const wxString& name)
{
	const auto index = lc->GetItemCount();
	lc->InsertItem(index, id_str);
	lc->SetItem(index, Column_idx_Name, name);
	lc->SetItem(index, Column_idx_Count, "1");
}

void IncremetItem(wxListCtrl* lc, long index)
{
	const auto count = ConvertToInt(static_cast<std::string>(lc->GetItemText(index, Column_idx_Count)));
	lc->SetItem(index, Column_idx_Count, std::to_string(count + 1));
}

void DecrementItem(wxListCtrl* lc, long index)
{
	auto count = ConvertToInt(static_cast<std::string>(lc->GetItemText(index, Column_idx_Count)));
	--count;

	if (count)
	{
		lc->SetItem(index, Column_idx_Count, std::to_string(count));
	}
	else
	{
		lc->DeleteItem(index);
	}
}

void cMain::OnEnterPressed(wxCommandEvent& evt)
{
	const auto evt_id = evt.GetId();

	TransferDataFromWindow();
	const auto ids_str = evt_id == ID_Income_Enter_Form ? static_cast<std::string>(id_income_str) : static_cast<std::string>(id_outcome_str);

	if (evt_id == ID_Income_Enter_Form)
	{
		id_income_str = "";
	}
	else
	{
		id_outcome_str = "";
	}
	TransferDataToWindow();

	const auto ids_arr = SplitBy(static_cast<std::string>(ids_str), " ");
	for (const auto& id_str : ids_arr)
	{
		if (id_str.size() != 24)
		{
			wxMessageBox(id_str + " не является валидным идентификатором");
			evt.Skip();
			return;
		}
	}

	for (const auto& id_str : ids_arr)
	{
		auto index_income = lc1->FindItem(-1, id_str);
		auto index_outcome = lc2->FindItem(-1, id_str);

		if (index_income == -1 && index_outcome == -1)
		{
			const auto* obj_name = data_holder->GetObj(id_str);
			if (!obj_name)
			{
				continue;
			}

			if (evt_id == ID_Income_Enter_Form)
			{
				AddItem(lc1, id_str, *obj_name);
			}
			else if (evt_id == ID_Outcome_Enter_Form)
			{
				AddItem(lc2, id_str, *obj_name);
			}
		}
		else if (index_income == -1)
		{
			if (evt_id == ID_Income_Enter_Form)
			{
				auto obj_name = lc2->GetItemText(index_outcome, Column_idx_Name);
				AddItem(lc1, id_str, std::move(obj_name));
				DecrementItem(lc2, index_outcome);
			}
			else if (evt_id == ID_Outcome_Enter_Form)
			{
				IncremetItem(lc2, index_outcome);
			}
		}
		else if (index_outcome == -1)
		{
			if (evt_id == ID_Income_Enter_Form)
			{
				IncremetItem(lc1, index_income);
			}
			else if (evt_id == ID_Outcome_Enter_Form)
			{
				auto obj_name = lc1->GetItemText(index_income, Column_idx_Name);
				AddItem(lc2, id_str, std::move(obj_name));
				DecrementItem(lc1, index_income);
			}
		}
		else
		{
			if (evt_id == ID_Income_Enter_Form)
			{
				IncremetItem(lc1, index_income);
				DecrementItem(lc2, index_outcome);
			}
			else if (evt_id == ID_Outcome_Enter_Form)
			{
				IncremetItem(lc2, index_outcome);
				DecrementItem(lc1, index_income);
			}
		}
	}

	//evt.Skip();
}

void cMain::OnButtonClicked(wxCommandEvent& evt)
{
	lc1->DeleteAllItems();
	lc2->DeleteAllItems();

	evt.Skip();
}
