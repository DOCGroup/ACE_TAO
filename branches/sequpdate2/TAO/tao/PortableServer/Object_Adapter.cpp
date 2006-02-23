// $Id$

// -- PortableServer Include --
#include "Object_Adapter.h"
#include "Non_Servant_Upcall.h"
#include "Servant_Upcall.h"
#include "Root_POA.h"
#include "Regular_POA.h"
#include "Creation_Time.h"
#include "POA_Guard.h"
#include "Default_Servant_Dispatcher.h"
#include "Collocated_Object_Proxy_Broker.h"
#include "POAManager.h"
#include "Servant_Base.h"

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
# include "Object_Adapter.i"
#endif /* __ACE_INLINE__ */

#include "ThreadPolicy.h"
#include "LifespanPolicy.h"
#include "IdAssignmentPolicy.h"
#include "IdUniquenessPolicy.h"
#include "ImplicitActivationPolicy.h"
#include "RequestProcessingPolicy.h"
#include "ServantRetentionPolicy.h"

ACE_RCSID (PortableServer,
           Object_Adapter,
           "$Id$")

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
    enable_locking_ (orb_core_.server_factory ()->enable_poa_locking ()),
    thread_lock_ (),
    lock_ (TAO_Object_Adapter::create_lock (enable_locking_,
                                            thread_lock_)),
    reverse_lock_ (*lock_),
    non_servant_upcall_condition_ (thread_lock_),
    non_servant_upcall_in_progress_ (0),
    non_servant_upcall_nesting_level_ (0),
    non_servant_upcall_thread_ (ACE_OS::NULL_thread),
    root_ (0),
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
      ACE_ERROR ((LM_ERROR,
                  "linear option for -ORBPersistentidPolicyDemuxStrategy "
                  "not supported with minimum POA maps. "
                  "Ingoring option to use default... \n"));
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
      ACE_ERROR ((LM_ERROR,
                  "linear and dynamic options for -ORBTransientidPolicyDemuxStrategy "
                  "are not supported with minimum POA maps. "
                  "Ingoring option to use default... \n"));
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
TAO_Object_Adapter::init_default_policies (TAO_POA_Policy_Set &policies
                                           ACE_ENV_ARG_DECL)
{
  // Initialize the default policies.
#if (TAO_HAS_MINIMUM_POA == 0)

  TAO::Portable_Server::ThreadPolicy thread_policy (PortableServer::ORB_CTRL_MODEL);
  policies.merge_policy (&thread_policy ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

#endif /* TAO_HAS_MINIMUM_POA == 0 */

  // Lifespan policy.
  TAO::Portable_Server::LifespanPolicy lifespan_policy (PortableServer::TRANSIENT);
  policies.merge_policy (&lifespan_policy ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // ID uniqueness policy.
  TAO::Portable_Server::IdUniquenessPolicy id_uniqueness_policy (PortableServer::UNIQUE_ID);
  policies.merge_policy (&id_uniqueness_policy ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // ID assignment policy.
  TAO::Portable_Server::IdAssignmentPolicy id_assignment_policy (PortableServer::SYSTEM_ID);
  policies.merge_policy (&id_assignment_policy ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

#if (TAO_HAS_MINIMUM_POA == 0)
  // Implicit activation policy.
  TAO::Portable_Server::ImplicitActivationPolicy implicit_activation_policy
    (PortableServer::NO_IMPLICIT_ACTIVATION);
  policies.merge_policy (&implicit_activation_policy ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Servant retention policy.
  TAO::Portable_Server::ServantRetentionPolicy servant_retention_policy
    (PortableServer::RETAIN);
  policies.merge_policy (&servant_retention_policy ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Request processing policy.
  TAO::Portable_Server::RequestProcessingPolicy request_processing_policy
    (PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY);
  policies.merge_policy (&request_processing_policy ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
#endif /* TAO_HAS_MINIMUM_POA == 0 */
}

TAO_Object_Adapter::~TAO_Object_Adapter (void)
{
  delete this->hint_strategy_;
  delete this->persistent_poa_name_map_;
  delete this->transient_poa_map_;
  delete this->lock_;

  delete this->servant_dispatcher_;
}

/* static */
ACE_Lock *
TAO_Object_Adapter::create_lock (int enable_locking,
                                 TAO_SYNCH_MUTEX &thread_lock)
{
#if defined (ACE_HAS_THREADS)
  if (enable_locking)
    {
      ACE_Lock *the_lock = 0;
      ACE_NEW_RETURN (the_lock,
                      ACE_Lock_Adapter<TAO_SYNCH_MUTEX> (thread_lock),
                      0);
      return the_lock;
    }
#else
  ACE_UNUSED_ARG (enable_locking);
  ACE_UNUSED_ARG (thread_lock);
#endif /* ACE_HAS_THREADS */

  ACE_Lock *the_lock = 0;
  ACE_NEW_RETURN (the_lock,
                  ACE_Lock_Adapter<ACE_SYNCH_NULL_MUTEX> (),
                  0);
  return the_lock;
}

int
TAO_Object_Adapter::dispatch_servant (const TAO::ObjectKey &key,
                                      TAO_ServerRequest &req,
                                      CORBA::Object_out forward_to
                                      ACE_ENV_ARG_DECL)
{
  ACE_FUNCTION_TIMEPROBE (TAO_OBJECT_ADAPTER_DISPATCH_SERVANT_START);

  // This object is magical, i.e., it has a non-trivial constructor
  // and destructor.
  TAO::Portable_Server::Servant_Upcall servant_upcall (&this->orb_core_);

  // Set up state in the POA et al (including the POA Current), so
  // that we know that this servant is currently in an upcall.
  const char *operation = req.operation ();
  int result =
    servant_upcall.prepare_for_upcall (key,
                                       operation,
                                       forward_to
                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (result);

  if (result != TAO_Adapter::DS_OK)
    return result;

  // Preprocess request.
  if (req.collocated ())
    {
      servant_upcall.pre_invoke_collocated_request (
        ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (result);
    }
  else
    {
      servant_upcall.pre_invoke_remote_request (req
                                                ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (result);
    }

  // Servant dispatch.
  {
    ACE_FUNCTION_TIMEPROBE (TAO_SERVANT_DISPATCH_START);

    do_dispatch (req, servant_upcall ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (result);
  }

  return result;
}

void
TAO_Object_Adapter::locate_poa (const TAO::ObjectKey &key,
                                PortableServer::ObjectId &system_id,
                                TAO_Root_POA *&poa
                                ACE_ENV_ARG_DECL)
{
  TAO_Object_Adapter::poa_name poa_system_name;
  CORBA::Boolean is_root = 0;
  CORBA::Boolean is_persistent = 0;
  CORBA::Boolean is_system_id = 0;
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
    ACE_THROW (CORBA::OBJ_ADAPTER ());

  {
    ACE_FUNCTION_TIMEPROBE (TAO_OBJECT_ADAPTER_FIND_POA_START);

    result = this->find_poa (poa_system_name,
                             is_persistent,
                             is_root,
                             poa_creation_time,
                             poa
                             ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }

  if (result != 0)
    ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
}

int
TAO_Object_Adapter::activate_poa (const poa_name &folded_name,
                                  TAO_Root_POA *&poa
                                  ACE_ENV_ARG_DECL)
{
  int result = -1;

#if (TAO_HAS_MINIMUM_POA == 0)

  iteratable_poa_name ipn (folded_name);
  iteratable_poa_name::iterator iterator = ipn.begin ();
  iteratable_poa_name::iterator end = ipn.end ();

  TAO_Root_POA *parent = this->root_;
  if (parent == 0 || parent->name () != *iterator)
    ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                      -1);
  else
    ++iterator;

  for (;
       iterator != end;
       ++iterator)
    {
      TAO_Root_POA *current = 0;

      ACE_TRY
        {
          current = parent->find_POA_i (*iterator,
                                        1
                                        ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCH (PortableServer::POA::AdapterNonExistent, ex)
        {
          return -1;
        }
      ACE_ENDTRY;
      ACE_CHECK_RETURN (-1);

      parent = current;
    }

  poa = parent;
  result = 0;
#else
  ACE_UNUSED_ARG (folded_name);
  ACE_UNUSED_ARG (poa);
  ACE_ENV_ARG_NOT_USED; // FUZZ: ignore check_for_ace_check
#endif /* TAO_HAS_MINIMUM_POA == 0 */

  return result;
}

int
TAO_Object_Adapter::find_transient_poa (const poa_name &system_name,
                                        CORBA::Boolean root,
                                        const TAO::Portable_Server::Temporary_Creation_Time &poa_creation_time,
                                        TAO_Root_POA *&poa
                                        ACE_ENV_ARG_DECL_NOT_USED)
{
  int result = 0;

  if (root)
    {
      poa = this->root_;
    }
  else
    {
      result = this->transient_poa_map_->find (system_name,
                                               poa);
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
    return this->bind_persistent_poa (folded_name,
                                      poa,
                                      system_name);
  else
    return this->bind_transient_poa (poa,
                                     system_name);
}

int
TAO_Object_Adapter::unbind_poa (TAO_Root_POA *poa,
                                const poa_name &folded_name,
                                const poa_name &system_name)
{
  if (poa->persistent ())
    return this->unbind_persistent_poa (folded_name,
                                        system_name);
  else
    return this->unbind_transient_poa (system_name);
}

int
TAO_Object_Adapter::locate_servant_i (const TAO::ObjectKey &key
                                      ACE_ENV_ARG_DECL)
{
  ACE_FUNCTION_TIMEPROBE (TAO_POA_LOCATE_SERVANT_START);

  PortableServer::ObjectId id;
  TAO_Root_POA *poa = 0;

  this->locate_poa (key,
                    id,
                    poa
                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  PortableServer::Servant servant = 0;
  TAO_SERVANT_LOCATION servant_location =
    poa->locate_servant_i (id,
                           servant
                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

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
                                    PortableServer::Servant &servant
                                    ACE_ENV_ARG_DECL)
{
  PortableServer::ObjectId id;
  TAO_Root_POA *poa = 0;

  this->locate_poa (key,
                    id,
                    poa
                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (TAO_SERVANT_NOT_FOUND);

  TAO_SERVANT_LOCATION servant_location = poa->locate_servant_i (id,
                                                                 servant
                                                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (TAO_SERVANT_NOT_FOUND);

  return servant_location;
}

void
TAO_Object_Adapter::open (ACE_ENV_SINGLE_ARG_DECL)
{
  // Add in the default POA policies to the default list.
  this->init_default_policies (this->default_poa_policies ()
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // If a POA extension hasn't changed the servant dispatcher, initialize the
  // default one.
  if (this->servant_dispatcher_ == 0)
    {
      ACE_NEW (this->servant_dispatcher_,
               TAO_Default_Servant_Dispatcher);
    }

  TAO_POA_Manager *poa_manager;
  ACE_NEW_THROW_EX (poa_manager,
                    TAO_POA_Manager (*this),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  PortableServer::POAManager_var safe_poa_manager = poa_manager;

  // This makes sure that the default resources are open when the Root
  // POA is created.
  this->orb_core_.thread_lane_resources_manager ().open_default_resources (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  TAO_POA_Policy_Set policies (this->default_poa_policies ());

#if (TAO_HAS_MINIMUM_POA == 0)
  // Specify the implicit activation policy since it should
  // be different from the default.  Note that merge_policy
  // takes a const reference and makes its own copy of the
  // policy.  (Otherwise, we'd have to allocate the policy
  // on the heap.)
  // Implicit activation policy.
  TAO::Portable_Server::ImplicitActivationPolicy implicit_activation_policy
    (PortableServer::IMPLICIT_ACTIVATION);
  policies.merge_policy (&implicit_activation_policy ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
#endif /* TAO_HAS_MINIMUM_POA == 0 */

  // Merge policies from the ORB level.
  this->validator ().merge_policies (policies.policies ()
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // If any of the policy objects specified are not valid for the ORB
  // implementation, if conflicting policy objects are specified, or
  // if any of the specified policy objects require prior
  // administrative action that has not been performed, an
  // InvalidPolicy exception is raised containing the index in the
  // policies parameter value of the first offending policy object.
  policies.validate_policies (this->validator (),
                              this->orb_core_
                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Construct a new POA
  TAO_Root_POA::String root_poa_name (TAO_DEFAULT_ROOTPOA_NAME);
  this->root_ =
    this->servant_dispatcher_->create_Root_POA (root_poa_name,
                                                *poa_manager,
                                                policies,
                                                this->lock (),
                                                this->thread_lock (),
                                                this->orb_core_,
                                                this
                                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // The Object_Adapter will keep a reference to the Root POA so that
  // on its destruction, it can check whether the Root POA has been
  // destroyed yet or not.
  this->root_->_add_ref ();

  // Lock access for the duration of this transaction.
  TAO::Portable_Server::POA_Guard poa_guard (*this->root_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Iterate over the registered IOR interceptors so that they may be
  // given the opportunity to add tagged components to the profiles
  // for this servant.
  this->root_->establish_components (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Release the POA_Manager_var since we got here without error.  The
  // TAO_POA object takes ownership of the POA_Manager object
  // (actually it shares the ownership with its peers).
  (void) safe_poa_manager._retn ();
}

void
TAO_Object_Adapter::close (int wait_for_completion
                           ACE_ENV_ARG_DECL)
{
  this->check_close (wait_for_completion ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Shutting down the ORB causes all object adapters to be destroyed,
  // since they cannot exist in the absence of an ORB. Shut down is
  // complete when all ORB processing (including request processing
  // and object deactivation or other operations associated with
  // object adapters) has completed and the object adapters have been
  // destroyed. In the case of the POA, this means that all object
  // etherealizations have finished and root POA has been destroyed
  // (implying that all descendent POAs have also been destroyed).

  TAO_Root_POA *root = 0;
  {
    ACE_GUARD (ACE_Lock, ace_mon, this->lock ());
    if (this->root_ == 0)
      return;
    root = this->root_;
    this->root_ = 0;
  }
  CORBA::Boolean etherealize_objects = 1;
  root->destroy (etherealize_objects,
                 wait_for_completion
                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  ::CORBA::release (root);
}

void
TAO_Object_Adapter::check_close (int wait_for_completion
                                 ACE_ENV_ARG_DECL)
{
  TAO_Root_POA::check_for_valid_wait_for_completions (this->orb_core (),
                                                 wait_for_completion
                                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

int
TAO_Object_Adapter::priority (void) const
{
  return 0;
}

int
TAO_Object_Adapter::dispatch (TAO::ObjectKey &key,
                              TAO_ServerRequest &request,
                              CORBA::Object_out forward_to
                              ACE_ENV_ARG_DECL)
{
  if (ACE_OS::memcmp (key.get_buffer (),
                      &TAO_Root_POA::objectkey_prefix[0],
                      TAO_Root_POA::TAO_OBJECTKEY_PREFIX_SIZE) != 0)
    {
      return TAO_Adapter::DS_MISMATCHED_KEY;
    }

  int result = 0;

#if TAO_HAS_INTERCEPTORS == 1
  TAO::ServerRequestInterceptor_Adapter *sri_adapter =
    orb_core_.serverrequestinterceptor_adapter ();

  ACE_TRY
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
                                                 ocs.out ()
                                                 ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

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
          if (!CORBA::is_nil (request.forward_location ()))
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
                                                         0   // nexceptions
                                                         ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // If a PortableInterceptor::ForwardRequest exception was
          // thrown, then set the forward_to object reference and return
          // with the appropriate return status.
          forward_to.ptr () = request.forward_location ();
          if (!CORBA::is_nil (request.forward_location ()))
            {
              return TAO_Adapter::DS_FORWARD;
            }
        }
#endif  /* TAO_HAS_INTERCEPTORS == 1 */

      result = this->dispatch_servant (key,
                                       request,
                                       forward_to
                                       ACE_ENV_ARG_PARAMETER);

#if TAO_HAS_INTERCEPTORS == 1
      ACE_TRY_CHECK;

      if (result == TAO_Adapter::DS_FORWARD)
        {
          request.forward_location (forward_to.ptr ());
          if (sri_adapter != 0)
            {
              sri_adapter->send_other (request,
                                       0,  // args
                                       0,  // nargs
                                       0,  // servant_upcall
                                       0,  // exceptions
                                       0   // nexceptions
                                       ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
        }
    }
  ACE_CATCHANY
    {
      // Just assume the current exception is a system exception, the
      // status can only change when the interceptor changes this
      // and this is only done when the sri_adapter is available. If we
      // don't have an sri_adapter we just rethrow the exception
      PortableInterceptor::ReplyStatus status =
        PortableInterceptor::SYSTEM_EXCEPTION;

      if (sri_adapter != 0)
        {
          request.caught_exception (&ACE_ANY_EXCEPTION);

          sri_adapter->send_exception (request,
                                       0,  // args
                                       0,  // nargs
                                       0,  // servant_upcall
                                       0,  // exceptions
                                       0   // nexceptions
                                       ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          status =
            request.reply_status ();
        }

      // Only re-throw the exception if it hasn't been transformed by
      // the send_exception() interception point (e.g. to a
      // LOCATION_FORWARD).
      if (status == PortableInterceptor::SYSTEM_EXCEPTION
          || status == PortableInterceptor::USER_EXCEPTION)
        {
          ACE_RE_THROW;
        }
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (result);
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
  // zero.
  CORBA::Object_ptr x;
  ACE_NEW_RETURN (x,
                  CORBA::Object (stub,
                                 1,
                                 sb),
                  CORBA::Object::_nil ());

  // Here we set the strategized Proxy Broker.
  x->_proxy_broker (the_tao_collocated_object_proxy_broker ());

  // Success.
  return x;
}

CORBA::Long
TAO_Object_Adapter::initialize_collocated_object (TAO_Stub *stub,
                                                  CORBA::Object_ptr obj)
{
  // @@ What about forwarding.  With this approach we are never
  //    forwarded  when we use collocation!
  const TAO_MProfile &mp = stub->base_profiles ();

  TAO_ServantBase *sb = this->get_collocated_servant (mp);

  // Set the servant ORB.  Do not duplicate the ORB here since
  // TAO_Stub::servant_orb() duplicates it.
  stub->servant_orb (this->orb_core_.orb ());

  // It is ok to set the object as a collocated object even when
  // <sb> is zero.
  obj->set_collocated_servant (sb);

  // Here we set the strategized Proxy Broker.
  obj->_proxy_broker (the_tao_collocated_object_proxy_broker ());

  // Success.
  return 0;
}

TAO_ServantBase *
TAO_Object_Adapter::get_collocated_servant (const TAO_MProfile &mp)
{
  for (TAO_PHandle j = 0;
       j != mp.profile_count ();
       ++j)
    {
      const TAO_Profile *profile = mp.get_profile (j);
      TAO::ObjectKey_var objkey = profile->_key ();

      if (ACE_OS::memcmp (objkey->get_buffer (),
                          &TAO_Root_POA::objectkey_prefix[0],
                          TAO_Root_POA::TAO_OBJECTKEY_PREFIX_SIZE) != 0)
        continue;

      TAO_ServantBase *servant = 0;

      ACE_DECLARE_NEW_CORBA_ENV;
      ACE_TRY
        {
          this->find_servant (objkey.in (),
              servant
              ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
        }
      ACE_ENDTRY;

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
  TAO_Root_POA *&poa
  ACE_ENV_ARG_DECL)
{
  poa_name folded_name;
  int result = this->persistent_poa_system_map_.recover_key (system_name,
                                                             folded_name);

  if (result == 0)
    {
      result = this->persistent_poa_system_map_.find (system_name,
                                                      poa);
      if (result != 0
          || folded_name != poa->folded_name ())
        {
          result =
            this->object_adapter_->persistent_poa_name_map_->find (folded_name,
                                                                   poa);
          if (result != 0)
            {
              result =
                this->object_adapter_->activate_poa (folded_name,
                                                     poa
                                                     ACE_ENV_ARG_PARAMETER);
              ACE_CHECK_RETURN (-1);
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
  TAO_Root_POA *&poa
  ACE_ENV_ARG_DECL)
{
  int result =
    this->object_adapter_->persistent_poa_name_map_->find (system_name,
                                                           poa);
  if (result != 0)
    {
      result =
        this->object_adapter_->activate_poa (system_name,
                                             poa
                                             ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
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
    this->object_adapter_->persistent_poa_name_map_->bind (folded_name,
                                                           poa);
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
TAO_Object_Adapter::wait_for_non_servant_upcalls_to_complete (CORBA::Environment &ACE_TRY_ENV)
{
#if defined (ACE_HAS_EXCEPTIONS)
  ACE_UNUSED_ARG (ACE_TRY_ENV); // FUZZ: ignore check_for_ace_check
#endif

  // Check if a non-servant upcall is in progress.  If a non-servant
  // upcall is in progress, wait for it to complete.  Unless of
  // course, the thread making the non-servant upcall is this thread.
  while (this->enable_locking_ &&
         this->non_servant_upcall_in_progress_ &&
         ! ACE_OS::thr_equal (this->non_servant_upcall_thread_,
                              ACE_OS::thr_self ()))
    {
      // If so wait...
      int result =
        this->non_servant_upcall_condition_.wait ();
      if (result == -1)
        ACE_THROW (CORBA::OBJ_ADAPTER ());
    }
}

void
TAO_Object_Adapter::wait_for_non_servant_upcalls_to_complete (void)
{
  // Non-exception throwing version.
  ACE_TRY_NEW_ENV
    {
      this->wait_for_non_servant_upcalls_to_complete (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_ERROR ((LM_ERROR,
                  "TAO_Object_Adapter::wait_for_non_servant_upcalls_to_complete "
                  "threw exception it should not have!\n"));
    }
  ACE_ENDTRY;
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
                                 TAO::Portable_Server::Servant_Upcall& upcall
                                 ACE_ENV_ARG_DECL)
{
  upcall.servant ()->_dispatch(req,
                               &upcall
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

TAO_END_VERSIONED_NAMESPACE_DECL
