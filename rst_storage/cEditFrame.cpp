#include <wx/listctrl.h>

#include "cEditFrame.h"
#include "id_conversion.h"

const wxString EDIT_WINDOW_TITLE_BASE = "Редактирование справочника";

cEditFrame::cEditFrame(wxFrame* parent, std::unique_ptr<Storage>* data_holder)
	: wxDialog(parent, ID_Edit_Window, EDIT_WINDOW_TITLE_BASE), main_data_holder(data_holder)
{
	this->Bind(wxEVT_CLOSE_WINDOW, &cEditFrame::OnClose, this);
	wxFont myFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	this->SetFont(myFont);
	wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);

	// Add section
	wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* vbox1 = new wxBoxSizer(wxVERTICAL);
	wxStaticText* st1 = new wxStaticText(this, wxID_ANY, wxT("Идентификатор"));
	vbox1->Add(st1, 0, wxLEFT | wxRIGHT, 10);
	vbox1->Add(-1, 20);
	wxStaticText* st2 = new wxStaticText(this, wxID_ANY, wxT("Наименование"));
	vbox1->Add(st2, 0, wxTOP | wxLEFT | wxRIGHT, 10);
	hbox->Add(vbox1, 0, wxALL, 8);

	wxBoxSizer* vbox2 = new wxBoxSizer(wxVERTICAL);
	wxTextCtrl* tc1 = new wxTextCtrl(this, ID_TC_Id, "", wxDefaultPosition, wxSize(250, 30));
	wxTextValidator validator(wxFILTER_EMPTY | wxFILTER_INCLUDE_CHAR_LIST, &id_str);
	validator.SetCharIncludes("0123456789ABCDEFabcdef");
	tc1->SetValidator(validator);
	tc1->ForceUpper();
	vbox2->Add(tc1, 0, wxEXPAND | wxALL, 8);
	tc2 = new wxTextCtrl(this, ID_TC_Name, "", wxDefaultPosition, wxSize(250, 30));
	vbox2->Add(tc2, 0, wxEXPAND | wxALL, 8);
	hbox->Add(vbox2, 0, wxEXPAND);

	main_sizer->Add(hbox, 0, wxEXPAND | wxALL, 10);

	wxButton* btn1 = new wxButton(this, ID_Button_Add, wxT("Добавить"));
	btn1->Bind(wxEVT_BUTTON, &cEditFrame::OnAdd, this);
	main_sizer->Add(btn1, 0, wxALIGN_RIGHT | wxALL, 10);

	// Delete section
	lc = new wxListCtrl(this, ID_Full_List, wxDefaultPosition, wxSize(400, 400), wxLC_REPORT);

	lc->InsertColumn(Column_IDX_ID, "Идентификатор", wxLIST_FORMAT_CENTRE, 250);
	lc->InsertColumn(Column_IDX_NAME, "Наименование", wxLIST_FORMAT_CENTRE, 150);
	for (const auto& [id, name] : (*main_data_holder)->GetData())
	{
		auto index = lc->InsertItem(0, IDToStr(id));
		lc->SetItem(index, Column_IDX_NAME, name);
	}
	main_sizer->Add(lc, 1, wxEXPAND | wxALL, 10);

	wxButton* btn2 = new wxButton(this, ID_Button_Del, wxT("Удалить"));
	btn2->Bind(wxEVT_BUTTON, &cEditFrame::OnDel, this);
	main_sizer->Add(btn2, 0, wxALIGN_RIGHT | wxALL, 10);

	wxBoxSizer* hbox_buttons = new wxBoxSizer(wxHORIZONTAL);
	btn_save = new wxButton(this, ID_Button_Del, wxT("Сохранить"));
	btn_save->Bind(wxEVT_BUTTON, &cEditFrame::OnSave, this);
	btn_cancel = new wxButton(this, ID_Button_Del, wxT("Отменить"));
	btn_cancel->Bind(wxEVT_BUTTON, &cEditFrame::OnCancel, this);
	DisableSave();
	hbox_buttons->Add(btn_save, 0, wxALL, 10);
	hbox_buttons->Add(btn_cancel, 0, wxALL, 10);
	main_sizer->Add(hbox_buttons, 0, wxALIGN_RIGHT | wxALL, 20);

	this->SetSizerAndFit(main_sizer);
	main_sizer->Layout();

	Centre();
}


void cEditFrame::EnableSave()
{
	if (!local_data_holder)
	{
		btn_save->Enable();
		btn_cancel->Enable();
		local_data_holder = (*main_data_holder)->Copy();
	}
}

void cEditFrame::DisableSave()
{
	btn_save->Disable();
	btn_cancel->Disable();
	this->SetTitle(EDIT_WINDOW_TITLE_BASE);
}

void cEditFrame::OnAdd(wxCommandEvent& evt)
{
	if (TransferDataFromWindow() && id_str.size() != 24)
	{
		wxMessageBox("ID должно содержать 24 символа");
		evt.Skip();
		return;
	}

	EnableSave();

	const auto name = tc2->GetValue();
	const auto success = local_data_holder->AddObj(static_cast<std::string>(id_str), static_cast<std::string>(name));

	if (success)
	{
		const auto index = lc->GetItemCount();
		lc->InsertItem(index, id_str);
		lc->SetItem(index, Column_IDX_NAME, name);
		this->SetTitle(EDIT_WINDOW_TITLE_BASE + " *");
	}

	evt.Skip();
}

void cEditFrame::OnDel(wxCommandEvent& evt)
{
	bool first = true;
	long index = -1;

	while (true)
	{
		auto index = lc->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

		if (index == -1)
		{
			break;
		}

		if (first)
		{
			EnableSave();
			this->SetTitle(EDIT_WINDOW_TITLE_BASE + " *");
			first = false;
		}

		const auto id = lc->GetItemText(index, Column_IDX_ID);
		const auto success = local_data_holder->DelObj(static_cast<std::string>(id));

		if (success)
		{
			lc->DeleteItem(index);
		}
	}

	evt.Skip();
}

void cEditFrame::OnSave(wxCommandEvent& evt)
{
	if (!local_data_holder)
	{
		evt.Skip();
		return;
	}

	local_data_holder->SaveToFile();
	*main_data_holder = std::move(local_data_holder);

	DisableSave();
	evt.Skip();
}

void cEditFrame::OnCancel(wxCommandEvent& evt)
{
	local_data_holder = nullptr;

	lc->DeleteAllItems();
	for (const auto& [id, name] : (*main_data_holder)->GetData())
	{
		auto index = lc->InsertItem(0, IDToStr(id));
		lc->SetItem(index, Column_IDX_NAME, name);
	}

	DisableSave();
	evt.Skip();
}

void cEditFrame::OnClose(wxCloseEvent& evt)
{
	if (evt.CanVeto() && local_data_holder)
	{
		if (wxMessageBox("Изменения в открытом справочнике не были сохранены.\nПродолжить без сохранения?", "Warning",
			wxICON_QUESTION | wxYES_NO) != wxYES)
		{
			evt.Veto();
			return;
		}
	}
	Destroy();
	evt.Skip();
}
