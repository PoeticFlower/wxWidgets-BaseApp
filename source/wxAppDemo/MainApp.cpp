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
#include "TableEx.hpp"
#include "ListViewEx.hpp"
#include "MainFrame.h"
#include "MainApp.h"

bool MainApp::OnInit()
{
  MainFrame* frame = new MainFrame(gcStringApplicationTitle);
  frame->Show(true);
  return true;
}

int MainApp::OnExit()
{
  return wxApp::OnExit();
}

wxIMPLEMENT_APP(MainApp);
