// -*- C++ -*-
//
// $Id$

ACE_INLINE CORBA::ULong
TAO_ORB_Core::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->lock_, 0);
  return this->refcount_++;
}

ACE_INLINE CORBA::ULong
TAO_ORB_Core::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->lock_, 0);
    --this->refcount_;
    if (this->refcount_ != 0)
      return this->refcount_;
  }

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

ACE_INLINE CORBA::Boolean
TAO_ORB_Core::service_profile_selection (TAO_MProfile &mprofile,
                                         TAO_Profile  *&profile)
{
  CORBA::Boolean retval = 0;
  // @@ If different services have the same feature we may want to
  // prioritise them here. We need to decide here whose selection of
  // profile is more important.
  if (this->ft_service_.service_callback ())
    {
      retval =
        this->ft_service_.service_callback ()->select_profile (&mprofile,
                                                                profile);
    }
  return retval;
}

ACE_INLINE CORBA::Boolean
TAO_ORB_Core::service_profile_reselection (TAO_Stub *stub,
                                           TAO_Profile *&profile)
{
  CORBA::Boolean retval = 0;
  // @@ If different services have the same feature we may want to
  // prioritise them here. We need to decide here whose selection of
  // profile is more important.
  if (this->ft_service_.service_callback ())
    {
      retval =
        this->ft_service_.service_callback ()->reselect_profile (stub,
                                                                 profile);
    }
  return retval;
}

ACE_INLINE void
TAO_ORB_Core::reset_service_profile_flags (void)
{
  // @@ If different services have the same feature we may want to
  // prioritise them here. We need to decide here whose selection of
  // profile is more important.

  if (this->ft_service_.service_callback ())
    {
      this->ft_service_.service_callback ()->reset_profile_flags ();
    }
  return;
}


ACE_INLINE CORBA::Boolean
TAO_ORB_Core::object_is_nil (CORBA::Object_ptr obj)
{
  CORBA::Boolean retval = 0;
  if (this->ft_service_.service_callback ())
    {
      retval =
        this->ft_service_.service_callback ()->object_is_nil (obj);
    }
  return retval;
}


ACE_INLINE CORBA::Boolean
TAO_ORB_Core::is_profile_equivalent (const TAO_Profile *this_p,
                                     const TAO_Profile *that_p)
{
  CORBA::Boolean retval = 1;

  if (this->ft_service_.service_callback ())
    {
      retval =
        this->ft_service_.service_callback ()->is_profile_equivalent (this_p,
                                                                      that_p);
    }

  return retval;
}

ACE_INLINE CORBA::ULong
TAO_ORB_Core::hash_service (TAO_Profile *p,
                            CORBA::ULong m)
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

ACE_INLINE TAO_Adapter_Registry *
TAO_ORB_Core::adapter_registry (void)
{
  return &this->adapter_registry_;
}

ACE_INLINE TAO_Request_Dispatcher *
TAO_ORB_Core::request_dispatcher (void)
{
  return this->request_dispatcher_;
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

ACE_INLINE TAO::PolicyFactory_Registry_Adapter *
TAO_ORB_Core::policy_factory_registry (void)
{
  return this->policy_factory_registry_;
}


ACE_INLINE TAO_Codeset_Manager *
TAO_ORB_Core::codeset_manager()
{
  return this->codeset_manager_;
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

ACE_INLINE int
TAO_ORB_Core::add_tss_cleanup_func (ACE_CLEANUP_FUNC cleanup,
                                    size_t &slot_id)
{
  return this->tss_cleanup_funcs_.register_cleanup_function (cleanup,
                                                             slot_id);
}

ACE_INLINE TAO_Cleanup_Func_Registry *
TAO_ORB_Core::tss_cleanup_funcs (void)
{
  return &(this->tss_cleanup_funcs_);
}

ACE_INLINE int
TAO_ORB_Core::has_shutdown (void)
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
TAO_ORB_Core::imr_endpoints_in_ior (void)
{
  return imr_endpoints_in_ior_;
}

ACE_INLINE CORBA::Object_ptr
TAO_ORB_Core::resolve_typecodefactory (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->lock_,
                    CORBA::Object::_nil ());
  if (CORBA::is_nil (this->typecode_factory_))
    {
      this->resolve_typecodefactory_i (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::Object::_nil ());
    }
  return CORBA::Object::_duplicate (this->typecode_factory_);
}

ACE_INLINE const char *
TAO_ORB_Core::server_id (void) const
{
  return this->server_id_.c_str();
}

ACE_INLINE CORBA::Object_ptr
TAO_ORB_Core::resolve_dynanyfactory (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->lock_,
                    CORBA::Object::_nil ());
  if (CORBA::is_nil (this->dynany_factory_))
    {
      this->resolve_dynanyfactory_i (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::Object::_nil ());
    }
  return CORBA::Object::_duplicate (this->dynany_factory_);
}

ACE_INLINE CORBA::Object_ptr
TAO_ORB_Core::resolve_ior_manipulation (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->lock_,
                    CORBA::Object::_nil ());
  if (CORBA::is_nil (this->ior_manip_factory_))
    {
      this->resolve_iormanipulation_i (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::Object::_nil ());
    }
  return CORBA::Object::_duplicate (this->ior_manip_factory_);
}

ACE_INLINE CORBA::Object_ptr
TAO_ORB_Core::resolve_ior_table (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->lock_,
                    CORBA::Object::_nil ());
  if (CORBA::is_nil (this->ior_table_))
    {
      this->resolve_ior_table_i (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::Object::_nil ());
    }
  return CORBA::Object::_duplicate (this->ior_table_);
}

// ****************************************************************

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

ACE_INLINE TAO_Eager_Buffering_Sync_Strategy &
TAO_ORB_Core::eager_buffering_sync_strategy (void)
{
  return *this->eager_buffering_sync_strategy_;
}

ACE_INLINE TAO_Delayed_Buffering_Sync_Strategy &
TAO_ORB_Core::delayed_buffering_sync_strategy (void)
{
  return *this->delayed_buffering_sync_strategy_;
}

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

ACE_INLINE TAO_Transport_Sync_Strategy &
TAO_ORB_Core::transport_sync_strategy (void)
{
  return *this->transport_sync_strategy_;
}

#if (TAO_HAS_CORBA_MESSAGING == 1)

ACE_INLINE TAO_Policy_Current &
TAO_ORB_Core::policy_current (void)
{
  return *this->policy_current_;
}

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

ACE_INLINE CORBA::Object_ptr
TAO_ORB_Core::poa_current (void)
{
  return CORBA::Object::_duplicate (this->poa_current_.in ());
}

ACE_INLINE void
TAO_ORB_Core::poa_current (CORBA::Object_ptr current)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);
  this->poa_current_ =
    CORBA::Object::_duplicate (current);
}

#if (TAO_HAS_CORBA_MESSAGING == 1)

ACE_INLINE  TAO_Policy_Set *
TAO_ORB_Core::get_default_policies (void)
{
  return this->default_policies_;
}

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

ACE_INLINE CORBA::Object_ptr
TAO_ORB_Core::resolve_rt_orb (ACE_ENV_SINGLE_ARG_DECL)
{
  if (CORBA::is_nil (this->rt_orb_.in ()))
    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->lock_,
                        CORBA::Object::_nil ());
      if (CORBA::is_nil (this->rt_orb_.in ()))
        {
          // Save a reference to the priority mapping manager.
          this->rt_orb_ =
              this->object_ref_table ().resolve_initial_references (
              TAO_OBJID_RTORB
               ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (CORBA::Object::_nil ());
        }
    }

  return CORBA::Object::_duplicate (this->rt_orb_.in ());
}

ACE_INLINE CORBA::Object_ptr
TAO_ORB_Core::resolve_rt_current (ACE_ENV_SINGLE_ARG_DECL)
{
  if (CORBA::is_nil (this->rt_current_.in ()))
    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->lock_,
                        CORBA::Object::_nil ());
      if (CORBA::is_nil (this->rt_current_.in ()))
        {
          // Save a reference to the priority mapping manager.
          this->rt_current_ =
              this->object_ref_table ().resolve_initial_references (
              TAO_OBJID_RTCURRENT
               ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (CORBA::Object::_nil ());
        }
    }
  return CORBA::Object::_duplicate (this->rt_current_.in ());
}

#if (TAO_HAS_INTERCEPTORS == 1)
ACE_INLINE TAO::PICurrent *
TAO_ORB_Core::pi_current (void)
{
  // A pointer/reference to PICurrent is cached in the ORB Core since
  // it is accessed in the critical path (i.e. the request invocation
  // path).  Caching it prevents additional overhead to due object
  // resolution from occurring.
  return this->pi_current_;
}

ACE_INLINE void
TAO_ORB_Core::pi_current (TAO::PICurrent *current)
{
  // Not duplicated since the ORB Core's "object_ref_table" already
  // contains a duplicate of the PICurrent object.
  this->pi_current_ = current;
}

ACE_INLINE void
TAO_ORB_Core::add_interceptor (
   PortableInterceptor::ClientRequestInterceptor_ptr interceptor
   ACE_ENV_ARG_DECL)
{
  this->client_request_interceptors_.add_interceptor (interceptor
                                                       ACE_ENV_ARG_PARAMETER);
}

ACE_INLINE void
TAO_ORB_Core::add_interceptor (
   PortableInterceptor::ServerRequestInterceptor_ptr interceptor
   ACE_ENV_ARG_DECL)
{
  this->server_request_interceptors_.add_interceptor (interceptor
                                                       ACE_ENV_ARG_PARAMETER);
}

// ------

ACE_INLINE TAO::ClientRequestInterceptor_List::TYPE &
TAO_ORB_Core::client_request_interceptors (void)
{
  return this->client_request_interceptors_.interceptors ();
}

// @@ It would be nice to move these to the PortableServer library,
//    perhaps to the RootPOA.  However, there is no "RootPOA" class so
//    there doesn't appear to be a way that only the RootPOA
//    implementation has these server-side interceptor methods and
//    attributes.  Leave them in the ORB Core for now.
ACE_INLINE TAO::ServerRequestInterceptor_List::TYPE &
TAO_ORB_Core::server_request_interceptors (void)
{
  return this->server_request_interceptors_.interceptors ();
}

#endif /* TAO_HAS_INTERCEPTORS */
