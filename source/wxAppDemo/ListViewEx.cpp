/*****************************************************************************
 *
 * Copyright (C) 2015-2025 Poetic Flower Studio. All Rights Reserved.
 *
 * CONFIDENTIALITY:  This software source code is considered confidential
 * information. It must be kept confidential in accordance with the terms
 * and conditions of your Software License Agreement.
 *
 *****************************************************************************/

#include <wx/listctrl.h>
#include <wx/menu.h>
#include <wx/filedlg.h>
#include "TableEx.hpp"
#include "TableExAdapter.hpp"
#include "ListViewEx.h"

ListViewEx::ListViewEx(
  wxWindow        *parent,
  wxWindowID       id,
  const wxPoint   &pos   /*= wxDefaultPosition */,
  const wxSize    &size  /*= wxDefaultSize     */,
  long             style /*= wxLC_REPORT       */)
  : wxListView(parent, id, pos, size, style)
  , m_adapter         (nullptr)
  , m_editCallback    (nullptr)
  , m_editCallbackPara(nullptr)
  , m_lastSortedCol   (-1)
  , m_sortAscending   (true)
  , m_rightClickedCol (-1)
{
  Bind(wxEVT_LIST_COL_CLICK       , &ListViewEx::OnColumnClick     , this);
  Bind(wxEVT_LIST_COL_RIGHT_CLICK , &ListViewEx::OnColumnRightClick, this);
  Bind(wxEVT_LIST_ITEM_RIGHT_CLICK, &ListViewEx::OnItemRightClick  , this);
  Bind(wxEVT_LIST_ITEM_ACTIVATED  , &ListViewEx::OnItemDoubleClick , this);

  Bind(wxEVT_MENU, &ListViewEx::OnSetupFilter  , this, MENU_ITEM_SETUP_FILTER   );
  Bind(wxEVT_MENU, &ListViewEx::OnClearFilter  , this, MENU_ITEM_CLEAR_FILTER   );
  Bind(wxEVT_MENU, &ListViewEx::OnEditItem     , this, MENU_ITEM_EDIT_ITEM      );
  Bind(wxEVT_MENU, &ListViewEx::OnBatchEditItem, this, MENU_ITEM_BATCH_EDIT_ITEM);
  Bind(wxEVT_MENU, &ListViewEx::OnExportCSV    , this, MENU_ITEM_EXPORT_CSV     );
}

void ListViewEx::SetEditCallback(
  ListViewEx::EditCallback     callback,
  void            *callbackParam)
{
  m_editCallback      = callback;
  m_editCallbackPara  = callbackParam;
}

void ListViewEx::SetTable(ITableExAdapter* adapter)
{
  if (m_adapter == adapter)
  {
    // Adapter remains the same, perform partial refresh
    m_adapter->PartialRefreshList(this);
  }
  else
  {
    // Adapter changed, perform full refresh
    m_adapter = adapter;
    m_adapter->FullRefreshList   (this);
  }
}

void ListViewEx::OnItemRightClick(wxListEvent& event)
{
  wxMenu               menu;
  long selectedCount = GetSelectedItemCount();
  // Dynamically add menu item
  if      (selectedCount == 1)
  {
    menu.Append(MENU_ITEM_EDIT_ITEM, "Edit");
  }
  else if (selectedCount >  1)
  {
    menu.Append(MENU_ITEM_BATCH_EDIT_ITEM, "Batch Edit");
  }

  menu.Append(MENU_ITEM_EXPORT_CSV, "Export CSV");
  PopupMenu(&menu);
}

void ListViewEx::OnItemDoubleClick(wxListEvent& event)
{
  OnEditItem(event);
}

void ListViewEx::OnColumnClick(wxListEvent& event)
{
  if (!m_adapter)
    return;

  int col = event.GetColumn();
  if (m_lastSortedCol == col)
  {
    m_sortAscending = !m_sortAscending;
  }
  else
  {
    m_lastSortedCol = col;
    m_sortAscending = true;
  }

  m_adapter->SortByColumn(col, m_sortAscending);
  SetTable(m_adapter);

  UpdateColumnText(col);
}

void ListViewEx::OnColumnRightClick(wxListEvent& event)
{
  wxMenu menu;
  menu.Append(MENU_ITEM_SETUP_FILTER    , "Setup Filter"    );
  menu.Append(MENU_ITEM_CLEAR_FILTER    , "Clear Filter"    );
  menu.Append(MENU_ITEM_CLEAR_ALL_FILTER, "Clear All Filter");
  PopupMenu(&menu);
}

void ListViewEx::OnEditItem(wxCommandEvent&)
{
  if (m_editCallback)
  {
    long itemIndex = GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (itemIndex != -1)
    {
      std::vector<wxString> editData;
      editData.push_back(GetItemText(itemIndex, 0));

      m_editCallback(editData, m_editCallbackPara);
      SetTable(m_adapter);
    }
  }
}

void ListViewEx::OnBatchEditItem(wxCommandEvent&)
{
  if (m_editCallback)
  {
    std::vector<wxString> editData;
    long                  itemIndex = -1;
    while (-1 != (itemIndex = GetNextItem(
      itemIndex, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)))
    {
      editData.push_back(GetItemText(itemIndex, 0));
    }

    if (editData.size() > 1)
    {
      m_editCallback(editData, m_editCallbackPara);
      SetTable(m_adapter);
    }
  }
}

void ListViewEx::OnExportCSV(wxCommandEvent&)
{
  wxFileDialog saveFileDialog(this, "Save CSV", "", "export.csv",
    "CSV files (*.csv)|*.csv", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

  if (saveFileDialog.ShowModal() == wxID_CANCEL)
    return;

  wxString filePath = saveFileDialog.GetPath();
  if (m_adapter)
  {
    m_adapter->ExportToCSV(filePath.ToStdString());
  }
}

void ListViewEx::OnSetupFilter(wxCommandEvent&)
{
}

void ListViewEx::OnClearFilter(wxCommandEvent&)
{
  if (m_adapter && m_rightClickedCol >= 0)
  {
    m_adapter->ClearFilter(m_rightClickedCol);
    SetTable(m_adapter);
  }
}

void ListViewEx::OnClearAllFilters(wxCommandEvent&)
{
  if (m_adapter)
  {
    m_adapter->ClearFilter(-1);
    SetTable(m_adapter);
  }
}

void ListViewEx::UpdateColumnText(int sortedCol)
{
  for (int i = 0; i < GetColumnCount(); ++i)
  {
    wxListItem item;
    item.SetMask(wxLIST_MASK_TEXT);
    GetColumn(i, item);
    wxString colName = item.GetText();

    colName.Replace(" ▲", "");
    colName.Replace(" ▼", "");

    if (i == sortedCol)
    {
      colName += m_sortAscending ? " ▲" : " ▼";
    }

    item.SetText(colName);
    SetColumn(i, item);
  }
}
