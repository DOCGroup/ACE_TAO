// $Id$

#ifndef CIAO_SERVANT_IMPL_T_C
#define CIAO_SERVANT_IMPL_T_C

namespace CIAO
{
  template <typename BASE_SKEL,
            typename EXEC,
            typename CONTEXT>
  Servant_Impl<BASE_SKEL, EXEC, CONTEXT>::Servant_Impl (
        EXEC * exe,
        Components::CCMHome_ptr home,
        const char * ins_name,
        Home_Servant_Impl_Base *home_servant,
        Container_ptr c)
    : CONTEXT::svnt_base_type (home, home_servant, c),
      activated_ (false),
      configuration_completed_ (false),
      executor_ (EXEC::_duplicate (exe)),
      context_ (0),
      ins_name_ (ins_name)
  {
    ACE_NEW (this->context_,
             CONTEXT (home, c, this));

    ::Components::SessionComponent_var scom =
      ::Components::SessionComponent::_narrow (exe);

    if (! ::CORBA::is_nil (scom.in ()))
      {
        scom->set_session_context (this->context_);
      }
    else
      {
        CIAO_DEBUG (6,
                    (LM_DEBUG, CLINFO
                     "Servant_Impl_T::Servant_Impl_T - "
                     "Couldn't set session context for %C\n",
                     ins_name));
      }
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename CONTEXT>
  Servant_Impl<BASE_SKEL, EXEC, CONTEXT>::~Servant_Impl (void)
  {
    if (this->executor_->_refcount_value () > 1)
      {
        CIAO_ERROR (1,
                    (LM_ERROR, CLINFO
                     "Servant_Impl_T::~Servant_Impl_T - "
                     "Executor object reference count is %u\n",
                     this->executor_->_refcount_value ()));
      }

    this->context_->_remove_ref ();
  }

  // Operations for CCMObject interface.

  template <typename BASE_SKEL,
            typename EXEC,
            typename CONTEXT>
  CORBA::Boolean
  Servant_Impl<BASE_SKEL, EXEC, CONTEXT>::same_component (
      CORBA::Object_ptr object_ref)
  {
    if (::CORBA::is_nil (object_ref))
      {
        throw ::CORBA::BAD_PARAM ();
      }

    ::CORBA::Object_var me = this->context_->get_CCM_object ();

    if (::CORBA::is_nil (me.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }

    ::CORBA::Object_var the_other = object_ref->_get_component ();

    return me->_is_equivalent (the_other.in ());

  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename CONTEXT>
  ::Components::CCMHome_ptr
  Servant_Impl<BASE_SKEL, EXEC, CONTEXT>::get_ccm_home (void)
  {
    return this->context_->get_CCM_home ();
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename CONTEXT>
  Components::SessionComponent_ptr
  Servant_Impl<BASE_SKEL, EXEC, CONTEXT>::get_executor (void)
  {
    ::Components::SessionComponent_var temp =
      ::Components::SessionComponent::_narrow (this->executor_.in ());

    return temp._retn ();
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename CONTEXT>
  CORBA::Object_ptr
  Servant_Impl<BASE_SKEL, EXEC, CONTEXT>::_get_component (void)
  {
    ::Components::SessionContext_var sc =
      ::Components::SessionContext::_narrow (this->context_);

    if (! ::CORBA::is_nil (sc.in ()))
      {
        return sc->get_CCM_object ();
      }

    throw ::CORBA::INTERNAL ();
  }

  // CIAO-specific operations.

  template <typename BASE_SKEL,
            typename EXEC,
            typename CONTEXT>
  void
  Servant_Impl<BASE_SKEL, EXEC, CONTEXT>::activate_component (void)
  {
    if (this->configuration_completed_ && !this->activated_)
      {
        ::Components::SessionComponent_var temp =
          ::Components::SessionComponent::_narrow (this->executor_.in ());

        if (! ::CORBA::is_nil (temp.in ()))
          {
            temp->ccm_activate ();
            this->activated_ = true;
          }
      }
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename CONTEXT>
  void
  Servant_Impl<BASE_SKEL, EXEC, CONTEXT>::passivate_component (void)
  {
    if (this->activated_)
      {
        ::Components::SessionComponent_var temp =
          ::Components::SessionComponent::_narrow (this->executor_.in ());

        if (! ::CORBA::is_nil (temp.in ()))
          {
            temp->ccm_passivate ();
            this->activated_ = false;
          }
      }
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename CONTEXT>
  void
  Servant_Impl<BASE_SKEL, EXEC, CONTEXT>::configuration_complete (
    )
  {
    if (!this->configuration_completed_)
      {
        ::Components::SessionComponent_var temp =
          ::Components::SessionComponent::_narrow (this->executor_.in ());

        if (! ::CORBA::is_nil (temp.in ()))
          {
            temp->configuration_complete ();
            this->configuration_completed_ = true;
          }
      }
  }
}

#endif /* CIAO_SERVANT_IMPL_T_C */
