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
#include "GlobalConstants.h"
#include "MainFrame.h"

MainFrame::MainFrame(const wxString& title)
  : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxDefaultSize)
{
  InitializeMenuBar();
  InitializeMessageBinding();
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
