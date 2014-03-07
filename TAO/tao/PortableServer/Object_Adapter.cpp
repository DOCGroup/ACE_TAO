// $Id$

// -- PortableServer Include --
#include "tao/PortableServer/Object_Adapter.h"
#include "tao/PortableServer/Non_Servant_Upcall.h"
#include "tao/PortableServer/Servant_Upcall.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/PortableServer/Regular_POA.h"
#include "tao/PortableServer/Creation_Time.h"
#include "tao/PortableServer/POA_Guard.h"
#include "tao/PortableServer/Default_Servant_Dispatcher.h"
#include "tao/PortableServer/Collocated_Object_Proxy_Broker.h"
#include "tao/PortableServer/POAManager.h"
#include "tao/PortableServer/POAManagerFactory.h"
#include "tao/PortableServer/Servant_Base.h"

// -- ACE Include --
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"

// -- TAO Include --
#include "tao/PortableInterceptorC.h"
#include "tao/ORB.h"
#include "tao/ORB_Core.h"
#include "tao/TSS_Resources.h"
#include "tao/TAO_Server_Request.h"
#include "tao/Stub.h"
#include "tao/Profile.h"
#include "tao/MProfile.h"
#include "tao/debug.h"
#include "tao/PortableInterceptor.h"
#include "tao/ORBInitializer_Registry.h"
#include "tao/Thread_Lane_Resources_Manager.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Protocols_Hooks.h"
#include "tao/ServerRequestInterceptor_Adapter.h"

#if !defined (__ACE_INLINE__)
# include "tao/PortableServer/Object_Adapter.inl"
#endif /* __ACE_INLINE__ */

#include "tao/PortableServer/ThreadPolicy.h"
#include "tao/PortableServer/LifespanPolicy.h"
#include "tao/PortableServer/IdAssignmentPolicy.h"
#include "tao/PortableServer/IdUniquenessPolicy.h"
#include "tao/PortableServer/ImplicitActivationPolicy.h"
#include "tao/PortableServer/RequestProcessingPolicy.h"
#include "tao/PortableServer/ServantRetentionPolicy.h"

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
namespace PortableServer
{
  class POAManagerFactory;
  typedef POAManagerFactory *POAManagerFactory_ptr;
}
#endif

// Timeprobes class
#include "tao/Timeprobe.h"

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *TAO_Object_Adapter_Timeprobe_Description[] =
{
  "Object_Adapter::dispatch_servant - start",
  "Object_Adapter::dispatch_servant - end",

  "POA::parse_key - start",
  "POA::parse_key - end",

  "Object_Adapter::find_poa - start",
  "Object_Adapter::find_poa - end",

  "POA::locate_servant - start",
  "POA::locate_servant - end",

  "Servant::_dispatch - start",
  "Servant::_dispatch - end",
};

enum
{
  // Timeprobe description table start key
  TAO_OBJECT_ADAPTER_DISPATCH_SERVANT_START = 200,
  TAO_OBJECT_ADAPTER_DISPATCH_SERVANT_END,

  TAO_POA_PARSE_KEY_START,
  TAO_POA_PARSE_KEY_END,

  TAO_OBJECT_ADAPTER_FIND_POA_START,
  TAO_OBJECT_ADAPTER_FIND_POA_END,

  TAO_POA_LOCATE_SERVANT_START,
  TAO_POA_LOCATE_SERVANT_END,

  TAO_SERVANT_DISPATCH_START,
  TAO_SERVANT_DISPATCH_END
};

// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (TAO_Object_Adapter_Timeprobe_Description,
                                  TAO_OBJECT_ADAPTER_DISPATCH_SERVANT_START);

#endif /* ACE_ENABLE_TIMEPROBES */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/* static */
CORBA::ULong TAO_Object_Adapter::transient_poa_name_size_ = 0;

void
TAO_Object_Adapter::set_transient_poa_name_size (const TAO_Server_Strategy_Factory::Active_Object_Map_Creation_Parameters &creation_parameters)
{
  if (TAO_Object_Adapter::transient_poa_name_size_ == 0)
    {
      switch (creation_parameters.poa_lookup_strategy_for_transient_id_policy_)
        {
#if (TAO_HAS_MINIMUM_POA_MAPS == 0)
        case TAO_LINEAR:
          TAO_Object_Adapter::transient_poa_name_size_ =
            sizeof (CORBA::ULong);
          break;
        case TAO_DYNAMIC_HASH:
          TAO_Object_Adapter::transient_poa_name_size_ =
            sizeof (CORBA::ULong);
          break;
#endif /* TAO_HAS_MINIMUM_POA_MAPS == 0 */
        case TAO_ACTIVE_DEMUX:
        default:
          TAO_Object_Adapter::transient_poa_name_size_ =
            static_cast <CORBA::ULong>(
                             ACE_Active_Map_Manager_Key::size ());
          break;
        }
    }
}

TAO_Object_Adapter::TAO_Object_Adapter (const TAO_Server_Strategy_Factory::Active_Object_Map_Creation_Parameters &creation_parameters,
                                        TAO_ORB_Core &orb_core)
  : hint_strategy_ (0),
    servant_dispatcher_ (0),
    persistent_poa_name_map_ (0),
    transient_poa_map_ (0),
    orb_core_ (orb_core),
    thread_lock_ (),
    lock_ (TAO_Object_Adapter::create_lock (thread_lock_)),
    reverse_lock_ (*lock_),
    non_servant_upcall_condition_ (thread_lock_),
    non_servant_upcall_in_progress_ (0),
    non_servant_upcall_nesting_level_ (0),
    non_servant_upcall_thread_ (ACE_OS::NULL_thread),
    root_ (0),
#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
    poa_manager_factory_ (0),
#endif
    default_validator_ (orb_core),
    default_poa_policies_ ()
{
  TAO_Object_Adapter::set_transient_poa_name_size (creation_parameters);

  Hint_Strategy *hint_strategy = 0;
  if (creation_parameters.use_active_hint_in_poa_names_)
    ACE_NEW (hint_strategy,
             Active_Hint_Strategy (creation_parameters.poa_map_size_));
  else
    ACE_NEW (hint_strategy,
             No_Hint_Strategy);

  // Give ownership to the auto pointer.
  auto_ptr<Hint_Strategy> new_hint_strategy (hint_strategy);

  new_hint_strategy->object_adapter (this);

  persistent_poa_name_map *ppnm = 0;
  switch (creation_parameters.poa_lookup_strategy_for_persistent_id_policy_)
    {
    case TAO_LINEAR:
#if (TAO_HAS_MINIMUM_POA_MAPS == 0)
      ACE_NEW (ppnm,
               persistent_poa_name_linear_map (creation_parameters.poa_map_size_));

      break;
#else
      TAOLIB_ERROR ((LM_ERROR,
                  "linear option for -ORBPersistentidPolicyDemuxStrategy "
                  "not supported with minimum POA maps. "
                  "Ingoring option to use default...\n"));
      /* FALL THROUGH */
#endif /* TAO_HAS_MINIMUM_POA_MAPS == 0 */
    case TAO_DYNAMIC_HASH:
    default:
      ACE_NEW (ppnm,
               persistent_poa_name_hash_map (creation_parameters.poa_map_size_));
      break;
    }
  // Give ownership to the auto pointer.
  auto_ptr<persistent_poa_name_map> new_persistent_poa_name_map (ppnm);

  transient_poa_map *tpm = 0;
  switch (creation_parameters.poa_lookup_strategy_for_transient_id_policy_)
    {
#if (TAO_HAS_MINIMUM_POA_MAPS == 0)
    case TAO_LINEAR:
      ACE_NEW (tpm,
               transient_poa_linear_map (creation_parameters.poa_map_size_));
      break;
    case TAO_DYNAMIC_HASH:
      ACE_NEW (tpm,
               transient_poa_hash_map (creation_parameters.poa_map_size_));
      break;
#else
    case TAO_LINEAR:
    case TAO_DYNAMIC_HASH:
      TAOLIB_ERROR ((LM_ERROR,
                  "linear and dynamic options for -ORBTransientidPolicyDemuxStrategy "
                  "are not supported with minimum POA maps. "
                  "Ingoring option to use default...\n"));
      /* FALL THROUGH */
#endif /* TAO_HAS_MINIMUM_POA_MAPS == 0 */
    case TAO_ACTIVE_DEMUX:
    default:
      ACE_NEW (tpm,
               transient_poa_active_map (creation_parameters.poa_map_size_));
      break;
    }
  // Give ownership to the auto pointer.
  auto_ptr<transient_poa_map> new_transient_poa_map (tpm);

  this->hint_strategy_ =
    new_hint_strategy.release ();
  this->persistent_poa_name_map_ =
    new_persistent_poa_name_map.release ();
  this->transient_poa_map_ =
    new_transient_poa_map.release ();
}

void
TAO_Object_Adapter::init_default_policies (TAO_POA_Policy_Set &policies)
{
  // Initialize the default policies.
#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

  TAO::Portable_Server::ThreadPolicy thread_policy (PortableServer::ORB_CTRL_MODEL);
  policies.merge_policy (&thread_policy);

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#if !defined (CORBA_E_MICRO)
  // Lifespan policy.
  TAO::Portable_Server::LifespanPolicy lifespan_policy (PortableServer::TRANSIENT);
  policies.merge_policy (&lifespan_policy);
#endif

#if !defined (CORBA_E_MICRO)
  // ID uniqueness policy.
  TAO::Portable_Server::IdUniquenessPolicy id_uniqueness_policy (PortableServer::UNIQUE_ID);
  policies.merge_policy (&id_uniqueness_policy);
#endif

#if !defined (CORBA_E_MICRO)
  // ID assignment policy.
  TAO::Portable_Server::IdAssignmentPolicy id_assignment_policy (PortableServer::SYSTEM_ID);
  policies.merge_policy (&id_assignment_policy);
#endif

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
  // Implicit activation policy.
  TAO::Portable_Server::ImplicitActivationPolicy implicit_activation_policy
    (PortableServer::NO_IMPLICIT_ACTIVATION);
  policies.merge_policy (&implicit_activation_policy);

  // Servant retention policy.
  TAO::Portable_Server::ServantRetentionPolicy servant_retention_policy
    (PortableServer::RETAIN);
  policies.merge_policy (&servant_retention_policy);

  // Request processing policy.
  TAO::Portable_Server::RequestProcessingPolicy request_processing_policy
    (PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY);
  policies.merge_policy (&request_processing_policy);
#endif /* TAO_HAS_MINIMUM_POA == 0 */
#if defined (CORBA_E_MICRO)
  ACE_UNUSED_ARG (policies);
#endif
}

TAO_Object_Adapter::~TAO_Object_Adapter (void)
{
  delete this->hint_strategy_;
  delete this->persistent_poa_name_map_;
  delete this->transient_poa_map_;
  delete this->lock_;

  delete this->servant_dispatcher_;

  // This cleanup may have already occurred in the close() method.  If
  // that is the case then this won't cause any harm since root_ and
  // poa_manager_factory_ would have been set to zero.  But, if close
  // wasn't called, then these would be leaked.  It may be better if
  // these pointers had a corresponding _var version so that this cleanup
  // could be automatic.
  ::CORBA::release (this->root_);
#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
  release_poa_manager_factory (this->poa_manager_factory_);
#endif
}

/* static */
ACE_Lock *
TAO_Object_Adapter::create_lock (TAO_SYNCH_MUTEX &thread_lock)
{
  ACE_Lock *the_lock = 0;
  ACE_NEW_RETURN (the_lock,
                  ACE_Lock_Adapter<TAO_SYNCH_MUTEX> (thread_lock),
                  0);
  return the_lock;
}

int
TAO_Object_Adapter::dispatch_servant (const TAO::ObjectKey &key,
                                      TAO_ServerRequest &req,
                                      CORBA::Object_out forward_to)
{
  ACE_FUNCTION_TIMEPROBE (TAO_OBJECT_ADAPTER_DISPATCH_SERVANT_START);

  // This object is magical, i.e., it has a non-trivial constructor
  // and destructor.
  TAO::Portable_Server::Servant_Upcall servant_upcall (&this->orb_core_);

  // Set up state in the POA et al (including the POA Current), so
  // that we know that this servant is currently in an upcall.
  const char *operation = req.operation ();
  int result = servant_upcall.prepare_for_upcall (key, operation, forward_to);

  if (result != TAO_Adapter::DS_OK)
    return result;

  // Preprocess request.
  if (req.collocated ())
    {
      servant_upcall.pre_invoke_collocated_request ();
    }
  else
    {
      servant_upcall.pre_invoke_remote_request (req);
    }

  // Servant dispatch.
  {
    ACE_FUNCTION_TIMEPROBE (TAO_SERVANT_DISPATCH_START);

    // Handle the one ways that are SYNC_WITH_SERVER and not collocated.
    // Do it here so that the client is unblocked before possibly queuing
    // the request as what happens with CSD.
    if (req.sync_with_server () && !req.collocated ())
      {
        req.send_no_exception_reply ();
      }

    do_dispatch (req, servant_upcall);
  }

#if TAO_HAS_INTERCEPTORS == 1
  // ServerInterceptor might have raised ForwardRequest. In case of
  // remote calls invocations the LocationForwardReply would have been
  // sent in earlier stage, but in colocal scenario no message is sent
  // and the LocationForward object must be passed over here to
  // calling operation's mem-space.
  if (req.collocated() && req.pi_reply_status () == PortableInterceptor::LOCATION_FORWARD)
    {
      forward_to = req.forward_location ();
      result = TAO_Adapter::DS_FORWARD;
    }
#endif

  return result;
}

void
TAO_Object_Adapter::locate_poa (const TAO::ObjectKey &key,
                                PortableServer::ObjectId &system_id,
                                TAO_Root_POA *&poa)
{
  TAO_Object_Adapter::poa_name poa_system_name;
  CORBA::Boolean is_root = false;
  CORBA::Boolean is_persistent = false;
  CORBA::Boolean is_system_id = false;
  TAO::Portable_Server::Temporary_Creation_Time poa_creation_time;

  int result = 0;

  {
    ACE_FUNCTION_TIMEPROBE (TAO_POA_PARSE_KEY_START);

    result = TAO_Root_POA::parse_key (key,
                                      poa_system_name,
                                      system_id,
                                      is_root,
                                      is_persistent,
                                      is_system_id,
                                      poa_creation_time);
  }

  if (result != 0)
    throw ::CORBA::OBJ_ADAPTER ();

  {
    ACE_FUNCTION_TIMEPROBE (TAO_OBJECT_ADAPTER_FIND_POA_START);

    result = this->find_poa (poa_system_name,
                             is_persistent,
                             is_root,
                             poa_creation_time,
                             poa);
  }

  if (result != 0)
    throw ::CORBA::OBJECT_NOT_EXIST (CORBA::OMGVMCID | 2, CORBA::COMPLETED_NO);
}

int
TAO_Object_Adapter::activate_poa (const poa_name &folded_name,
                                  TAO_Root_POA *&poa)
{
  int result = -1;

#if (TAO_HAS_MINIMUM_POA == 0) &&  !defined (CORBA_E_MICRO)

  iteratable_poa_name ipn (folded_name);
  iteratable_poa_name::iterator iterator = ipn.begin ();
  iteratable_poa_name::iterator end = ipn.end ();

  TAO_Root_POA *parent = this->root_;
  if (parent == 0 || parent->name () != *iterator)
    throw ::CORBA::OBJ_ADAPTER ();
  else
    ++iterator;

  for (;
       iterator != end;
       ++iterator)
    {
      TAO_Root_POA *current = 0;

      try
        {
          current = parent->find_POA_i (*iterator, 1);
        }
      catch (const PortableServer::POA::AdapterNonExistent&)
        {
          return -1;
        }

      parent = current;
    }

  poa = parent;
  result = 0;
#else
  ACE_UNUSED_ARG (folded_name);
  ACE_UNUSED_ARG (poa);
#endif /* TAO_HAS_MINIMUM_POA == 0 */

  return result;
}

int
TAO_Object_Adapter::find_transient_poa (const poa_name &system_name,
                                        CORBA::Boolean root,
                                        const TAO::Portable_Server::Temporary_Creation_Time &poa_creation_time,
                                        TAO_Root_POA *&poa)
{
  int result = 0;

  if (root)
    {
      poa = this->root_;
    }
  else
    {
      result = this->transient_poa_map_->find (system_name, poa);
    }

  if (poa == 0
      || (result == 0 && !poa->validate_lifespan (false, poa_creation_time)))
    result = -1;

  return result;
}

int
TAO_Object_Adapter::bind_poa (const poa_name &folded_name,
                              TAO_Root_POA *poa,
                              poa_name_out system_name)
{
  if (poa->persistent ())
    return this->bind_persistent_poa (folded_name, poa, system_name);
  else
    return this->bind_transient_poa (poa, system_name);
}

int
TAO_Object_Adapter::unbind_poa (TAO_Root_POA *poa,
                                const poa_name &folded_name,
                                const poa_name &system_name)
{
  if (poa->persistent ())
    return this->unbind_persistent_poa (folded_name, system_name);
  else
    return this->unbind_transient_poa (system_name);
}

int
TAO_Object_Adapter::locate_servant_i (const TAO::ObjectKey &key)
{
  ACE_FUNCTION_TIMEPROBE (TAO_POA_LOCATE_SERVANT_START);

  PortableServer::ObjectId id;
  TAO_Root_POA *poa = 0;

  this->locate_poa (key, id, poa);

  PortableServer::Servant servant = 0;
  TAO_SERVANT_LOCATION const servant_location =
    poa->locate_servant_i (id, servant);

  switch (servant_location)
    {
    case TAO_SERVANT_FOUND:
      // Optimistic attitude
    case TAO_DEFAULT_SERVANT:
    case TAO_SERVANT_MANAGER:
      return 0;

    case TAO_SERVANT_NOT_FOUND:
      return -1;
    }

  return -1;
}

TAO_SERVANT_LOCATION
TAO_Object_Adapter::find_servant_i (const TAO::ObjectKey &key,
                                    PortableServer::Servant &servant)
{
  PortableServer::ObjectId id;
  TAO_Root_POA *poa = 0;

  this->locate_poa (key, id, poa);

  return poa->locate_servant_i (id, servant);
}

void
TAO_Object_Adapter::open (void)
{
  // Add in the default POA policies to the default list.
  this->init_default_policies (this->default_poa_policies ());

  // If a POA extension hasn't changed the servant dispatcher, initialize the
  // default one.
  if (this->servant_dispatcher_ == 0)
    {
      ACE_NEW (this->servant_dispatcher_,
               TAO_Default_Servant_Dispatcher);
    }

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
  ACE_NEW_THROW_EX (this->poa_manager_factory_,
                    TAO_POAManager_Factory (*this),
                    CORBA::NO_MEMORY ());

  ::CORBA::PolicyList policy;
  PortableServer::POAManager_var poa_manager
    = poa_manager_factory_->create_POAManager (TAO_DEFAULT_ROOTPOAMANAGER_NAME,
                                              policy);
#else
  PortableServer::POAManager_ptr poa_manager_ptr;
  ::CORBA::PolicyList policy_list;
  ACE_NEW_THROW_EX (poa_manager_ptr,
                    TAO_POA_Manager (*this, TAO_DEFAULT_ROOTPOAMANAGER_NAME),
                    CORBA::NO_MEMORY
                    (CORBA::SystemException::_tao_minor_code (0, ENOMEM),
                      CORBA::COMPLETED_NO));

  PortableServer::POAManager_var poa_manager = poa_manager_ptr;
  // Keep reference of POAManager in TAO_Object_Adapter so the POAManager
  // object is destructed after RootPOA is destructed.
  the_poa_manager_ = poa_manager;

#endif

  // This makes sure that the default resources are open when the Root
  // POA is created.
  this->orb_core_.thread_lane_resources_manager ().open_default_resources ();

  TAO_POA_Policy_Set policies (this->default_poa_policies ());

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
  // Specify the implicit activation policy since it should
  // be different from the default.  Note that merge_policy
  // takes a const reference and makes its own copy of the
  // policy.  (Otherwise, we'd have to allocate the policy
  // on the heap.)
  // Implicit activation policy.
  TAO::Portable_Server::ImplicitActivationPolicy implicit_activation_policy
    (PortableServer::IMPLICIT_ACTIVATION);
  policies.merge_policy (&implicit_activation_policy);
#endif /* TAO_HAS_MINIMUM_POA == 0 */

  // Merge policies from the ORB level.
  this->validator ().merge_policies (policies.policies ());

  // If any of the policy objects specified are not valid for the ORB
  // implementation, if conflicting policy objects are specified, or
  // if any of the specified policy objects require prior
  // administrative action that has not been performed, an
  // InvalidPolicy exception is raised containing the index in the
  // policies parameter value of the first offending policy object.
  policies.validate_policies (this->validator (), this->orb_core_);

  // Construct a new POA
  TAO_Root_POA::String root_poa_name (TAO_DEFAULT_ROOTPOA_NAME);
  this->root_ =
    this->servant_dispatcher_->create_Root_POA (root_poa_name,
                                                poa_manager.in (),
                                                policies,
                                                this->lock (),
                                                this->thread_lock (),
                                                this->orb_core_,
                                                this);

  // The Object_Adapter will keep a reference to the Root POA so that
  // on its destruction, it can check whether the Root POA has been
  // destroyed yet or not.
  this->root_->_add_ref ();

  // Lock access for the duration of this transaction.
  TAO::Portable_Server::POA_Guard poa_guard (*this->root_);

  // Iterate over the registered IOR interceptors so that they may be
  // given the opportunity to add tagged components to the profiles
  // for this servant.
  this->root_->establish_components ();
}

void
TAO_Object_Adapter::close (int wait_for_completion)
{
  this->check_close (wait_for_completion);

  // Shutting down the ORB causes all object adapters to be destroyed,
  // since they cannot exist in the absence of an ORB. Shut down is
  // complete when all ORB processing (including request processing
  // and object deactivation or other operations associated with
  // object adapters) has completed and the object adapters have been
  // destroyed. In the case of the POA, this means that all object
  // etherealizations have finished and root POA has been destroyed
  // (implying that all descendent POAs have also been destroyed).
  TAO_Root_POA *root = 0;
#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
  TAO_POAManager_Factory* factory = 0;
#endif
  {
    ACE_GUARD (ACE_Lock, ace_mon, this->lock ());
    if (this->root_ == 0)
      return;
    root = this->root_;
    this->root_ = 0;

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
    if (this->poa_manager_factory_ == 0)
      return;
    factory = this->poa_manager_factory_;
    this->poa_manager_factory_ = 0;
#endif
  }
  CORBA::Boolean etherealize_objects = true;
  root->destroy (etherealize_objects, wait_for_completion);
  ::CORBA::release (root);
#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
  release_poa_manager_factory (factory);
#endif
}

void
TAO_Object_Adapter::check_close (int wait_for_completion)
{
  TAO_Root_POA::check_for_valid_wait_for_completions (this->orb_core (),
                                                 wait_for_completion);
}

int
TAO_Object_Adapter::priority (void) const
{
  return 0;
}

int
TAO_Object_Adapter::dispatch (TAO::ObjectKey &key,
                              TAO_ServerRequest &request,
                              CORBA::Object_out forward_to)
{
  if (key.length() < TAO_Root_POA::TAO_OBJECTKEY_PREFIX_SIZE
      || ACE_OS::memcmp (key.get_buffer (),
                         &TAO_Root_POA::objectkey_prefix[0],
                         TAO_Root_POA::TAO_OBJECTKEY_PREFIX_SIZE) != 0)
    {
      return TAO_Adapter::DS_MISMATCHED_KEY;
    }

  int result = 0;

#if TAO_HAS_INTERCEPTORS == 1
  TAO::ServerRequestInterceptor_Adapter *sri_adapter =
    orb_core_.serverrequestinterceptor_adapter ();

  try
    {
      if (sri_adapter != 0)
        {
#if TAO_HAS_EXTENDED_FT_INTERCEPTORS == 1
          CORBA::OctetSeq_var ocs;
          sri_adapter->tao_ft_interception_point (request,
                                                 0,  // args
                                                 0,  // nargs
                                                 0,  // servant_upcall
                                                 0,  // exceptions
                                                 0, // nexceptions
                                                 ocs.out ());

          /// If we have a cached result, just go ahead and send the reply
          /// and let us  return
          if (ocs.ptr () != 0)
            {
              // request.result_seq (
              request.send_cached_reply (ocs.inout ());

              return TAO_Adapter::DS_OK;
            }

          // If a PortableInterceptor::ForwardRequest exception was
          // thrown, then set the forward_to object reference and return
          // with the appropriate return status.
          forward_to.ptr () = request.forward_location ();
          if (request.is_forwarded ())
            {
              return TAO_Adapter::DS_FORWARD;
            }
#endif /*TAO_HAS_EXTENDED_FT_INTERCEPTORS*/

          // The receive_request_service_contexts() interception point
          // must be invoked before the operation is dispatched to the
          // servant.
          sri_adapter->receive_request_service_contexts (request,
                                                         0,  // args
                                                         0,  // nargs
                                                         0,  // servant_upcall
                                                         0,  // exceptions
                                                         0);   // nexceptions

          // If a PortableInterceptor::ForwardRequest exception was
          // thrown, then set the forward_to object reference and return
          // with the appropriate return status.
          forward_to.ptr () = request.forward_location ();
          if (request.is_forwarded ())
            {
              return TAO_Adapter::DS_FORWARD;
            }
        }
#endif  /* TAO_HAS_INTERCEPTORS == 1 */

      result = this->dispatch_servant (key, request, forward_to);

#if TAO_HAS_INTERCEPTORS == 1

      if (result == TAO_Adapter::DS_FORWARD)
        {
          request.reply_status (GIOP::LOCATION_FORWARD);
          request.pi_reply_status (PortableInterceptor::LOCATION_FORWARD);
          request.forward_location (forward_to.ptr ());
          if (sri_adapter != 0)
            {
              sri_adapter->send_other (request,
                                       0,  // args
                                       0,  // nargs
                                       0,  // servant_upcall
                                       0,  // exceptions
                                       0   // nexceptions
                                      );
            }
        }
    }
  catch ( ::CORBA::Exception& ex)
    {
      // Just assume the current exception is a system exception, the
      // status can only change when the interceptor changes this
      // and this is only done when the sri_adapter is available. If we
      // don't have an sri_adapter we just rethrow the exception
      PortableInterceptor::ReplyStatus status =
        PortableInterceptor::SYSTEM_EXCEPTION;

      if (sri_adapter != 0)
        {
          request.caught_exception (&ex);

          sri_adapter->send_exception (request,
                                       0,  // args
                                       0,  // nargs
                                       0,  // servant_upcall
                                       0,  // exceptions
                                       0);   // nexceptions

          status = request.pi_reply_status ();
        }

      // Only re-throw the exception if it hasn't been transformed by
      // the send_exception() interception point (e.g. to a
      // LOCATION_FORWARD).
      if (status == PortableInterceptor::SYSTEM_EXCEPTION
          || status == PortableInterceptor::USER_EXCEPTION)
        {
          throw;
        }
    }
#endif  /* TAO_HAS_INTERCEPTORS == 1 */

  return result;
}

const char *
TAO_Object_Adapter::name (void) const
{
  return TAO_OBJID_ROOTPOA;
}

CORBA::Object_ptr
TAO_Object_Adapter::root (void)
{
  return CORBA::Object::_duplicate (this->root_);
}

CORBA::Object_ptr
TAO_Object_Adapter::create_collocated_object (TAO_Stub *stub,
                                              const TAO_MProfile &mp)
{
  TAO_ServantBase *sb = this->get_collocated_servant (mp);

  // Set the servant ORB.  Do not duplicate the ORB here since
  // TAO_Stub::servant_orb() duplicates it.
  stub->servant_orb (this->orb_core_.orb ());

  // It is ok to create a collocated object even when <sb> is
  // zero. This constructor will set the stub collocated indicator and
  // the strategized proxy broker if required.
  CORBA::Object_ptr x;
  ACE_NEW_RETURN (x,
                  CORBA::Object (stub,
                                 1,
                                 sb),
                  CORBA::Object::_nil ());

  // Success.
  return x;
}

CORBA::Long
TAO_Object_Adapter::initialize_collocated_object (TAO_Stub *stub)
{
  // If we have been forwarded: use the forwarded profiles
  const TAO_MProfile &mp = stub->forward_profiles () ? *(stub->forward_profiles ())
                                                     : stub->base_profiles ();

  TAO_ServantBase *sb = this->get_collocated_servant (mp);

  // Set the servant ORB.  Do not duplicate the ORB here since
  // TAO_Stub::servant_orb() duplicates it.
  stub->servant_orb (this->orb_core_.orb ());

  // It is ok to set the object as a collocated object even when
  // <sb> is zero.
  stub->collocated_servant (sb);

  // Mark the stub as collocated. This will set the strategized object
  // proxy broker if required.
  stub->is_collocated (true);

  // Return 0 (success) if we found a servant.
  return ! sb;
}

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
void
TAO_Object_Adapter::release_poa_manager_factory (
                                     TAO_POAManager_Factory *factory)
{
  if (factory != 0)
    {
      factory->remove_all_poamanagers ();
      ::CORBA::release (factory);
    }
}
#endif

TAO_ServantBase *
TAO_Object_Adapter::get_collocated_servant (const TAO_MProfile &mp)
{
  for (TAO_PHandle j = 0;
       j != mp.profile_count ();
       ++j)
    {
      const TAO_Profile *profile = mp.get_profile (j);
      TAO::ObjectKey_var objkey = profile->_key ();

      if (objkey->length() < TAO_Root_POA::TAO_OBJECTKEY_PREFIX_SIZE
          || ACE_OS::memcmp (objkey->get_buffer (),
                             &TAO_Root_POA::objectkey_prefix[0],
                             TAO_Root_POA::TAO_OBJECTKEY_PREFIX_SIZE) != 0)
        continue;

      TAO_ServantBase *servant = 0;

      try
        {
          this->find_servant (objkey.in (), servant);
        }
      catch (const ::CORBA::Exception&)
        {
        }

      return servant;
    }

  return 0;
}

// ****************************************************************

TAO_Object_Adapter::Hint_Strategy::~Hint_Strategy (void)
{
}

void
TAO_Object_Adapter::Hint_Strategy::object_adapter (TAO_Object_Adapter *oa)
{
  this->object_adapter_ = oa;
}

TAO_Object_Adapter::Active_Hint_Strategy::Active_Hint_Strategy (CORBA::ULong map_size)
  : persistent_poa_system_map_ (map_size)
{
}

TAO_Object_Adapter::Active_Hint_Strategy::~Active_Hint_Strategy (void)
{
}

int
TAO_Object_Adapter::Active_Hint_Strategy::find_persistent_poa (
  const poa_name &system_name,
  TAO_Root_POA *&poa)
{
  poa_name folded_name;
  int result = this->persistent_poa_system_map_.recover_key (system_name,
                                                             folded_name);

  if (result == 0)
    {
      result = this->persistent_poa_system_map_.find (system_name, poa);
      if (result != 0
          || folded_name != poa->folded_name ())
        {
          result =
            this->object_adapter_->persistent_poa_name_map_->find (folded_name,
                                                                   poa);
          if (result != 0)
            {
              result = this->object_adapter_->activate_poa (folded_name, poa);
            }
        }
    }

  return result;
}

int
TAO_Object_Adapter::Active_Hint_Strategy::bind_persistent_poa (
  const poa_name &folded_name,
  TAO_Root_POA *poa,
  poa_name_out system_name)
{
  poa_name name = folded_name;
  int result = this->persistent_poa_system_map_.bind_modify_key (poa,
                                                                 name);

  if (result == 0)
    {
      result =
        this->object_adapter_->persistent_poa_name_map_->bind (folded_name,
                                                               poa);

      if (result != 0)
        this->persistent_poa_system_map_.unbind (name);
      else
        ACE_NEW_RETURN (system_name,
                        poa_name (name),
                        -1);
    }

  return result;
}

int
TAO_Object_Adapter::Active_Hint_Strategy::unbind_persistent_poa (
  const poa_name &folded_name,
  const poa_name &system_name)
{
  int result = this->persistent_poa_system_map_.unbind (system_name);

  if (result == 0)
    result =
      this->object_adapter_->persistent_poa_name_map_->unbind (folded_name);

  return result;
}

TAO_Object_Adapter::No_Hint_Strategy::~No_Hint_Strategy (void)
{
}

int
TAO_Object_Adapter::No_Hint_Strategy::find_persistent_poa (
  const poa_name &system_name,
  TAO_Root_POA *&poa)
{
  int result =
    this->object_adapter_->persistent_poa_name_map_->find (system_name,
                                                           poa);
  if (result != 0)
    {
      result =
        this->object_adapter_->activate_poa (system_name, poa);
    }

  return result;
}

int
TAO_Object_Adapter::No_Hint_Strategy::bind_persistent_poa (
  const poa_name &folded_name,
  TAO_Root_POA *poa,
  poa_name_out system_name)
{
  int result =
    this->object_adapter_->persistent_poa_name_map_->bind (folded_name, poa);
  if (result == 0)
    ACE_NEW_RETURN (system_name,
                    poa_name (folded_name),
                    -1);
  return result;
}

int
TAO_Object_Adapter::No_Hint_Strategy::unbind_persistent_poa (
  const poa_name & folded_name,
  const poa_name & /* system_name */)
{
  return this->object_adapter_->persistent_poa_name_map_->unbind (folded_name);
}

TAO_Object_Adapter::poa_name_iterator::poa_name_iterator (
  int begin,
  CORBA::ULong size,
  const CORBA::Octet *folded_buffer)
  : size_ (size),
    folded_buffer_ (folded_buffer),
    last_separator_ ((CORBA::ULong) ~0)
{
  if (begin)
    {
      this->position_ = (CORBA::ULong) ~0;
      this->operator++ ();
    }
  else
    this->position_ = this->size_;
}

bool
TAO_Object_Adapter::poa_name_iterator::operator== (const poa_name_iterator &rhs) const
{
  return this->position_ == rhs.position_;
}

bool
TAO_Object_Adapter::poa_name_iterator::operator!= (const poa_name_iterator &rhs) const
{
  return !this->operator== (rhs);
}

ACE_CString
TAO_Object_Adapter::poa_name_iterator::operator* () const
{
  CORBA::ULong start_at =
    this->last_separator_ +
    TAO_Root_POA::name_separator_length ();

  CORBA::ULong how_many =
    this->position_
    - this->last_separator_
    - TAO_Root_POA::name_separator_length ();

  return ACE_CString (reinterpret_cast <const char *>
                                       (&this->folded_buffer_[start_at]),
                      how_many);
}

TAO_Object_Adapter::poa_name_iterator &
TAO_Object_Adapter::poa_name_iterator::operator++ (void)
{
  for (this->last_separator_ = this->position_;
       ;
       )
    {
      ++this->position_;
      if (this->position_ < this->size_)
        {
          if (this->folded_buffer_[this->position_] == TAO_Root_POA::name_separator ())
            break;
        }
      else
        break;
    }

  return *this;
}

TAO_Object_Adapter::iteratable_poa_name::iteratable_poa_name (
  const poa_name &folded_name)
  : folded_name_ (folded_name)
{
}

TAO_Object_Adapter::iteratable_poa_name::iterator
TAO_Object_Adapter::iteratable_poa_name::begin (void) const
{
  return iterator (1,
                   this->folded_name_.length (),
                   this->folded_name_.get_buffer ());
}

TAO_Object_Adapter::iteratable_poa_name::iterator
TAO_Object_Adapter::iteratable_poa_name::end (void) const
{
  return iterator (0,
                   this->folded_name_.length (),
                   this->folded_name_.get_buffer ());
}

void
TAO_Object_Adapter::wait_for_non_servant_upcalls_to_complete (void)
{
  // Check if a non-servant upcall is in progress.  If a non-servant
  // upcall is in progress, wait for it to complete.  Unless of
  // course, the thread making the non-servant upcall is this thread.
  while (this->non_servant_upcall_in_progress_ &&
         ! ACE_OS::thr_equal (this->non_servant_upcall_thread_,
                              ACE_OS::thr_self ()))
    {
      // If so wait...
      int const result = this->non_servant_upcall_condition_.wait ();
      if (result == -1)
        throw ::CORBA::OBJ_ADAPTER ();
    }
}

void
TAO_Object_Adapter::wait_for_non_servant_upcalls_to_complete_no_throw (void)
{
  // Non-exception throwing version.
  try
    {
      this->wait_for_non_servant_upcalls_to_complete ();
    }
  catch (const ::CORBA::Exception&)
    {
      TAOLIB_ERROR ((LM_ERROR,
                  "TAO_Object_Adapter::wait_for_non_servant_upcalls_to_complete "
                  "threw exception it should not have!\n"));
    }
}

void
TAO_Object_Adapter::servant_dispatcher (TAO_Servant_Dispatcher *dispatcher)
{
  if (this->servant_dispatcher_)
    delete this->servant_dispatcher_;

  this->servant_dispatcher_ = dispatcher;
}

void
TAO_Object_Adapter::do_dispatch (TAO_ServerRequest& req,
                                 TAO::Portable_Server::Servant_Upcall& upcall)
{
  upcall.servant ()->_dispatch(req, &upcall);
}

TAO_END_VERSIONED_NAMESPACE_DECL
