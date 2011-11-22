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
            typename COMP_SVNT,
            typename CONTAINER>
  Home_Servant_Impl<BASE_SKEL,
                    EXEC,
                    COMP_SVNT,
                    CONTAINER>::Home_Servant_Impl (
      typename EXEC::_ptr_type exe,
      typename CONTAINER::_ptr_type c,
      const char *ins_name)
    : Home_Servant_Impl_Base (),
      ins_name_ (ins_name),
      executor_ (EXEC::_duplicate (exe)),
      serial_number_ (0),
      container_ (CONTAINER::_duplicate(c))
  {
    CIAO_DEBUG (9, (LM_TRACE, CLINFO "Home_Servant_Impl<>::Home_Servant_Impl - "
                 "Creating servant for home with ID %C\n",
                 ins_name));
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename COMP_SVNT,
            typename CONTAINER>
  Home_Servant_Impl<BASE_SKEL,
                    EXEC,
                    COMP_SVNT,
                    CONTAINER>::~Home_Servant_Impl (void)
  {
    CIAO_TRACE ("Home_Servant_Impl<>::destructor");

    this->remove_components ();
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename COMP_SVNT,
            typename CONTAINER>
  void
  Home_Servant_Impl<BASE_SKEL,
                    EXEC,
                    COMP_SVNT,
                    CONTAINER>::remove_components (void)
  {
    while (this->objref_map_.current_size () > 0)
      {
        OBJ_ITERATOR const first = this->objref_map_.begin ();
        this->remove_component (((*first).int_id_).in ());
        if (this->objref_map_.unbind (first) != 0)
          {
            CIAO_ERROR (1, (LM_WARNING, CLINFO
                        "Home_Servant_Impl<>::remove_components - "
                        "Failed to unbind component\n"));
            throw Components::RemoveFailure ();
          }
      }
  }

  // Operations for CCMHome interface.

  template <typename BASE_SKEL,
            typename EXEC,
            typename COMP_SVNT,
            typename CONTAINER>
  void
  Home_Servant_Impl<BASE_SKEL,
                    EXEC,
                    COMP_SVNT,
                    CONTAINER>::remove_component (
      ::Components::CCMObject_ptr comp)
  {
    CIAO_TRACE ("Home_Servant_Impl<>::remove_component");

    typename CONTAINER::_var_type cnt_safe =
      CONTAINER::_duplicate (this->container_.in());
    PortableServer::POA_var poa = cnt_safe->the_POA ();
    PortableServer::ObjectId_var oid = poa->reference_to_id (comp);

    Components::CCMObject_var ccm_obj_var;
    if (objref_map_.find (oid.in (), ccm_obj_var) != 0)
      {
        CIAO_ERROR (1, (LM_WARNING, CLINFO
                    "Home_Servant_Impl<>::remove_component - "
                    "Invalid component object reference\n"));
        throw Components::RemoveFailure ();
      }

    typedef typename COMP_SVNT::_stub_type stub_type;
    typename COMP_SVNT::_stub_var_type _ciao_comp =
      stub_type::_narrow (ccm_obj_var.in ());

    if (::CORBA::is_nil (_ciao_comp.in ()))
      {
        throw Components::RemoveFailure ();
      }
    else
      {
        _ciao_comp->remove ();
      }

    CIAO_DEBUG (6, (LM_INFO, CLINFO
                "Home_Servant_Impl<>::remove_component - "
                "Removed the component\n"));
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename COMP_SVNT,
            typename CONTAINER>
  void
  Home_Servant_Impl<BASE_SKEL,
                    EXEC,
                    COMP_SVNT,
                    CONTAINER>::update_component_map (
      PortableServer::ObjectId &oid)
  {
    CIAO_TRACE ("Home_Servant_Impl<>::update_component_map");

    Components::CCMObject_var ccm_obj_ptr;
    if (objref_map_.unbind (oid, ccm_obj_ptr) != 0)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                    "Home_Servant_Impl<>::update_component_map - "
                    "Invalid component object reference\n"));
      }
  }

  // Operations for keyless home interface.
  template <typename BASE_SKEL,
            typename EXEC,
            typename COMP_SVNT,
            typename CONTAINER>
  Components::CCMObject_ptr
  Home_Servant_Impl<BASE_SKEL,
                    EXEC,
                    COMP_SVNT,
                    CONTAINER>::create_component (void)
  {
    CIAO_TRACE ("Home_Servant_Impl<>::create_component");

    return this->create ();
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename COMP_SVNT,
            typename CONTAINER>
  typename COMP_SVNT::_stub_ptr_type
  Home_Servant_Impl<BASE_SKEL,
                    EXEC,
                    COMP_SVNT,
                    CONTAINER>::create (void)
  {
    CIAO_TRACE ("Home_Servant_Impl<>::create");

    if (::CORBA::is_nil (this->executor_.in ()))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                    "Home_Servant_Impl<>:create - "
                    "nil executor reference\n"));
        throw CORBA::INTERNAL ();
      }

    typename EXEC::_var_type exec_safe =
      EXEC::_duplicate (this->executor_.in());

    ::Components::EnterpriseComponent_var _ciao_ec = exec_safe->create ();

    typedef typename COMP_SVNT::_exec_type exec_type;
    typename COMP_SVNT::_exec_type::_var_type _ciao_comp =
      exec_type::_narrow (_ciao_ec.in ());

    return this->_ciao_activate_component (_ciao_comp.in ());
  }

  template <typename BASE_SKEL,
            typename EXEC,
            typename COMP_SVNT,
            typename CONTAINER>
  typename COMP_SVNT::_stub_ptr_type
  Home_Servant_Impl<BASE_SKEL,
                    EXEC,
                    COMP_SVNT,
                    CONTAINER>::_ciao_activate_component (
      typename COMP_SVNT::_exec_type::_ptr_type exe)
  {
    CIAO_TRACE ("Home_Servant_Impl<>::_ciao_activate_component");

    typename CONTAINER::_var_type cnt_safe =
      CONTAINER::_duplicate (this->container_.in());

    CORBA::Object_var hobj = cnt_safe->get_objref (this);

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
                                 cnt_safe),
                      CORBA::NO_MEMORY ());

    PortableServer::ServantBase_var safe (svt);
    PortableServer::ObjectId_var oid;

    CORBA::Object_var objref =
      cnt_safe->install_servant (svt,
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
            typename COMP_SVNT,
            typename CONTAINER>
  void
  Home_Servant_Impl<BASE_SKEL,
                    EXEC,
                    COMP_SVNT,
                    CONTAINER>::_ciao_passivate_component (
      typename COMP_SVNT::_stub_ptr_type comp)
  {
    CIAO_TRACE ("Home_Servant_Impl<>::_ciao_passivate_component");

    typename CONTAINER::_var_type cnt_safe =
      CONTAINER::_duplicate (this->container_.in());

    cnt_safe->uninstall_component (comp);
  }
}

#endif /* CIAO_HOME_SERVANT_IMPL_T_C */
