// -*- C++ -*-

//=============================================================================
/**
 *  @file    DTP_Thread_Lane_Resources_Manager.h
 *
 *  @author  Irfan Pyarali
 */
// ===================================================================

#ifndef TAO_RT_THREAD_LANE_RESOURCES_MANAGER_H
#define TAO_RT_THREAD_LANE_RESOURCES_MANAGER_H

#include /**/ "ace/pre.h"
#include "tao/orbconf.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Dynamic_TP/dynamic_tp_export.h"
#include "tao/Thread_Lane_Resources_Manager.h"
#include "ace/Service_Config.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_DTP_Thread_Pool_Manager;

/**
 * @class TAO_DTP_Thread_Lane_Resources_Manager
 *
 * @brief Manager for thread lane resources.
 *
 * \nosubgrouping
 *
 **/
class TAO_Dynamic_TP_Export TAO_DTP_Thread_Lane_Resources_Manager :
  public TAO_Thread_Lane_Resources_Manager
{
public:
  /// Constructor.
  TAO_DTP_Thread_Lane_Resources_Manager (TAO_ORB_Core &orb_core);

  /// Destructor.
  ~TAO_DTP_Thread_Lane_Resources_Manager ();

  /// Finalize resources.
  void finalize ();

  /// Open default resources.
  int open_default_resources ();

  /// Shutdown reactor.
  void shutdown_reactor ();

  /// Certain ORB policies such as dropping replies on shutdown
  /// would need cleanup of transports to wake threads up.
  void close_all_transports ();

  /// Does @a mprofile belong to us?
  int is_collocated (const TAO_MProfile &mprofile);

  /// @name Accessors
  // @{
  TAO_Thread_Lane_Resources &lane_resources ();

  TAO_Thread_Lane_Resources &default_lane_resources ();

  TAO_DTP_Thread_Pool_Manager &tp_manager ();

  // @}

private:
  void operator= (const TAO_DTP_Thread_Lane_Resources_Manager &);
  TAO_DTP_Thread_Lane_Resources_Manager (const TAO_DTP_Thread_Lane_Resources_Manager &);

protected:
  /// Default lane resources.
  TAO_Thread_Lane_Resources *default_lane_resources_;

  /// Thread Pool Manager.
  TAO_DTP_Thread_Pool_Manager *tp_manager_;
};

/**
 * @class TAO_DTP_Thread_Lane_Resources_Manager_Factory
 *
 * @brief This class is a factory for managers of thread resources.
 *
 * \nosubgrouping
 *
 **/
class TAO_Dynamic_TP_Export TAO_DTP_Thread_Lane_Resources_Manager_Factory
  : public TAO_Thread_Lane_Resources_Manager_Factory
{
public:
  /// Factory method.
  TAO_Thread_Lane_Resources_Manager *create_thread_lane_resources_manager (
    TAO_ORB_Core &core);
};


ACE_STATIC_SVC_DECLARE_EXPORT (TAO_Dynamic_TP,
                               TAO_DTP_Thread_Lane_Resources_Manager_Factory)
ACE_FACTORY_DECLARE (TAO_Dynamic_TP,
                     TAO_DTP_Thread_Lane_Resources_Manager_Factory)

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */

#include /**/ "ace/post.h"

#endif /* TAO_RT_THREAD_LANE_RESOURCES_MANAGER_H */
