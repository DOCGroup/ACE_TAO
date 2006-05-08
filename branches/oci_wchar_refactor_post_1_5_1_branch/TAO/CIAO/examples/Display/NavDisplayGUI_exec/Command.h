// $Id$

#ifndef __COMMAND_H
#define __COMMAND_H

#include <qdatetime.h>

class CommandBase
{
public:
  CommandBase() {timestamp_ = QTime::currentTime();}
  virtual ~CommandBase() {}

public:
  virtual int execute(void *context = NULL) = 0;
  QTime timestamp_;
};


#endif // __COMMAND_H
