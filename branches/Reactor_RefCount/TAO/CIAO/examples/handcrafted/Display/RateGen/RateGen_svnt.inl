// $Id$ -*- C++ -*-

//    The generated filename for files using this template shoule be
//       [idl-basename]GS.i        GS --> GlueSession

// @@ Notice: [ciao module name] can expand to either CIAO_GLUE or
//    CIAO_GLUE_[module name] as defined in the header file.

/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
/// @@@ Notice that all component and interface names need to be
/// fully qualified as we are creating a new namespace for the CIAO's
/// container glue code.
/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


//////////////////////////////////////////////////////////////////
// Component specific context implementation
//////////////////////////////////////////////////////////////////

ACE_INLINE
CIAO_GLUE_HUDisplay::RateGen_Context::RateGen_Context (::Components::CCMHome_ptr home,
                                                       ::CIAO::Session_Container *c,
                                                       CIAO_GLUE_HUDisplay::RateGen_Servant *sv)
  : home_ (::Components::CCMHome::_duplicate (home)),
    container_ (c),
    servant_ (sv)
{

}

ACE_INLINE
CIAO_GLUE_HUDisplay::RateGen_Context::~RateGen_Context ()
{
}

// Operations for emits interfaces.

// Operations for ::Components::CCMContext
ACE_INLINE ::Components::Principal_ptr
CIAO_GLUE_HUDisplay::RateGen_Context::get_caller_principal (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ We don't support Security in CIAO yet.
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

ACE_INLINE ::Components::CCMHome_ptr
CIAO_GLUE_HUDisplay::RateGen_Context::get_CCM_home (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return ::Components::CCMHome::_duplicate (this->home_.in ());
}

ACE_INLINE CORBA::Boolean
CIAO_GLUE_HUDisplay::RateGen_Context::get_rollback_only (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::IllegalState))
{
  // @@ We don't support Transaction in CIAO yet.
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

ACE_INLINE ::Components::Transaction::UserTransaction_ptr
CIAO_GLUE_HUDisplay::RateGen_Context::get_user_transaction (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::IllegalState))
{
  // @@ We don't support Transaction in CIAO yet.
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

ACE_INLINE CORBA::Boolean
CIAO_GLUE_HUDisplay::RateGen_Context::is_caller_in_role (const char * role
                                                                 ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_UNUSED_ARG (role);

  // @@ We don't support Transaction in CIAO yet.
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

ACE_INLINE void
CIAO_GLUE_HUDisplay::RateGen_Context::set_rollback_only (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::IllegalState))
{
  // @@ We don't support Transaction in CIAO yet.
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

//////////////////////////////////////////////////////////////////
// Component Servant Glue code implementation
//////////////////////////////////////////////////////////////////

ACE_INLINE void
CIAO_GLUE_HUDisplay::RateGen_Servant::start (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->executor_->start (ACE_ENV_SINGLE_ARG_PARAMETER);
}

ACE_INLINE void
CIAO_GLUE_HUDisplay::RateGen_Servant::stop (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Simply relay to executor.  May not need to return the result...
  this->executor_->stop (ACE_ENV_SINGLE_ARG_PARAMETER);
}

ACE_INLINE CORBA::Boolean
CIAO_GLUE_HUDisplay::RateGen_Servant::active (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Simply relay to executor.  May not need to return the result...
  return this->executor_->active (ACE_ENV_SINGLE_ARG_PARAMETER);
}

ACE_INLINE CORBA::Long
CIAO_GLUE_HUDisplay::RateGen_Servant::hertz (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Simply relay to executor.  May not need to return the result...
  return this->executor_->hertz (ACE_ENV_SINGLE_ARG_PARAMETER);
}

ACE_INLINE void
CIAO_GLUE_HUDisplay::RateGen_Servant::hertz (CORBA::Long hertz
                                             ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Simply relay to executor.  May not need to return the result...
  this->executor_->hertz (hertz
                          ACE_ENV_ARG_PARAMETER);
}

// Operations for publishes interfaces.
ACE_INLINE ::Components::Cookie_ptr
CIAO_GLUE_HUDisplay::RateGen_Servant::subscribe_Pulse (HUDisplay::tickConsumer_ptr c
                                                       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ::Components::ExceededConnectionLimit))
{
  return this->context_->subscribe_Pulse (c
                                          ACE_ENV_ARG_PARAMETER);
}

ACE_INLINE HUDisplay::tickConsumer_ptr
CIAO_GLUE_HUDisplay::RateGen_Servant::unsubscribe_Pulse (::Components::Cookie_ptr ck
                                                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ::Components::InvalidConnection))
{
  return this->context_->unsubscribe_Pulse (ck
                                            ACE_ENV_ARG_PARAMETER);
}

//////////////////////////////////////////////////////////////////
// Component Home Glue code implementation
//////////////////////////////////////////////////////////////////

ACE_INLINE
CIAO_GLUE_HUDisplay::RateGenHome_Servant::RateGenHome_Servant (HUDisplay::CCM_RateGenHome_ptr exe,
                                                              CIAO::Session_Container *c)
  : executor_ (HUDisplay::CCM_RateGenHome::_duplicate (exe)),
    container_ (c)
{
}

ACE_INLINE
CIAO_GLUE_HUDisplay::RateGenHome_Servant::~RateGenHome_Servant (void)
{
}

// for factory operations inherit from parent home(s), they should return
// the corresponding component types their homes manage
ACE_INLINE HUDisplay::RateGen_ptr
CIAO_GLUE_HUDisplay::RateGenHome_Servant::new_RateGen (CORBA::Long hertz
                                                       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ::Components::CreateFailure))
{
  Components::EnterpriseComponent_var _ciao_ec =
    this->executor_->new_RateGen (hertz
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  HUDisplay::CCM_RateGen_var _ciao_comp
    = HUDisplay::CCM_RateGen::_narrow (_ciao_ec.in ()
                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->_ciao_activate_component (_ciao_comp.in ()
                                         ACE_ENV_ARG_PARAMETER);
}

// Operations for KeylessHome interface
ACE_INLINE ::Components::CCMObject_ptr
CIAO_GLUE_HUDisplay::RateGenHome_Servant::create_component (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CreateFailure))
{
  // Simply forward to the create method.
  return this->create (ACE_ENV_SINGLE_ARG_PARAMETER);
}

// Operations for CCMHome interface
ACE_INLINE ::CORBA::IRObject_ptr
CIAO_GLUE_HUDisplay::RateGenHome_Servant::get_component_def (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ TO-DO.  Contact IfR?

  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

ACE_INLINE CORBA::IRObject_ptr
CIAO_GLUE_HUDisplay::RateGenHome_Servant::get_home_def (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ TO-DO.  Contact IfR?

  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}
