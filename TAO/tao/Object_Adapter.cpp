// $Id$

#include "tao/Object_Adapter.h"
#include "tao/POA.h"
#include "tao/ORB.h"
#include "tao/ORB_Core.h"
#include "tao/Server_Request.h"
#include "ace/Auto_Ptr.h"

#if !defined (__ACE_INLINE__)
# include "tao/Object_Adapter.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, POA, "$Id$")

////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////

/* static */
size_t TAO_Object_Adapter::transient_poa_name_size_ (0);

void
TAO_Object_Adapter::set_transient_poa_name_size (const TAO_Server_Strategy_Factory::Active_Object_Map_Creation_Parameters &creation_parameters)
{
  if (TAO_Object_Adapter::transient_poa_name_size_ == 0)
    {
      switch (creation_parameters.poa_lookup_strategy_for_transient_id_policy_)
        {
        case TAO_LINEAR:

          TAO_Object_Adapter::transient_poa_name_size_ = sizeof (CORBA::ULong);
          break;

        case TAO_DYNAMIC_HASH:

          TAO_Object_Adapter::transient_poa_name_size_ = sizeof (CORBA::ULong);
          break;

        case TAO_ACTIVE_DEMUX:
        default:

          TAO_Object_Adapter::transient_poa_name_size_ = ACE_Active_Map_Manager_Key::size ();
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
{
  TAO_Object_Adapter::set_transient_poa_name_size (creation_parameters);

  Hint_Strategy *hint_strategy = 0;
  if (creation_parameters.use_active_hint_in_poa_names_)
    {
      ACE_NEW (hint_strategy,
               Active_Hint_Strategy (creation_parameters.poa_map_size_));
    }
  else
    {
      ACE_NEW (hint_strategy,
               No_Hint_Strategy);
    }
  // Give ownership to the auto pointer.
  auto_ptr<Hint_Strategy> new_hint_strategy (hint_strategy);

  new_hint_strategy->object_adapter (this);

  persistent_poa_name_map *ppnm;
  switch (creation_parameters.poa_lookup_strategy_for_persistent_id_policy_)
    {
    case TAO_LINEAR:

      ACE_NEW (ppnm,
               persistent_poa_name_linear_map (creation_parameters.poa_map_size_));
      break;

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
    case TAO_LINEAR:

      ACE_NEW (tpm,
               transient_poa_linear_map (creation_parameters.poa_map_size_));
      break;

    case TAO_DYNAMIC_HASH:

      ACE_NEW (tpm,
               transient_poa_hash_map (creation_parameters.poa_map_size_));
      break;

    case TAO_ACTIVE_DEMUX:
    default:

      ACE_NEW (tpm,
               transient_poa_active_map (creation_parameters.poa_map_size_));
      break;
    }
  // Give ownership to the auto pointer.
  auto_ptr<transient_poa_map> new_transient_poa_map (tpm);

  this->hint_strategy_ = new_hint_strategy.release ();
  this->persistent_poa_name_map_ = new_persistent_poa_name_map.release ();
  this->transient_poa_map_ = new_transient_poa_map.release ();
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
      ACE_Lock *the_lock = 0;

      ACE_NEW_RETURN (the_lock,
                      ACE_Lock_Adapter<ACE_SYNCH_MUTEX> (thread_lock),
                      0);

      return the_lock;
    }
#endif /* ACE_HAS_THREADS */

  ACE_Lock *the_lock = 0;

  ACE_NEW_RETURN (the_lock,
                  ACE_Lock_Adapter<ACE_SYNCH_NULL_MUTEX> (),
                  0);

  return the_lock;
}


void
TAO_Object_Adapter::deactivate_i (CORBA::Boolean wait_for_completion,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  // If the ORB::shutdown operation is called, it makes a call on
  // deactivate with a TRUE etherealize_objects parameter for each POA
  // manager known in the process; the wait_for_completion parameter
  // to deactivate will be the same as the similarly named parameter
  // of ORB::shutdown.

  poa_manager_set::iterator end = this->poa_manager_set_.end ();

  for (poa_manager_set::iterator iterator = this->poa_manager_set_.begin ();
       iterator != end;
       ++iterator)
    {
      TAO_POA_Manager *poa_manager = *iterator;
      poa_manager->deactivate_i (1,
                                 wait_for_completion,
                                 ACE_TRY_ENV);
      ACE_CHECK;
    }
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
  Servant_Upcall servant_upcall (*this);

  // Set up state in the POA et al (including the POA Current), so
  // that we know that this servant is currently in an upcall.
  const char *operation = req.operation ();
  servant_upcall.prepare_for_upcall (key,
                                     operation,
                                     ACE_TRY_ENV);
  ACE_CHECK;

  // Servant dispatch.
  {
    ACE_FUNCTION_TIMEPROBE (TAO_SERVANT_DISPATCH_START);

    servant_upcall.servant ()->_dispatch (req,
                                          context,
                                          ACE_TRY_ENV);
    ACE_CHECK;
  }
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
    {
      ACE_THROW (CORBA::OBJ_ADAPTER ());
    }

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
    {
      ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
    }
}

int
TAO_Object_Adapter::activate_poa (const poa_name &folded_name,
                                  TAO_POA *&poa,
                                  CORBA_Environment &ACE_TRY_ENV)
{
  int result = -1;

#if !defined (TAO_HAS_MINIMUM_CORBA)

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

  TAO_POA *parent = this->orb_core_.root_poa ();
  if (parent->name () != *iterator)
    {
      ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                        -1);
    }
  else
    {
      ++iterator;
    }

  for (;
       iterator != end;
       ++iterator)
    {
      TAO_POA *current = parent->find_POA_i (*iterator,
                                             1,
                                             ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);

      parent = current;
    }

  poa = parent;
  result = 0;

#endif /* TAO_HAS_MINIMUM_CORBA */

  return result;
}

int
TAO_Object_Adapter::find_transient_poa (const poa_name &system_name,
                                        CORBA::Boolean root,
                                        const TAO_Temporary_Creation_Time &poa_creation_time,
                                        TAO_POA *&poa)
{
  int result = 0;

  if (root)
    poa = this->orb_core_.root_poa ();
  else
    result = this->transient_poa_map_->find (system_name,
                                             poa);
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
    {
      return this->bind_persistent_poa (folded_name,
                                        poa,
                                        system_name);
    }
  else
    {
      return this->bind_transient_poa (poa,
                                       system_name);
    }
}

int
TAO_Object_Adapter::unbind_poa (TAO_POA *poa,
                                const poa_name &folded_name,
                                const poa_name &system_name)
{
  if (poa->persistent ())
    {
      return this->unbind_persistent_poa (folded_name,
                                          system_name);
    }
  else
    {
      return this->unbind_transient_poa (system_name);
    }
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
  TAO_POA::LOCATION_RESULT result = poa->locate_servant_i (id,
                                                           servant,
                                                           ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  switch (result)
    {
    case TAO_POA::FOUND:
      // Optimistic attitude
    case TAO_POA::DEFAULT_SERVANT:
    case TAO_POA::SERVANT_MANAGER:
      return 0;

    case TAO_POA::NOT_FOUND:
      return -1;
    }

  return -1;
}

PortableServer::Servant
TAO_Object_Adapter::find_servant_i (const TAO_ObjectKey &key,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  PortableServer::ObjectId id;
  TAO_POA *poa = 0;

  this->locate_poa (key,
                    id,
                    poa,
                    ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  PortableServer::Servant servant = 0;
  TAO_POA::LOCATION_RESULT result = poa->locate_servant_i (id,
                                                           servant,
                                                           ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  switch (result)
    {
    case TAO_POA::FOUND:
      return servant;

    case TAO_POA::DEFAULT_SERVANT:
    case TAO_POA::SERVANT_MANAGER:
    case TAO_POA::NOT_FOUND:
      return 0;
    }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////

TAO_Object_Adapter::Hint_Strategy::~Hint_Strategy (void)
{
}

void
TAO_Object_Adapter::Hint_Strategy::object_adapter (TAO_Object_Adapter *oa)
{
  this->object_adapter_ = oa;
}

////////////////////////////////////////////////////////////////////////////////

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
      if (result != 0 ||
          folded_name != poa->folded_name ())
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
      result = this->object_adapter_->persistent_poa_name_map_->bind (folded_name,
                                                                      poa);

      if (result != 0)
        {
          this->persistent_poa_system_map_.unbind (name);
        }
      else
        {
          ACE_NEW_RETURN (system_name,
                          poa_name (name),
                          -1);
        }
    }

  return result;
}

int
TAO_Object_Adapter::Active_Hint_Strategy::unbind_persistent_poa (const poa_name &folded_name,
                                                                 const poa_name &system_name)
{
  int result = this->persistent_poa_system_map_.unbind (system_name);

  if (result == 0)
    {
      result = this->object_adapter_->persistent_poa_name_map_->unbind (folded_name);
    }

  return result;
}

////////////////////////////////////////////////////////////////////////////////

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
      result = this->object_adapter_->activate_poa (system_name,
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
  int result = this->object_adapter_->persistent_poa_name_map_->bind (folded_name,
                                                                      poa);
  if (result == 0)
    {
      ACE_NEW_RETURN (system_name,
                      poa_name (folded_name),
                      -1);
    }

  return result;
}

int
TAO_Object_Adapter::No_Hint_Strategy::unbind_persistent_poa (const poa_name &folded_name,
                                                             const poa_name &system_name)
{
  ACE_UNUSED_ARG (system_name);

  return this->object_adapter_->persistent_poa_name_map_->unbind (folded_name);
}

////////////////////////////////////////////////////////////////////////////////

TAO_Object_Adapter::poa_name_iterator::poa_name_iterator (int begin,
                                                          CORBA::ULong size,
                                                          const CORBA::Octet *folded_buffer)
  : size_ (size),
    folded_buffer_ (folded_buffer),
    last_separator_ (~0)
{
  if (begin)
    {
      this->position_ = ~0;
      this->operator++ ();
    }
  else
    {
      this->position_ = this->size_;
    }
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
    this->position_ -
    this->last_separator_ -
    TAO_POA::name_separator_length ();

  return ACE_CString (ACE_reinterpret_cast (const char *,
                                            &this->folded_buffer_[start_at]),
                      how_many);
}

TAO_Object_Adapter::poa_name_iterator &
TAO_Object_Adapter::poa_name_iterator::operator++ (void)
{
  this->last_separator_ = this->position_;

  while (1)
    {
      ++this->position_;
      if (this->position_ < this->size_)
        {
          if (this->folded_buffer_[this->position_] == TAO_POA::name_separator ())
            {
              break;
            }
        }
      else
        {
          break;
        }
    }

  return *this;
}

////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////

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
  this->object_adapter_.non_servant_upcall_thread_ = ACE_OS::NULL_thread;

  // If locking is enabled.
  if (this->object_adapter_.enable_locking_)
    {
      // Wakeup all waiting threads.
      this->object_adapter_.non_servant_upcall_condition_.broadcast ();
    }
}

////////////////////////////////////////////////////////////////////////////////

TAO_Object_Adapter::Servant_Upcall::Servant_Upcall (TAO_Object_Adapter &object_adapter)
  : object_adapter_ (object_adapter),
    poa_ (0),
    servant_ (0),
    state_ (INITIAL_STAGE),
    id_ (),
    current_context_ ()
{
}

void
TAO_Object_Adapter::Servant_Upcall::prepare_for_upcall (const TAO_ObjectKey &key,
                                                        const char *operation,
                                                        CORBA::Environment &ACE_TRY_ENV)
{
  // Acquire the object adapter lock first.
  int result = this->object_adapter_.lock ().acquire ();
  if (result == -1)
    {
      // Locking error.
      ACE_THROW (CORBA::OBJ_ADAPTER ());
    }

  // We have acquired the object adapater lock.  Record this for later
  // use.
  this->state_ = OBJECT_ADAPTER_LOCK_ACQUIRED;

  // Check if a non-servant upcall is in progress.  If a non-servant
  // upcall is in progress, wait for it to complete.  Unless of
  // course, the thread making the non-servant upcall is this thread.
  while (this->object_adapter_.enable_locking_ &&
         this->object_adapter_.non_servant_upcall_in_progress_ &&
         ! ACE_OS::thr_equal (this->object_adapter_.non_servant_upcall_thread_,
                              ACE_OS::thr_self ()))
    {
      // If so wait...
      result = this->object_adapter_.non_servant_upcall_condition_.wait ();
      if (result == -1)
        {
          ACE_THROW (CORBA::OBJ_ADAPTER ());
        }
    }

  // Locate the POA.
  this->object_adapter_.locate_poa (key,
                                    this->id_,
                                    this->poa_,
                                    ACE_TRY_ENV);
  ACE_CHECK;

  // Check the state of the POA Manager.
  this->poa_->check_poa_manager_state (ACE_TRY_ENV);
  ACE_CHECK;

  // Setup current for this request.
  this->current_context_.setup (this->poa_,
                                key,
                                0,
                                operation);

  {
    ACE_FUNCTION_TIMEPROBE (TAO_POA_LOCATE_SERVANT_START);

    // Lookup the servant.
    this->servant_ = this->poa_->locate_servant_i (operation,
                                                   this->id_,
                                                   &this->current_context_,
                                                   ACE_TRY_ENV);
    ACE_CHECK;
  }

  // Now that we know the servant.
  this->current_context_.servant (this->servant_);

  // Increase <poa->outstanding_requests_> for the duration of the
  // upcall.
  //
  // Note that the object adapter lock is released after
  // <POA::outstanding_requests_> is increased.
  this->poa_->increment_outstanding_requests ();

  // Release the object adapter lock.
  this->object_adapter_.lock ().release ();

  // We have release the object adapater lock.  Record this for later
  // use.
  this->state_ = OBJECT_ADAPTER_LOCK_RELEASED;

#if !defined (TAO_HAS_MINIMUM_CORBA)

  // Lock servant (if necessary).
  //
  // Note that this lock must be acquired *after* the object adapter
  // lock has been released.  This is necessary since we cannot block
  // waiting for the servant lock while holding the object adapter
  // lock.  Otherwise, the thread that wants to release this lock will
  // not be able to do so since it can't acquire the object adapterx
  // lock.
  //
  if (this->poa_->policies ().thread () == PortableServer::SINGLE_THREAD_MODEL)
    {
      result = this->servant_->_single_threaded_poa_lock ().acquire ();

      if (result == -1)
        {
          // Locking error.
          ACE_THROW (CORBA::OBJ_ADAPTER ());
        }
    }

#endif /* TAO_HAS_MINIMUM_CORBA */

  // We have acquired the servant lock.  Record this for later use.
  this->state_ = SERVANT_LOCK_ACQUIRED;

  //
  // After this point, <this->servant_> is ready for dispatching.
  //
}

TAO_Object_Adapter::Servant_Upcall::~Servant_Upcall ()
{
  switch (this->state_)
    {
    case SERVANT_LOCK_ACQUIRED:

      // Since the servant lock was acquired, we must release it.
#if !defined (TAO_HAS_MINIMUM_CORBA)
      if (this->poa_->policies ().thread () == PortableServer::SINGLE_THREAD_MODEL)
        {
          this->servant_->_single_threaded_poa_lock ().release ();
        }
#endif /* TAO_HAS_MINIMUM_CORBA */

      /** Fall through **/

    case OBJECT_ADAPTER_LOCK_RELEASED:

      // Since the object adapter lock was released, we must acquire
      // it.
      //
      // Note that errors are ignored here since we cannot do much
      // with it.
      this->object_adapter_.lock ().acquire ();

      {
        // Decrease <poa->outstanding_requests_> now that the upcall
        // is complete.
        //
        // Note that the object adapter lock is acquired before
        // <POA::outstanding_requests_> is increased.
        CORBA::ULong outstanding_requests =
          this->poa_->decrement_outstanding_requests ();

        // If locking is enabled and some thread is waiting in POA::destroy.
        if (this->object_adapter_.enable_locking_ &&
            outstanding_requests == 0 &&
            this->poa_->wait_for_completion_pending_)
          {
            // Wakeup all waiting threads.
            this->poa_->outstanding_requests_condition_.broadcast ();
          }
      }

      /** Fall through **/

    case OBJECT_ADAPTER_LOCK_ACQUIRED:

      // Finally, since the object adapter lock was acquired, we must
      // release it.
      this->object_adapter_.lock ().release ();
    }
}

////////////////////////////////////////////////////////////////////////////////

TAO_POA_Current_Impl::TAO_POA_Current_Impl (void)
  : poa_impl_ (0),
    object_id_ (),
    object_key_ (0),

#if !defined (TAO_HAS_MINIMUM_CORBA)

    cookie_ (0),

#endif /* TAO_HAS_MINIMUM_CORBA */

    servant_ (0),
    operation_ (0),
    previous_current_impl_ (0),
    active_object_map_entry_ (0),
    setup_done_ (0)
{
}

void
TAO_POA_Current_Impl::setup (TAO_POA *impl,
                             const TAO_ObjectKey &key,
                             PortableServer::Servant servant,
                             const char *operation)
{
  // Remember information about this upcall.
  this->poa_impl_ = impl;
  this->object_key_ = &key;
  this->servant_ = servant;
  this->operation_ = operation;

  // Set the current context and remember the old one.
  TAO_ORB_Core_TSS_Resources *tss =
    TAO_ORB_CORE_TSS_RESOURCES::instance ();

  this->previous_current_impl_ = tss->poa_current_impl_;
  tss->poa_current_impl_ = this;

  // Setup is complete.
  this->setup_done_ = 1;
}

TAO_POA_Current_Impl::~TAO_POA_Current_Impl (void)
{
#if !defined (TAO_HAS_MINIMUM_CORBA)

  if (this->cookie_ != 0)
    {
      // A recursive thread lock without using a recursive thread
      // lock.  Non_Servant_Upcall has a magic constructor and
      // destructor.  We unlock the Object_Adapter lock for the
      // duration of the servant activator upcalls; reacquiring once
      // the upcalls complete.  Even though we are releasing the lock,
      // other threads will not be able to make progress since
      // <Object_Adapter::non_servant_upcall_in_progress_> has been
      // set.
      TAO_Object_Adapter::Non_Servant_Upcall non_servant_upcall (*this->poa_impl_->orb_core_.object_adapter ());
      ACE_UNUSED_ARG (non_servant_upcall);

      ACE_DECLARE_NEW_CORBA_ENV;
      ACE_TRY_EX (LOCATOR)
        {
          PortableServer::POA_var poa = this->get_POA (ACE_TRY_ENV);
          ACE_TRY_CHECK_EX (LOCATOR);

          this->poa_impl_->servant_locator_->postinvoke (this->object_id (),
                                                         poa.in (),
                                                         this->operation_,
                                                         this->cookie_,
                                                         this->servant_,
                                                         ACE_TRY_ENV);
          ACE_TRY_CHECK_EX (LOCATOR);
        }
      ACE_CATCHANY
        {
          // Ignore errors from servant locator ....
        }
      ACE_ENDTRY;
    }

#endif /* TAO_HAS_MINIMUM_CORBA */

  // Cleanup servant related stuff.
  if (this->active_object_map_entry_ != 0)
    {
      // Decrement the reference count.
      CORBA::UShort new_count = --this->active_object_map_entry_->reference_count_;

      if (new_count == 0)
        {
          ACE_DECLARE_NEW_CORBA_ENV;
          ACE_TRY_EX (SERVANT)
            {
              this->poa_impl_->cleanup_servant (this->active_object_map_entry_,
                                                ACE_TRY_ENV);

              ACE_TRY_CHECK_EX (SERVANT);
            }
          ACE_CATCHANY
            {
              // Ignore errors from servant cleanup ....
            }
          ACE_ENDTRY;
        }
    }

  if (this->setup_done_)
    {
      TAO_ORB_Core_TSS_Resources *tss =
        TAO_ORB_CORE_TSS_RESOURCES::instance ();

      // Reset the old context.
      tss->poa_current_impl_ = this->previous_current_impl_;
    }
}

PortableServer::POA_ptr
TAO_POA_Current_Impl::get_POA (CORBA::Environment &ACE_TRY_ENV)
{
  PortableServer::POA_var result = this->poa_impl_->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (PortableServer::POA::_nil ());

  return result._retn ();
}

PortableServer::ObjectId *
TAO_POA_Current_Impl::get_object_id (CORBA::Environment &)
{
  // Create a new one and pass it back
  return new PortableServer::ObjectId (this->object_id_);
}

TAO_ORB_Core &
TAO_POA_Current_Impl::orb_core (void) const
{
  return this->poa_impl_->orb_core_;
}

////////////////////////////////////////////////////////////////////////////////

PortableServer::POA_ptr
TAO_POA_Current::get_POA (CORBA::Environment &ACE_TRY_ENV)
{
  TAO_POA_Current_Impl *impl = this->implementation ();

  if (impl == 0)
    {
      ACE_THROW_RETURN (PortableServer::Current::NoContext (),
                        0);
    }

  return impl->get_POA (ACE_TRY_ENV);
}

PortableServer::ObjectId *
TAO_POA_Current::get_object_id (CORBA::Environment &ACE_TRY_ENV)
{
  TAO_POA_Current_Impl *impl = this->implementation ();

  if (impl == 0)
    {
      ACE_THROW_RETURN (PortableServer::Current::NoContext (),
                        0);
    }

  return impl->get_object_id (ACE_TRY_ENV);
}

TAO_POA_Current_Impl *
TAO_POA_Current::implementation (void)
{
  return TAO_ORB_CORE_TSS_RESOURCES::instance ()->poa_current_impl_;
}

TAO_POA_Current_Impl *
TAO_POA_Current::implementation (TAO_POA_Current_Impl *new_current)
{
  TAO_ORB_Core_TSS_Resources *tss =
    TAO_ORB_CORE_TSS_RESOURCES::instance ();

  TAO_POA_Current_Impl *old = tss->poa_current_impl_;
  tss->poa_current_impl_ = new_current;
  return old;
}

////////////////////////////////////////////////////////////////////////////////

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

// Common typedefs.
typedef TAO_Object_Adapter::poa_name key;
typedef TAO_POA *value;
typedef ACE_Reverse_Lock<ACE_Lock> REVERSE_LOCK;

typedef ACE_Pair<key, value> expanded_value;
typedef ACE_Reference_Pair<const key, value> value_type;
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
template class ACE_Iterator<value_type>;
template class ACE_Reverse_Iterator<value_type>;

// Iterator base classes.
template class ACE_Iterator_Impl<value_type>;
template class ACE_Reverse_Iterator_Impl<value_type>;

// Active Map Manager related.
template class ACE_Pair<key, value>;
template class ACE_Active_Map_Manager_Adapter<key, value, TAO_Ignore_Original_Key_Adapter>;
template class ACE_Active_Map_Manager_Adapter<key, value, TAO_Preserve_Original_Key_Adapter>;
template class ACE_Active_Map_Manager_Iterator_Adapter<value_type, expanded_value>;
template class ACE_Active_Map_Manager_Reverse_Iterator_Adapter<value_type, expanded_value>;
template class ACE_Active_Map_Manager<expanded_value>;
template class ACE_Map_Manager<ACE_Active_Map_Manager_Key, expanded_value, ACE_Null_Mutex>;
template class ACE_Map_Iterator_Base<ACE_Active_Map_Manager_Key, expanded_value, ACE_Null_Mutex>;
template class ACE_Map_Iterator<ACE_Active_Map_Manager_Key, expanded_value, ACE_Null_Mutex>;
template class ACE_Map_Reverse_Iterator<ACE_Active_Map_Manager_Key, expanded_value, ACE_Null_Mutex>;
template class ACE_Map_Entry<ACE_Active_Map_Manager_Key, expanded_value>;

// Hash Map Manager related.
template class ACE_Hash_Map_Manager_Ex_Adapter<key, value, hash_key, compare_keys, TAO_Incremental_Key_Generator>;
template class ACE_Hash_Map_Manager_Ex_Adapter<key, value, hash_key, compare_keys, noop_key_generator>;
template class ACE_Hash_Map_Manager_Ex_Iterator_Adapter<value_type, key, value, hash_key, compare_keys>;
template class ACE_Hash_Map_Manager_Ex_Reverse_Iterator_Adapter<value_type, key, value, hash_key, compare_keys>;
template class ACE_Hash_Map_Manager_Ex<key, value, hash_key, compare_keys, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<key, value, hash_key, compare_keys, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<key, value, hash_key, compare_keys, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<key, value, hash_key, compare_keys, ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry<key, value>;

// Already in Active_Object_Map.cpp
// template class ACE_Equal_To<key>;

// Map Manager related.
template class ACE_Map_Manager_Iterator_Adapter<value_type, key, value>;
template class ACE_Map_Manager_Reverse_Iterator_Adapter<value_type, key, value>;
template class ACE_Map_Manager_Adapter<key, value, TAO_Incremental_Key_Generator>;
template class ACE_Map_Manager_Adapter<key, value, noop_key_generator>;
template class ACE_Map_Manager<key, value, ACE_Null_Mutex>;
template class ACE_Map_Iterator_Base<key, value, ACE_Null_Mutex>;
template class ACE_Map_Iterator<key, value, ACE_Null_Mutex>;
template class ACE_Map_Reverse_Iterator<key, value, ACE_Null_Mutex>;
template class ACE_Map_Entry<key, value>;

template class ACE_Unbounded_Set<TAO_POA_Manager *>;
template class ACE_Node<TAO_POA_Manager *>;
template class ACE_Unbounded_Set_Iterator<TAO_POA_Manager *>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

// Common typedefs.
typedef TAO_Object_Adapter::poa_name key;
typedef TAO_POA *value;
typedef ACE_Reverse_Lock<ACE_Lock> REVERSE_LOCK;

typedef ACE_Pair<key, value> expanded_value;
typedef ACE_Reference_Pair<const key, value> value_type;
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
#pragma instantiate ACE_Iterator<value_type>
#pragma instantiate ACE_Reverse_Iterator<value_type>

// Iterator base classes.
#pragma instantiate ACE_Iterator_Impl<value_type>
#pragma instantiate ACE_Reverse_Iterator_Impl<value_type>

// Active Map Manager related.
#pragma instantiate ACE_Pair<key, value>
#pragma instantiate ACE_Active_Map_Manager_Adapter<key, value, TAO_Ignore_Original_Key_Adapter>
#pragma instantiate ACE_Active_Map_Manager_Adapter<key, value, TAO_Preserve_Original_Key_Adapter>
#pragma instantiate ACE_Active_Map_Manager_Iterator_Adapter<value_type, expanded_value>
#pragma instantiate ACE_Active_Map_Manager_Reverse_Iterator_Adapter<value_type, expanded_value>
#pragma instantiate ACE_Active_Map_Manager<expanded_value>
#pragma instantiate ACE_Map_Manager<ACE_Active_Map_Manager_Key, expanded_value, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator_Base<ACE_Active_Map_Manager_Key, expanded_value, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator<ACE_Active_Map_Manager_Key, expanded_value, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Reverse_Iterator<ACE_Active_Map_Manager_Key, expanded_value, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Entry<ACE_Active_Map_Manager_Key, expanded_value>

// Hash Map Manager related.
#pragma instantiate ACE_Hash_Map_Manager_Ex_Adapter<key, value, hash_key, compare_keys, TAO_Incremental_Key_Generator>
#pragma instantiate ACE_Hash_Map_Manager_Ex_Adapter<key, value, hash_key, compare_keys, noop_key_generator>
#pragma instantiate ACE_Hash_Map_Manager_Ex_Iterator_Adapter<value_type, key, value, hash_key, compare_keys>
#pragma instantiate ACE_Hash_Map_Manager_Ex_Reverse_Iterator_Adapter<value_type, key, value, hash_key, compare_keys>
#pragma instantiate ACE_Hash_Map_Manager_Ex<key, value, hash_key, compare_keys, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<key, value, hash_key, compare_keys, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<key, value, hash_key, compare_keys, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<key, value, hash_key, compare_keys, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Entry<key, value>

// Already in Active_Object_Map.cpp
// #pragma instantiate ACE_Equal_To<key>

// Map Manager related.
#pragma instantiate ACE_Map_Manager_Iterator_Adapter<value_type, key, value>
#pragma instantiate ACE_Map_Manager_Reverse_Iterator_Adapter<value_type, key, value>
#pragma instantiate ACE_Map_Manager_Adapter<key, value, TAO_Incremental_Key_Generator>
#pragma instantiate ACE_Map_Manager_Adapter<key, value, noop_key_generator>
#pragma instantiate ACE_Map_Manager<key, value, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator_Base<key, value, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator<key, value, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Reverse_Iterator<key, value, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Entry<key, value>

#pragma instantiate ACE_Unbounded_Set<TAO_POA_Manager *>
#pragma instantiate ACE_Node<TAO_POA_Manager *>
#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_POA_Manager *>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
