// $Id$

#include "QuitCmd.h"
#include "RootPanel.h"

QuitCmd::QuitCmd()
{
}

QuitCmd *
QuitCmd::create(RootPanel *form)
{
  QuitCmd *cmd = new QuitCmd ();
  cmd->form_ = form;
  return cmd;
}

int
QuitCmd::execute (void * /* context */)
{
  return form_->close ();
}
