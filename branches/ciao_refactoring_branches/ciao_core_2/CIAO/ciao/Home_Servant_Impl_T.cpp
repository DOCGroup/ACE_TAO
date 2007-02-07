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
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::RemoveFailure))
  {
    CIAO_TRACE ("Home_Servant_Impl<>::remove_component");

    PortableServer::ObjectId_var oid =
      this->container_->the_POA ()->reference_to_id (comp);

    Components::CCMObject_var ccm_obj_var = Components::CCMObject::_nil ();
    if (objref_map_.find (oid.in (), ccm_obj_var) != 0)
      {
        ACE_DEBUG ((LM_DEBUG, "Invalid component object reference\n"));
        return;
      }

    typedef typename COMP_SVNT::_stub_type stub_type;
    typename COMP_SVNT::_stub_var_type _ciao_comp =
      stub_type::_narrow (ccm_obj_var.in ());

    if (CORBA::is_nil (_ciao_comp.in ()))
      {
        throw Components::RemoveFailure ();
      }

    _ciao_comp->remove ();

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
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CreateFailure))
  {
    CIAO_TRACE ("Home_Servant_Impl<>::create_component");

    return this->create ();
  }

  // Operations for implicit home interface.

  template <typename BASE_SKEL,
            typename EXEC,
            typename COMP_SVNT>
  typename COMP_SVNT::_stub_ptr_type
  Home_Servant_Impl<BASE_SKEL,
                    EXEC,
                    COMP_SVNT>::create (
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CreateFailure))
  {
    CIAO_TRACE ("Home_Servant_Impl<>::create");

    if (this->executor_.in () == 0)
      {
        throw CORBA::INTERNAL ();
      }

    ::Components::EnterpriseComponent_var _ciao_ec =
      this->executor_->create ();

	typedef typename COMP_SVNT::_exec_type exec_type;
    typename COMP_SVNT::_exec_type::_var_type _ciao_comp =
      exec_type::_narrow (_ciao_ec.in ());

    return this->_ciao_activate_component (_ciao_comp.in ());
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
    )
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    CIAO_TRACE ("Home_Servant_Impl<>::_ciao_activate_component");

    CORBA::Object_var hobj =
      this->container_->get_objref (this);

    Components::CCMHome_var home =
      Components::CCMHome::_narrow (hobj.in ());

    typedef typename COMP_SVNT::_stub_type stub_type;
    COMP_SVNT *svt = 0;
    ACE_NEW_RETURN (svt,
                    COMP_SVNT (exe,
                               home.in (),
                               this->ins_name_,
                               this,
                               this->container_),
                    stub_type::_nil ());

    PortableServer::ServantBase_var safe (svt);
    PortableServer::ObjectId_var oid;

    CORBA::Object_var objref =
      this->container_->install_component (svt,
                                           oid.out ());

    typedef typename COMP_SVNT::_stub_type stub_type;
    typename COMP_SVNT::_stub_var_type ho =
      stub_type::_narrow (objref.in ());

    Components::CCMObject_var ccmobjref =
      Components::CCMObject::_narrow (objref.in ());

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
    )
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    CIAO_TRACE ("Home_Servant_Impl<>::_ciao_passivate_component");

    PortableServer::ObjectId_var oid;
    this->container_->uninstall_component (comp,
                                           oid.out ());
  }
}

#endif /* CIAO_HOME_SERVANT_IMPL_T_C */
