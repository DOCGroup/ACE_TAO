/* -*- C++ -*- */
// $Id$

#ifndef THERMOMETER_H
#define THERMOMETER_H

#include "ace/OS_NS_stdlib.h"
#include "ace/Log_Msg.h"

class Thermometer
{
public:
  Thermometer (const char *addr)
    : addr_(addr), threshold_(5)
  { }

  float temperature (void)
  {
    int success = ACE_OS::rand () % 10;
    if (success < this->threshold_)
      {
        this->threshold_ = 7;
        return -1.0;
      }

    this->threshold_ = 3;
    int itemp = 80 + ACE_OS::rand () % 10; // 80 <= t <= 90
    return (float)itemp;
  }

  const char *address (void)
  {
    return this->addr_;
  }

  void reset (void)
  {
    this->threshold_ = 4;
    ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Resetting thermometer %C\n"),
                this->address ()));
  }

private:
  const char *addr_;
  int threshold_;
};

#endif /* THERMOMETER_H */
