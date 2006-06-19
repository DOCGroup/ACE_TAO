// $Id$

#ifndef CIAO_HOME_SERVANT_IMPL_T_C
#define CIAO_HOME_SERVANT_IMPL_T_C

#include "Home_Servant_Impl_T.h"
#include "CIAO_common.h"

namespace CIAO
{
  template <typename BASE_SKEL,
            typename EXEC,
            typename COMP_SVNT>
  Home_Servant_Impl<BASE_SKEL,
                    EXEC,
                    COMP_SVNT>::Home_Servant_Impl (
      typename EXEC::_ptr_type exe,
      Session_Container * c,
      const char *ins_name
    )
    : Home_Servant_Impl_Base (c),
      ins_name_ (ins_name),
      executor_ (EXEC::_duplicate (exe))
  {
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename COMP_SVNT>
  Home_Servant_Impl<BASE_SKEL,
                    EXEC,
                    COMP_SVNT>::~Home_Servant_Impl (void)
  {
    CIAO_TRACE ("Home_Servant_Impl<>::destructor");

    const OBJ_ITERATOR end = this->objref_map_.end ();

    for (OBJ_ITERATOR iter = this->objref_map_.begin ();
         iter != end;
         ++iter)
      {
        this->remove_component (((*iter).int_id_).in ());
      }
  }

  // Operations for CCMHome interface.

  template <typename BASE_SKEL,
            typename EXEC,
            typename COMP_SVNT>
  void
  Home_Servant_Impl<BASE_SKEL,
                    EXEC,
                    COMP_SVNT>::remove_component (
      ::Components::CCMObject_ptr comp
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::RemoveFailure))
  {
    CIAO_TRACE ("Home_Servant_Impl<>::remove_component");

    PortableServer::ObjectId_var oid =
      this->container_->the_POA ()->reference_to_id (comp
                                                     ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    Components::CCMObject_var ccm_obj_var = Components::CCMObject::_nil ();
    if (objref_map_.find (oid.in (), ccm_obj_var) != 0)
      {
        ACE_DEBUG ((LM_DEBUG, "Invalid component object reference\n"));
        return;
      }

    typename COMP_SVNT::_stub_var_type _ciao_comp =
      COMP_SVNT::_stub_type::_narrow (ccm_obj_var.in ()
                                      ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    if (CORBA::is_nil (_ciao_comp.in ()))
      {
        ACE_THROW (Components::RemoveFailure ());
      }

    _ciao_comp->remove (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    if (CIAO::debug_level () > 3)
      {
        ACE_DEBUG ((LM_DEBUG, "removed the component\n"));
      }
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename COMP_SVNT>
  void
  Home_Servant_Impl<BASE_SKEL,
                    EXEC,
                    COMP_SVNT>::update_component_map (
      PortableServer::ObjectId &oid)
  {
    CIAO_TRACE ("update_component_map");

    Components::CCMObject_var ccm_obj_ptr;
    if (objref_map_.unbind (oid, ccm_obj_ptr) != 0)
      {
        ACE_DEBUG ((LM_DEBUG, "Invalid component object reference\n"));
        return;
      }

    return;
  }

  // Operations for keyless home interface.

  template <typename BASE_SKEL,
            typename EXEC,
            typename COMP_SVNT>
  Components::CCMObject_ptr
  Home_Servant_Impl<BASE_SKEL,
                    EXEC,
                    COMP_SVNT>::create_component (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CreateFailure))
  {
    CIAO_TRACE ("Home_Servant_Impl<>::create_component");

    return this->create (ACE_ENV_SINGLE_ARG_PARAMETER);
  }

  // Operations for implicit home interface.

  template <typename BASE_SKEL,
            typename EXEC,
            typename COMP_SVNT>
  typename COMP_SVNT::_stub_ptr_type
  Home_Servant_Impl<BASE_SKEL,
                    EXEC,
                    COMP_SVNT>::create (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CreateFailure))
  {
    CIAO_TRACE ("Home_Servant_Impl<>::create");

    if (this->executor_.in () == 0)
    {
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        COMP_SVNT::_stub_type::_nil ());
    }

    ::Components::EnterpriseComponent_var _ciao_ec =
      this->executor_->create (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (COMP::_nil ());

    typename COMP_SVNT::_exec_type::_var_type _ciao_comp =
      COMP_SVNT::_exec_type::_narrow (_ciao_ec.in ()
                                      ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (COMP::_nil ());

    return this->_ciao_activate_component (_ciao_comp.in ()
                                           ACE_ENV_ARG_PARAMETER);
  }

  // CIAO-specific operations.

  template <typename BASE_SKEL,
            typename EXEC,
            typename COMP_SVNT>
  typename COMP_SVNT::_stub_ptr_type
  Home_Servant_Impl<BASE_SKEL,
                    EXEC,
                    COMP_SVNT>::_ciao_activate_component (
      typename COMP_SVNT::_exec_type::_ptr_type exe
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    CIAO_TRACE ("Home_Servant_Impl<>::_ciao_activate_component");

    CORBA::Object_var hobj =
      this->container_->get_objref (this
                                    ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (COMP_SVNT::_stub_type::_nil ());

    Components::CCMHome_var home =
      Components::CCMHome::_narrow (hobj.in ()
                                    ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (COMP_SVNT::_stub_type::_nil ());

    COMP_SVNT *svt = 0;
    ACE_NEW_RETURN (svt,
                    COMP_SVNT (exe,
                               home.in (),
                               this->ins_name_,
                               this,
                               this->container_),
                    COMP_SVNT::_stub_type::_nil ());

    PortableServer::ServantBase_var safe (svt);
    PortableServer::ObjectId_var oid;

    CORBA::Object_var objref =
      this->container_->install_component (svt,
                                           oid.out ()
                                           ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (COMP_SVNT::_stub_type::_nil ());

    typename COMP_SVNT::_stub_var_type ho =
      COMP_SVNT::_stub_type::_narrow (objref.in ()
                                      ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (COMP_SVNT::_stub_type::_nil ());

    Components::CCMObject_var ccmobjref =
      Components::CCMObject::_narrow (objref.in ()
                                      ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (COMP_SVNT::_stub_type::_nil ());

    this->objref_map_.bind (
      oid.in (),
      Components::CCMObject::_duplicate (ccmobjref.in ()));

    return ho._retn ();
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename COMP_SVNT>
  void
  Home_Servant_Impl<BASE_SKEL,
                    EXEC,
                    COMP_SVNT>::_ciao_passivate_component (
      typename COMP_SVNT::_stub_ptr_type comp
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    CIAO_TRACE ("Home_Servant_Impl<>::_ciao_passivate_component");

    PortableServer::ObjectId_var oid;
    this->container_->uninstall_component (comp,
                                           oid.out ()
                                           ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }
}

#endif /* CIAO_HOME_SERVANT_IMPL_T_C */
