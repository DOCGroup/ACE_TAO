// $Id$

// Define the DC_Singleton here - it needs to be in a separate file to
// get picked up correctly on AIX with auto template instantiation using
// IBM C++ compiler (xlC).

#ifndef __TEST_SINGLETON_H
#define __TEST_SINGLETON_H

#include "ace/Synch.h"

class DC_Singleton
{
public:
  DC_Singleton (void) {}
  void svc (void) {}
  static DC_Singleton *instance (void);

private:
  static ACE_SYNCH_MUTEX lock_;
  static DC_Singleton *instance_;
};

#endif /* __TEST_SINGLETON_H */
