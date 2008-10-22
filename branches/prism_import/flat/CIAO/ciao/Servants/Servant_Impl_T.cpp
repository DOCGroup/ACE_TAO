// $Id$

#ifndef CIAO_SERVANT_IMPL_T_C
#define CIAO_SERVANT_IMPL_T_C

#include "Servant_Impl_T.h"
#include <ccm/CCM_EntityContextC.h>

namespace CIAO
{
  template <typename BASE_SKEL,
            typename EXEC,
            typename CONTEXT>
  Servant_Impl<BASE_SKEL, EXEC, CONTEXT>::Servant_Impl (
      EXEC * exe,
      Components::CCMHome_ptr home,
      Home_Servant_Impl_Base *home_servant,
      Container_ptr c
    )
    : Servant_Impl_Base (home, home_servant, c),
      activated_ (false),
      configuration_completed_ (false),
      executor_ (EXEC::_duplicate (exe))
  {
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename CONTEXT>
  Servant_Impl<BASE_SKEL, EXEC, CONTEXT>::~Servant_Impl (void)
  {
    CIAO_DEBUG ((LM_INFO, "Servant_Impl_T::~Servant_Impl - "
                 "Executor object reference  count is %u\n",
                 this->executor_->_refcount_value ()));
    
  // **********************************************************************
  /*
            try
            {
              ::Components::SessionComponent_var scom =
                ::Components::SessionComponent::_narrow (
                    this->executor_.in ()
                  );

              if (! ::CORBA::is_nil (scom.in ()))
                {
                  scom->ccm_remove ();
                }
            }
            catch (const CORBA::Exception& ex)
            {
            }
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
    )
  {
    if (::CORBA::is_nil (object_ref))
      {
        throw ::CORBA::BAD_PARAM ();
      }

    ::CORBA::Object_var the_other =
      object_ref->_get_component ();

    ::CORBA::Object_var me =
      this->context_->get_CCM_object ();

    return me->_is_equivalent (the_other.in ());

  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename CONTEXT>
  ::Components::CCMHome_ptr
  Servant_Impl<BASE_SKEL, EXEC, CONTEXT>::get_ccm_home (
    )
  {
    return this->context_->get_CCM_home ();
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename CONTEXT>
  Components::SessionComponent_ptr
  Servant_Impl<BASE_SKEL, EXEC, CONTEXT>::get_executor (
    )
  {
    ::Components::SessionComponent_var temp =
      ::Components::SessionComponent::_narrow (
          this->executor_.in ()
        );
    return temp._retn ();
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename CONTEXT>
  CORBA::Object_ptr
  Servant_Impl<BASE_SKEL, EXEC, CONTEXT>::_get_component (
    )
  {
    ::Components::SessionContext_var sc =
      ::Components::SessionContext::_narrow (
          this->context_
        );

    if (! ::CORBA::is_nil (sc.in ()))
      {
        return sc->get_CCM_object ();
      }

    ::Components::EntityContext_var ec =
      ::Components::EntityContext::_narrow (
          this->context_
        );

    if (! ::CORBA::is_nil (ec.in ()))
      {
        return ec->get_CCM_object ();
      }

    throw CORBA::INTERNAL ();
  }

  // CIAO-specific operations.

  template <typename BASE_SKEL,
            typename EXEC,
            typename CONTEXT>
  void
  Servant_Impl<BASE_SKEL, EXEC, CONTEXT>::activate_component (
    )
  {
    if (this->configuration_completed_ == 1)
      {
        this->ciao_activate ();
      }
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename CONTEXT>
  void
  Servant_Impl<BASE_SKEL, EXEC, CONTEXT>::passivate_component (
    )
  {
    if (this->activated_ == 1)
      {
        this->ciao_passivate ();
      }
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename CONTEXT>
  void
  Servant_Impl<BASE_SKEL, EXEC, CONTEXT>::configuration_complete (
    )
  {
    ::Components::SessionComponent_var temp =
      ::Components::SessionComponent::_narrow (
          this->executor_.in ()
        );

    if (! ::CORBA::is_nil (temp.in ()))
      {
        if (this->configuration_completed_ == 0)
          {
            this->configuration_completed_ = 1;
            temp->configuration_complete ();
          }
      }
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename CONTEXT>
  void
  Servant_Impl<BASE_SKEL, EXEC, CONTEXT>::ciao_activate ()
  {
    ::Components::SessionComponent_var temp =
      ::Components::SessionComponent::_narrow (
          this->executor_.in ()
        );

    if (! ::CORBA::is_nil (temp.in ()))
      {
        if (this->activated_ == 0)
          {
            this->activated_ = 1;
            temp->ccm_activate ();
          }
      }
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename CONTEXT>
  CORBA::Boolean
  Servant_Impl<BASE_SKEL, EXEC, CONTEXT>::is_activated (
    )
  {
    return this->activated_;
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename CONTEXT>
  void
  Servant_Impl<BASE_SKEL, EXEC, CONTEXT>::ciao_passivate (
    )
  {
    // @@ Jai, could you please see why this is required?
    ::Components::SessionComponent_var temp =
      ::Components::SessionComponent::_narrow (
          this->executor_.in ()
        );

    if (! ::CORBA::is_nil (temp.in ()))
      temp->ccm_passivate ();

    this->activated_ = 0;
  }
}

#endif /* CIAO_SERVANT_IMPL_T_C */
