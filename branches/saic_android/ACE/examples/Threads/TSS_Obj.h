// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    TSS_Test.cpp
//
// = DESCRIPTION
//     This program tests various features of ACE_Thread and the
//     thread-specific storage variant of <ACE_SingletonEx>.
//
// = AUTHOR
//    Prashant Jain and Doug Schmidt
//
// ============================================================================

#include "ace/Atomic_Op.h"
#include "ace/Synch_Traits.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TSS_Obj
  // = TITLE
  //     This object is stored in thread-specific storage.
{
public:
  TSS_Obj (void);
  ~TSS_Obj (void);

private:

 static ACE_Atomic_Op<ACE_SYNCH_MUTEX, int> count_;
};

