// $Id$

#ifndef __ADDNAVUNITCMD_H
#define __ADDNAVUNITCMD_H

#include "Command.h"

class RootPanel;
class NavUnit;

class AddNavUnitCmd : public CommandBase
{
public:
  static AddNavUnitCmd *create(RootPanel *form, NavUnit *unit);

public:
  virtual int execute(void *context = NULL);

protected:
  AddNavUnitCmd();

  RootPanel *form_;
  NavUnit *unit_;
};


#endif // __ADDNAVUNITCMD_H
