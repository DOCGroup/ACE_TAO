// $Id$

#ifndef __COMMAND_H
#define __COMMAND_H

#include <QtCore/qdatetime.h>

class CommandBase
{
public:
  CommandBase() {timestamp_ = QTime::currentTime();}
  virtual ~CommandBase() {}

public:
  virtual int execute(void *context = 0) = 0;
  QTime timestamp_;
};


#endif // __COMMAND_H
