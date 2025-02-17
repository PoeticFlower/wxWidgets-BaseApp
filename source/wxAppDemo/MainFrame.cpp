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
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <TableEx.hpp>
#include <TableExAdapter.hpp>
#include <ListViewEx.h>
#include "GlobalConstants.h"
#include "MainFrame.h"

MainFrame::MainFrame(const wxString& title)
  : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxDefaultSize)
  , m_pListViewMain(nullptr)
  , m_adapterDemo(&m_tableDemo)
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
  using ColumnType = ColumnInfo<TableExtraInfo>::ColumnType;

  ListViewEx *pListView = m_pListViewMain;
  if (nullptr == pListView)
    return;

  m_tableDemo.SetColumnInfo(0, { ColumnType::UINT32, "%d", nullptr, { wxLIST_FORMAT_CENTRE, 50 }, "ID"      });
  m_tableDemo.SetColumnInfo(1, { ColumnType::UINT32, "%X", nullptr, { wxLIST_FORMAT_LEFT  , 90 }, "Addr"    });
  m_tableDemo.SetColumnInfo(2, { ColumnType::STRING, "%s", nullptr, { wxLIST_FORMAT_RIGHT , 80 }, "Name"    });
  m_tableDemo.SetColumnInfo(3, { ColumnType::UINT32, "%d", nullptr, { wxLIST_FORMAT_CENTRE, 80 }, "Score 1" });
  m_tableDemo.SetColumnInfo(4, { ColumnType::UINT32, "%d", nullptr, { wxLIST_FORMAT_CENTRE, 80 }, "Score 2" });
  m_tableDemo.SetColumnInfo(5, { ColumnType::UINT32, "%d", nullptr, { wxLIST_FORMAT_CENTRE, 80 }, "Score 3" });

  m_tableDemo.UpsertRow(0, { 0, 900, "Ethan"   , 90,  80, 130 });
  m_tableDemo.UpsertRow(1, { 1, 910, "Olivia"  , 80,  90,  99 });
  m_tableDemo.UpsertRow(2, { 2, 920, "Lucas"   , 70, 100,  95 });
  m_tableDemo.UpsertRow(3, { 3, 930, "Sophia"  , 60,  30, 121 });
  m_tableDemo.UpsertRow(4, { 4, 940, "Mason"   , 50,  56,  95 });
  m_tableDemo.UpsertRow(5, { 5, 950, "Isabella", 45,  77, 131 });

  pListView->SetTable(&m_adapterDemo);
}
