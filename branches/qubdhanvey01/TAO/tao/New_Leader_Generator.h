//=============================================================================
/**
 *  @file    New_Leader_Generator.h
 *
 *  $Id$
 *
 *  @author Irfan Pyarali
 */
// ===================================================================

#ifndef TAO_NEW_LEADER_GENERATOR_H
#define TAO_NEW_LEADER_GENERATOR_H

#include "ace/pre.h"
#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_RT_New_Leader_Generator
 *
 * @brief Class for creating dynamic threads.
 *
 * \nosubgrouping
 *
 **/
class TAO_Export TAO_New_Leader_Generator
{
public:

  /// Leader/Follower class uses this method to notify the system that
  /// we are out of leaders.
  virtual void no_leaders_available (void) = 0;
};

#include "ace/post.h"

#endif /* TAO_NEW_LEADER_GENERATOR_H */
