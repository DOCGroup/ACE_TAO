// $Id$

#ifndef CIAO_SERVANT_IMPL_T_C
#define CIAO_SERVANT_IMPL_T_C

#include "Servant_Impl_T.h"

namespace CIAO
{
  template <typename BASE_SKEL,
            typename EXEC,
            typename EXEC_VAR,
            typename CONTEXT>
  Servant_Impl<BASE_SKEL, EXEC, EXEC_VAR, CONTEXT>::Servant_Impl (
      EXEC * exe,
      Session_Container * c
    )
    : Servant_Impl_Base (c),
      executor_ (EXEC::_duplicate (exe))
  {
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename EXEC_VAR,
            typename CONTEXT>
  Servant_Impl<BASE_SKEL, EXEC, EXEC_VAR, CONTEXT>::~Servant_Impl (void)
  {
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
        }
    }
    ACE_CATCHANY
    {
    }
    ACE_ENDTRY;

    this->context_->_remove_ref ();
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename EXEC_VAR,
            typename CONTEXT>
  ::Components::FacetDescriptions *
  Servant_Impl<BASE_SKEL, EXEC, EXEC_VAR, CONTEXT>::get_all_facets (
      ACE_ENV_SINGLE_ARG_DECL_NOT_USED
    )
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ::Components::FacetDescriptions *tmp = 0;
    ACE_NEW_RETURN (tmp,
                    ::Components::FacetDescriptions,
                    0);

    ::Components::FacetDescriptions_var retval = tmp;

    retval->length (this->facet_table_.current_size ());
    CORBA::ULong i = 0;

    for (FacetTable::iterator iter = this->facet_table_.begin ();
         iter != this->facet_table_.end ();
         ++iter, ++i)
      {
        FacetTable::ENTRY & entry = *iter;
        retval[i] = entry.int_id_;
      }

    return retval._retn ();
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename EXEC_VAR,
            typename CONTEXT>
  ::Components::ConsumerDescriptions *
  Servant_Impl<BASE_SKEL, EXEC, EXEC_VAR, CONTEXT>::get_all_consumers (
      ACE_ENV_SINGLE_ARG_DECL_NOT_USED
    )
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ::Components::ConsumerDescriptions *tmp = 0;
    ACE_NEW_RETURN (tmp,
                    ::Components::ConsumerDescriptions,
                    0);

    ::Components::ConsumerDescriptions_var retval = tmp;

    retval->length (this->consumer_table_.current_size ());
    CORBA::ULong i = 0;

    for (ConsumerTable::iterator iter = this->consumer_table_.begin ();
         iter != this->consumer_table_.end ();
         ++iter, ++i)
      {
        ConsumerTable::ENTRY & entry = *iter;
        retval[i] = entry.int_id_;
      }

    return retval._retn ();
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename EXEC_VAR,
            typename CONTEXT>
  CORBA::Boolean
  Servant_Impl<BASE_SKEL, EXEC, EXEC_VAR, CONTEXT>::same_component (
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
            typename EXEC_VAR,
            typename CONTEXT>
  ::Components::CCMHome_ptr
  Servant_Impl<BASE_SKEL, EXEC, EXEC_VAR, CONTEXT>::get_ccm_home (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return this->context_->get_CCM_home (ACE_ENV_SINGLE_ARG_PARAMETER);
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename EXEC_VAR,
            typename CONTEXT>
  ::Components::PrimaryKeyBase *
  Servant_Impl<BASE_SKEL, EXEC, EXEC_VAR, CONTEXT>::get_primary_key (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ::Components::NoKeyAvailable))
  {
    ACE_THROW_RETURN (::Components::NoKeyAvailable (), 0);
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename EXEC_VAR,
            typename CONTEXT>
  ::Components::ComponentPortDescription *
  Servant_Impl<BASE_SKEL, EXEC, EXEC_VAR, CONTEXT>::get_all_ports (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    OBV_Components::ComponentPortDescription *cps = 0;
    ACE_NEW_RETURN (cps,
                    OBV_Components::ComponentPortDescription,
                    0);
    ::Components::ComponentPortDescription_var retv = cps;

    ::Components::FacetDescriptions_var facets_desc =
      this->get_all_facets (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);

    ::Components::ReceptacleDescriptions_var receptacle_desc =
      this->get_all_receptacles (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);

    ::Components::ConsumerDescriptions_var consumer_desc =
      this->get_all_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);

    ::Components::EmitterDescriptions_var emitter_desc =
      this->get_all_emitters (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);

    ::Components::PublisherDescriptions_var publisher_desc =
      this->get_all_publishers (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);

    retv->facets (facets_desc.in ());
    retv->receptacles (receptacle_desc.in ());
    retv->consumers (consumer_desc.in ());
    retv->emitters (emitter_desc.in ());
    retv->publishers (publisher_desc.in ());

    return retv._retn ();
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename EXEC_VAR,
            typename CONTEXT>
  CORBA::Object_ptr
  Servant_Impl<BASE_SKEL, EXEC, EXEC_VAR, CONTEXT>::_get_component (
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
            typename EXEC_VAR,
            typename CONTEXT>
  void
  Servant_Impl<BASE_SKEL, EXEC, EXEC_VAR, CONTEXT>::ciao_preactivate (
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
        temp->ciao_preactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
      }
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename EXEC_VAR,
            typename CONTEXT>
  void
  Servant_Impl<BASE_SKEL, EXEC, EXEC_VAR, CONTEXT>::ciao_activate (
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
        temp->ccm_activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      }
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename EXEC_VAR,
            typename CONTEXT>
  void
  Servant_Impl<BASE_SKEL, EXEC, EXEC_VAR, CONTEXT>::ciao_postactivate (
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
        temp->ciao_postactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
      }
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename EXEC_VAR,
            typename CONTEXT>
  void
  Servant_Impl<BASE_SKEL, EXEC, EXEC_VAR, CONTEXT>::_ciao_passivate (
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
        temp->ccm_passivate (ACE_ENV_SINGLE_ARG_PARAMETER);
      }
  }
}

#endif /* CIAO_SERVANT_IMPL_T_C */
