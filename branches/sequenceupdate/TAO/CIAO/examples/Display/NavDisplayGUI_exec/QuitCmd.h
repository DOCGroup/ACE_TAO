// $Id$

#ifndef __QUITCMD_H
#define __QUITCMD_H

#include "Command.h"

class RootPanel;

class QuitCmd : public CommandBase
{
public:
  static QuitCmd *create(RootPanel *form);

public:
  virtual int execute(void *context = NULL);

protected:
  QuitCmd();

  RootPanel *form_;
};


#endif // __QUITCMD_H
