// $Id$

#include "tao/Dynamic_TP/DTP_Thread_Lane_Resources_Manager.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/ORB_Core.h"
#include "tao/ORB_Core_TSS_Resources.h"
#include "tao/Acceptor_Registry.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Dynamic_TP/DTP_Thread_Pool.h"
#include "tao/LF_Follower.h"
#include "tao/Leader_Follower.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_DTP_Thread_Lane_Resources_Manager::TAO_DTP_Thread_Lane_Resources_Manager
  (TAO_ORB_Core &orb_core)
    : TAO_Thread_Lane_Resources_Manager (orb_core),
      default_lane_resources_ (0),
      tp_manager_ (0)
{
  // Create the default resources.
  ACE_NEW (this->default_lane_resources_,
           TAO_Thread_Lane_Resources (orb_core));

  // Create the thread-pool manager.
  ACE_NEW (this->tp_manager_,
           TAO_DTP_Thread_Pool_Manager (orb_core));

}

TAO_DTP_Thread_Lane_Resources_Manager::~TAO_DTP_Thread_Lane_Resources_Manager (void)
{
  // Delete the default resources.
  delete this->default_lane_resources_;

  // Delete the thread-pool manager.
  delete this->tp_manager_;
}

int
TAO_DTP_Thread_Lane_Resources_Manager::open_default_resources (void)
{
  TAO_ORB_Parameters *params =
    this->orb_core_->orb_params ();

  TAO_EndpointSet endpoint_set;

  params->get_endpoint_set (TAO_DEFAULT_LANE, endpoint_set);

  bool ignore_address = false;

  int const result =
    this->default_lane_resources_->open_acceptor_registry (endpoint_set,
                                                           ignore_address);

  return result;
}

void
TAO_DTP_Thread_Lane_Resources_Manager::finalize (void)
{
  // Finalize default resources.
  this->default_lane_resources_->finalize ();
}

void
TAO_DTP_Thread_Lane_Resources_Manager::shutdown_reactor (void)
{
  // Shutdown default reactors.
  this->default_lane_resources_->shutdown_reactor ();
  // This is the only reactor in use with this thread pool
}

void
TAO_DTP_Thread_Lane_Resources_Manager::close_all_transports (void)
{
  // Shutdown default reactors.
  this->default_lane_resources_->close_all_transports ();
}

int
TAO_DTP_Thread_Lane_Resources_Manager::is_collocated (const TAO_MProfile &mprofile)
{
  return this->default_lane_resources_->is_collocated (mprofile);
}

TAO_Thread_Lane_Resources &
TAO_DTP_Thread_Lane_Resources_Manager::lane_resources (void)
{
    return *this->default_lane_resources_;
}

TAO_Thread_Lane_Resources &
TAO_DTP_Thread_Lane_Resources_Manager::default_lane_resources (void)
{
  return *this->default_lane_resources_;
}

TAO_DTP_Thread_Pool_Manager &
TAO_DTP_Thread_Lane_Resources_Manager::tp_manager (void)
{
  return *this->tp_manager_;
}

TAO_Thread_Lane_Resources_Manager *
TAO_DTP_Thread_Lane_Resources_Manager_Factory::create_thread_lane_resources_manager (
  TAO_ORB_Core &core)
{
  TAO_Thread_Lane_Resources_Manager *manager = 0;

  /// Create the RT Thread Lane Resources Manager.
  ACE_NEW_RETURN (manager,
                  TAO_DTP_Thread_Lane_Resources_Manager (core),
                  0);

  return manager;
}

ACE_STATIC_SVC_DEFINE (TAO_DTP_Thread_Lane_Resources_Manager_Factory,
                       ACE_TEXT ("DTP_Thread_Lane_Resources_Manager_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_DTP_Thread_Lane_Resources_Manager_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_Dynamic_TP, TAO_DTP_Thread_Lane_Resources_Manager_Factory)

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
