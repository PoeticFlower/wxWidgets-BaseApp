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
#include "GlobalConstants.h"
#include "ListViewEx.h"
#include "MainFrame.h"

MainFrame::MainFrame(const wxString& title)
  : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxDefaultSize)
  , m_pListViewMain(nullptr)
{
  m_pListViewMain            = new ListViewEx(this, wxID_ANY);

  InitializeMenuBar();
  InitializeMessageBinding();

  WriteDemoData();
}

void MainFrame::InitializeMenuBar()
{
  wxMenuBar *pMenuBar = new wxMenuBar();
  wxMenu    *pMenu    = nullptr;

  pMenu = new wxMenu();
  pMenu->Append(ID_MENU_FILE_EXIT, "E&xit");
  pMenuBar->Append(pMenu, "&File");

  SetMenuBar(pMenuBar);
}

void MainFrame::InitializeMessageBinding()
{
  Bind(wxEVT_MENU, &MainFrame::OnMenuFileExit                             , this, ID_MENU_FILE_EXIT);
}


void MainFrame::OnMenuFileExit(wxCommandEvent& event)
{
  Close();
  wxApp::GetInstance()->Exit();
}

void MainFrame::WriteDemoData()
{
  ListViewEx *pListView = m_pListViewMain;
  if (nullptr == pListView)
    return;

  // Add Column
  pListView->InsertColumn(0, "ID"  , wxLIST_FORMAT_LEFT,  50);
  pListView->InsertColumn(1, "Name", wxLIST_FORMAT_LEFT, 100);
  pListView->InsertColumn(2, "Age" , wxLIST_FORMAT_LEFT,  50);

  // Add Data
  long index = 0;

  index = pListView->InsertItem(0, "1");
  pListView->SetItem(index, 1, "John");
  pListView->SetItem(index, 2, "20");

  index = pListView->InsertItem(1, "2");
  pListView->SetItem(index, 1, "Cecilia");
  pListView->SetItem(index, 2, "20");

  index = pListView->InsertItem(2, "3");
  pListView->SetItem(index, 1, "Henry");
  pListView->SetItem(index, 2, "35");

  index = pListView->InsertItem(3, "4");
  pListView->SetItem(index, 1, "Percy");
  pListView->SetItem(index, 2, "26");

  index = pListView->InsertItem(4, "5");
  pListView->SetItem(index, 1, "William");
  pListView->SetItem(index, 2, "31");

  index = pListView->InsertItem(5, "10");
  pListView->SetItem(index, 1, "Mary");
  pListView->SetItem(index, 2, "30");
}
