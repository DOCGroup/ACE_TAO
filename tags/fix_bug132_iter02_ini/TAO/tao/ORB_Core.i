// -*- C++ -*-
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
    this->refcount_--;
    if (this->refcount_ != 0)
      return this->refcount_;
  }

  this->fini ();
  return 0;
}

ACE_INLINE TAO_Connection_Cache_Manager &
TAO_ORB_Core::connection_cache (void)
{
  return this->connection_cache_;
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

ACE_INLINE void
TAO_ORB_Core::service_context_list (
    TAO_Stub *&stub,
    IOP::ServiceContextList &service_list,
    CORBA::Boolean restart,
    CORBA::Environment &ACE_TRY_ENV)
{
  // @@ We look at the services if they are loaded. But if more
  // services offer this feature we may want to keep expanding the
  // 'if' conditions with a check for whether a service returned a
  // valid Policy object.
  if (this->ft_service_.service_callback ())
    {
      this->ft_service_.service_callback ()->service_context_list (stub,
                                                                   service_list,
                                                                   restart,
                                                                   ACE_TRY_ENV);
      ACE_CHECK;
    }
}

ACE_INLINE void
TAO_ORB_Core:: services_log_msg_rcv (TAO_Message_State_Factory &state)
{
    if (this->ft_service_.service_callback ())
      {
        this->ft_service_.service_callback ()->service_log_msg_rcv (state);
      }
}

ACE_INLINE void
TAO_ORB_Core:: services_log_msg_pre_upcall (TAO_ServerRequest &req)
{
    if (this->ft_service_.service_callback ())
      {
        this->ft_service_.service_callback ()->service_log_msg_pre_upcall (req);
      }
}

ACE_INLINE void
TAO_ORB_Core:: services_log_msg_post_upcall (TAO_ServerRequest &req)
{
    if (this->ft_service_.service_callback ())
      {
        this->ft_service_.service_callback ()->service_log_msg_post_upcall (req);
      }
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
  return this->orb_.in ();
}

ACE_INLINE TAO_Adapter_Registry *
TAO_ORB_Core::adapter_registry (void)
{
  return &this->adapter_registry_;
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
  ((this->member##_ == 0) ? (this->member##_ = this->resource_factory ()->get_##member ()) : (this->member##_) )

ACE_INLINE TAO_ProtocolFactorySet *
TAO_ORB_Core::protocol_factories (void)
{
  return TAO_OC_RETRIEVE (protocol_factories);
}

ACE_INLINE TAO_Connector_Registry *
TAO_ORB_Core::connector_registry (void)
{
  return TAO_OC_RETRIEVE (connector_registry);
}

ACE_INLINE TAO_Acceptor_Registry *
TAO_ORB_Core::acceptor_registry (void)
{
  return TAO_OC_RETRIEVE (acceptor_registry);
}

ACE_INLINE TAO_Parser_Registry *
TAO_ORB_Core::parser_registry (void)
{
  return &this->parser_registry_;
}

ACE_INLINE TAO_PolicyFactory_Registry *
TAO_ORB_Core::policy_factory_registry (void)
{
  return &this->policy_factory_registry_;
}


#undef TAO_OC_RETRIEVE

ACE_INLINE ACE_Char_Codeset_Translator *
TAO_ORB_Core::from_iso8859 (void) const
{
  return this->from_iso8859_;
}

ACE_INLINE ACE_Char_Codeset_Translator *
TAO_ORB_Core::to_iso8859 (void) const
{
  return this->to_iso8859_;
}

ACE_INLINE ACE_WChar_Codeset_Translator *
TAO_ORB_Core::from_unicode (void) const
{
  return this->from_unicode_;
}

ACE_INLINE ACE_WChar_Codeset_Translator *
TAO_ORB_Core::to_unicode (void) const
{
  return this->to_unicode_;
}

#if (TAO_HAS_CORBA_MESSAGING == 1)

ACE_INLINE TAO_Policy_Manager *
TAO_ORB_Core::policy_manager (void)
{
  return this->policy_manager_;
}

ACE_INLINE CORBA::Policy_ptr
TAO_ORB_Core::get_default_policy (
      CORBA::PolicyType policy,
      CORBA::Environment &ACE_TRY_ENV)
{
  return this->default_policies_->get_policy (policy, ACE_TRY_ENV);
}

ACE_INLINE CORBA::Policy *
TAO_ORB_Core::default_relative_roundtrip_timeout (void) const
{
  return this->default_policies_->relative_roundtrip_timeout ();
}

ACE_INLINE CORBA::Policy *
TAO_ORB_Core::default_sync_scope (void) const
{
  return this->default_policies_->sync_scope ();
}

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

#if (TAO_HAS_CLIENT_PRIORITY_POLICY == 1)

ACE_INLINE TAO_Client_Priority_Policy *
TAO_ORB_Core::default_client_priority (void) const
{
  return this->default_policies_->client_priority ();
}

#endif /* TAO_HAS_CLIENT_PRIORITY_POLICY == 1 */

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

ACE_INLINE TAO_Buffering_Constraint_Policy *
TAO_ORB_Core::default_buffering_constraint (void) const
{
  return this->default_policies_->buffering_constraint ();
}

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

#if (TAO_HAS_RT_CORBA == 1)

ACE_INLINE CORBA::Policy *
TAO_ORB_Core::default_threadpool (void) const
{
  return this->default_policies_->threadpool ();
}

ACE_INLINE CORBA::Policy *
TAO_ORB_Core::default_priority_model (void) const
{
  return this->default_policies_->priority_model ();
}

ACE_INLINE CORBA::Policy *
TAO_ORB_Core::default_server_protocol (void) const
{
  return this->default_policies_->server_protocol ();
}

ACE_INLINE CORBA::Policy *
TAO_ORB_Core::default_client_protocol (void) const
{
  return this->default_policies_->client_protocol ();
}

ACE_INLINE CORBA::Policy *
TAO_ORB_Core::default_private_connection (void) const
{
  return this->default_policies_->private_connection ();
}

ACE_INLINE CORBA::Policy *
TAO_ORB_Core::default_priority_banded_connection (void) const
{
  return this->default_policies_->priority_banded_connection ();
}

#endif /* TAO_HAS_RT_CORBA == 1 */

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
  size_t old_size = tss_resources->ts_objects_.size ();
  size_t new_size = slot_id + 1;
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

ACE_INLINE void
TAO_ORB_Core::check_shutdown (CORBA_Environment &ACE_TRY_ENV)
{
  if (this->has_shutdown ())
    {
      // As defined by the CORBA 2.3 specification, throw a
      // CORBA::BAD_INV_ORDER exception with minor code 4 if the ORB
      // has shutdown by the time an ORB function is called.

      ACE_THROW (CORBA::BAD_INV_ORDER (4, CORBA::COMPLETED_NO));
    }
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

ACE_INLINE CORBA::Object_ptr
TAO_ORB_Core::resolve_typecodefactory (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->lock_,
                    CORBA::Object::_nil ());
  if (CORBA::is_nil (this->typecode_factory_))
    {
      this->resolve_typecodefactory_i (ACE_TRY_ENV);
      ACE_CHECK_RETURN (CORBA::Object::_nil ());
    }
  return CORBA::Object::_duplicate (this->typecode_factory_);
}

/*ACE_INLINE void
TAO_ORB_Core::typecode_factory (const CORBA::Object_ptr tf)
{
  this->typecode_factory_ = tf;
}
*/

ACE_INLINE CORBA::Object_ptr
TAO_ORB_Core::resolve_dynanyfactory (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->lock_,
                    CORBA::Object::_nil ());
  if (CORBA::is_nil (this->dynany_factory_))
    {
      this->resolve_dynanyfactory_i (ACE_TRY_ENV);
      ACE_CHECK_RETURN (CORBA::Object::_nil ());
    }
  return CORBA::Object::_duplicate (this->dynany_factory_);
}

ACE_INLINE CORBA::Object_ptr
TAO_ORB_Core::resolve_ior_manipulation (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->lock_,
                    CORBA::Object::_nil ());
  if (CORBA::is_nil (this->ior_manip_factory_))
    {
      this->resolve_iormanipulation_i (ACE_TRY_ENV);
      ACE_CHECK_RETURN (CORBA::Object::_nil ());
    }
  return CORBA::Object::_duplicate (this->ior_manip_factory_);
}

ACE_INLINE CORBA::Object_ptr
TAO_ORB_Core::resolve_ior_table (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->lock_,
                    CORBA::Object::_nil ());
  if (CORBA::is_nil (this->ior_table_))
    {
      this->resolve_ior_table_i (ACE_TRY_ENV);
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
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->lock_, 0);

  if (CORBA::is_nil (this->poa_current_.in ()))
    {
      ACE_TRY_NEW_ENV
        {
          // @@ This is a hack.  FIXME!
          // This forces the POACurrent to be initialized.
          CORBA::Object_var root = this->root_poa (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          return CORBA::Object::_nil ();
        }
      ACE_ENDTRY;
    }

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

ACE_INLINE  TAO_Policy_Manager_Impl *
TAO_ORB_Core::get_default_policies (void)
{
  return this->default_policies_;
}

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

ACE_INLINE CORBA_Environment *
TAO_ORB_Core::default_environment (void) const
{
  return TAO_TSS_RESOURCES::instance ()->default_environment_;
}

ACE_INLINE void
TAO_ORB_Core::default_environment (CORBA_Environment *env)
{
  TAO_TSS_RESOURCES::instance ()->default_environment_ = env;
}

ACE_INLINE TAO_Endpoint_Selector_Factory *
TAO_ORB_Core::endpoint_selector_factory (void)
{
  return this->endpoint_selector_factory_;
}

ACE_INLINE TAO_Default_Endpoint_Selector *
TAO_ORB_Core::default_endpoint_selector (void)
{
  return this->default_endpoint_selector_;
}

#if (TAO_HAS_RT_CORBA == 1)

ACE_INLINE TAO_Protocol_Endpoint_Selector *
TAO_ORB_Core::protocol_endpoint_selector (void)
{
  return this->protocol_endpoint_selector_;
}

ACE_INLINE TAO_Priority_Endpoint_Selector *
TAO_ORB_Core::priority_endpoint_selector (void)
{
  return this->priority_endpoint_selector_;
}

ACE_INLINE TAO_Bands_Endpoint_Selector *
TAO_ORB_Core::bands_endpoint_selector (void)
{
  return this->bands_endpoint_selector_;
}

ACE_INLINE TAO_Priority_Protocol_Selector *
TAO_ORB_Core::priority_protocol_selector (void)
{
  return this->priority_protocol_selector_;
}

ACE_INLINE TAO_Bands_Protocol_Selector *
TAO_ORB_Core::bands_protocol_selector (void)
{
  return this->bands_protocol_selector_;
}

ACE_INLINE TAO_Client_Priority_Policy_Selector *
TAO_ORB_Core::client_priority_policy_selector (void)
{
  return this->client_priority_policy_selector_;
}

ACE_INLINE CORBA::Object_ptr
TAO_ORB_Core::rt_orb (CORBA::Environment &ACE_TRY_ENV)
{
  if (CORBA::is_nil (this->rt_orb_.in ()))
    {
      this->resolve_rt_orb_i (ACE_TRY_ENV);
      ACE_CHECK_RETURN (CORBA::Object::_nil ());
    }

  return CORBA::Object::_duplicate (this->rt_orb_.in ());
}

ACE_INLINE CORBA::Object_ptr
TAO_ORB_Core::rt_current (void)
{
  if (CORBA::is_nil (this->rt_current_.in ()))
    {
      ACE_TRY_NEW_ENV
        {
          // Make sure the RT ORB is loaded and initialized since it
          // also initializes the RTCurrent object.
          this->resolve_rt_orb_i (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          return CORBA::Object::_nil ();
        }
      ACE_ENDTRY;
    }

  return CORBA::Object::_duplicate (this->rt_current_.in ());
}

ACE_INLINE void
TAO_ORB_Core::rt_current (CORBA::Object_ptr current)
{
  this->rt_current_ = CORBA::Object::_duplicate (current);
}

#endif /* TAO_HAS_RT_CORBA == 1 */

#if (TAO_HAS_INTERCEPTORS == 1)
ACE_INLINE void
TAO_ORB_Core::add_interceptor (
   PortableInterceptor::ClientRequestInterceptor_ptr interceptor,
   CORBA_Environment &ACE_TRY_ENV)
{
  this->client_request_interceptors_.add_interceptor (interceptor,
                                                      ACE_TRY_ENV);
}

ACE_INLINE void
TAO_ORB_Core::add_interceptor (
   PortableInterceptor::ServerRequestInterceptor_ptr interceptor,
   CORBA_Environment &ACE_TRY_ENV)
{
  this->server_request_interceptors_.add_interceptor (interceptor,
                                                      ACE_TRY_ENV);
}

// ------

ACE_INLINE TAO_ClientRequestInterceptor_List::TYPE &
TAO_ORB_Core::client_request_interceptors (void)
{
  return this->client_request_interceptors_.interceptors ();
}

// @@ It would be nice to move these to the PortableServer library,
//    perhaps to the RootPOA.  However, there is no "RootPOA" class so
//    there doesn't appear to be a way that only the RootPOA
//    implementation has these server-side interceptor methods and
//    attributes.  Leave them in the ORB Core for now.
ACE_INLINE TAO_ServerRequestInterceptor_List::TYPE &
TAO_ORB_Core::server_request_interceptors (void)
{
  return this->server_request_interceptors_.interceptors ();
}

#endif /* TAO_HAS_INTERCEPTORS */

ACE_INLINE void
TAO_ORB_Core::add_interceptor (
   PortableInterceptor::IORInterceptor_ptr interceptor,
   CORBA_Environment &ACE_TRY_ENV)
{
  this->ior_interceptors_.add_interceptor (interceptor,
                                           ACE_TRY_ENV);
}

ACE_INLINE TAO_IORInterceptor_List::TYPE &
TAO_ORB_Core::ior_interceptors (void)
{
  return this->ior_interceptors_.interceptors ();
}

// ****************************************************************

ACE_INLINE
TAO_ORB_Core_Auto_Ptr::TAO_ORB_Core_Auto_Ptr (TAO_ORB_Core *p)
  : p_ (p)
{
  ACE_TRACE ("TAO_ORB_Core_Auto_Ptr::TAO_ORB_Core_Auto_Ptr");
}

ACE_INLINE TAO_ORB_Core *
TAO_ORB_Core_Auto_Ptr::get (void) const
{
  ACE_TRACE ("TAO_ORB_Core_Auto_Ptr::get");
  return this->p_;
}

ACE_INLINE TAO_ORB_Core *
TAO_ORB_Core_Auto_Ptr::release (void)
{
  ACE_TRACE ("TAO_ORB_Core_Auto_Ptr::release");
  TAO_ORB_Core *old = this->p_;
  this->p_ = 0;
  return old;
}

ACE_INLINE void
TAO_ORB_Core_Auto_Ptr::reset (TAO_ORB_Core *p)
{
  ACE_TRACE ("TAO_ORB_Core_Auto_Ptr::reset");
  if (this->get () != p && this->get () != 0)
    this->get ()->_decr_refcnt ();
  this->p_ = p;
}

ACE_INLINE TAO_ORB_Core *
TAO_ORB_Core_Auto_Ptr::operator-> () const
{
  ACE_TRACE ("TAO_ORB_Core_Auto_Ptr::operator->");
  return this->get ();
}

ACE_INLINE
TAO_ORB_Core_Auto_Ptr::TAO_ORB_Core_Auto_Ptr (TAO_ORB_Core_Auto_Ptr &rhs)
  : p_ (rhs.release ())
{
  ACE_TRACE ("TAO_ORB_Core_Auto_Ptr::TAO_ORB_Core_Auto_Ptr");
}

ACE_INLINE TAO_ORB_Core_Auto_Ptr &
TAO_ORB_Core_Auto_Ptr::operator= (TAO_ORB_Core_Auto_Ptr &rhs)
{
  ACE_TRACE ("TAO_ORB_Core_Auto_Ptr::operator=");
  if (this != &rhs)
    {
      this->reset (rhs.release ());
    }
  return *this;
}

ACE_INLINE
TAO_ORB_Core_Auto_Ptr::~TAO_ORB_Core_Auto_Ptr (void)
{
  ACE_TRACE ("TAO_ORB_Core_Auto_Ptr::~TAO_ORB_Core_Auto_Ptr");
  if (this->get() != 0)
    this->get ()->_decr_refcnt ();
}

// Accessor methods to the underlying ORB_Core Object

ACE_INLINE TAO_ORB_Core &
TAO_ORB_Core_Auto_Ptr::operator *() const
{
  ACE_TRACE ("TAO_ORB_Core_Auto_Ptr::operator *()");
  // @@ Potential problem if this->p_ is zero!
  return *this->get ();
}
