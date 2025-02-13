/*****************************************************************************
 *
 * Copyright (C) 2015-2025 Poetic Flower Studio. All Rights Reserved.
 *
 * CONFIDENTIALITY:  This software source code is considered confidential
 * information. It must be kept confidential in accordance with the terms
 * and conditions of your Software License Agreement.
 *
 *****************************************************************************/

#ifndef   GUI_WXWIDGETS_MAIN_APP_LIST_VIEW_EX_H_
#define   GUI_WXWIDGETS_MAIN_APP_LIST_VIEW_EX_H_

class ListViewEx : public wxListView
{
public:
  ListViewEx(
    wxWindow*      parent,
    wxWindowID     id,
    const wxPoint& pos  = wxDefaultPosition,
    const wxSize&  size = wxDefaultSize,
    long           style = wxLC_REPORT);

  long InsertItem(long index, const wxString& label);
  long InsertItem(long index, const wxString& label, int imageIndex);
  long InsertItem(const wxListItem& info);

  bool SetItem(long index, int column, const wxString& text);
  bool SetItem(wxListItem& info);
  bool SetItemText(long index, const wxString& text);

protected:
  void OnColumnClick(wxListEvent& event);
  void SortItems();
  void RefreshList();
  void UpdateColumnHeader();

private:
  int m_sortColumn;
  bool m_ascending;
  std::vector<std::vector<wxString>> m_items;
};

#endif // GUI_WXWIDGETS_MAIN_APP_LIST_VIEW_EX_H_
