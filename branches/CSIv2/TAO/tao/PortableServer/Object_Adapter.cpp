// $Id$

// -- PortableServer Include --
#include "Object_Adapter.h"
#include "POA.h"
#include "Strategized_Object_Proxy_Broker.h"
#include "ServerRequestInfo.h"
#include "Default_Servant_Dispatcher.h"
#include "ServerInterceptorAdapter.h"
#include "PortableServer_ORBInitializer.h"

// -- ACE Include --
#include "ace/Auto_Ptr.h"

// -- TAO Include --
#include "tao/ORB.h"
#include "tao/ORB_Core.h"
#include "tao/TAO_Server_Request.h"
#include "tao/Stub.h"
#include "tao/Profile.h"
#include "tao/MProfile.h"
#include "tao/debug.h"
#include "tao/PortableInterceptor.h"
#include "tao/Thread_Lane_Resources_Manager.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Protocols_Hooks.h"

#if !defined (__ACE_INLINE__)
# include "Object_Adapter.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID (TAO_PortableServer,
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
            ACE_static_cast (CORBA::ULong,
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

  persistent_poa_name_map *ppnm;
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

  // Thread policy.
  TAO_Thread_Policy thread_policy (PortableServer::ORB_CTRL_MODEL);
  policies.merge_policy (&thread_policy ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

#endif /* TAO_HAS_MINIMUM_POA == 0 */

  // Lifespan policy.
  TAO_Lifespan_Policy lifespan_policy (PortableServer::TRANSIENT);
  policies.merge_policy (&lifespan_policy ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // ID uniqueness policy.
  TAO_Id_Uniqueness_Policy id_uniqueness_policy (PortableServer::UNIQUE_ID);
  policies.merge_policy (&id_uniqueness_policy ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // ID assignment policy.
  TAO_Id_Assignment_Policy id_assignment_policy (PortableServer::SYSTEM_ID);
  policies.merge_policy (&id_assignment_policy ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

#if (TAO_HAS_MINIMUM_POA == 0)
  // Implicit activation policy.
  TAO_Implicit_Activation_Policy implicit_activation_policy
                           (PortableServer::NO_IMPLICIT_ACTIVATION);
  policies.merge_policy (&implicit_activation_policy ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Servant retention policy.
  TAO_Servant_Retention_Policy servant_retention_policy (PortableServer::RETAIN);
  policies.merge_policy (&servant_retention_policy ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Request processing policy.
  TAO_Request_Processing_Policy request_processing_policy
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
      ACE_Lock *the_lock;
      ACE_NEW_RETURN (the_lock,
                      ACE_Lock_Adapter<TAO_SYNCH_MUTEX> (thread_lock),
                      0);
      return the_lock;
    }
#else
  ACE_UNUSED_ARG (enable_locking);
  ACE_UNUSED_ARG (thread_lock);
#endif /* ACE_HAS_THREADS */

  ACE_Lock *the_lock;
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
  Servant_Upcall servant_upcall (&this->orb_core_);

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

  // Preprocess remote request.
  servant_upcall.pre_invoke_remote_request (req
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (result);

  // Servant dispatch.
  {
    ACE_FUNCTION_TIMEPROBE (TAO_SERVANT_DISPATCH_START);

    servant_upcall.servant ()->_dispatch (req,
                                          &servant_upcall
                                          ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (result);
  }

  return result;
}

void
TAO_Object_Adapter::locate_poa (const TAO::ObjectKey &key,
                                PortableServer::ObjectId &system_id,
                                TAO_POA *&poa
                                ACE_ENV_ARG_DECL)
{
  TAO_Object_Adapter::poa_name poa_system_name;
  CORBA::Boolean is_root = 0;
  CORBA::Boolean is_persistent = 0;
  CORBA::Boolean is_system_id = 0;
  TAO_Temporary_Creation_Time poa_creation_time;

  int result = 0;

  {
    ACE_FUNCTION_TIMEPROBE (TAO_POA_PARSE_KEY_START);

    result = TAO_POA::parse_key (key,
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
                                  TAO_POA *&poa
                                  ACE_ENV_ARG_DECL)
{
  int result = -1;

#if (TAO_HAS_MINIMUM_POA == 0)

  iteratable_poa_name ipn (folded_name);
  iteratable_poa_name::iterator iterator = ipn.begin ();
  iteratable_poa_name::iterator end = ipn.end ();

  TAO_POA *parent = this->root_;
  if (parent == 0 || parent->name () != *iterator)
    ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                      -1);
  else
    ++iterator;

  // A recursive thread lock without using a recursive thread lock.
  // Non_Servant_Upcall has a magic constructor and destructor.  We
  // unlock the Object_Adapter lock for the duration of the adapter
  // activator(s) upcalls; reacquiring once the upcalls complete.
  // Even though we are releasing the lock, other threads will not be
  // able to make progress since
  // <Object_Adapter::non_servant_upcall_in_progress_> has been set.
  Non_Servant_Upcall non_servant_upcall (*parent);
  ACE_UNUSED_ARG (non_servant_upcall);

  for (;
       iterator != end;
       ++iterator)
    {
      TAO_POA *current = 0;

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
                                        const TAO_Temporary_Creation_Time &poa_creation_time,
                                        TAO_POA *&poa
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
      || (result == 0 && poa->creation_time () != poa_creation_time))
    result = -1;

  return result;
}

int
TAO_Object_Adapter::bind_poa (const poa_name &folded_name,
                              TAO_POA *poa,
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
TAO_Object_Adapter::unbind_poa (TAO_POA *poa,
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
  PortableServer::ObjectId id;
  TAO_POA *poa = 0;

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
  TAO_POA *poa = 0;

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
  this->orb_core_.thread_lane_resources_manager ().open_default_resources (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Set the default Server Protocol Policy.
  this->set_default_server_protocol_policy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  TAO_POA_Policy_Set policies (this->default_poa_policies ());

#if (TAO_HAS_MINIMUM_POA == 0)
  // Specify the implicit activation policy since it should
  // be different from the default.  Note that merge_policy
  // takes a const reference and makes its own copy of the
  // policy.  (Otherwise, we'd have to allocate the policy
  // on the heap.)
  TAO_Implicit_Activation_Policy
    implicit_activation_policy (PortableServer::IMPLICIT_ACTIVATION);

  policies.merge_policy (&implicit_activation_policy
                         ACE_ENV_ARG_PARAMETER);
#endif /* TAO_HAS_MINIMUM_POA == 0 */

  // Merge policies from the ORB level.
  this->validator ().merge_policies (policies.policies ()
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Construct a new POA
  TAO_POA::String root_poa_name (TAO_DEFAULT_ROOTPOA_NAME);
  this->root_ =
    this->servant_dispatcher_->create_POA (root_poa_name,
                                           *poa_manager,
                                           policies,
                                           0,
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
  TAO_POA_Guard poa_guard (*this->root_ ACE_ENV_ARG_PARAMETER);
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
TAO_Object_Adapter::set_default_server_protocol_policy (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_Thread_Lane_Resources &default_lane_resources =
    this->orb_core_.thread_lane_resources_manager ().default_lane_resources ();

  TAO_Acceptor_Registry &acceptor_registry =
    default_lane_resources.acceptor_registry ();

  TAO_Protocols_Hooks *protocols_hooks =
    this->orb_core_.get_protocols_hooks (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  protocols_hooks->set_default_server_protocol_policy (acceptor_registry
                                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
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

  TAO_POA *root;
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
  CORBA::release (root);
}

void
TAO_Object_Adapter::check_close (int wait_for_completion
                                 ACE_ENV_ARG_DECL)
{
  TAO_POA::check_for_valid_wait_for_completions (this->orb_core (),
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
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (ACE_OS::memcmp (key.get_buffer (),
                      &TAO_POA::objectkey_prefix[0],
                      TAO_POA::TAO_OBJECTKEY_PREFIX_SIZE) != 0)
    {
      return TAO_Adapter::DS_MISMATCHED_KEY;
    }

  int result = 0;

#if TAO_HAS_INTERCEPTORS == 1
  TAO_ServerRequestInterceptor_Adapter sri_adapter (
    this->orb_core_.server_request_interceptors (),
    request.interceptor_count ());

  TAO_ServerRequestInfo ri (request, 0);

  ACE_TRY
    {
      // The receive_request_service_contexts() interception point
      // must be invoked before the operation is dispatched to the
      // servant.
      sri_adapter.receive_request_service_contexts (&ri ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // If a PortableInterceptor::ForwardRequest exception was
      // thrown, then set the forward_to object reference and return
      // with the appropriate return status.
      if (sri_adapter.location_forwarded ())
        {
          forward_to = ri.forward_reference (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          return TAO_Adapter::DS_FORWARD;
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
          ri.forward_reference (forward_to.ptr ());
          sri_adapter.send_other (&ri
                                  ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ri.exception (&ACE_ANY_EXCEPTION);

      sri_adapter.send_exception (&ri
                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableInterceptor::ReplyStatus status =
        ri.reply_status (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Only re-throw the exception if it hasn't been transformed by
      // the send_exception() interception point (e.g. to a
      // LOCATION_FORWARD).
      if (status == PortableInterceptor::SYSTEM_EXCEPTION
          || status == PortableInterceptor::USER_EXCEPTION)
        ACE_RE_THROW;
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
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      TAO_ServantBase *sb =
        this->get_collocated_servant (mp
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

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
      x->_proxy_broker (the_tao_strategized_object_proxy_broker ());

      // Success.
      return x;
    }
  ACE_CATCHANY
    {
      // Ignore the exception and continue with the next one.
    }
  ACE_ENDTRY;

  // Failure.
  return CORBA::Object::_nil ();
}

CORBA::Long
TAO_Object_Adapter::initialize_collocated_object (TAO_Stub *stub,
                                                  CORBA::Object_ptr obj)
{
  // @@ What about forwarding.  With this approach we are never
  //    forwarded  when we use collocation!
  const TAO_MProfile &mp =
    stub->base_profiles ();

  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      TAO_ServantBase *sb =
        this->get_collocated_servant (mp
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Set the servant ORB.  Do not duplicate the ORB here since
      // TAO_Stub::servant_orb() duplicates it.
      stub->servant_orb (this->orb_core_.orb ());

      // It is ok to set the object as a collocated object even when
      // <sb> is zero.
      obj->set_collocated_servant (sb);

      // Here we set the strategized Proxy Broker.
      obj->_proxy_broker (the_tao_strategized_object_proxy_broker ());

     // Success.
     return 0;
    }
  ACE_CATCHANY
    {
      // Ignore exceptions..
    }
  ACE_ENDTRY;

  // Failure.
  return -1;
}

TAO_ServantBase *
TAO_Object_Adapter::get_collocated_servant (const TAO_MProfile &mp
                                            ACE_ENV_ARG_DECL)
{
  for (TAO_PHandle j = 0;
       j != mp.profile_count ();
       ++j)
    {
      const TAO_Profile *profile = mp.get_profile (j);
      TAO::ObjectKey_var objkey = profile->_key ();

      if (ACE_OS::memcmp (objkey->get_buffer (),
                          &TAO_POA::objectkey_prefix[0],
                          TAO_POA::TAO_OBJECTKEY_PREFIX_SIZE) != 0)
        continue;

      TAO_ServantBase *servant = 0;

      this->find_servant (objkey.in (),
                          servant
                          ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      return servant;
    }

  return 0;
}


// ****************************************************************

TAO_Object_Adapter_Factory::TAO_Object_Adapter_Factory (void)
{
}

TAO_Adapter*
TAO_Object_Adapter_Factory::create (TAO_ORB_Core *orb_core)
{
  return new TAO_Object_Adapter (orb_core->server_factory ()->
                                    active_object_map_creation_parameters (),
                                 *orb_core);
}

int
TAO_Object_Adapter_Factory::init (int /* argc */,
                                  ACE_TCHAR* /* argv */ [])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      /// Register the Messaging ORBInitializer.
      PortableInterceptor::ORBInitializer_ptr temp_orb_initializer =
        PortableInterceptor::ORBInitializer::_nil ();

      ACE_NEW_THROW_EX (temp_orb_initializer,
                        TAO_PortableServer_ORBInitializer,
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_TRY_CHECK;

      PortableInterceptor::ORBInitializer_var orb_initializer =
        temp_orb_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ()
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "(%P | %t) Caught exception:");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

ACE_FACTORY_DEFINE (TAO_PortableServer, TAO_Object_Adapter_Factory)
ACE_STATIC_SVC_DEFINE (TAO_Object_Adapter_Factory,
                       ACE_TEXT ("TAO_POA"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Object_Adapter_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)

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
  TAO_POA *&poa
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
  TAO_POA *poa,
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
  TAO_POA *&poa
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
  TAO_POA *poa,
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

int
TAO_Object_Adapter::poa_name_iterator::operator== (const poa_name_iterator &rhs) const
{
  return this->position_ == rhs.position_;
}

int
TAO_Object_Adapter::poa_name_iterator::operator!= (const poa_name_iterator &rhs) const
{
  return !this->operator== (rhs);
}

ACE_CString
TAO_Object_Adapter::poa_name_iterator::operator* () const
{
  CORBA::ULong start_at =
    this->last_separator_ +
    TAO_POA::name_separator_length ();

  CORBA::ULong how_many =
    this->position_
    - this->last_separator_
    - TAO_POA::name_separator_length ();

  return ACE_CString (ACE_reinterpret_cast (const char *,
                                            &this->folded_buffer_[start_at]),
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
          if (this->folded_buffer_[this->position_] == TAO_POA::name_separator ())
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

TAO_Object_Adapter::Non_Servant_Upcall::Non_Servant_Upcall (TAO_POA &poa)
  : object_adapter_ (poa.object_adapter ()),
    poa_ (poa),
    previous_ (0)
{
  // Check if this is a nested non_servant_upcall.
  if (this->object_adapter_.non_servant_upcall_nesting_level_ != 0)
    {
      // Remember previous instance of non_servant_upcall.
      this->previous_ = this->object_adapter_.non_servant_upcall_in_progress_;

      // Assert that the thread is the same as the one before.
      ACE_ASSERT (this->object_adapter_.non_servant_upcall_thread_ == ACE_OS::thr_self ());
    }

  // Remember which thread is calling the adapter activators.
  this->object_adapter_.non_servant_upcall_thread_ = ACE_OS::thr_self ();

  // Mark the fact that a non-servant upcall is in progress.
  this->object_adapter_.non_servant_upcall_in_progress_ = this;

  // Adjust the nesting level.
  this->object_adapter_.non_servant_upcall_nesting_level_++;

  // Release the Object Adapter lock.
  this->object_adapter_.lock ().release ();
}

TAO_Object_Adapter::Non_Servant_Upcall::~Non_Servant_Upcall (void)
{
  // Reacquire the Object Adapter lock.
  this->object_adapter_.lock ().acquire ();

  // We are done with this nested upcall.
  this->object_adapter_.non_servant_upcall_in_progress_ = this->previous_;

  // Adjust the nesting level.
  this->object_adapter_.non_servant_upcall_nesting_level_--;

  // If we are at the outer nested upcall.
  if (this->object_adapter_.non_servant_upcall_nesting_level_ == 0)
    {
      // Reset thread id.
      this->object_adapter_.non_servant_upcall_thread_ =
        ACE_OS::NULL_thread;

      // Check if all pending requests are over.
      if (this->poa_.waiting_destruction () &&
          this->poa_.outstanding_requests () == 0)
        {
          ACE_TRY_NEW_ENV
            {
              this->poa_.complete_destruction_i (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
          ACE_CATCHANY
            {
              // Ignore exceptions
              ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
				   "TAO_POA::complete_destruction_i");
            }
          ACE_ENDTRY;
        }

      // If locking is enabled.
      if (this->object_adapter_.enable_locking_)
        // Wakeup all waiting threads.
        this->object_adapter_.non_servant_upcall_condition_.broadcast ();
    }
}

TAO_Object_Adapter::Servant_Upcall::Servant_Upcall (TAO_ORB_Core *oc)
  : object_adapter_ (0),
    poa_ (0),
    servant_ (0),
    state_ (INITIAL_STAGE),
    system_id_ (),
    user_id_ (0),
    current_context_ (),
#if (TAO_HAS_MINIMUM_POA == 0)
    cookie_ (0),
    operation_ (0),
#endif /* TAO_HAS_MINIMUM_POA == 0 */
    active_object_map_entry_ (0),
    using_servant_locator_ (0)
{
  TAO_Adapter *adapter = oc->poa_adapter ();
  TAO_Object_Adapter *object_adapter =
    ACE_dynamic_cast(TAO_Object_Adapter *, adapter);
  this->object_adapter_ = object_adapter;
}

int
TAO_Object_Adapter::Servant_Upcall::prepare_for_upcall (
  const TAO::ObjectKey &key,
  const char *operation,
  CORBA::Object_out forward_to
  ACE_ENV_ARG_DECL)
{
  while (1)
    {
      int wait_occurred_restart_call = 0;

      int result =
        this->prepare_for_upcall_i (key,
                                    operation,
                                    forward_to,
                                    wait_occurred_restart_call
                                    ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (TAO_Adapter::DS_FAILED);

      if (result == TAO_Adapter::DS_FAILED &&
          wait_occurred_restart_call)
        {
          // We ended up waiting on a condition variable.  The POA
          // state may have changed while we are waiting.  Therefore,
          // we need to call prepare_for_upcall_i() again.  We also
          // need to cleanup the state of the upcall object before
          // continuing.
          this->upcall_cleanup ();
          continue;
        }
      else
        {
          return result;
        }
    }
}

int
TAO_Object_Adapter::Servant_Upcall::prepare_for_upcall_i (
  const TAO::ObjectKey &key,
  const char *operation,
  CORBA::Object_out forward_to,
  int &wait_occurred_restart_call
  ACE_ENV_ARG_DECL)
{
  // Acquire the object adapter lock first.
  int result = this->object_adapter_->lock ().acquire ();
  if (result == -1)
    // Locking error.
    ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                      TAO_Adapter::DS_FAILED);

  // We have acquired the object adapater lock.  Record this for later
  // use.
  this->state_ = OBJECT_ADAPTER_LOCK_ACQUIRED;

  // Check if a non-servant upcall is in progress.  If a non-servant
  // upcall is in progress, wait for it to complete.  Unless of
  // course, the thread making the non-servant upcall is this thread.
  this->object_adapter_->wait_for_non_servant_upcalls_to_complete (
    ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (TAO_Adapter::DS_FAILED);

  // Locate the POA.
  this->object_adapter_->locate_poa (key,
                                     this->system_id_,
                                     this->poa_
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (TAO_Adapter::DS_FAILED);

  // Check the state of the POA Manager.
  this->poa_->check_poa_manager_state (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (TAO_Adapter::DS_FAILED);

  // Setup current for this request.
  this->current_context_.setup (this->poa_,
                                key);

  // Increase <poa->outstanding_requests_> for the duration of finding
  // the POA, finding the servant, and making the upcall.
  this->poa_->increment_outstanding_requests ();

  // We have setup the POA Current.  Record this for later use.
  this->state_ = POA_CURRENT_SETUP;

  ACE_TRY
    {
      ACE_FUNCTION_TIMEPROBE (TAO_POA_LOCATE_SERVANT_START);

      // Lookup the servant.
      this->servant_ = this->poa_->locate_servant_i (operation,
                                                     this->system_id_,
                                                     *this,
                                                     this->current_context_,
                                                     wait_occurred_restart_call
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (wait_occurred_restart_call)
        {
          return TAO_Adapter::DS_FAILED;
        }
    }
#if (TAO_HAS_MINIMUM_CORBA == 0)
  ACE_CATCH (PortableServer::ForwardRequest, forward_request)
    {
      forward_to =
        CORBA::Object::_duplicate (forward_request.forward_reference.in ());
      return TAO_Adapter::DS_FORWARD;
    }
#else
  ACE_CATCHANY
    {
      ACE_UNUSED_ARG (forward_to);
      ACE_RE_THROW;
    }
#endif /* TAO_HAS_MINIMUM_CORBA */
  ACE_ENDTRY;

  // Now that we know the servant.
  this->current_context_.servant (this->servant_);

  // For servants from Servant Locators, there is no active object map
  // entry.
  if (this->active_object_map_entry ())
    this->current_context_.priority (this->active_object_map_entry ()->priority_);

  if (this->state_ != OBJECT_ADAPTER_LOCK_RELEASED)
    {
      // Release the object adapter lock.
      this->object_adapter_->lock ().release ();

      // We have release the object adapater lock.  Record this for
      // later use.
      this->state_ = OBJECT_ADAPTER_LOCK_RELEASED;
    }

  // Serialize servants (if appropriate).
  this->single_threaded_poa_setup (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (TAO_Adapter::DS_FAILED);

  // We have acquired the servant lock.  Record this for later use.
  this->state_ = SERVANT_LOCK_ACQUIRED;

  // After this point, <this->servant_> is ready for dispatching.
  return TAO_Adapter::DS_OK;
}

void
TAO_Object_Adapter::Servant_Upcall::pre_invoke_remote_request (
  TAO_ServerRequest &req
  ACE_ENV_ARG_DECL)
{
  this->object_adapter_->servant_dispatcher_->pre_invoke_remote_request (
    this->poa (),
    this->priority (),
    req,
    this->pre_invoke_state_
    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_Object_Adapter::Servant_Upcall::pre_invoke_collocated_request (
  ACE_ENV_SINGLE_ARG_DECL)
{
  this->object_adapter_->servant_dispatcher_->pre_invoke_collocated_request (
    this->poa (),
    this->priority (),
    this->pre_invoke_state_
    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_Object_Adapter::Servant_Upcall::post_invoke (void)
{
  this->object_adapter_->servant_dispatcher_->post_invoke (
    this->poa (),
    this->pre_invoke_state_);
}

TAO_Object_Adapter::Servant_Upcall::Pre_Invoke_State::Pre_Invoke_State (void)
  : state_ (NO_ACTION_REQUIRED),
    original_native_priority_ (0),
    original_CORBA_priority_ (0)
{
}

TAO_POA *
TAO_Object_Adapter::Servant_Upcall::lookup_POA (const TAO::ObjectKey &key
                                                ACE_ENV_ARG_DECL)
{
  // Acquire the object adapter lock first.
  int result = this->object_adapter_->lock ().acquire ();
  if (result == -1)
    // Locking error.
    ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                      0);

  // We have acquired the object adapater lock.  Record this for later
  // use.
  this->state_ = OBJECT_ADAPTER_LOCK_ACQUIRED;

  // Check if a non-servant upcall is in progress.  If a non-servant
  // upcall is in progress, wait for it to complete.  Unless of
  // course, the thread making the non-servant upcall is this thread.
  this->object_adapter_->wait_for_non_servant_upcalls_to_complete (
    ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Locate the POA.
  this->object_adapter_->locate_poa (key,
                                     this->system_id_,
                                     this->poa_
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->poa_;
}

TAO_Object_Adapter::Servant_Upcall::~Servant_Upcall (void)
{
  this->upcall_cleanup ();
}

void
TAO_Object_Adapter::Servant_Upcall::upcall_cleanup (void)
{
  this->post_invoke ();

  switch (this->state_)
    {
    case SERVANT_LOCK_ACQUIRED:
      // Unlock servant (if appropriate).
      this->single_threaded_poa_cleanup ();

      /* FALLTHRU */

    case OBJECT_ADAPTER_LOCK_RELEASED:
      // Cleanup servant locator related state.  Note that because
      // this operation does not change any Object Adapter related
      // state, it is ok to call it outside the lock.
      this->servant_locator_cleanup ();

      // Since the object adapter lock was released, we must acquire
      // it.
      //
      // Note that errors are ignored here since we cannot do much
      // with it.
      this->object_adapter_->lock ().acquire ();

      // Check if a non-servant upcall is in progress.  If a
      // non-servant upcall is in progress, wait for it to complete.
      // Unless of course, the thread making the non-servant upcall is
      // this thread.
      this->object_adapter_->wait_for_non_servant_upcalls_to_complete ();

      // Cleanup servant related state.
      this->servant_cleanup ();

      /* FALLTHRU */

    case POA_CURRENT_SETUP:
      // Cleanup POA related state.
      this->poa_cleanup ();

      // Teardown current for this request.
      this->current_context_.teardown ();

      /* FALLTHRU */

    case OBJECT_ADAPTER_LOCK_ACQUIRED:
      // Finally, since the object adapter lock was acquired, we must
      // release it.
      this->object_adapter_->lock ().release ();

      /* FALLTHRU */

    case INITIAL_STAGE:
    default:
      // @@ Keep compiler happy, the states above are the only
      //    possible ones.
      break;
    }
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
TAO_Object_Adapter::Servant_Upcall::servant_locator_cleanup (void)
{
#if (TAO_HAS_MINIMUM_POA == 0)

  if (this->using_servant_locator_)
    {
      ACE_DECLARE_NEW_CORBA_ENV;
      ACE_TRY
        {
          this->poa_->servant_locator_->postinvoke (this->current_context_.object_id (),
                                                    this->poa_,
                                                    this->operation_,
                                                    this->cookie_,
                                                    this->servant_
                                                    ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          // Ignore errors from servant locator ....
        }
      ACE_ENDTRY;
    }

#endif /* TAO_HAS_MINIMUM_POA == 0 */
}

void
TAO_Object_Adapter::Servant_Upcall::single_threaded_poa_setup (ACE_ENV_SINGLE_ARG_DECL)
{
#if (TAO_HAS_MINIMUM_POA == 0)
  // Serialize servants (if necessary).
  //
  // Note that this lock must be acquired *after* the object adapter
  // lock has been released.  This is necessary since we cannot block
  // waiting for the servant lock while holding the object adapter
  // lock.  Otherwise, the thread that wants to release this lock will
  // not be able to do so since it can't acquire the object adapterx
  // lock.
  if (this->poa_->thread_policy () == PortableServer::SINGLE_THREAD_MODEL)
    {
      int result = this->poa_->single_threaded_lock ().acquire ();

      if (result == -1)
        // Locking error.
        ACE_THROW (CORBA::OBJ_ADAPTER ());
    }
#else
  ACE_ENV_ARG_NOT_USED; // FUZZ: ignore check_for_ace_check
#endif /* !TAO_HAS_MINIMUM_POA == 0 */
}

void
TAO_Object_Adapter::Servant_Upcall::single_threaded_poa_cleanup (void)
{
#if (TAO_HAS_MINIMUM_POA == 0)
  // Since the servant lock was acquired, we must release it.
  if (this->poa_->thread_policy () == PortableServer::SINGLE_THREAD_MODEL)
    this->poa_->single_threaded_lock ().release ();
#endif /* TAO_HAS_MINIMUM_POA == 0 */
}

void
TAO_Object_Adapter::Servant_Upcall::servant_cleanup (void)
{
  // Cleanup servant related stuff.
  if (this->active_object_map_entry_ != 0)
    {
      // Decrement the reference count.
      CORBA::UShort new_count = --this->active_object_map_entry_->reference_count_;

      if (new_count == 0)
        {
          ACE_DECLARE_NEW_CORBA_ENV;
          ACE_TRY
            {
              this->poa_->cleanup_servant (this->active_object_map_entry_
                                           ACE_ENV_ARG_PARAMETER);

              ACE_TRY_CHECK;
            }
          ACE_CATCHANY
            {
              // Ignore errors from servant cleanup ....
            }
          ACE_ENDTRY;

          if (this->poa_->waiting_servant_deactivation_ > 0 &&
              this->object_adapter_->enable_locking_)
            {
              // Wakeup all waiting threads.
              this->poa_->servant_deactivation_condition_.broadcast ();
            }
        }
    }
}

void
TAO_Object_Adapter::Servant_Upcall::poa_cleanup (void)
{
  // Decrease <poa->outstanding_requests_> now that the upcall
  // is complete.
  //
  // Note that the object adapter lock is acquired before
  // <POA::outstanding_requests_> is decreased.
  CORBA::ULong outstanding_requests =
    this->poa_->decrement_outstanding_requests ();

  // Check if all pending requests are over.
  if (outstanding_requests == 0)
    {
      // If locking is enabled and some thread is waiting in POA::destroy.
      if (this->object_adapter_->enable_locking_ &&
          this->poa_->wait_for_completion_pending_)
        {
          // Wakeup all waiting threads.
          this->poa_->outstanding_requests_condition_.broadcast ();
        }

      // Note that there is no need to check for
      // <non_servant_upcall_in_progress> since it is not possible for
      // non-servant upcalls to be in progress at this point.
      if (this->poa_->waiting_destruction_)
        {
          ACE_TRY_NEW_ENV
            {
              this->poa_->complete_destruction_i (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
          ACE_CATCHANY
            {
              // Ignore exceptions
              ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_POA::~complete_destruction_i");
            }
          ACE_ENDTRY;

          this->poa_ = 0;
        }
    }
}

void
TAO_Object_Adapter::servant_dispatcher (TAO_Servant_Dispatcher *dispatcher)
{
  if (this->servant_dispatcher_)
    delete this->servant_dispatcher_;

  this->servant_dispatcher_ = dispatcher;
}

TAO_POA_Current_Impl::TAO_POA_Current_Impl (void)
  : poa_ (0),
    object_id_ (),
    object_key_ (0),
    servant_ (0),
    priority_ (TAO_INVALID_PRIORITY),
    previous_current_impl_ (0),
    setup_done_ (0)
{
}

void
TAO_POA_Current_Impl::setup (TAO_POA *p,
                             const TAO::ObjectKey &key)
{
  // Remember information about this upcall.
  this->poa_ = p;
  this->object_key_ = &key;

  // Set the current context and remember the old one.
  this->tss_resources_ = TAO_TSS_RESOURCES::instance ();

  this->previous_current_impl_ =
    ACE_static_cast (TAO_POA_Current_Impl *,
                     this->tss_resources_->poa_current_impl_);
  this->tss_resources_->poa_current_impl_ = this;

  // Setup is complete.
  this->setup_done_ = 1;
}

TAO_POA_Current_Impl *
TAO_POA_Current_Impl::previous (void) const
{
  return this->previous_current_impl_;
}

void
TAO_POA_Current_Impl::teardown (void)
{
  if (this->setup_done_)
    {
      // Reset the old context.
      this->tss_resources_->poa_current_impl_ = this->previous_current_impl_;
    }
}

PortableServer::POA_ptr
TAO_POA_Current_Impl::get_POA (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::Current::NoContext))
{
  return PortableServer::POA::_duplicate (this->poa_);
}

PortableServer::ObjectId *
TAO_POA_Current_Impl::get_object_id (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::Current::NoContext))
{
  PortableServer::ObjectId *objid = 0;

  // Create a new one and pass it back
  ACE_NEW_RETURN (objid,
                  PortableServer::ObjectId (this->object_id_),
                  0);
  return objid;
}

TAO_ORB_Core &
TAO_POA_Current_Impl::orb_core (void) const

{
  return this->poa_->orb_core_;
}

PortableServer::POA_ptr
TAO_POA_Current::get_POA (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::Current::NoContext))
{
  TAO_POA_Current_Impl *impl = this->implementation ();

  if (impl == 0)
    ACE_THROW_RETURN (PortableServer::Current::NoContext (),
                      0);
  return impl->get_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
}

PortableServer::ObjectId *
TAO_POA_Current::get_object_id (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::Current::NoContext))
{
  TAO_POA_Current_Impl *impl = this->implementation ();

  if (impl == 0)
    ACE_THROW_RETURN (PortableServer::Current::NoContext (),
                      0);
  return impl->get_object_id (ACE_ENV_SINGLE_ARG_PARAMETER);
}

TAO_POA_Current_Impl *
TAO_POA_Current::implementation (void)
{
  return ACE_static_cast (TAO_POA_Current_Impl *,
                          TAO_TSS_RESOURCES::instance ()->poa_current_impl_);
}

TAO_POA_Current_Impl *
TAO_POA_Current::implementation (TAO_POA_Current_Impl *new_current)
{
  TAO_TSS_Resources *tss =
    TAO_TSS_RESOURCES::instance ();

  TAO_POA_Current_Impl *old =
    ACE_static_cast (TAO_POA_Current_Impl *,
                     tss->poa_current_impl_);
  tss->poa_current_impl_ = new_current;
  return old;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

// Common typedefs.
typedef TAO_Object_Adapter::poa_name key;
typedef TAO_POA *value;

typedef ACE_Pair<key, value> expanded_value;
typedef ACE_Reference_Pair<const key, value> tao_value_type;
typedef ACE_Equal_To<key> compare_keys;
typedef TAO_ObjectId_Hash hash_key;
typedef ACE_Noop_Key_Generator<key> noop_key_generator;

template class auto_ptr<TAO_Object_Adapter::Hint_Strategy>;
template class auto_ptr<TAO_Object_Adapter::transient_poa_map>;

#  if defined (ACE_LACKS_AUTO_PTR) \
      || !(defined (ACE_HAS_STANDARD_CPP_LIBRARY) \
           && (ACE_HAS_STANDARD_CPP_LIBRARY != 0))
template class ACE_Auto_Basic_Ptr<TAO_Object_Adapter::Hint_Strategy>;
template class ACE_Auto_Basic_Ptr<TAO_Object_Adapter::transient_poa_map>;
#  endif  /* ACE_LACKS_AUTO_PTR */

template class ACE_Noop_Key_Generator<key>;

// Common
template class ACE_Reference_Pair<const key, value>;

// Map and iterator classes.
template class ACE_Map<key, value>;
template class ACE_Iterator<tao_value_type>;
template class ACE_Reverse_Iterator<tao_value_type>;

// Iterator base classes.
template class ACE_Iterator_Impl<tao_value_type>;
template class ACE_Reverse_Iterator_Impl<tao_value_type>;

// Active Map Manager related.
template class ACE_Pair<key, value>;
template class ACE_Active_Map_Manager_Adapter<key, value, TAO_Ignore_Original_Key_Adapter>;
template class ACE_Active_Map_Manager_Adapter<key, value, TAO_Preserve_Original_Key_Adapter>;
template class ACE_Active_Map_Manager_Iterator_Adapter<tao_value_type, expanded_value>;
template class ACE_Active_Map_Manager_Reverse_Iterator_Adapter<tao_value_type, expanded_value>;
template class ACE_Active_Map_Manager<expanded_value>;
template class ACE_Map_Manager<ACE_Active_Map_Manager_Key, expanded_value, ACE_Null_Mutex>;
template class ACE_Map_Iterator_Base<ACE_Active_Map_Manager_Key, expanded_value, ACE_Null_Mutex>;
template class ACE_Map_Iterator<ACE_Active_Map_Manager_Key, expanded_value, ACE_Null_Mutex>;
template class ACE_Map_Reverse_Iterator<ACE_Active_Map_Manager_Key, expanded_value, ACE_Null_Mutex>;
template class ACE_Map_Entry<ACE_Active_Map_Manager_Key, expanded_value>;

// Hash Map Manager related.
#if (TAO_HAS_MINIMUM_POA_MAPS == 0)
template class ACE_Hash_Map_Manager_Ex_Adapter<key, value, hash_key, compare_keys, TAO_Incremental_Key_Generator>;
#endif /* TAO_HAS_MINIMUM_POA_MAPS == 0 */
template class ACE_Hash_Map_Manager_Ex_Adapter<key, value, hash_key, compare_keys, noop_key_generator>;
template class ACE_Hash_Map_Manager_Ex_Iterator_Adapter<tao_value_type, key, value, hash_key, compare_keys>;
template class ACE_Hash_Map_Manager_Ex_Reverse_Iterator_Adapter<tao_value_type, key, value, hash_key, compare_keys>;
template class ACE_Hash_Map_Manager_Ex<key, value, hash_key, compare_keys, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<key, value, hash_key, compare_keys, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<key, value, hash_key, compare_keys, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<key, value, hash_key, compare_keys, ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry<key, value>;

// Already in Active_Object_Map.cpp
// template class ACE_Equal_To<key>;

#if (TAO_HAS_MINIMUM_POA_MAPS == 0)
// Map Manager related.
template class ACE_Map_Manager_Iterator_Adapter<tao_value_type, key, value>;
template class ACE_Map_Manager_Reverse_Iterator_Adapter<tao_value_type, key, value>;
template class ACE_Map_Manager_Adapter<key, value, TAO_Incremental_Key_Generator>;
template class ACE_Map_Manager_Adapter<key, value, noop_key_generator>;
template class ACE_Map_Manager<key, value, ACE_Null_Mutex>;
template class ACE_Map_Iterator_Base<key, value, ACE_Null_Mutex>;
template class ACE_Map_Iterator<key, value, ACE_Null_Mutex>;
template class ACE_Map_Reverse_Iterator<key, value, ACE_Null_Mutex>;
template class ACE_Map_Entry<key, value>;
#endif /* TAO_HAS_MINIMUM_POA_MAPS == 0 */

template class ACE_Unbounded_Set<TAO_POA_Manager *>;
template class ACE_Node<TAO_POA_Manager *>;
template class ACE_Unbounded_Set_Iterator<TAO_POA_Manager *>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

// Common typedefs.
typedef TAO_Object_Adapter::poa_name key;
typedef TAO_POA *value;

typedef ACE_Pair<key, value> expanded_value;
typedef ACE_Reference_Pair<const key, value> tao_value_type;
typedef ACE_Equal_To<key> compare_keys;
typedef TAO_ObjectId_Hash hash_key;
typedef ACE_Noop_Key_Generator<key> noop_key_generator;

#pragma instantiate auto_ptr<TAO_Object_Adapter::Hint_Strategy>
#pragma instantiate auto_ptr<TAO_Object_Adapter::transient_poa_map>

#  if defined (ACE_LACKS_AUTO_PTR) \
      || !(defined (ACE_HAS_STANDARD_CPP_LIBRARY) \
           && (ACE_HAS_STANDARD_CPP_LIBRARY != 0))
#    pragma instantiate ACE_Auto_Basic_Ptr<TAO_Object_Adapter::Hint_Strategy>
#    pragma instantiate ACE_Auto_Basic_Ptr<TAO_Object_Adapter::transient_poa_map>
#  endif  /* ACE_LACKS_AUTO_PTR */

#pragma instantiate ACE_Noop_Key_Generator<key>

// Common
#pragma instantiate ACE_Reference_Pair<const key, value>

// Map and iterator classes.
#pragma instantiate ACE_Map<key, value>
#pragma instantiate ACE_Iterator<tao_value_type>
#pragma instantiate ACE_Reverse_Iterator<tao_value_type>

// Iterator base classes.
#pragma instantiate ACE_Iterator_Impl<tao_value_type>
#pragma instantiate ACE_Reverse_Iterator_Impl<tao_value_type>

// Active Map Manager related.
#pragma instantiate ACE_Pair<key, value>
#pragma instantiate ACE_Active_Map_Manager_Adapter<key, value, TAO_Ignore_Original_Key_Adapter>
#pragma instantiate ACE_Active_Map_Manager_Adapter<key, value, TAO_Preserve_Original_Key_Adapter>
#pragma instantiate ACE_Active_Map_Manager_Iterator_Adapter<tao_value_type, expanded_value>
#pragma instantiate ACE_Active_Map_Manager_Reverse_Iterator_Adapter<tao_value_type, expanded_value>
#pragma instantiate ACE_Active_Map_Manager<expanded_value>
#pragma instantiate ACE_Map_Manager<ACE_Active_Map_Manager_Key, expanded_value, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator_Base<ACE_Active_Map_Manager_Key, expanded_value, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator<ACE_Active_Map_Manager_Key, expanded_value, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Reverse_Iterator<ACE_Active_Map_Manager_Key, expanded_value, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Entry<ACE_Active_Map_Manager_Key, expanded_value>

// Hash Map Manager related.
#if (TAO_HAS_MINIMUM_POA_MAPS == 0)
#pragma instantiate ACE_Hash_Map_Manager_Ex_Adapter<key, value, hash_key, compare_keys, TAO_Incremental_Key_Generator>
#endif /* TAO_HAS_MINIMUM_POA_MAPS == 0 */
#pragma instantiate ACE_Hash_Map_Manager_Ex_Adapter<key, value, hash_key, compare_keys, noop_key_generator>
#pragma instantiate ACE_Hash_Map_Manager_Ex_Iterator_Adapter<tao_value_type, key, value, hash_key, compare_keys>
#pragma instantiate ACE_Hash_Map_Manager_Ex_Reverse_Iterator_Adapter<tao_value_type, key, value, hash_key, compare_keys>
#pragma instantiate ACE_Hash_Map_Manager_Ex<key, value, hash_key, compare_keys, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<key, value, hash_key, compare_keys, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<key, value, hash_key, compare_keys, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<key, value, hash_key, compare_keys, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Entry<key, value>

// Already in Active_Object_Map.cpp
// #pragma instantiate ACE_Equal_To<key>

#if (TAO_HAS_MINIMUM_POA_MAPS == 0)
// Map Manager related.
#pragma instantiate ACE_Map_Manager_Iterator_Adapter<tao_value_type, key, value>
#pragma instantiate ACE_Map_Manager_Reverse_Iterator_Adapter<tao_value_type, key, value>
#pragma instantiate ACE_Map_Manager_Adapter<key, value, TAO_Incremental_Key_Generator>
#pragma instantiate ACE_Map_Manager_Adapter<key, value, noop_key_generator>
#pragma instantiate ACE_Map_Manager<key, value, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator_Base<key, value, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator<key, value, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Reverse_Iterator<key, value, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Entry<key, value>
#endif /* TAO_HAS_MINIMUM_POA_MAPS == 0 */

#pragma instantiate ACE_Unbounded_Set<TAO_POA_Manager *>
#pragma instantiate ACE_Node<TAO_POA_Manager *>
#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_POA_Manager *>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
