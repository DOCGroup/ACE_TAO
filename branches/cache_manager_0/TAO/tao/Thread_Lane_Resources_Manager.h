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
class TAO_MProfile;

/**
 * @class TAO_Thread_Lane_Resources_Manager
 *
 * @brief This class is a manager for thread resources.
 *
 * \nosubgrouping
 *
 **/
class TAO_Export TAO_Thread_Lane_Resources_Manager
{
public:

  /// Constructor.
  TAO_Thread_Lane_Resources_Manager (TAO_ORB_Core &orb_core);

  /// Destructor.
  virtual ~TAO_Thread_Lane_Resources_Manager (void);

  /// Finalize resources.
  virtual void finalize (void) = 0;

  /// Open default resources.
  virtual int open_default_resources (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS) = 0;

  /// Shutdown reactor.
  virtual void shutdown_reactor (void) = 0;

  // Does <mprofile> belong to us?
  virtual int is_collocated (const TAO_MProfile& mprofile) = 0;

  /// @name Accessors
  // @{

  virtual TAO_Thread_Lane_Resources &lane_resources (void) = 0;

  virtual TAO_Thread_Lane_Resources &default_lane_resources (void) = 0;

  TAO_LF_Strategy &lf_strategy (void);

  // @}

protected:
  /// The ORB Core.
  TAO_ORB_Core *orb_core_;

  /// The leader follower strategy
  TAO_LF_Strategy *lf_strategy_;
};

/**
 * @class TAO_Thread_Lane_Resources_Manager_Factory
 *
 * @brief This class is a factory for managers of thread resources.
 *
 * \nosubgrouping
 *
 **/
class TAO_Export TAO_Thread_Lane_Resources_Manager_Factory
  : public ACE_Service_Object
{
public:

  /// Virtual destructor.
  virtual ~TAO_Thread_Lane_Resources_Manager_Factory (void);

  /// Factory method.
  virtual TAO_Thread_Lane_Resources_Manager *create_thread_lane_resources_manager (TAO_ORB_Core &core) = 0;

};

#if defined (__ACE_INLINE__)
# include "tao/Thread_Lane_Resources_Manager.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"

#endif /* TAO_THREAD_LANE_RESOURCES_MANAGER_H */
