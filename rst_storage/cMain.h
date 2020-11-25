#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/textctrl.h>
#include <memory>

#include "storage.h"
#include "cEditFrame.h"


class cMain : public wxFrame
{
public:
	cMain();

private:
	std::unique_ptr<Storage> data_holder;

	wxMenuBar* m_MenuBar = nullptr;
	wxString id_income_str = "";
	wxString id_outcome_str = "";
	wxListCtrl* lc1 = nullptr;
	wxListCtrl* lc2 = nullptr;

	void m_SetMenuBar();

	void OnMenuEdit(wxCommandEvent &evt);
	void OnMenuExit(wxCommandEvent &evt);
	
	void OnEnterPressed(wxCommandEvent &evt);
	void OnButtonClicked(wxCommandEvent &evt);

	wxDECLARE_EVENT_TABLE();
};


enum
{
	ID_Menu_New = 100,
	ID_Menu_Open = 101,
	ID_Menu_SaveAs = 102,
	ID_Menu_Edit = 103,
	ID_Menu_Exit = 104,
	ID_Income_Enter_Form = 105,
	ID_Outcome_Enter_Form = 106,
	ID_Income_List = 107,
	ID_Outcome_List = 108,
	ID_Button_Clear = 109,
};

enum
{
	Column_idx_id = 0,
	Column_idx_Name = 1,
	Column_idx_Count = 2,
};
