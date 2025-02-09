/*****************************************************************************
 *
 * Copyright (C) 2015-2025 Poetic Flower Studio. All Rights Reserved.
 *
 * CONFIDENTIALITY:  This software source code is considered confidential
 * information. It must be kept confidential in accordance with the terms
 * and conditions of your Software License Agreement.
 *
 *****************************************************************************/

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "ListViewEx.h"

ListViewEx::ListViewEx(wxWindow* parent, wxWindowID id, const wxPoint& pos,
  const wxSize& size, long style)
  : wxListView(parent, id, pos, size, style)
  , m_sortColumn(-1)
  , m_ascending(true)
{
  Bind(wxEVT_LIST_COL_CLICK, &ListViewEx::OnColumnClick, this);
}

long ListViewEx::InsertItem(long index, const wxString& label)
{
  return InsertItem(index, label, -1);
}

long ListViewEx::InsertItem(long index, const wxString& label, int imageIndex)
{
  long itemIndex = wxListView::InsertItem(index, label, imageIndex);
  if (itemIndex >= 0)
  {
    if (m_items.size() <= static_cast<size_t>(itemIndex))
    {
      m_items.resize(itemIndex + 1);
    }
    m_items[itemIndex].push_back(label);
  }
  return itemIndex;
}

long ListViewEx::InsertItem(const wxListItem& info)
{
  long itemIndex = wxListView::InsertItem(info);
  if (itemIndex >= 0)
  {
    if (m_items.size() <= static_cast<size_t>(itemIndex))
    {
      m_items.resize(itemIndex + 1);
    }
  }
  return itemIndex;
}

bool ListViewEx::SetItem(long index, int column, const wxString& text)
{
  if (index >= 0 && index < static_cast<long>(m_items.size()))
  {
    if (m_items[index].size() <= static_cast<size_t>(column))
    {
      m_items[index].resize(column + 1);
    }
    m_items[index][column] = text;
  }
  return wxListView::SetItem(index, column, text);
}

bool ListViewEx::SetItem(wxListItem& info)
{
  long index = info.GetId();
  int column = info.GetColumn();
  wxString text = info.GetText();
  if (index >= 0 && index < static_cast<long>(m_items.size()))
  {
    if (m_items[index].size() <= static_cast<size_t>(column))
    {
      m_items[index].resize(column + 1);
    }
    m_items[index][column] = text;
  }
  return wxListView::SetItem(info);
}

bool ListViewEx::SetItemText(long index, const wxString& text)
{
  if (index >= 0 && index < static_cast<long>(m_items.size()))
  {
    m_items[index][0] = text;
  }
  return wxListView::SetItem(index, 0, text);
}

void ListViewEx::OnColumnClick(wxListEvent& event)
{
  int column = event.GetColumn();
  if (column == m_sortColumn)
  {
    m_ascending = !m_ascending;
  }
  else
  {
    m_sortColumn = column;
    m_ascending = true;
  }
  UpdateColumnHeader();
  SortItems();
}

void ListViewEx::SortItems()
{
  if (m_sortColumn < 0)
    return;

  std::stable_sort(
    m_items.begin(),
    m_items.end(),
    [this](const std::vector<wxString>& a, const std::vector<wxString>& b)
  {
    wxString sa = a[m_sortColumn], sb = b[m_sortColumn];
    double da = 0, db = 0;
    long la = 0, lb = 0;
    bool isNumberA = sa.ToDouble(&da) || sa.ToLong(&la);
    bool isNumberB = sb.ToDouble(&db) || sb.ToLong(&lb);

    if (isNumberA && isNumberB)
    {
      return m_ascending ? (da < db) : (da > db);
    }
    return m_ascending ? (sa.Cmp(sb) < 0) : (sa.Cmp(sb) > 0);
  });

  RefreshList();
}

void ListViewEx::RefreshList()
{
  Freeze();
  for (size_t i = 0; i < m_items.size(); ++i)
  {
    for (size_t j = 0; j < m_items[i].size(); ++j)
    {
      SetItem(i, j, m_items[i][j]);
    }
  }
  Thaw();
  Refresh();
}

void ListViewEx::UpdateColumnHeader()
{
  for (int i = 0; i < GetColumnCount(); ++i)
  {
    wxListItem colItem;
    colItem.SetMask(wxLIST_MASK_TEXT);
    GetColumn(i, colItem);
    wxString title = colItem.GetText();

    size_t arrowPos = title.find(" ↑");
    if (arrowPos == wxString::npos)
      arrowPos = title.find(" ↓");
    if (arrowPos != wxString::npos)
      title = title.substr(0, arrowPos);

    if (i == m_sortColumn)
    {
      title += m_ascending ? " ↑" : " ↓";
    }

    colItem.SetText(title);
    SetColumn(i, colItem);
  }
}
