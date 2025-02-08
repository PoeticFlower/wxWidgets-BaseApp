/*****************************************************************************
 *
 * Copyright (C) 2015-2025 Poetic Flower Studio. All Rights Reserved.
 *
 * CONFIDENTIALITY:  This software source code is considered confidential
 * information. It must be kept confidential in accordance with the terms
 * and conditions of your Software License Agreement.
 *
 *****************************************************************************/

#ifndef   GUI_WXWIDGETS_MAIN_FRAME_H_
#define   GUI_WXWIDGETS_MAIN_FRAME_H_

class MainFrame : public wxFrame
{
public:
  MainFrame(const wxString& title);
private:
  void InitializeMenuBar();
  void InitializeMessageBinding();
public:
  void OnMenuFileExit                              (wxCommandEvent& event);
};

#endif // GUI_WXWIDGETS_MAIN_FRAME_H_
