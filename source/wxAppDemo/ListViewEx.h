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
  // Callback function for handling edit operations
  using EditCallback = std::function<void(std::vector<wxString>&, void*)>;

  // Constructor
  explicit ListViewEx      (wxWindow        *parent,
                            wxWindowID       id,
                            const wxPoint   &pos   = wxDefaultPosition,
                            const wxSize    &size  = wxDefaultSize,
                            long             style = wxLC_REPORT);

  // Set the callback function for handling edit operations
  void SetEditCallback     (EditCallback callback, void* callbackParam);
  // Set the table adapter and refresh ListView
  void SetTable            (ITableExAdapter* adapter);

  // Handle right-click on a data row
  void OnItemRightClick    (wxListEvent& event);
  // Handle double-click on a data row
  void OnItemDoubleClick   (wxListEvent& event);
  // Handle column click event for sorting
  void OnColumnClick       (wxListEvent& event);
  // Handle right-click on the column header
  void OnColumnRightClick  (wxListEvent& event);
  // Handle single row edit
  void OnEditItem          (wxCommandEvent&);
  // Handle batch editing
  void OnBatchEditItem     (wxCommandEvent&);
  // Handle exporting data to CSV
  void OnExportCSV         (wxCommandEvent&);
  // Show a dialog for updating the filter
  void OnSetupFilter       (wxCommandEvent&);
  // Clear the filter for the selected column
  void OnClearFilter       (wxCommandEvent&);
  // Clear all filters
  void OnClearAllFilters   (wxCommandEvent&);
protected:
  // Update column text with sorting indicator
  void UpdateColumnText    (int sortedCol);
protected:
  // Prevent direct modification of wxListView
  void InsertItem          (long, const wxString&)         = delete;
  void SetItem             (long, int, const wxString&)    = delete;
  void DeleteAllItems      ()                              = delete;

protected:
  // Pointer to the TableExAdapter managing data
  ITableExAdapter       *m_adapter;
  // Callback function for handling edit operations
  EditCallback           m_editCallback;
  // Additional parameter for the edit callback
  void                  *m_editCallbackPara;
  // Index of the last sorted column
  int                    m_lastSortedCol;
  // Sorting direction flag (true for ascending, false for descending)
  bool                   m_sortAscending;
  // Index of the column that was right-clicked
  int                    m_rightClickedCol;
  // Define menu item IDs
  const int32_t MENU_ITEM_SETUP_FILTER           = 32100;
  const int32_t MENU_ITEM_CLEAR_FILTER           = 32101;
  const int32_t MENU_ITEM_CLEAR_ALL_FILTER       = 32102;
  const int32_t MENU_ITEM_EDIT_ITEM              = 32103;
  const int32_t MENU_ITEM_BATCH_EDIT_ITEM        = 32104;
  const int32_t MENU_ITEM_EXPORT_CSV             = 32105;
};

#endif // GUI_WXWIDGETS_MAIN_APP_LIST_VIEW_EX_H_
