//=============================================================================
/**
 *  @file    Thread_Lane_Resources_Manager.h
 *
 *  $Id$
 *
 *  @author  Irfan Pyarali
 */
// ===================================================================

#ifndef TAO_THREAD_LANE_RESOURCES_MANAGER_H
#define TAO_THREAD_LANE_RESOURCES_MANAGER_H

#include "ace/pre.h"
#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corbafwd.h"
#include "ace/Service_Object.h"

class TAO_ORB_Core;
class TAO_Thread_Lane_Resources;
class TAO_LF_Strategy;

/**
 * @class TAO_Thread_Lane_Resources_Manager
 *
 * This class is a manager for thread resources.
 **/
class TAO_Export TAO_Thread_Lane_Resources_Manager
  : public ACE_Service_Object
{
public:
  TAO_Thread_Lane_Resources_Manager (void);
  virtual ~TAO_Thread_Lane_Resources_Manager (void);

  virtual int initialize (TAO_ORB_Core &orb_core);
  virtual void finalize (void) = 0;

  virtual int open_default_resources (CORBA_Environment &ACE_TRY_ENV) = 0;

  virtual TAO_Thread_Lane_Resources &lane_resources (void) = 0;
  virtual TAO_Thread_Lane_Resources &default_lane_resources (void) = 0;

  virtual int shutdown_all_reactors (CORBA_Environment &ACE_TRY_ENV) = 0;

  /// Return the leader follower strategy
  TAO_LF_Strategy &lf_strategy (void);

protected:
  /// The ORB Core.
  TAO_ORB_Core *orb_core_;

  /// The leader follower strategy
  TAO_LF_Strategy *lf_strategy_;
};

#if defined (__ACE_INLINE__)
# include "tao/Thread_Lane_Resources_Manager.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"

#endif /* TAO_THREAD_LANE_RESOURCES_MANAGER_H */
