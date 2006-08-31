// $Id$

#include "StdAfx.h"
#include "Resource.h"
#include "Main_Dialog.h"

//
// Main_Dialog
//
Main_Dialog::Main_Dialog (CWnd * parent)
: CDialog (IDD_MAINDIALOG, parent),
  opt_ (0)
{

}

//
// ~Main_Dialog
//
Main_Dialog::~Main_Dialog (void)
{

}

//
// DoDataExchange
//
void Main_Dialog::DoDataExchange (CDataExchange * pDX)
{
  CDialog::DoDataExchange (pDX);

  DDX_Radio (pDX, IDC_OPERATION, this->opt_);
}

//
// option
//
int Main_Dialog::option (void) const
{
  return this->opt_;
}
