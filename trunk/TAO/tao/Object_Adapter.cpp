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
TAO_Object_Adapter::dispatch_servant (const TAO_ObjectKey &key,
                                      CORBA::ServerRequest &req,
                                      void *context,
                                      CORBA::Environment &ACE_TRY_ENV)
{
  ACE_FUNCTION_TIMEPROBE (TAO_OBJECT_ADAPTER_DISPATCH_SERVANT_START);

  // Lock access to the POA for the duration of this transaction
  TAO_POA_GUARD (ACE_Lock, monitor, this->lock (), ACE_TRY_ENV);

  this->dispatch_servant_i (key,
                            req,
                            context,
                            ACE_TRY_ENV);
}

void
TAO_Object_Adapter::dispatch_servant_i (const TAO_ObjectKey &key,
                                        CORBA::ServerRequest &req,
                                        void *context,
                                        CORBA_Environment &ACE_TRY_ENV)
{
  // Check if a non-servant upcall is in progress, and make sure it is
  // not this thread which is making the upcall.
  while (this->enable_locking_ &&
         this->non_servant_upcall_in_progress_ &&
         this->non_servant_upcall_thread_ != ACE_OS::thr_self ())
    {
      // If so wait...
      int result = this->non_servant_upcall_condition_.wait ();
      if (result == -1)
        {
          ACE_THROW (CORBA::OBJ_ADAPTER ());
        }
    }

  PortableServer::ObjectId id;
  TAO_POA *poa = 0;

  this->locate_poa (key,
                    id,
                    poa,
                    ACE_TRY_ENV);
  ACE_CHECK;

  // Setup for POA Current
  const char *operation = req.operation ();
  TAO_POA_Current current_context (poa,
                                   key,
                                   0,
                                   operation,
                                   this->orb_core_);

  PortableServer::Servant servant = 0;

  {
    ACE_FUNCTION_TIMEPROBE (TAO_POA_LOCATE_SERVANT_START);

    servant = poa->locate_servant_i (operation,
                                     id,
                                     &current_context,
                                     ACE_TRY_ENV);
    ACE_CHECK;
  }

  // Now that we know the servant.
  current_context.servant (servant);

  {
    // Outstanding_Requests has a magic constructor and destructor.
    // We increment <POA::outstanding_requests_> in the constructor.
    // We decrement <POA::outstanding_requests_> in the destructor.
    // Note that the lock is released after
    // <POA::outstanding_requests_> is increased and
    // <POA::outstanding_requests_> is decreased after the lock has
    // been reacquired.
    Outstanding_Requests outstanding_requests (*poa,
                                               *this);
    ACE_UNUSED_ARG (outstanding_requests);

    // This class helps us by locking servants in a single threaded
    // POA for the duration of the upcall.  Single_Threaded_POA_Lock
    // has a magic constructor and destructor.  We acquire the servant
    // lock in the constructor.  We release the servant lock in the
    // destructor.
    Single_Threaded_POA_Lock single_threaded_poa_lock (*poa,
                                                       servant);
    ACE_UNUSED_ARG (single_threaded_poa_lock);

    // Unlock for the duration of the servant upcall.  Reacquire once
    // the upcall completes. Even though we are releasing the lock,
    // the servant entry in the active object map is reference counted
    // and will not get removed/deleted prematurely.
    TAO_POA_GUARD (ACE_Lock, monitor, this->reverse_lock (), ACE_TRY_ENV);

    ACE_FUNCTION_TIMEPROBE (TAO_SERVANT_DISPATCH_START);

    // Upcall
    servant->_dispatch (req,
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
  }

  if (result != 0)
    {
      ACE_THROW (CORBA::OBJECT_NOT_EXIST (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_NO));
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
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (ACE_Lock, monitor, this->lock (), 0, ACE_TRY_ENV);

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

TAO_Object_Adapter::Outstanding_Requests::Outstanding_Requests (TAO_POA &poa,
                                                                TAO_Object_Adapter &object_adapter)
  : poa_ (poa),
    object_adapter_ (object_adapter)
{
  // Increase <poa->outstanding_requests_>.
  this->poa_.increment_outstanding_requests ();
}

TAO_Object_Adapter::Outstanding_Requests::~Outstanding_Requests (void)
{
  // Decrease <poa->outstanding_requests_>.
  CORBA::ULong outstanding_requests = this->poa_.decrement_outstanding_requests ();

  // If locking is enabled and some thread is waiting in POA::destroy.
  if (this->object_adapter_.enable_locking_ &&
      outstanding_requests == 0 &&
      this->poa_.destroy_pending_)
    {
      // Wakeup all waiting threads.
      this->poa_.outstanding_requests_condition_.broadcast ();
    }
}

////////////////////////////////////////////////////////////////////////////////

TAO_Object_Adapter::Single_Threaded_POA_Lock::Single_Threaded_POA_Lock (TAO_POA &poa,
                                                                        PortableServer::Servant servant)
  : poa_ (poa),
    servant_ (servant)
{
  if (this->poa_.policies ().thread () == PortableServer::SINGLE_THREAD_MODEL)
    {
      this->servant_->_single_threaded_poa_lock ().acquire ();
    }
}

TAO_Object_Adapter::Single_Threaded_POA_Lock::~Single_Threaded_POA_Lock (void)
{
  if (this->poa_.policies ().thread () == PortableServer::SINGLE_THREAD_MODEL)
    {
      this->servant_->_single_threaded_poa_lock ().release ();
    }
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

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
