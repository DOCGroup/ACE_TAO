/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    UPIPE_Addr.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_UPIPE_ADDR_H
#define ACE_UPIPE_ADDR_H

#include "ace/SPIPE_Addr.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

typedef ACE_SPIPE_Addr ACE_UPIPE_Addr;

#if 0
// We need this "class" to make the class2man documentation utility
// happy.
class ACE_Export ACE_UPIPE_Addr
{
  // = TITLE
  //    Defines the ACE "user pipe" address family address format.
  //
  // = DESCRIPTION
  //    This class has an identical interface to the <ACE_SPIPE_Addr>
  //    class.  In fact, it's simply a typedef!
public:
  // = Same interface as <ACE_SPIPE_Addr>.
};
#endif /* 0 */
#endif /* ACE_UPIPE_ADDR_H */
