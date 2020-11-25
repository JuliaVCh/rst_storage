#pragma once

#include <wx/wx.h>

#include "storage.h"


class cEditFrame : public wxDialog {
public:
	cEditFrame(wxFrame* parent, std::unique_ptr<Storage>* data_holder);

private:
	std::unique_ptr<Storage>* main_data_holder;
	std::unique_ptr<Storage> local_data_holder;

	wxString id_str;
	wxTextCtrl* tc2 = nullptr;
	wxListCtrl* lc = nullptr;
	wxButton* btn_save = nullptr;
	wxButton* btn_cancel = nullptr;

	void EnableSave();
	void DisableSave();

	void OnAdd(wxCommandEvent& evt);
	void OnDel(wxCommandEvent& evt);
	void OnSave(wxCommandEvent& evt);
	void OnCancel(wxCommandEvent& evt);
	void OnClose(wxCloseEvent& evt);
};

enum
{
	ID_TC_Id = 1000,
	ID_TC_Name = 1001,
	ID_Button_Add = 1002,
	ID_Full_List = 1003,
	ID_Button_Del = 1004,
	ID_Button_Save = 1005,
	ID_Button_Cancel = 1006,
	ID_Edit_Window = 1007,
};

enum
{
	Column_IDX_ID = 0,
	Column_IDX_NAME = 1,
};

