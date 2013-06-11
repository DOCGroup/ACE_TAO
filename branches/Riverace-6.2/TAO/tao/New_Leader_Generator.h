// -*- C++ -*-

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

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"
#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_New_Leader_Generator
 *
 * @brief Class for creating dynamic threads.
 *
 * \nosubgrouping
 *
 **/
class TAO_Export TAO_New_Leader_Generator
{
public:

  /// Destructor.
  virtual ~TAO_New_Leader_Generator (void);

  /// Leader/Follower class uses this method to notify the system that
  /// we are out of leaders.
  virtual bool no_leaders_available (void) = 0;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_NEW_LEADER_GENERATOR_H */
