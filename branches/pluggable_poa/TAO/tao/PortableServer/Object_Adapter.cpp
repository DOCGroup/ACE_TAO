// $Id$

#include "Object_Adapter.h"
#include "POA.h"
#include "Collocated_Object.h"
#include "tao/ORB.h"
#include "tao/ORB_Core.h"
#include "tao/Server_Request.h"
#include "tao/Stub.h"
#include "tao/Profile.h"
#include "tao/MProfile.h"
#include "ace/Auto_Ptr.h"

#if !defined (__ACE_INLINE__)
# include "Object_Adapter.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, Object_Adapter, "$Id$")

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
size_t TAO_Object_Adapter::transient_poa_name_size_ = 0;

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
            ACE_Active_Map_Manager_Key::size ();
          break;
        }
    }
}

TAO_Object_Adapter::TAO_Object_Adapter (const TAO_Server_Strategy_Factory::Active_Object_Map_Creation_Parameters &creation_parameters,
                                        TAO_ORB_Core &orb_core)
  : hint_strategy_ (0),
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
    non_servant_upcall_thread_ (ACE_OS::NULL_thread)
  , root_ (0)
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

TAO_Object_Adapter::~TAO_Object_Adapter (void)
{
  delete this->hint_strategy_;
  delete this->persistent_poa_name_map_;
  delete this->transient_poa_map_;
  delete this->lock_;
}

/* static */
ACE_Lock *
TAO_Object_Adapter::create_lock (int enable_locking,
                                 ACE_SYNCH_MUTEX &thread_lock)
{
#if defined (ACE_HAS_THREADS)
  if (enable_locking)
    {
      ACE_Lock *the_lock;
      ACE_NEW_RETURN (the_lock,
                      ACE_Lock_Adapter<ACE_SYNCH_MUTEX> (thread_lock),
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

void
TAO_Object_Adapter::dispatch_servant (const TAO_ObjectKey &key,
                                      CORBA::ServerRequest &req,
                                      void *context,
                                      CORBA::Environment &ACE_TRY_ENV)
{
  ACE_FUNCTION_TIMEPROBE (TAO_OBJECT_ADAPTER_DISPATCH_SERVANT_START);

  // This object is magical, i.e., it has a non-trivial constructor
  // and destructor.
  Servant_Upcall servant_upcall (&this->orb_core_);

  // Set up state in the POA et al (including the POA Current), so
  // that we know that this servant is currently in an upcall.
  const char *operation = req.operation ();
  servant_upcall.prepare_for_upcall (key,
                                     operation,
                                     ACE_TRY_ENV);
  ACE_CHECK;

#if (TAO_HAS_RT_CORBA == 1)

  // RTCORBA PriorityModelPolicy processing (may need to be
  // moved/adjusted when POA threadpools are added).
  // This is the earliest place we can do the processing, since only
  // at this point we know the target POA.

  Priority_Model_Processing priority_processing (servant_upcall.poa ());

  // Set thread's priority.
  priority_processing.pre_invoke (req.service_info (), ACE_TRY_ENV);
  ACE_CHECK;

#endif /* TAO_HAS_RT_CORBA == 1 */

  // Servant dispatch.
  {
    ACE_FUNCTION_TIMEPROBE (TAO_SERVANT_DISPATCH_START);

    servant_upcall.servant ()->_dispatch (req,
                                          context,
                                          ACE_TRY_ENV);
    ACE_CHECK;
  }

#if (TAO_HAS_RT_CORBA == 1)

  // Reset thread's priority to its original value.  If this method
  // isn't reached, i.e., because of an exception, the reset takes
  // place in Priority_Model_Processing destructor.
  priority_processing.post_invoke (ACE_TRY_ENV);
  ACE_CHECK;

#endif /* TAO_HAS_RT_CORBA == 1 */
}

void
TAO_Object_Adapter::locate_poa (const TAO_ObjectKey &key,
                                PortableServer::ObjectId &system_id,
                                TAO_POA *&poa,
                                CORBA_Environment &ACE_TRY_ENV)
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
                             poa,
                             ACE_TRY_ENV);
    ACE_CHECK;
  }

  if (result != 0)
    ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
}

int
TAO_Object_Adapter::activate_poa (const poa_name &folded_name,
                                  TAO_POA *&poa,
                                  CORBA_Environment &ACE_TRY_ENV)
{
  int result = -1;

#if (TAO_HAS_MINIMUM_POA == 0)

  // A recursive thread lock without using a recursive thread lock.
  // Non_Servant_Upcall has a magic constructor and destructor.  We
  // unlock the Object_Adapter lock for the duration of the adapter
  // activator(s) upcalls; reacquiring once the upcalls complete.
  // Even though we are releasing the lock, other threads will not be
  // able to make progress since
  // <Object_Adapter::non_servant_upcall_in_progress_> has been set.
  Non_Servant_Upcall non_servant_upcall (*this);
  ACE_UNUSED_ARG (non_servant_upcall);

  iteratable_poa_name ipn (folded_name);
  iteratable_poa_name::iterator iterator = ipn.begin ();
  iteratable_poa_name::iterator end = ipn.end ();

  TAO_POA *parent = this->root_;
  // @@ PPOA: this->orb_core_.root_poa (ACE_TRY_ENV);
  // @@ PPOA: ACE_CHECK_RETURN (-1);

  if (parent->name () != *iterator)
    ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                      -1);
  else
    ++iterator;

  for (;
       iterator != end;
       ++iterator)
    {
      TAO_POA *current = 0;

      ACE_TRY
        {
          current = parent->find_POA_i (*iterator,
                                        1,
                                        ACE_TRY_ENV);
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
  ACE_UNUSED_ARG (ACE_TRY_ENV);
#endif /* TAO_HAS_MINIMUM_POA == 0 */

  return result;
}

int
TAO_Object_Adapter::find_transient_poa (const poa_name &system_name,
                                        CORBA::Boolean root,
                                        const TAO_Temporary_Creation_Time &poa_creation_time,
                                        TAO_POA *&poa,
                                        CORBA::Environment &)
{
  int result = 0;

  if (root)
    {
      poa = this->root_;
      // @@ PPOA: this->orb_core_.root_poa (ACE_TRY_ENV);
      // @@ PPOA: ACE_CHECK_RETURN (-1);
    }
  else
    {
      result = this->transient_poa_map_->find (system_name,
                                               poa);
    }

  if (result == 0 && poa->creation_time () != poa_creation_time)
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
TAO_Object_Adapter::locate_servant_i (const TAO_ObjectKey &key,
                                      CORBA::Environment &ACE_TRY_ENV)
{
  PortableServer::ObjectId id;
  TAO_POA *poa = 0;

  this->locate_poa (key,
                    id,
                    poa,
                    ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  PortableServer::Servant servant = 0;
  TAO_SERVANT_LOCATION servant_location =
    poa->locate_servant_i (id,
                           servant,
                           ACE_TRY_ENV);
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
TAO_Object_Adapter::find_servant_i (const TAO_ObjectKey &key,
                                    PortableServer::Servant &servant,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  PortableServer::ObjectId id;
  TAO_POA *poa = 0;

  this->locate_poa (key,
                    id,
                    poa,
                    ACE_TRY_ENV);
  ACE_CHECK_RETURN (TAO_SERVANT_NOT_FOUND);

  TAO_SERVANT_LOCATION servant_location = poa->locate_servant_i (id,
                                                                 servant,
                                                                 ACE_TRY_ENV);
  ACE_CHECK_RETURN (TAO_SERVANT_NOT_FOUND);

  return servant_location;
}

void
TAO_Object_Adapter::open (CORBA::Environment &ACE_TRY_ENV)
{
  // Only set the auto_ptr if poa_manager is allocated here.
  auto_ptr<TAO_POA_Manager> safe_poa_manager;

  TAO_POA_Manager *poa_manager;
  ACE_NEW_THROW_EX (poa_manager,
                    TAO_POA_Manager (*this),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  ACE_AUTO_PTR_RESET (safe_poa_manager,
                      poa_manager,
                      TAO_POA_Manager);

#if 0
  TAO_POA_Policies root_poa_policies (this->orb_core_,
                                      ACE_TRY_ENV);
  ACE_CHECK;

  if (policies == 0)
    {
      // RootPOA policies defined in spec
      root_poa_policies.implicit_activation (
           PortableServer::IMPLICIT_ACTIVATION);

      policies = &root_poa_policies;
    }
#else
  TAO_POA_Policies policies (this->orb_core_,
                             ACE_TRY_ENV);
  ACE_CHECK;

  policies.implicit_activation (PortableServer::IMPLICIT_ACTIVATION);
#endif /* 0 */

  // Construct a new POA
  ACE_NEW_THROW_EX (this->root_,
                    TAO_POA (TAO_DEFAULT_ROOTPOA_NAME,
                             *poa_manager,
                             policies,
                             0,
                             this->lock (),
                             this->thread_lock (),
                             this->orb_core_,
                             this,
                             ACE_TRY_ENV),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  // The Object_Adapter will keep a reference to the Root POA so that
  // on its destruction, it can check whether the Root POA has
  // been destroyed yet or not.
  this->root_->_add_ref ();

  // Release the auto_ptr since we got here without error, the TAO_POA
  // object takes ownership of the POA_Manager object (actually it
  // shares the ownership with its peers).

  (void) safe_poa_manager.release ();
}

void
TAO_Object_Adapter::close (int wait_for_completion,
                           CORBA::Environment &ACE_TRY_ENV)
{
  this->check_close (wait_for_completion, ACE_TRY_ENV);
  ACE_CHECK;

  // Shutting down the ORB causes all object adapters to be destroyed,
  // since they cannot exist in the absence of an ORB. Shut down is
  // complete when all ORB processing (including request processing
  // and object deactivation or other operations associated with
  // object adapters) has completed and the object adapters have been
  // destroyed. In the case of the POA, this means that all object
  // etherealizations have finished and root POA has been destroyed
  // (implying that all descendent POAs have also been destroyed).
  if (this->root_ != 0)
    {
      CORBA::Boolean etherealize_objects = 1;
      this->root_->destroy (etherealize_objects,
                            wait_for_completion,
                            ACE_TRY_ENV);
      ACE_CHECK;
      CORBA::release (this->root_);
      this->root_ = 0;
    }
}

void
TAO_Object_Adapter::check_close (int wait_for_completion,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  // @@ Irfan: this seems suspect, say I am in the middle of an upcall
  // to ORB A, and I decide to shutdown ORB B as part of that, does
  // this global function have enough context to verify that this is
  // valid?  IMHO it should be a method in the Object_Adapter, that
  // only bails out if we are in the middle of an upcall to its
  // ORB. Comments?
  TAO_POA::check_for_valid_wait_for_completions (wait_for_completion,
                                                 ACE_TRY_ENV);
  ACE_CHECK;
}

int
TAO_Object_Adapter::priority (void) const
{
  return 0;
}

int
TAO_Object_Adapter::dispatch (TAO_ObjectKey &key,
                              CORBA::ServerRequest &request,
                              void *context,
                              CORBA::Object_out forward_to,
                              CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (ACE_OS::memcmp (key.get_buffer (),
                      &TAO_POA::objectkey_prefix[0],
                      TAO_POA::TAO_OBJECTKEY_PREFIX_SIZE) != 0)
    {
      return TAO_Adapter::DS_MISMATCHED_KEY;
    }

  ACE_TRY
    {
      this->dispatch_servant (key,
                              request,
                              context,
                              ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (PortableServer::ForwardRequest, forward_request)
    {
      forward_to =
        CORBA::Object::_duplicate (forward_request.forward_reference.in ());
      return TAO_Adapter::DS_FORWARD;
    }
  ACE_ENDTRY;

  return TAO_Adapter::DS_OK;
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
  for (TAO_PHandle j = 0;
       j != mp.profile_count ();
       ++j)
    {
      const TAO_Profile *profile = mp.get_profile (j);
      TAO_ObjectKey_var objkey = profile->_key ();

      if (ACE_OS::memcmp (objkey->get_buffer (),
                          &TAO_POA::objectkey_prefix[0],
                          TAO_POA::TAO_OBJECTKEY_PREFIX_SIZE) != 0)
        continue;

      ACE_DECLARE_NEW_CORBA_ENV;
      ACE_TRY
        {
          TAO_ServantBase *servant = 0;

          TAO_SERVANT_LOCATION servant_location =
            this->find_servant (objkey.in (),
                                servant,
                                ACE_TRY_ENV);
          ACE_TRY_CHECK;

          if (servant_location != TAO_SERVANT_NOT_FOUND)
            {
              // Found collocated object.  Perhaps we can get around
              // by simply setting the servant_orb, but let get this
              // to work first.

              // There could only be one ORB which is us.

              // @@ Do not duplicate the ORB here!
              //    TAO_Stub::servant_orb()  duplicates it.
              //       -Ossama
              stub->servant_orb (this->orb_core_.orb ());

              CORBA::Object_ptr x;
              ACE_NEW_RETURN (x,
                              TAO_Collocated_Object (stub, 1,
                                                     servant),
                              0);
              return x;
            }
        }
      ACE_CATCHANY
        {
          // Ignore the exception and continue with the next one.
        }
      ACE_ENDTRY;
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
  // Setup the POA_Current object in the ORB
  CORBA::Object_var current = new TAO_POA_Current;
  orb_core->poa_current (current.in ());

  return new TAO_Object_Adapter (orb_core->server_factory ()->
                                    active_object_map_creation_parameters (),
                                 *orb_core);
}

ACE_FACTORY_DEFINE (TAO, TAO_Object_Adapter_Factory)
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
TAO_Object_Adapter::Active_Hint_Strategy::find_persistent_poa (const poa_name &system_name,
                                                               TAO_POA *&poa,
                                                               CORBA_Environment &ACE_TRY_ENV)
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
          result = this->object_adapter_->persistent_poa_name_map_->find (folded_name,
                                                                          poa);
          if (result != 0)
            {
              result = this->object_adapter_->activate_poa (folded_name,
                                                            poa,
                                                            ACE_TRY_ENV);
              ACE_CHECK_RETURN (-1);
            }
        }
    }

  return result;
}

int
TAO_Object_Adapter::Active_Hint_Strategy::bind_persistent_poa (const poa_name &folded_name,
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
TAO_Object_Adapter::Active_Hint_Strategy::unbind_persistent_poa (const poa_name &folded_name,
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
TAO_Object_Adapter::No_Hint_Strategy::find_persistent_poa (const poa_name &system_name,
                                                           TAO_POA *&poa,
                                                           CORBA_Environment &ACE_TRY_ENV)
{
  int result = this->object_adapter_->persistent_poa_name_map_->find (system_name,
                                                                      poa);
  if (result != 0)
    {
      result =
        this->object_adapter_->activate_poa (system_name,
                                             poa,
                                             ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);
    }

  return result;
}

int
TAO_Object_Adapter::No_Hint_Strategy::bind_persistent_poa (const poa_name &folded_name,
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
TAO_Object_Adapter::No_Hint_Strategy::unbind_persistent_poa (const poa_name &folded_name,
                                                             const poa_name &system_name)
{
  ACE_UNUSED_ARG (system_name);

  return this->object_adapter_->persistent_poa_name_map_->unbind (folded_name);
}

TAO_Object_Adapter::poa_name_iterator::poa_name_iterator (int begin,
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

TAO_Object_Adapter::iteratable_poa_name::iteratable_poa_name (const poa_name &folded_name)
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

TAO_Object_Adapter::Non_Servant_Upcall::Non_Servant_Upcall (TAO_Object_Adapter &object_adapter)
  : object_adapter_ (object_adapter)
{
  // Mark the fact that a non-servant upcall is in progress.
  this->object_adapter_.non_servant_upcall_in_progress_ = 1;

  // Remember which thread is calling the adapter activators.
  this->object_adapter_.non_servant_upcall_thread_ = ACE_OS::thr_self ();

  // Release the Object Adapter lock.
  this->object_adapter_.lock ().release ();
}

TAO_Object_Adapter::Non_Servant_Upcall::~Non_Servant_Upcall (void)
{
  // Reacquire the Object Adapter lock.
  this->object_adapter_.lock ().acquire ();

  // We are no longer in a non-servant upcall.
  this->object_adapter_.non_servant_upcall_in_progress_ = 0;

  // Reset thread id.
  this->object_adapter_.non_servant_upcall_thread_ =
    ACE_OS::NULL_thread;

  // If locking is enabled.
  if (this->object_adapter_.enable_locking_)
    // Wakeup all waiting threads.
    this->object_adapter_.non_servant_upcall_condition_.broadcast ();
}

// @@ PPOA TAO_Object_Adapter::Servant_Upcall::Servant_Upcall (TAO_Object_Adapter &object_adapter)
TAO_Object_Adapter::Servant_Upcall::Servant_Upcall (TAO_ORB_Core *oc)
  : object_adapter_ (0),
    poa_ (0),
    servant_ (0),
    state_ (INITIAL_STAGE),
    id_ (),
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

void
TAO_Object_Adapter::Servant_Upcall::prepare_for_upcall (const TAO_ObjectKey &key,
                                                        const char *operation,
                                                        CORBA::Environment &ACE_TRY_ENV)
{
  // Acquire the object adapter lock first.
  int result = this->object_adapter_->lock ().acquire ();
  if (result == -1)
    // Locking error.
    ACE_THROW (CORBA::OBJ_ADAPTER ());

  // We have acquired the object adapater lock.  Record this for later
  // use.
  this->state_ = OBJECT_ADAPTER_LOCK_ACQUIRED;

  // Check if a non-servant upcall is in progress.  If a non-servant
  // upcall is in progress, wait for it to complete.  Unless of
  // course, the thread making the non-servant upcall is this thread.
  this->object_adapter_->wait_for_non_servant_upcalls_to_complete (ACE_TRY_ENV);
  ACE_CHECK;

  // Locate the POA.
  this->object_adapter_->locate_poa (key,
                                    this->id_,
                                    this->poa_,
                                    ACE_TRY_ENV);
  ACE_CHECK;

  // Check the state of the POA Manager.
  this->poa_->check_poa_manager_state (ACE_TRY_ENV);
  ACE_CHECK;

  // Setup current for this request.
  this->current_context_.setup (this->poa_,
                                key);

  // We have setup the POA Current.  Record this for later use.
  this->state_ = POA_CURRENT_SETUP;

  {
    ACE_FUNCTION_TIMEPROBE (TAO_POA_LOCATE_SERVANT_START);

    // Lookup the servant.
    this->servant_ = this->poa_->locate_servant_i (operation,
                                                   this->id_,
                                                   *this,
                                                   this->current_context_,
                                                   ACE_TRY_ENV);
    ACE_CHECK;
  }

  // Now that we know the servant.
  this->current_context_.servant (this->servant_);

  if (this->active_object_map_entry ())
    this->current_context_.priority (this->active_object_map_entry ()->priority_);

  // Increase <poa->outstanding_requests_> for the duration of the
  // upcall.
  //
  // Note that the object adapter lock is released after
  // <POA::outstanding_requests_> is increased.
  this->poa_->increment_outstanding_requests ();

  // Release the object adapter lock.
  this->object_adapter_->lock ().release ();

  // We have release the object adapater lock.  Record this for later
  // use.
  this->state_ = OBJECT_ADAPTER_LOCK_RELEASED;

  // Lock servant (if appropriate).
  this->single_threaded_poa_setup (ACE_TRY_ENV);
  ACE_CHECK;

  // We have acquired the servant lock.  Record this for later use.
  this->state_ = SERVANT_LOCK_ACQUIRED;

  // After this point, <this->servant_> is ready for dispatching.
}

TAO_Object_Adapter::Servant_Upcall::~Servant_Upcall ()
{
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

      // Cleanup servant related state.
      this->servant_cleanup ();

      // Cleanup POA related state.
      this->poa_cleanup ();
      /* FALLTHRU */

    case POA_CURRENT_SETUP:
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
TAO_Object_Adapter::Servant_Upcall::servant_locator_cleanup (void)
{
#if (TAO_HAS_MINIMUM_POA == 0)

  if (this->using_servant_locator_)
    {
      // If we are a single threaded POA, teardown the appropriate
      // locking in the servant.
      //
      // Note that teardown of the servant lock must happen before the
      // post_invoke() call since that might end up deleting the
      // servant.
      //
      this->poa_->teardown_servant_lock (this->servant_);

      ACE_DECLARE_NEW_CORBA_ENV;
      ACE_TRY
        {
          this->poa_->servant_locator_->postinvoke (this->current_context_.object_id (),
                                                    this->poa_,
                                                    this->operation_,
                                                    this->cookie_,
                                                    this->servant_
                                                    TAO_ENV_ARG_PARAMETER);
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
TAO_Object_Adapter::Servant_Upcall::single_threaded_poa_setup (CORBA::Environment &ACE_TRY_ENV)
{
#if (TAO_HAS_MINIMUM_POA == 0)

  // Lock servant (if necessary).
  //
  // Note that this lock must be acquired *after* the object adapter
  // lock has been released.  This is necessary since we cannot block
  // waiting for the servant lock while holding the object adapter
  // lock.  Otherwise, the thread that wants to release this lock will
  // not be able to do so since it can't acquire the object adapterx
  // lock.
  if (this->poa_->policies ().thread () == PortableServer::SINGLE_THREAD_MODEL)
    {
      int result = this->servant_->_single_threaded_poa_lock ().acquire ();

      if (result == -1)
        // Locking error.
        ACE_THROW (CORBA::OBJ_ADAPTER ());
    }
#else
  ACE_UNUSED_ARG (ACE_TRY_ENV);
#endif /* !TAO_HAS_MINIMUM_POA == 0 */
}

void
TAO_Object_Adapter::Servant_Upcall::single_threaded_poa_cleanup (void)
{
  // Since the servant lock was acquired, we must release it.
#if (TAO_HAS_MINIMUM_POA == 0)
  if (this->poa_->policies ().thread () == PortableServer::SINGLE_THREAD_MODEL)
    this->servant_->_single_threaded_poa_lock ().release ();
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
              this->poa_->cleanup_servant (this->active_object_map_entry_,
                                           ACE_TRY_ENV);

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
  // <POA::outstanding_requests_> is increased.
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
      if (this->poa_->waiting_destruction_)
        {
          ACE_TRY_NEW_ENV
            {
              this->poa_->complete_destruction_i (ACE_TRY_ENV);
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

#if (TAO_HAS_RT_CORBA == 1)

TAO_Object_Adapter::Priority_Model_Processing::~Priority_Model_Processing
(void)
{
  if (this->state_ == PRIORITY_RESET_REQUIRED)
    {
      this->state_ = NO_ACTION_REQUIRED;

      // Reset the priority of the current thread back to its original
      // value.
      if (poa_.orb_core ().set_thread_priority (this->original_priority_)
          == -1)
        // At this point we cannot throw an exception.  Just log the
        // error.
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("TAO (%P|%t) - Priority_Model_Processing:: ")
                    ACE_TEXT (" Priority_Model_Processing\n")));
    }
}

void
TAO_Object_Adapter::Priority_Model_Processing::pre_invoke (
   IOP::ServiceContextList &service_context_list,
   CORBA::Environment &ACE_TRY_ENV)
{
  TAO_POA_Policies &poa_policies = this->poa_.policies ();

  // CLIENT_PROPAGATED PriorityModel processing.
  if (poa_policies.priority_model ()
      == TAO_POA_Policies::CLIENT_PROPAGATED
      && poa_policies.server_priority ()
      != TAO_INVALID_PRIORITY)
    {
      // Remember current thread's priority.
      if (poa_.orb_core ().get_thread_priority (this->original_priority_)
          == -1)
        ACE_THROW (CORBA::DATA_CONVERSION (1,
                                           CORBA::COMPLETED_NO));

      // Attempt to extract client-propagated priority from the
      //  ServiceContextList of the request.
      RTCORBA::Priority target_priority;
      int priority_found = 0;
      for (CORBA::ULong i = 0;
           i < service_context_list.length () && !priority_found;
           ++i)
        {
          IOP::ServiceContext &context = service_context_list[i];

          if (context.context_id == IOP::RTCorbaPriority)
            {
              TAO_InputCDR cdr (ACE_reinterpret_cast
                                (const char*,
                                 context.context_data.get_buffer ()),
                                context.context_data.length ());

              CORBA::Boolean byte_order;
              if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
                ACE_THROW (CORBA::MARSHAL ());
              cdr.reset_byte_order (ACE_static_cast(int,byte_order));

              if ((cdr >> target_priority) == 0)
                ACE_THROW (CORBA::MARSHAL ());

              priority_found = 1;
              break;
            }
        }

      // Use default priority if none came in the request.  (Request
      // must have come from a non-RT ORB.)
      if (!priority_found)
        target_priority = poa_policies.server_priority ();

      // Change the priority of the current thread to the
      // client-propagated value for the duration of
      // request.
      if (original_priority_ != target_priority)
        {
          if (poa_.orb_core ().set_thread_priority (target_priority) == -1)
            ACE_THROW (CORBA::DATA_CONVERSION (1, CORBA::COMPLETED_NO));

          this->state_ = PRIORITY_RESET_REQUIRED;
        }
    }
}

void
TAO_Object_Adapter::Priority_Model_Processing::post_invoke (
    CORBA::Environment &ACE_TRY_ENV)
{
  if (this->state_ == PRIORITY_RESET_REQUIRED)
    {
      this->state_ = NO_ACTION_REQUIRED;

      // Reset the priority of the current thread back to its original
      // value.
      if (poa_.orb_core ().set_thread_priority (this->original_priority_)
          == -1)
        ACE_THROW (CORBA::DATA_CONVERSION (1, CORBA::COMPLETED_NO));
    }
}

#endif /* TAO_HAS_RT_CORBA == 1 */

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
                             const TAO_ObjectKey &key)
{
  // Remember information about this upcall.
  this->poa_ = p;
  this->object_key_ = &key;

  // Set the current context and remember the old one.
  this->tss_resources_ = TAO_TSS_RESOURCES::instance ();

  this->previous_current_impl_ = this->tss_resources_->poa_current_impl_;
  this->tss_resources_->poa_current_impl_ = this;

  // Setup is complete.
  this->setup_done_ = 1;
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
TAO_POA_Current_Impl::get_POA (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::Current::NoContext))
{
  return PortableServer::POA::_duplicate (this->poa_);
}

PortableServer::ObjectId *
TAO_POA_Current_Impl::get_object_id (CORBA::Environment &)
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
TAO_POA_Current::get_POA (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::Current::NoContext))
{
  TAO_POA_Current_Impl *impl = this->implementation ();

  if (impl == 0)
    ACE_THROW_RETURN (PortableServer::Current::NoContext (),
                      0);
  return impl->get_POA (ACE_TRY_ENV);
}

PortableServer::ObjectId *
TAO_POA_Current::get_object_id (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::Current::NoContext))
{
  TAO_POA_Current_Impl *impl = this->implementation ();

  if (impl == 0)
    ACE_THROW_RETURN (PortableServer::Current::NoContext (),
                      0);
  return impl->get_object_id (ACE_TRY_ENV);
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
typedef ACE_Reverse_Lock<ACE_Lock> REVERSE_LOCK;

typedef ACE_Pair<key, value> expanded_value;
typedef ACE_Reference_Pair<const key, value> tao_value_type;
typedef ACE_Equal_To<key> compare_keys;
typedef TAO_ObjectId_Hash hash_key;
typedef ACE_Noop_Key_Generator<key> noop_key_generator;

template class ACE_Reverse_Lock<ACE_Lock>;
template class ACE_Guard<REVERSE_LOCK>;

template class auto_ptr<TAO_Object_Adapter::Hint_Strategy>;
template class auto_ptr<TAO_Object_Adapter::transient_poa_map>;

template class ACE_Auto_Basic_Ptr<TAO_Object_Adapter::Hint_Strategy>;
template class ACE_Auto_Basic_Ptr<TAO_Object_Adapter::transient_poa_map>;

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
typedef ACE_Reverse_Lock<ACE_Lock> REVERSE_LOCK;

typedef ACE_Pair<key, value> expanded_value;
typedef ACE_Reference_Pair<const key, value> tao_value_type;
typedef ACE_Equal_To<key> compare_keys;
typedef TAO_ObjectId_Hash hash_key;
typedef ACE_Noop_Key_Generator<key> noop_key_generator;

#pragma instantiate ACE_Reverse_Lock<ACE_Lock>
#pragma instantiate ACE_Guard<REVERSE_LOCK>

#pragma instantiate auto_ptr<TAO_Object_Adapter::Hint_Strategy>
#pragma instantiate auto_ptr<TAO_Object_Adapter::transient_poa_map>

#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Object_Adapter::Hint_Strategy>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Object_Adapter::transient_poa_map>

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
