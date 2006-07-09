// $Id$

#ifndef CIAO_SWAPPING_SERVANT_IMPL_T_C
#define CIAO_SWAPPING_SERVANT_IMPL_T_C

#include "ciao/Swapping_Servant_Home_Impl_T.h"
#include "ciao/Dynamic_Component_Servant_T.h"

namespace CIAO
{
  template <typename BASE_SKEL,
            typename EXEC,
            typename COMP_SVNT>
  Swapping_Home_Servant_Impl<BASE_SKEL,
                             EXEC,
                             COMP_SVNT>::Swapping_Home_Servant_Impl (
    typename EXEC::_ptr_type exe,
    Session_Container * c,
    const char* ins_name,
    const char* obj_id,
    const char* repo_id)
    : Home_Servant_Impl_Base (c),
      executor_ (EXEC::_duplicate (exe)),
      ins_name_ (ins_name),
      obj_id_ (obj_id),
      repo_id_ (repo_id)
  {
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename COMP_SVNT>
  Swapping_Home_Servant_Impl<BASE_SKEL,
                             EXEC,
                             COMP_SVNT>::~Swapping_Home_Servant_Impl ()
  {
    const DYNAMIC_SERVANT_MAP_ITERATOR end =
      this->dynamic_servant_map_.end ();

    PortableServer::ObjectId_var oid =
      PortableServer::string_to_ObjectId (this->obj_id_);

    ACE_TRY_NEW_ENV;
      {
        this->container_->delete_servant_map (oid ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }
      ACE_CATCHANY;
      {
        ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                             "~Swapping_Home_Servant_Impl\t\n");
      }
    ACE_ENDTRY;

    for (DYNAMIC_SERVANT_MAP_ITERATOR iter =
           this->dynamic_servant_map_.begin ();
         iter != end; ++iter)
      {
        (*iter).int_id_->destroy (oid);
      }
  }

  // Operations for CCMHome interface.

  template <typename BASE_SKEL,
            typename EXEC,
            typename COMP_SVNT>
  void
  Swapping_Home_Servant_Impl<BASE_SKEL,
                             EXEC,
                             COMP_SVNT>::remove_component (
      ::Components::CCMObject_ptr
      ACE_ENV_ARG_DECL_NOT_USED
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::RemoveFailure))
  {
    PortableServer::ObjectId_var oid =
      PortableServer::string_to_ObjectId (this->obj_id_);

    Dynamic_Component_Servant_Base *servant = 0;

    if (this->dynamic_servant_map_.find (oid.in (), servant) == 0)
    {
      servant->destroy (oid);
    }
  }

  // Operations for keyless home interface.

  template <typename BASE_SKEL,
            typename EXEC,
            typename COMP_SVNT>
  Components::CCMObject_ptr
  Swapping_Home_Servant_Impl<BASE_SKEL,
                             EXEC,
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
            typename COMP_SVNT>
  typename COMP_SVNT::_stub_ptr_type
  Swapping_Home_Servant_Impl<BASE_SKEL,
                             EXEC,
                             COMP_SVNT>::create (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CreateFailure))
  {
    if (this->executor_.in () == 0)
    {
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        COMP_SVNT::_stub_type::_nil ());
    }


    ::Components::EnterpriseComponent_var _ciao_ec =
      this->executor_->create (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (COMP_SVNT::_stub_type::_nil ());

    return this->_ciao_activate_component (_ciao_ec.in ()
                                           ACE_ENV_ARG_PARAMETER);
  }

  // CIAO-specific operations.

  template <typename BASE_SKEL,
            typename EXEC,
            typename COMP_SVNT>
  typename COMP_SVNT::_stub_ptr_type
  Swapping_Home_Servant_Impl<BASE_SKEL,
                             EXEC,
                             COMP_SVNT>::_ciao_activate_component (
      ::Components::EnterpriseComponent_ptr ec
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    CORBA::Object_var hobj =
      this->container_->get_home_objref (this
                                    ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (COMP_SVNT::_stub_type::_nil ());

    Components::CCMHome_var home =
      Components::CCMHome::_narrow (hobj.in ()
                                    ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (COMP_SVNT::_stub_type::_nil ());

    PortableServer::ObjectId_var oid =
      PortableServer::string_to_ObjectId (this->obj_id_);

    CORBA::Object_var objref =
      this->container_->generate_reference (
        this->obj_id_,
        this->repo_id_,
        Container::Component
        ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (COMP_SVNT::_stub_type::_nil ());

    Dynamic_Component_Servant_Base *svt =
      new Dynamic_Component_Servant<COMP_SVNT> (ec,
                                                home.in (),
                                                this->ins_name_,
                                                this,
                                                this->container_);

    this->container_->add_servant_map (oid, svt ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (COMP_SVNT::_stub_type::_nil ());

    this->dynamic_servant_map_.bind (oid.in (), svt);

    typedef typename COMP_SVNT::_stub_type stub_type;
    typename COMP_SVNT::_stub_var_type ho =
      stub_type::_narrow (objref.in ()
                          ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (COMP_SVNT::_stub_type::_nil ());

    return ho._retn ();
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename COMP_SVNT>
  void
  Swapping_Home_Servant_Impl<BASE_SKEL,
                             EXEC,
                             COMP_SVNT>::update_component_map (
    PortableServer::ObjectId &oid)
  {
    Dynamic_Component_Servant_Base *servant = 0;

    if (this->dynamic_servant_map_.find (oid, servant) == 0)
      {
        servant->update_destroy_count ();
        this->dynamic_servant_map_.unbind (oid);
      }

    return;
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename COMP_SVNT>
  void
  Swapping_Home_Servant_Impl<BASE_SKEL,
                             EXEC,
                             COMP_SVNT>::_ciao_passivate_component (
      typename COMP_SVNT::_stub_ptr_type comp
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

    if (this->component_map_.find (oid.in (), servant) == 0)
    {
      PortableServer::ServantBase_var safe (servant);

      servant->ciao_passivate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      this->component_map_.unbind (oid.in ());
    }
  }
}

#endif /* CIAO_SWAPPING_SERVANT_IMPL_T_C */
