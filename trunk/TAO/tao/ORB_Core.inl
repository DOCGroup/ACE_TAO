// -*- C++ -*-
//
// $Id$

#include "tao/ORB_Core_TSS_Resources.h"
#include "tao/ORB_Table.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE ACE_Service_Gestalt*
TAO_ORB_Core::configuration (void) const
{
  return this->config_.get ();
}

ACE_INLINE unsigned long
TAO_ORB_Core::_incr_refcnt (void)
{
  return this->refcount_++;
}

ACE_INLINE unsigned long
TAO_ORB_Core::_refcnt (void) const
{
  return this->refcount_.value ();
}

ACE_INLINE unsigned long
TAO_ORB_Core::_decr_refcnt (void)
{
  unsigned long const count = --this->refcount_;
  if (count != 0)
    return count;

  this->fini ();
  return 0;
}

ACE_INLINE ACE_Lock *
TAO_ORB_Core::locking_strategy (void)
{
  if (this->resource_factory ()->use_locked_data_blocks ())
    return &this->data_block_lock_;

  return 0;
}

ACE_INLINE CORBA::Boolean
TAO_ORB_Core::bidir_giop_policy (void)
{
  return this->bidir_giop_policy_;
}

ACE_INLINE void
TAO_ORB_Core::bidir_giop_policy (CORBA::Boolean val)
{
  this->bidir_giop_policy_ = val;
}

ACE_INLINE TAO_Object_Ref_Table &
TAO_ORB_Core::object_ref_table (void)
{
  return this->object_ref_table_;
}

ACE_INLINE TAO::ObjectKey_Table &
TAO_ORB_Core::object_key_table (void)
{
  return this->object_key_table_;
}

ACE_INLINE TAO_Flushing_Strategy *
TAO_ORB_Core::flushing_strategy (void)
{
  return this->flushing_strategy_;
}

ACE_INLINE TAO_Protocols_Hooks *
TAO_ORB_Core::get_protocols_hooks (void)
{
  return this->protocols_hooks_;
}

ACE_INLINE TAO_Network_Priority_Protocols_Hooks *
TAO_ORB_Core::get_network_priority_protocols_hooks (void)
{
  return this->network_priority_protocols_hooks_;
}

ACE_INLINE CORBA::Boolean
TAO_ORB_Core::service_profile_selection (const TAO_MProfile &mprofile,
                                         TAO_Profile  *&profile)
{
  CORBA::Boolean retval = false;
  // @@ If different services have the same feature we may want to
  // prioritise them here. We need to decide here whose selection of
  // profile is more important.
  if (this->ft_service_.service_callback ())
    {
      retval =
        this->ft_service_.service_callback ()->select_profile (mprofile,
                                                                profile);
    }
  return retval;
}

ACE_INLINE CORBA::Boolean
TAO_ORB_Core::object_is_nil (CORBA::Object_ptr obj)
{
  CORBA::Boolean retval = false;
  if (this->ft_service_.service_callback ())
    {
      retval = this->ft_service_.service_callback ()->object_is_nil (obj);
    }
  return retval;
}


ACE_INLINE TAO_Service_Callbacks::Profile_Equivalence
TAO_ORB_Core::is_profile_equivalent (const TAO_Profile *this_p,
                                     const TAO_Profile *that_p)
{
  TAO_Service_Callbacks::Profile_Equivalence retval
    = TAO_Service_Callbacks::DONT_KNOW;

  if (this->ft_service_.service_callback ())
    {
      retval =
        this->ft_service_.service_callback ()->is_profile_equivalent (this_p,
                                                                      that_p);
    }

  return retval;
}

ACE_INLINE CORBA::ULong
TAO_ORB_Core::hash_service (TAO_Profile *p, CORBA::ULong m)
{
  if (this->ft_service_.service_callback ())
    {
      return this->ft_service_.service_callback ()->hash_ft (p, m);
    }

  return 0;
}

ACE_INLINE TAO_Fault_Tolerance_Service &
TAO_ORB_Core::fault_tolerance_service (void)
{
  return this->ft_service_;
}

ACE_INLINE CORBA::Boolean
TAO_ORB_Core::ft_send_extended_sc (void)
{
  return this->ft_send_extended_sc_;
}

ACE_INLINE ACE_Thread_Manager *
TAO_ORB_Core::thr_mgr (void)
{
  return &this->tm_;
}

ACE_INLINE CORBA::ORB_ptr
TAO_ORB_Core::orb (void)
{
  return this->orb_;
}

ACE_INLINE TAO_Adapter_Registry &
TAO_ORB_Core::adapter_registry (void)
{
  return this->adapter_registry_;
}

ACE_INLINE TAO_Request_Dispatcher *
TAO_ORB_Core::request_dispatcher (void)
{
  return this->request_dispatcher_;
}

ACE_INLINE TAO_ORB_Core::InitRefMap *
TAO_ORB_Core::init_ref_map (void)
{
  return &this->init_ref_map_;
}

ACE_INLINE void
TAO_ORB_Core::set_default (const char * orb_id)
{
  TAO::ORB_Table * const table = TAO::ORB_Table::instance ();
  table->set_default (orb_id);
}

ACE_INLINE void
TAO_ORB_Core::not_default (const char * orb_id)
{
  TAO::ORB_Table * const table = TAO::ORB_Table::instance ();
  table->not_default (orb_id);
}

ACE_INLINE void
TAO_ORB_Core::optimize_collocation_objects (CORBA::Boolean opt)
{
  this->opt_for_collocation_ = opt;
}

ACE_INLINE CORBA::Boolean
TAO_ORB_Core::optimize_collocation_objects (void) const
{
  return this->opt_for_collocation_;
}

ACE_INLINE void
TAO_ORB_Core::use_global_collocation (CORBA::Boolean opt)
{
  this->use_global_collocation_ = opt;
}

ACE_INLINE CORBA::Boolean
TAO_ORB_Core::use_global_collocation (void) const
{
  return this->use_global_collocation_;
}

ACE_INLINE CORBA::ULong
TAO_ORB_Core::get_collocation_strategy (void) const
{
  return this->collocation_strategy_;
}

ACE_INLINE TAO_ORB_Parameters *
TAO_ORB_Core::orb_params(void)
{
  return &(this->orb_params_);
}

#define TAO_OC_RETRIEVE(member) \
((this->member##_ == 0) \
  ? (this->member##_ = this->resource_factory ()->get_##member ()) \
  : (this->member##_))

ACE_INLINE TAO_ProtocolFactorySet *
TAO_ORB_Core::protocol_factories (void)
{
  return TAO_OC_RETRIEVE (protocol_factories);
}

ACE_INLINE TAO_Parser_Registry *
TAO_ORB_Core::parser_registry (void)
{
  return &this->parser_registry_;
}

#undef TAO_OC_RETRIEVE

#if (TAO_HAS_CORBA_MESSAGING == 1)

ACE_INLINE TAO_Policy_Manager *
TAO_ORB_Core::policy_manager (void)
{
  return this->policy_manager_;
}

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

ACE_INLINE TAO_ORB_Core_TSS_Resources*
TAO_ORB_Core::get_tss_resources (void)
{
  return ACE_TSS_GET (&this->tss_resources_,TAO_ORB_Core_TSS_Resources);
}

ACE_INLINE void *
TAO_ORB_Core::get_tss_resource (size_t slot_id)
{
  TAO_ORB_Core_TSS_Resources *tss_resources =
    this->get_tss_resources ();

  if (slot_id >= tss_resources->ts_objects_.size ())
    return 0;

  return tss_resources->ts_objects_[slot_id];
}

ACE_INLINE int
TAO_ORB_Core::set_tss_resource (size_t slot_id, void *ts_object)
{
  TAO_ORB_Core_TSS_Resources *tss_resources =
    this->get_tss_resources ();

  // The number of allocated slots is equal to the number of
  // registered TSS cleanup functions, *not* the size of the array in
  // the ORB core TSS resources.
  if (slot_id >= this->tss_cleanup_funcs_.size ())
    {
      errno = EINVAL;
      return -1;
    }

  // If the TSS array isn't large enough, then increase its size.
  // We're guaranteed not to exceed the number of allocated slots by
  // the above check.
  const size_t old_size = tss_resources->ts_objects_.size ();
  const size_t new_size = slot_id + 1;
  if (slot_id >= old_size
      && tss_resources->ts_objects_.size (new_size) != 0)
    return -1;

  // Initialize intermediate array elements to zero, since they
  // haven't been initialized yet.  This ensures that garbage is not
  // returned when accessing any of those elements at a later point in
  // time.
  for (size_t i = old_size; i < slot_id; ++i)
    tss_resources->ts_objects_[i] = 0;

  tss_resources->ts_objects_[slot_id] = ts_object;

  // Make sure the ORB core pointer is set in the ORB core's TSS
  // resources so that the TSS cleanup functions stored in the ORB
  // core can be invoked.
  tss_resources->orb_core_ = this;

  return 0;
}

ACE_INLINE void
TAO_ORB_Core::tss_cleanup (ACE_Array_Base<void *> &ts_objects)
{
  this->tss_cleanup_funcs_.cleanup (ts_objects);
}

ACE_INLINE bool
TAO_ORB_Core::has_shutdown (void) const
{
  return this->has_shutdown_;
}

ACE_INLINE int
TAO_ORB_Core::thread_per_connection_timeout (ACE_Time_Value &timeout) const
{
  timeout = this->thread_per_connection_timeout_;
  return this->thread_per_connection_use_timeout_;
}

ACE_INLINE const char *
TAO_ORB_Core::orbid (void) const
{
  return this->orbid_;
}

ACE_INLINE void
TAO_ORB_Core::implrepo_service (const CORBA::Object_ptr ir)
{
  this->implrepo_service_ = ir;
}

ACE_INLINE CORBA::Boolean
TAO_ORB_Core::use_implrepo (void)
{
  return use_implrepo_;
}

ACE_INLINE CORBA::Boolean
TAO_ORB_Core::imr_endpoints_in_ior (void)
{
  return imr_endpoints_in_ior_;
}

ACE_INLINE CORBA::Object_ptr
TAO_ORB_Core::resolve_typecodefactory (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->lock_,
                    CORBA::Object::_nil ());
  if (CORBA::is_nil (this->typecode_factory_))
    {
      this->resolve_typecodefactory_i ();
    }
  return CORBA::Object::_duplicate (this->typecode_factory_);
}

#if TAO_HAS_INTERCEPTORS == 1

ACE_INLINE CORBA::Object_ptr
TAO_ORB_Core::resolve_picurrent (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->lock_,
                    CORBA::Object::_nil ());
  if (CORBA::is_nil (this->pi_current_))
    {
      this->resolve_picurrent_i ();
    }
  return CORBA::Object::_duplicate (this->pi_current_);
}

#endif  /* TAO_HAS_INTERCEPTORS == 1 */

ACE_INLINE CORBA::Object_ptr
TAO_ORB_Core::resolve_codecfactory (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->lock_,
                    CORBA::Object::_nil ());
  if (CORBA::is_nil (this->codec_factory_))
    {
      this->resolve_codecfactory_i ();
    }
  return CORBA::Object::_duplicate (this->codec_factory_);
}

ACE_INLINE CORBA::Object_ptr
TAO_ORB_Core::resolve_compression_manager (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->lock_,
                    CORBA::Object::_nil ());
  if (CORBA::is_nil (this->compression_manager_))
    {
      this->resolve_compression_manager_i ();
    }
  return CORBA::Object::_duplicate (this->compression_manager_);
}

ACE_INLINE const char *
TAO_ORB_Core::server_id (void) const
{
  return this->server_id_.c_str();
}

ACE_INLINE TAO_Codeset_Manager *
TAO_ORB_Core::codeset_manager()
{
  if (this->orb_params()->negotiate_codesets() == 0)
    return 0;
  if (this->codeset_manager_ == 0)
    {
      // This causes a factory to be loaded which will call
      // the codeset_manager setter in this thread.
      this->codeset_manager_ =
        this->resource_factory()->codeset_manager();
      if (this->codeset_manager_ == 0)
        this->orb_params()->negotiate_codesets(false);
    }
  return this->codeset_manager_;
}

ACE_INLINE TAO_ZIOP_Adapter *
TAO_ORB_Core::ziop_adapter () const
{
  return this->ziop_adapter_;
}

ACE_INLINE void
TAO_ORB_Core::ziop_adapter (TAO_ZIOP_Adapter *adapter)
{
  this->ziop_adapter_ = adapter;
}

ACE_INLINE TAO::ORBInitializer_Registry_Adapter *
TAO_ORB_Core::orbinitializer_registry ()
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->lock_,
                    0);
  if (orbinitializer_registry_ == 0)
    {
      return this->orbinitializer_registry_i ();
    }
  return this->orbinitializer_registry_;
}

ACE_INLINE TAO_Service_Context_Registry &
TAO_ORB_Core::service_context_registry (void)
{
  return this->service_context_registry_;
}

ACE_INLINE TAO::PolicyFactory_Registry_Adapter *
TAO_ORB_Core::policy_factory_registry ()
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->lock_,
                    0);
  if (policy_factory_registry_ == 0)
    {
      return this->policy_factory_registry_i ();
    }
  return this->policy_factory_registry_;
}

ACE_INLINE CORBA::Object_ptr
TAO_ORB_Core::resolve_dynanyfactory (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->lock_,
                    CORBA::Object::_nil ());
  if (CORBA::is_nil (this->dynany_factory_))
    {
      this->resolve_dynanyfactory_i ();
    }
  return CORBA::Object::_duplicate (this->dynany_factory_);
}

ACE_INLINE CORBA::Object_ptr
TAO_ORB_Core::resolve_ior_manipulation (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->lock_,
                    CORBA::Object::_nil ());
  if (CORBA::is_nil (this->ior_manip_factory_))
    {
      this->resolve_iormanipulation_i ();
    }
  return CORBA::Object::_duplicate (this->ior_manip_factory_);
}

ACE_INLINE CORBA::Object_ptr
TAO_ORB_Core::resolve_ior_table (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->lock_,
                    CORBA::Object::_nil ());
  if (CORBA::is_nil (this->ior_table_.in()))
    {
      this->resolve_ior_table_i ();
    }
  return CORBA::Object::_duplicate (this->ior_table_.in());
}

ACE_INLINE CORBA::Object_ptr
TAO_ORB_Core::resolve_async_ior_table (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->lock_,
                    CORBA::Object::_nil ());
  if (CORBA::is_nil (this->async_ior_table_.in()))
    {
      this->resolve_async_ior_table_i ();
    }
  return CORBA::Object::_duplicate (this->async_ior_table_.in());
}

ACE_INLINE CORBA::Object_ptr
TAO_ORB_Core::resolve_monitor (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->lock_,
                    CORBA::Object::_nil ());
  if (CORBA::is_nil (this->monitor_))
    {
      this->resolve_monitor_i ();
    }
  return CORBA::Object::_duplicate (this->monitor_);
}

// ****************************************************************

#if (TAO_HAS_CORBA_MESSAGING == 1)

ACE_INLINE TAO_Policy_Current &
TAO_ORB_Core::policy_current (void)
{
  return *this->policy_current_;
}

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

ACE_INLINE CORBA::Object_ptr
TAO_ORB_Core::resolve_poa_current (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->lock_,
                    CORBA::Object::_nil ());
  if (CORBA::is_nil (this->poa_current_.in ()))
    {
      this->resolve_poa_current_i ();
    }
  return CORBA::Object::_duplicate (this->poa_current_.in ());
}

#if (TAO_HAS_CORBA_MESSAGING == 1)

ACE_INLINE  TAO_Policy_Set *
TAO_ORB_Core::get_default_policies (void)
{
  return this->default_policies_;
}

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
ACE_INLINE CORBA::Object_ptr
TAO_ORB_Core::resolve_rt_orb (void)
{
  if (CORBA::is_nil (this->rt_orb_.in ()))
    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->lock_,
                        CORBA::Object::_nil ());
      if (CORBA::is_nil (this->rt_orb_.in ()))
        {
          // Save a reference to the priority mapping manager.
          this->rt_orb_ =
              this->object_ref_table ().resolve_initial_reference (
              TAO_OBJID_RTORB);
        }
    }

  return CORBA::Object::_duplicate (this->rt_orb_.in ());
}
#endif

#if (TAO_HAS_INTERCEPTORS == 1)
ACE_INLINE CORBA::Object_ptr
TAO_ORB_Core::pi_current (void)
{
  // A pointer/reference to PICurrent is cached in the ORB Core since
  // it is accessed in the critical path (i.e. the request invocation
  // path).  Caching it prevents additional overhead to due object
  // resolution from occurring.
  return this->pi_current_;
}

ACE_INLINE void
TAO_ORB_Core::pi_current (CORBA::Object_ptr current)
{
  // Not duplicated since the ORB Core's "object_ref_table" already
  // contains a duplicate of the PICurrent object.
  this->pi_current_ = current;
}

ACE_INLINE TAO::ClientRequestInterceptor_Adapter *
TAO_ORB_Core::clientrequestinterceptor_adapter (void)
{
  return this->client_request_interceptor_adapter_;
}

ACE_INLINE TAO::ServerRequestInterceptor_Adapter *
TAO_ORB_Core::serverrequestinterceptor_adapter (void)
{
  return this->server_request_interceptor_adapter_;
}

#endif /* TAO_HAS_INTERCEPTORS */

/// Verify condition for  permanent forward is given,
/// both parameters must provide group attributes.
ACE_INLINE  CORBA::Boolean
TAO_ORB_Core::is_permanent_forward_condition
(const CORBA::Object_ptr obj,
 const TAO_Service_Context &service_context)
{
  const TAO_Service_Callbacks *service_callback =
      this->fault_tolerance_service ().service_callback ();

  // Since forward location is allowed to be nil then check for it.
  CORBA::Boolean const permanent_forward_condition =
      service_callback && obj &&
      service_callback->is_permanent_forward_condition (obj,
                                                        service_context);

  return permanent_forward_condition;
}

ACE_INLINE
void
TAO_ORB_Core::set_sync_scope_hook (Sync_Scope_Hook hook)
{
  this->sync_scope_hook_ = hook;
}

ACE_INLINE
void
TAO_ORB_Core::set_timeout_hook (Timeout_Hook hook)
{
  // Saving the hook pointer so that we can use it later when needed.
  this->timeout_hook_ = hook;
}

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

ACE_INLINE
void
TAO_ORB_Core::set_eager_transport_queueing_strategy (
  TAO::Transport_Queueing_Strategy* strategy)
{
  this->eager_transport_queueing_strategy_ = strategy;
}

ACE_INLINE
void
TAO_ORB_Core::set_delayed_transport_queueing_strategy (
  TAO::Transport_Queueing_Strategy* strategy)
{
  this->delayed_transport_queueing_strategy_ = strategy;
}

#endif

TAO_END_VERSIONED_NAMESPACE_DECL
