// $Id$

#ifndef CIAO_SWAPPING_SERVANT_IMPL_T_C
#define CIAO_SWAPPING_SERVANT_IMPL_T_C

#include "Swapping_Servant_Impl_T.h"

namespace CIAO
{
  template <typename BASE_SKEL, 
            typename EXEC, 
            typename EXEC_VAR,
            typename COMP,
            typename COMP_VAR,
            typename COMP_EXEC,
            typename COMP_EXEC_VAR,
            typename COMP_SVNT>
  Swapping_Servant_Impl<BASE_SKEL,
                    EXEC,
                    EXEC_VAR,
                    COMP,
                    COMP_VAR,
                    COMP_EXEC,
                    COMP_EXEC_VAR,
                    COMP_SVNT>::Swapping_Servant_Impl (
      EXEC * exe,
      SWapping_Container * c
    )
    : Swapping_Servant_Impl_Base (c),
      executor_ (EXEC::_duplicate (exe))
  {
  }

  template <typename BASE_SKEL, 
            typename EXEC, 
            typename EXEC_VAR,
            typename COMP,
            typename COMP_VAR,
            typename COMP_EXEC,
            typename COMP_EXEC_VAR,
            typename COMP_SVNT>
  Swapping_Servant_Impl<BASE_SKEL,
                    EXEC,
                    EXEC_VAR,
                    COMP,
                    COMP_VAR,
                    COMP_EXEC,
                    COMP_EXEC_VAR,
                    COMP_SVNT>::~Swapping_Servant_Impl (void)
  {
  }

  // Operations for CCMHome interface.

  template <typename BASE_SKEL, 
            typename EXEC, 
            typename EXEC_VAR,
            typename COMP,
            typename COMP_VAR,
            typename COMP_EXEC,
            typename COMP_EXEC_VAR,
            typename COMP_SVNT>
  void
  Swapping_Servant_Impl<BASE_SKEL,
                    EXEC,
                    EXEC_VAR,
                    COMP,
                    COMP_VAR,
                    COMP_EXEC,
                    COMP_EXEC_VAR,
                    COMP_SVNT>::remove_component (
      ::Components::CCMObject_ptr comp
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::RemoveFailure))
  {
    COMP_VAR _ciao_comp = COMP::_narrow (comp
                                         ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    if (CORBA::is_nil (_ciao_comp.in ()))
    {
      ACE_THROW (CORBA::INTERNAL ());
    }

    _ciao_comp->remove (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    this->_ciao_passivate_component (_ciao_comp.in ()
                                     ACE_ENV_ARG_PARAMETER);
  }

  // Operations for keyless home interface.

  template <typename BASE_SKEL, 
            typename EXEC, 
            typename EXEC_VAR,
            typename COMP,
            typename COMP_VAR,
            typename COMP_EXEC,
            typename COMP_EXEC_VAR,
            typename COMP_SVNT>
  Components::CCMObject_ptr
  Swapping_Servant_Impl<BASE_SKEL,
                    EXEC,
                    EXEC_VAR,
                    COMP,
                    COMP_VAR,
                    COMP_EXEC,
                    COMP_EXEC_VAR,
                    COMP_SVNT>::create_component (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CreateFailure))
  {
    return this->create (ACE_ENV_SINGLE_ARG_PARAMETER);
  }

  // Operations for implicit home interface.

  template <typename BASE_SKEL, 
            typename EXEC, 
            typename EXEC_VAR,
            typename COMP,
            typename COMP_VAR,
            typename COMP_EXEC,
            typename COMP_EXEC_VAR,
            typename COMP_SVNT>
  COMP *
  Swapping_Servant_Impl<BASE_SKEL,
                    EXEC,
                    EXEC_VAR,
                    COMP,
                    COMP_VAR,
                    COMP_EXEC,
                    COMP_EXEC_VAR,
                    COMP_SVNT>::create (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CreateFailure))
  {
    if (this->executor_.in () == 0)
    {
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        COMP::_nil ());
    }

    ::Components::EnterpriseComponent_var _ciao_ec =
      this->executor_->create (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (COMP::_nil ());

    COMP_EXEC_VAR _ciao_comp =
      COMP_EXEC::_narrow (_ciao_ec.in ()
                          ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (COMP::_nil ());

    return this->_ciao_activate_component (_ciao_comp.in ()
                                           ACE_ENV_ARG_PARAMETER);
  }

  // CIAO-specific operations.

  template <typename BASE_SKEL, 
            typename EXEC, 
            typename EXEC_VAR,
            typename COMP,
            typename COMP_VAR,
            typename COMP_EXEC,
            typename COMP_EXEC_VAR,
            typename COMP_SVNT>
  COMP *
  Swapping_Servant_Impl<BASE_SKEL,
                    EXEC,
                    EXEC_VAR,
                    COMP,
                    COMP_VAR,
                    COMP_EXEC,
                    COMP_EXEC_VAR,
                    COMP_SVNT>::_ciao_activate_component (
      COMP_EXEC *exe
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    const char* obj_id = "composition_name_home_name";
    const char* repo_id = "repo_id";

    PortableServer::ObjectId_var oid =
      PortableServer::string_to_ObjectId (obj_id);

    CORBA::Object_var objref = this->container_->generate_reference
                                 (obj_id, repo_id,
                                  Container::Component
                                  ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (CORBA::Object::_nil ());

    COMP_VAR ho = COMP::_narrow (objref.in ()
                                 ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (COMP::_nil ());

    if (this->component_map_.bind (oid.in (), svt) == 0)
      {
        safe._retn ();
      }

    return ho._retn ();
  }

  template <typename BASE_SKEL, 
            typename EXEC, 
            typename EXEC_VAR,
            typename COMP,
            typename COMP_VAR,
            typename COMP_EXEC,
            typename COMP_EXEC_VAR,
            typename COMP_SVNT>
  void
  Swapping_Servant_Impl<BASE_SKEL,
                    EXEC,
                    EXEC_VAR,
                    COMP,
                    COMP_VAR,
                    COMP_EXEC,
                    COMP_EXEC_VAR,
                    COMP_SVNT>::_ciao_passivate_component (
      COMP *comp
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    PortableServer::ObjectId_var oid;
    this->container_->uninstall_component (comp,
                                           oid.out ()
                                           ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    COMP_SVNT *servant = 0;

    if (this->component_map_.unbind (oid.in (), servant) == 0)
    {
      PortableServer::ServantBase_var safe (servant);

      servant->_ciao_passivate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
  }
}

#endif /* CIAO_SWAPPING_SERVANT_IMPL_T_C */
