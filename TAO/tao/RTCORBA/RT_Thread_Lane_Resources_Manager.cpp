#include "tao/RTCORBA/RT_Thread_Lane_Resources_Manager.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/ORB_Core.h"
#include "tao/ORB_Core_TSS_Resources.h"
#include "tao/Acceptor_Registry.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/RTCORBA/Thread_Pool.h"
#include "tao/LF_Follower.h"
#include "tao/Leader_Follower.h"

#include <memory>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_RT_Thread_Lane_Resources_Manager::TAO_RT_Thread_Lane_Resources_Manager (TAO_ORB_Core &orb_core)
  : TAO_Thread_Lane_Resources_Manager (orb_core)
  , tp_manager_ (std::make_unique<TAO_Thread_Pool_Manager> (orb_core))
  , default_lane_resources_ (std::make_unique<TAO_Thread_Lane_Resources> (orb_core))
{
}

TAO_RT_Thread_Lane_Resources_Manager::~TAO_RT_Thread_Lane_Resources_Manager () = default;

int
TAO_RT_Thread_Lane_Resources_Manager::open_default_resources ()
{
  TAO_ORB_Parameters * const params =
    this->orb_core_->orb_params ();

  TAO_EndpointSet endpoint_set;

  params->get_endpoint_set (TAO_DEFAULT_LANE, endpoint_set);

  bool ignore_address {};

  int const result =
    this->default_lane_resources_->open_acceptor_registry (endpoint_set,
                                                           ignore_address);

  return result;
}

void
TAO_RT_Thread_Lane_Resources_Manager::finalize ()
{
  // Finalize resources managed by the thread-pool manager.
  this->tp_manager_->finalize ();

  // Finalize default resources.
  this->default_lane_resources_->finalize ();
}

void
TAO_RT_Thread_Lane_Resources_Manager::shutdown_reactor ()
{
  // Shutdown default reactors.
  this->default_lane_resources_->shutdown_reactor ();

  // Shutdown reactors managed by the thread-pool manager.
  this->tp_manager_->shutdown_reactor ();
}

void
TAO_RT_Thread_Lane_Resources_Manager::close_all_transports ()
{
  // Shutdown default reactors.
  this->default_lane_resources_->close_all_transports ();
}

bool
TAO_RT_Thread_Lane_Resources_Manager::is_collocated (TAO_MProfile const &mprofile)
{
  bool const collocated =
    this->default_lane_resources_->is_collocated (mprofile);

  if (collocated)
    {
      return true;
    }

  return this->tp_manager_->is_collocated (mprofile);
}

TAO_Thread_Lane_Resources &
TAO_RT_Thread_Lane_Resources_Manager::lane_resources ()
{
  // Get the ORB_Core's TSS resources.
  TAO_ORB_Core_TSS_Resources const &tss =
    *this->orb_core_->get_tss_resources ();

  // Get the lane for this thread.
  TAO_Thread_Lane * const lane =
    static_cast <TAO_Thread_Lane *> (tss.lane_);

  // If we have a valid lane, use that lane's resources.
  if (lane != nullptr)
    {
      return lane->resources ();
    }
  else
    {
      // Otherwise, return the default resources.
      return *this->default_lane_resources_;
    }
}

TAO_Thread_Lane_Resources &
TAO_RT_Thread_Lane_Resources_Manager::default_lane_resources ()
{
  return *this->default_lane_resources_;
}

TAO_Thread_Pool_Manager &
TAO_RT_Thread_Lane_Resources_Manager::tp_manager ()
{
  return *this->tp_manager_;
}

TAO_Thread_Lane_Resources_Manager *
TAO_RT_Thread_Lane_Resources_Manager_Factory::create_thread_lane_resources_manager (TAO_ORB_Core &core)
{
  TAO_Thread_Lane_Resources_Manager *manager {};

  /// Create the RT Thread Lane Resources Manager.
  ACE_NEW_RETURN (manager,
                  TAO_RT_Thread_Lane_Resources_Manager (core),
                  nullptr);

  return manager;
}

ACE_STATIC_SVC_DEFINE (TAO_RT_Thread_Lane_Resources_Manager_Factory,
                       ACE_TEXT ("RT_Thread_Lane_Resources_Manager_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_RT_Thread_Lane_Resources_Manager_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_RTCORBA, TAO_RT_Thread_Lane_Resources_Manager_Factory)

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
