// $Id$ -*- C++ -*-

//    The generated filename for files using this template shoule be
//       [idl-basename]GS.i        GS --> GlueSession

// @@ Notice: [ciao module name] can expand to either CIAO_GLUE or
//    CIAO_GLUE_[module name] as defined in the header file.

//////////////////////////////////////////////////////////////////
// Component specific context implementation
//////////////////////////////////////////////////////////////////

ACE_INLINE
CIAO_GLUE_HUDisplay::GPS_Context::GPS_Context (::Components::CCMHome_ptr home,
                                               ::CIAO::Session_Container *c,
                                               CIAO_GLUE_HUDisplay::GPS_Servant *sv)
  : home_ (::Components::CCMHome::_duplicate (home)),
    container_ (c),
    servant_ (sv)
{
}

ACE_INLINE
CIAO_GLUE_HUDisplay::GPS_Context::~GPS_Context ()
{
}

// Operations for emits interfaces.

// Operations for ::Components::CCMContext
ACE_INLINE ::Components::Principal_ptr
CIAO_GLUE_HUDisplay::GPS_Context::get_caller_principal (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ We don't support Security in CIAO yet.
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

ACE_INLINE ::Components::CCMHome_ptr
CIAO_GLUE_HUDisplay::GPS_Context::get_CCM_home (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return ::Components::CCMHome::_duplicate (this->home_.in ());
}

ACE_INLINE CORBA::Boolean
CIAO_GLUE_HUDisplay::GPS_Context::get_rollback_only (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::IllegalState))
{
  // @@ We don't support Transaction in CIAO yet.
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

ACE_INLINE ::Components::Transaction::UserTransaction_ptr
CIAO_GLUE_HUDisplay::GPS_Context::get_user_transaction (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::IllegalState))
{
  // @@ We don't support Transaction in CIAO yet.
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

ACE_INLINE CORBA::Boolean
CIAO_GLUE_HUDisplay::GPS_Context::is_caller_in_role (const char * role
                                                     ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_UNUSED_ARG (role);

  // @@ We don't support Transaction in CIAO yet.
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

ACE_INLINE void
CIAO_GLUE_HUDisplay::GPS_Context::set_rollback_only (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::IllegalState))
{
  // @@ We don't support Transaction in CIAO yet.
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

//////////////////////////////////////////////////////////////////
// Component Servant Glue code implementation
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
// EventConsumer Glue Code implementation
// @@ We are assuming that these consumers are declared under the same
//    module as the component (thus, we are placing this glue code
//    here under the same namespace.  If they are not, we will
//    either be generating them in separate namespaces, or include
//    some other CIDL generated files to get the glue code
//    implementation.
//////////////////////////////////////////////////////////////////

// Constructor and destructor.
ACE_INLINE
CIAO_GLUE_HUDisplay::GPS_Servant::tickConsumer_Refresh_Servant::tickConsumer_Refresh_Servant
  (HUDisplay::CCM_GPS_ptr executor,
   HUDisplay::CCM_GPS_Context_ptr c)
    : executor_ (HUDisplay::CCM_GPS::_duplicate (executor)),
      ctx_ (HUDisplay::CCM_GPS_Context::_duplicate (c))
{
}

ACE_INLINE
CIAO_GLUE_HUDisplay::GPS_Servant::tickConsumer_Refresh_Servant::~tickConsumer_Refresh_Servant ()
{
}

ACE_INLINE CORBA::Object_ptr
CIAO_GLUE_HUDisplay::GPS_Servant::tickConsumer_Refresh_Servant::_get_component (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->ctx_->get_CCM_object (ACE_ENV_SINGLE_ARG_PARAMETER);
}

ACE_INLINE void
CIAO_GLUE_HUDisplay::GPS_Servant::tickConsumer_Refresh_Servant::push_tick
  (HUDisplay::tick_ptr evt
   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->executor_->push_Refresh (evt
                                 ACE_ENV_ARG_PARAMETER);
}

// Operations for emits interfaces.

// Operations for publishes interfaces.
ACE_INLINE ::Components::Cookie_ptr
CIAO_GLUE_HUDisplay::GPS_Servant::subscribe_Ready (HUDisplay::tickConsumer_ptr c
                                                   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ::Components::ExceededConnectionLimit))
{
  return this->context_->subscribe_Ready (c
                                          ACE_ENV_ARG_PARAMETER);
}

ACE_INLINE HUDisplay::tickConsumer_ptr
CIAO_GLUE_HUDisplay::GPS_Servant::unsubscribe_Ready (::Components::Cookie_ptr ck
                                                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ::Components::InvalidConnection))
{
  return this->context_->unsubscribe_Ready (ck
                                            ACE_ENV_ARG_PARAMETER);
}

//////////////////////////////////////////////////////////////////
// Component Home Glue code implementation
//////////////////////////////////////////////////////////////////

ACE_INLINE
CIAO_GLUE_HUDisplay::GPSHome_Servant::GPSHome_Servant (HUDisplay::CCM_GPSHome_ptr exe,
                                                       CIAO::Session_Container *c)
  : executor_ (HUDisplay::CCM_GPSHome::_duplicate (exe)),
    container_ (c)
{
}

ACE_INLINE
CIAO_GLUE_HUDisplay::GPSHome_Servant::~GPSHome_Servant (void)
{
}

// Operations for KeylessHome interface
ACE_INLINE ::Components::CCMObject_ptr
CIAO_GLUE_HUDisplay::GPSHome_Servant::create_component (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CreateFailure))
{
  // Simply forward to the create method.
  return this->create (ACE_ENV_SINGLE_ARG_PARAMETER);
}

// Operations for CCMHome interface
ACE_INLINE ::CORBA::IRObject_ptr
CIAO_GLUE_HUDisplay::GPSHome_Servant::get_component_def (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ TO-DO.  Contact IfR?

  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

ACE_INLINE CORBA::IRObject_ptr
CIAO_GLUE_HUDisplay::GPSHome_Servant::get_home_def (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ TO-DO.  Contact IfR?

  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}
