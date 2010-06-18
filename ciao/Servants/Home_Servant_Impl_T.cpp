// $Id$

#ifndef CIAO_HOME_SERVANT_IMPL_T_C
#define CIAO_HOME_SERVANT_IMPL_T_C

#include "Home_Servant_Impl_T.h"
#include "ccm/CCM_ObjectC.h"
#include "ccm/CCM_EnterpriseComponentC.h"
#include "ciao/Containers/Container_BaseC.h"
#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  template <typename BASE_SKEL,
            typename EXEC,
            typename COMP_SVNT>
  Home_Servant_Impl<BASE_SKEL,
                    EXEC,
                    COMP_SVNT>::Home_Servant_Impl (
      typename EXEC::_ptr_type exe,
      Container_ptr c,
      const char *ins_name
    )
    : Home_Servant_Impl_Base (c),
      ins_name_ (ins_name),
      executor_ (EXEC::_duplicate (exe)),
      serial_number_ (0)
  {
    CIAO_DEBUG (9, (LM_TRACE, CLINFO "Home_Servant_Impl<>::Home_Servant_Impl - "
                 "Creating servant for home with ID %C\n",
                 ins_name));
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename COMP_SVNT>
  Home_Servant_Impl<BASE_SKEL,
                    EXEC,
                    COMP_SVNT>::~Home_Servant_Impl (void)
  {
    CIAO_TRACE ("Home_Servant_Impl<>::destructor");

    OBJ_ITERATOR const end = this->objref_map_.end ();

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
      ::Components::CCMObject_ptr comp)
  {
    CIAO_TRACE ("Home_Servant_Impl<>::remove_component");

    PortableServer::ObjectId_var oid =
      this->container_->the_POA ()->reference_to_id (comp);

    Components::CCMObject_var ccm_obj_var = Components::CCMObject::_nil ();
    if (objref_map_.find (oid.in (), ccm_obj_var) != 0)
      {
        CIAO_ERROR (1, (LM_WARNING, CLINFO "Home_Servant_Impl<>::remove_component - Invalid component object reference\n"));
        throw Components::RemoveFailure ();
      }

    typedef typename COMP_SVNT::_stub_type stub_type;
    typename COMP_SVNT::_stub_var_type _ciao_comp =
      stub_type::_narrow (ccm_obj_var.in ());

    if (CORBA::is_nil (_ciao_comp.in ()))
      {
        throw Components::RemoveFailure ();
      }
    else
      {
        _ciao_comp->remove ();
      }

    CIAO_DEBUG (6, (LM_INFO, CLINFO "Home_Servant_Impl<>::remove_component - Removed the component\n"));
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
    CIAO_TRACE ("Home_Servant_Impl<>::update_component_map");

    Components::CCMObject_var ccm_obj_ptr;
    if (objref_map_.unbind (oid, ccm_obj_ptr) != 0)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO "Home_Servant_Impl<>::update_component_map - "
                     "Invalid component object reference\n"));
      }
  }

  // Operations for keyless home interface.

  template <typename BASE_SKEL,
            typename EXEC,
            typename COMP_SVNT>
  Components::CCMObject_ptr
  Home_Servant_Impl<BASE_SKEL,
                    EXEC,
                    COMP_SVNT>::create_component (void)
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
                    COMP_SVNT>::create (void)
  {
    CIAO_TRACE ("Home_Servant_Impl<>::create");

    if (::CORBA::is_nil (this->executor_.in ()))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO "Home_Servant_Impl<>:create - nil executor reference\n"));
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
      typename COMP_SVNT::_exec_type::_ptr_type exe)
  {
    CIAO_TRACE ("Home_Servant_Impl<>::_ciao_activate_component");
    

    CORBA::Object_var hobj = this->container_->get_objref (this);

    Components::CCMHome_var home =
      Components::CCMHome::_narrow (hobj.in ());

    char buffer[256];
    unsigned long const serial = this->serial_number_++;
    if (ACE_OS::sprintf (buffer, "%ld", serial) < 0)
      {
        throw CORBA::INTERNAL ();
      }

    typedef typename COMP_SVNT::_stub_type stub_type;
    COMP_SVNT *svt = 0;
    ACE_NEW_THROW_EX (svt,
                      COMP_SVNT (exe,
                                 home.in (),
                                 (this->ins_name_ + buffer).c_str (),
                                 this,
                                 this->container_),
                      CORBA::NO_MEMORY ());

    PortableServer::ServantBase_var safe (svt);
    PortableServer::ObjectId_var oid;

    CORBA::Object_var objref =
      this->container_->install_servant (svt,
                                         Container_Types::COMPONENT_t,
                                         oid.out ());
    typedef typename COMP_SVNT::_stub_type stub_type;
    typename COMP_SVNT::_stub_var_type ho = stub_type::_narrow (objref.in ());

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
      typename COMP_SVNT::_stub_ptr_type comp)
  {
    CIAO_TRACE ("Home_Servant_Impl<>::_ciao_passivate_component");

    this->container_->uninstall_component (comp);
  }
}

#endif /* CIAO_HOME_SERVANT_IMPL_T_C */
