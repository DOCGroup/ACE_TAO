/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    netsvcs
//
// = FILENAME
//    Base_Optimizer.h
//
// = AUTHOR
//    Per Andersson.
//
// ============================================================================

#if !defined (BASE_OPTIMIZER_H)
#define BASE_OPTIMIZER_H

#include "ace/OS.h"

template<class Base, class Member>
struct Base_Optimizer : public Base
  // = TITLE
  //
  // = DESCRIPTION
  //     Thanks to Nathan Myers and Fergus Henderson for this little beauty
{
  Base_Optimizer (void);
  Base_Optimizer (const Base &base);
  Base_Optimizer (const Base &base, const Member &member);

  Member m_;
};

#endif /* BASE_OPTIMIZER_H */
