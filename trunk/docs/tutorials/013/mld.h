
// $Id$

#ifndef MLD_H
#define MLD_H

#include "ace/Synch.h"
#include "ace/Singleton.h"

/*
  This is a cheap memory leak detector.  Each class I want to watch
  over contains an mld object.  The mld object's ctor increments a
  global counter while the dtor decrements it.  If the counter is
  non-zero when the program is ready to exit then there may be a leak.
*/

class mld
{
public:
    mld(void);
    ~mld(void);

    static int value(void);
        
protected:
    static ACE_Atomic_Op<ACE_Mutex,int> counter_;
};

//================================================

/*
  Just drop 'MLD' anywhere in your class definition to get cheap
  memory leak detection for your class.

  
  Use 'MLD_COUNTER->value()' in main() to see if things are OK.  We
  don't really need a singleton since the counter itself is static and 
  thread safe but it makes the interface to mld simpler.
 */
#define MLD            mld mld_

//================================================

#endif
