// $Id$

#ifndef CIAO_SWAPPING_SERVANT_IMPL_T_C
#define CIAO_SWAPPING_SERVANT_IMPL_T_C

#include "Swapping_Servant_Home_Impl_T.h"
#include "Dynamic_Component_Servant_T.h"

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
  Swapping_Servant_Home_Impl<BASE_SKEL,
                    EXEC,
                    EXEC_VAR,
                    COMP,
                    COMP_VAR,
                    COMP_EXEC,
                    COMP_EXEC_VAR,
                    COMP_SVNT>::Swapping_Servant_Home_Impl (
      EXEC * exe,
      Swapping_Container * c, const char* obj_id, const char* repo_id)
    : Swapping_Servant_Home_Impl_Base (c),
      executor_ (EXEC::_duplicate (exe)), obj_id_ (obj_id), repo_id_ (repo_id)
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
  Swapping_Servant_Home_Impl<BASE_SKEL,
                    EXEC,
                    EXEC_VAR,
                    COMP,
                    COMP_VAR,
                    COMP_EXEC,
                    COMP_EXEC_VAR,
                    COMP_SVNT>::~Swapping_Servant_Home_Impl (void)
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
  Swapping_Servant_Home_Impl<BASE_SKEL,
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
  Swapping_Servant_Home_Impl<BASE_SKEL,
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
  Swapping_Servant_Home_Impl<BASE_SKEL,
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

/*
    COMP_EXEC_VAR _ciao_comp =
      COMP_EXEC::_narrow (_ciao_ec.in ()
                          ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (COMP::_nil ());
*/
    return this->_ciao_activate_component (_ciao_ec.in ()
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
  Swapping_Servant_Home_Impl<BASE_SKEL,
                    EXEC,
                    EXEC_VAR,
                    COMP,
                    COMP_VAR,
                    COMP_EXEC,
                    COMP_EXEC_VAR,
                    COMP_SVNT>::_ciao_activate_component (
      ::Components::EnterpriseComponent_ptr ec
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    CORBA::Object_var hobj =
      this->container_->get_objref (this
                                    ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (COMP::_nil ());

    Components::CCMHome_var home =
      Components::CCMHome::_narrow (hobj.in ()
                                    ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (COMP::_nil ());

    PortableServer::ObjectId_var oid =
      PortableServer::string_to_ObjectId (this->obj_id_);

    CORBA::Object_var objref =
      this->container_->generate_reference (
        this->obj_id_,
        this->repo_id_,
        Container::Component
        ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (CORBA::Object::_nil ());
/*
    Dynamic_Component_Servant_Base *svt =
      new Dynamic_Component_Servant
       <COMP_SVNT, COMP_EXEC, COMP_EXEC_VAR, EXEC, EXEC_VAR, COMP>
          (this->executor_.in (), home, this->container_);
*/

    Dynamic_Component_Servant_Base *svt =
      new Dynamic_Component_Servant
       <COMP_SVNT, COMP_EXEC, COMP_EXEC_VAR, EXEC, EXEC_VAR, COMP>
          (ec, home, this->container_);

    this->container_->update_servant_map (oid, svt);


    COMP_VAR ho = COMP::_narrow (objref.in ()
                                 ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (COMP::_nil ());

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
  Swapping_Servant_Home_Impl<BASE_SKEL,
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
