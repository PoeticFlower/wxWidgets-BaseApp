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

#include <fstream>

/*****************************************************************************
 *
 * CLASS   : ITableExAdapter
 * PURPOSE : Abstract base class, adapting to different TableEx<C, N>
 * AUTHOR  : Peixuan Zhang <Zhang@PoeticFlower.CN>
 * COMMENTS: Used to improve the dynamic expansion capability of ListViewEx
 *
 *****************************************************************************/

class ITableExAdapter
{
public:
  virtual void            ExportToCSV(const std::string& filename)      = 0;
  virtual void              SetFilter(size_t col, std::function<bool(
                                                  const void*)> filter) = 0;
  virtual void            ClearFilter(size_t col = -1)                  = 0;
  virtual void           SortByColumn(size_t col,
                                      bool ascending = true)            = 0;
  virtual void     FullRefreshList   (wxListView* listView)             = 0;
  virtual void  PartialRefreshList   (wxListView* listView)             = 0;
  virtual      ~ITableExAdapter      (                    )       = default;
};

/*****************************************************************************
 *
 * CLASS   : TableExAdapter
 * PURPOSE : Template-based concrete adapter for TableEx<C, N>
 * AUTHOR  : Peixuan Zhang <Zhang@PoeticFlower.CN>
 * COMMENTS: Used to improve the dynamic expansion capability of ListViewEx
 *
 *****************************************************************************/

template <typename C, size_t N>
class TableExAdapter : public ITableExAdapter
{
public:
  TableEx<C, N>     *table;                 // TableEx object actually used
  TableEx<C, N>      previousTableSnapshot; // Used to compare data changes

  // Constructor
  explicit TableExAdapter(TableEx<C, N> *t)
    : table(t)
    , previousTableSnapshot(*t)
  {
  }

  // Export data to CSV file
  void ExportToCSV(const std::string& filename)
  {
    if (!table)
      return;

    std::ofstream file(filename);
    if (!file.is_open())
      return;

    // Write column headers
    for (size_t col = 0; col < N; ++col)
    {
      file << table->GetColumnInfo(col).name;
      if (col < N - 1)
        file << ",";
    }
    file << "\n";

    // Write row data
    table->ForEach([&file](const typename TableEx<C, N>::RowData& row)
    {
      for (size_t col = 0; col < N; ++col)
      {
        file << row[col].FormatValue();
        if (col < N - 1)
          file << ",";
      }
      file << "\n";
    });

    file.close();
  }
  // Set filter function for a specific column
  void SetFilter(size_t col, std::function<bool(const void*)> filter)
  {
    table->SetFilter(col, filter);
  }
  // Clear filter for a specific column or all columns if col is out of range
  void ClearFilter(size_t col = -1)
  {
    table->ClearFilter(col);
  }
  // Sort rows by a specific column
  void SortByColumn(size_t col, bool ascending = true)
  {
    table->SortByColumn(col, ascending);
  }
  // Full Refresh: Clear all data and reload
  void FullRefreshList(wxListView* listView) override
  {
    if (!table || !listView)
      return;

    listView->Freeze();
    listView->ClearAll();

    // Add columns
    for (size_t col = 0; col < N; ++col)
    {
      listView->InsertColumn(
        col,
        table->GetColumnInfo(col).name,
        wxLIST_FORMAT_LEFT,
        150);
    }

    // Insert all data
    int rowIndex = 0;
    table->ForEach([&](const typename TableEx<C, N>::RowData& row)
    {
      long itemIndex = listView->InsertItem(rowIndex, row[0].FormatValue());
      for (size_t col = 1; col < N; ++col)
      {
        listView->SetItem(itemIndex, col, row[col].FormatValue());
      }
      ++rowIndex;
    });

    // Record the latest data snapshot
    previousTableSnapshot = *table;
    listView->Thaw();
  }
  // Partial Refresh: Only update changed rows
  void PartialRefreshList(wxListView* listView) override
  {
    if (!table || !listView)
      return;

    listView->Freeze();

    // Read new data
    std::vector<typename TableEx<C, N>::RowData> newData;
    table->ForEach([&](const typename TableEx<C, N>::RowData& row)
    {
      newData.push_back(row);
    });
    // Read old data
    std::vector<typename TableEx<C, N>::RowData> oldData;
    previousTableSnapshot.ForEach([&](const typename TableEx<C, N>::RowData& row)
    {
      oldData.push_back(row);
    });

    // Calculate the number of rows
    size_t minSize = (newData.size() < oldData.size()) ? newData.size() : oldData.size();
    size_t maxSize = (newData.size() > oldData.size()) ? newData.size() : oldData.size();

    // Update existing row data column by column
    for (size_t rowIndex = 0; rowIndex < minSize; ++rowIndex)
      {
        for (size_t col = 0; col < N; ++col)
        {
          wxString newValue = newData[rowIndex][col].FormatValue();
          wxString oldValue = oldData[rowIndex][col].FormatValue();

          if (newValue != oldValue) // **只有该列数据不同才更新**
          {
            listView->SetItem(rowIndex, col, newValue);
          }
        }
      }

    // If the new data is more than the old data, insert a new row
    for (size_t rowIndex = minSize; rowIndex < newData.size(); ++rowIndex)
    {
      long itemIndex = listView->InsertItem(
        rowIndex, newData[rowIndex][0].FormatValue());
      for (size_t col = 1; col < N; ++col)
      {
        listView->SetItem(
          itemIndex, col, newData[rowIndex][col].FormatValue());
      }
    }

    // If the old data is more than the new data, delete the extra rows
    for (size_t rowIndex = newData.size(); rowIndex < oldData.size(); ++rowIndex)
    {
      listView->DeleteItem(newData.size());
    }

    // Record the latest data snapshot
    previousTableSnapshot = *table;
    listView->Thaw();
  }
};

/*****************************************************************************
 *
 * CLASS   : ListViewEx
 * PURPOSE : Extended wxListView class that integrates with TableExAdapter
 * AUTHOR  : Peixuan Zhang <Zhang@PoeticFlower.CN>
 * COMMENTS: Supports batch data updates, column sorting, data filtering,
 *           and exporting to CSV.
 *
 *****************************************************************************/

class ListViewEx : public wxListView
{
public:
  explicit ListViewEx(
    wxWindow     * parent,
    wxWindowID     id,
    const wxPoint& pos   = wxDefaultPosition,
    const wxSize & size  = wxDefaultSize,
    long           style = wxLC_REPORT | wxLC_SINGLE_SEL
    )
    : wxListView(parent, id, pos, size, style)
    , m_adapter(nullptr)
  {
    Bind(wxEVT_LIST_COL_CLICK, &ListViewEx::OnColumnClick, this);
  }
  // Set the table adapter and refresh ListView
  void SetTable(ITableExAdapter* adapter)
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
      m_adapter->FullRefreshList(this);
    }
  }
  // Export data to CSV file
  void ExportToCSV(const std::string& filename)
  {
    if (m_adapter)
    {
      m_adapter->ExportToCSV(filename);
    }
  }
protected:
  ITableExAdapter  * m_adapter;
  int                m_lastSortedCol = -1;
  bool               m_sortAscending = true;
  // Handle column click event for sorting
  void OnColumnClick(wxListEvent& event)
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
    m_adapter->FullRefreshList(this);
  }
  // Prevent direct modification of wxListView
  void InsertItem    (long, const wxString&) = delete;
  void SetItem       (long, int, const wxString&) = delete;
  void DeleteAllItems() = delete;
};
#endif // GUI_WXWIDGETS_MAIN_APP_LIST_VIEW_EX_H_
