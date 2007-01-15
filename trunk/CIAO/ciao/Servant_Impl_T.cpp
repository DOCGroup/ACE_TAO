// $Id$

#ifndef CIAO_SERVANT_IMPL_T_C
#define CIAO_SERVANT_IMPL_T_C

#include "Servant_Impl_T.h"
#include "CCM_EntityContextC.h"

namespace CIAO
{
  template <typename BASE_SKEL,
            typename EXEC,
            typename CONTEXT>
  Servant_Impl<BASE_SKEL, EXEC, CONTEXT>::Servant_Impl (
      EXEC * exe,
      Components::CCMHome_ptr home,
      Home_Servant_Impl_Base *home_servant,
      Session_Container * c
    )
    : Servant_Impl_Base (home, home_servant, c),
      activated_ (false),
      pre_activated_ (false),
      post_activated_ (false),
      executor_ (EXEC::_duplicate (exe))
  {
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename CONTEXT>
  Servant_Impl<BASE_SKEL, EXEC, CONTEXT>::~Servant_Impl (void)
  {
  // **********************************************************************
  /*
            ACE_TRY_NEW_ENV
            {
              ::Components::SessionComponent_var scom =
                ::Components::SessionComponent::_narrow (
                    this->executor_.in ()
                    ACE_ENV_ARG_PARAMETER
                  );
              ACE_TRY_CHECK;

              if (! ::CORBA::is_nil (scom.in ()))
                {
                  scom->ccm_remove (ACE_ENV_SINGLE_ARG_PARAMETER);
                  ACE_TRY_CHECK;
                }
            }
            ACE_CATCHANY
            {
            }
            ACE_ENDTRY;
   */
   // *********************************************************************

    this->context_->_remove_ref ();
  }

  // Operations for CCMObject interface.

  template <typename BASE_SKEL,
            typename EXEC,
            typename CONTEXT>
  CORBA::Boolean
  Servant_Impl<BASE_SKEL, EXEC, CONTEXT>::same_component (
      CORBA::Object_ptr object_ref
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    if (::CORBA::is_nil (object_ref))
      {
        ACE_THROW_RETURN (::CORBA::BAD_PARAM (), 0);
      }

    ::CORBA::Object_var the_other =
      object_ref->_get_component (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);

    ::CORBA::Object_var me =
      this->context_->get_CCM_object (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);

    return me->_is_equivalent (the_other.in ()
                               ACE_ENV_ARG_PARAMETER);

  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename CONTEXT>
  ::Components::CCMHome_ptr
  Servant_Impl<BASE_SKEL, EXEC, CONTEXT>::get_ccm_home (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return this->context_->get_CCM_home (ACE_ENV_SINGLE_ARG_PARAMETER);
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename CONTEXT>
  Components::SessionComponent_ptr
  Servant_Impl<BASE_SKEL, EXEC, CONTEXT>::get_executor (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ::Components::SessionComponent_var temp =
      ::Components::SessionComponent::_narrow (
          this->executor_.in ()
          ACE_ENV_ARG_PARAMETER
        );
    ACE_CHECK_RETURN (Components::SessionComponent::_nil ());
    return temp._retn ();
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename CONTEXT>
  CORBA::Object_ptr
  Servant_Impl<BASE_SKEL, EXEC, CONTEXT>::_get_component (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ::Components::SessionContext_var sc =
      ::Components::SessionContext::_narrow (
          this->context_
          ACE_ENV_ARG_PARAMETER
        );
    ACE_CHECK_RETURN (::CORBA::Object::_nil ());

    if (! ::CORBA::is_nil (sc.in ()))
      {
        return sc->get_CCM_object (ACE_ENV_SINGLE_ARG_PARAMETER);
      }

    ::Components::EntityContext_var ec =
      ::Components::EntityContext::_narrow (
          this->context_
          ACE_ENV_ARG_PARAMETER
        );
    ACE_CHECK_RETURN (::CORBA::Object::_nil ());

    if (! ::CORBA::is_nil (ec.in ()))
      {
        return ec->get_CCM_object (ACE_ENV_SINGLE_ARG_PARAMETER);
      }

    ACE_THROW_RETURN (CORBA::INTERNAL (),
                      ::CORBA::Object::_nil ());
  }

  // CIAO-specific operations.

  template <typename BASE_SKEL,
            typename EXEC,
            typename CONTEXT>
  void
  Servant_Impl<BASE_SKEL, EXEC, CONTEXT>::activate_component (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    if (this->is_activated () == 0)
      {
        this->ciao_preactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_CHECK;

        this->ciao_activate (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_CHECK;

        this->ciao_postactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_CHECK;
      }
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename CONTEXT>
  void
  Servant_Impl<BASE_SKEL, EXEC, CONTEXT>::ciao_preactivate (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ::Components::SessionComponent_var temp =
      ::Components::SessionComponent::_narrow (
          this->executor_.in ()
          ACE_ENV_ARG_PARAMETER
        );
    ACE_CHECK;

    if (! ::CORBA::is_nil (temp.in ()))
      {
        if (this->pre_activated_ == 0)
          {
            this->pre_activated_ = 1;
            temp->ciao_preactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_CHECK;
          }
      }
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename CONTEXT>
  void
  Servant_Impl<BASE_SKEL, EXEC, CONTEXT>::ciao_activate (
  ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ::Components::SessionComponent_var temp =
      ::Components::SessionComponent::_narrow (
          this->executor_.in ()
          ACE_ENV_ARG_PARAMETER
        );
    ACE_CHECK;

    if (! ::CORBA::is_nil (temp.in ()))
      {
        if (this->activated_ == 0)
          {
            this->activated_ = 1;
            temp->ccm_activate (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_CHECK;
          }
      }
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename CONTEXT>
  void
  Servant_Impl<BASE_SKEL, EXEC, CONTEXT>::ciao_postactivate (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ::Components::SessionComponent_var temp =
      ::Components::SessionComponent::_narrow (
          this->executor_.in ()
          ACE_ENV_ARG_PARAMETER
        );
    ACE_CHECK;

    if (! ::CORBA::is_nil (temp.in ()))
      {
        if (this->post_activated_ == 0)
          {
            this->post_activated_ = 1;
            temp->ciao_postactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_CHECK;
          }
      }
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename CONTEXT>
  CORBA::Boolean
  Servant_Impl<BASE_SKEL, EXEC, CONTEXT>::is_activated (
      ACE_ENV_SINGLE_ARG_DECL_NOT_USED
    )
  ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return this->pre_activated_;
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename CONTEXT>
  void
  Servant_Impl<BASE_SKEL, EXEC, CONTEXT>::ciao_passivate (
      ACE_ENV_SINGLE_ARG_DECL
    )
  ACE_THROW_SPEC ((CORBA::SystemException))
  {
    // @@ Jai, could you please see why this is required?
    ::Components::SessionComponent_var temp =
      ::Components::SessionComponent::_narrow (
          this->executor_.in ()
          ACE_ENV_ARG_PARAMETER
        );
    ACE_CHECK;

    if (! ::CORBA::is_nil (temp.in ()))
      temp->ccm_passivate (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    this->pre_activated_ = 0;
    this->activated_ = 0;
    this->post_activated_ = 0;
  }
}

#endif /* CIAO_SERVANT_IMPL_T_C */
