/* -*- C++ -*- */


//=============================================================================
/**
 *  @file    UPIPE_Addr.h
 *
 *  $Id$
 *
 *  @author Doug Schmidt
 */
//=============================================================================


#ifndef ACE_UPIPE_ADDR_H
#define ACE_UPIPE_ADDR_H
#include "ace/pre.h"

#include "ace/IPC/SPIPE_Addr.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

typedef ACE_SPIPE_Addr ACE_UPIPE_Addr;

#if 0
// We need this "class" to make the class2man documentation utility
// happy.
/**
 * @class ACE_UPIPE_Addr
 *
 * @brief Defines the ACE "user pipe" address family address format.
 *
 * This class has an identical interface to the <ACE_SPIPE_Addr>
 * class.  In fact, it's simply a typedef!
 */
class ACE_Export ACE_UPIPE_Addr
{
public:
  // = Same interface as <ACE_SPIPE_Addr>.
};
#endif /* 0 */
#include "ace/post.h"
#endif /* ACE_UPIPE_ADDR_H */
