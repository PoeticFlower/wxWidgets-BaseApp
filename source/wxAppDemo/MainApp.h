/*****************************************************************************
 *
 * Copyright (C) 2015-2025 Poetic Flower Studio. All Rights Reserved.
 *
 * CONFIDENTIALITY:  This software source code is considered confidential
 * information. It must be kept confidential in accordance with the terms
 * and conditions of your Software License Agreement.
 *
 *****************************************************************************/

#ifndef   GUI_WXWIDGETS_MAIN_APP_H_
#define   GUI_WXWIDGETS_MAIN_APP_H_

class MainApp : public wxApp
{
public:
  virtual bool OnInit();
  virtual int  OnExit();
};

#endif // GUI_WXWIDGETS_MAIN_APP_H_
