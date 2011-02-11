// $Id$

#ifndef CIAO_CONNECTOR_SERVANT_IMPL_UTILS_T_C
#define CIAO_CONNECTOR_SERVANT_IMPL_UTILS_T_C

#include "ciao/Servants/Connector_Servant_Impl_Base.h"
#include "ciao/Valuetype_Factories/Cookies.h"
#include "ciao/Logger/Log_Macros.h"

#if !defined (CCM_LW)

namespace CIAO
{
  template<typename T>
  void
  Servant::describe_simplex_receptacle (
      const char *port_name,
      const char *port_type_repo_id,
      typename T::_ptr_type connection,
      ::Components::ReceptacleDescriptions_var &descriptions,
      CORBA::ULong slot)
  {
    CIAO_TRACE ("Servant::describe_simplex_receptacle");

    ::Components::ReceptacleDescription *elem = 0;
    ACE_NEW_THROW_EX (elem,
                      ::OBV_Components::ReceptacleDescription,
                      CORBA::NO_MEMORY ());

    ::Components::ReceptacleDescription_var safe_elem = elem;

    elem->name (port_name);
    elem->type_id (port_type_repo_id);
    elem->is_multiple (false);
    elem->connections ().length (1UL);

    ::Components::ConnectionDescription *conn = 0;
    ACE_NEW (conn, ::OBV_Components::ConnectionDescription);
    ::Components::ConnectionDescription_var safe_conn = conn;

    conn->ck (0);
    conn->objref (connection);

    elem->connections ()[0UL] = safe_conn._retn ();
    descriptions[slot] = safe_elem._retn ();
  }

  template<typename T>
  void
  Servant::describe_multiplex_receptacle (
    const char *port_name,
    const char *port_type_repo_id,
    const T &objrefs,
    ::Components::ReceptacleDescriptions_var &descriptions,
    CORBA::ULong slot)
  {
    CIAO_TRACE ("Servant::describe_multiplex_receptacle");

    ::Components::ReceptacleDescription *elem = 0;
    ACE_NEW_THROW_EX (elem,
                      ::OBV_Components::ReceptacleDescription,
                      CORBA::NO_MEMORY ());

    ::Components::ReceptacleDescription_var safe_elem = elem;

    elem->name (port_name);
    elem->type_id (port_type_repo_id);
    elem->is_multiple (true);
    elem->connections ().length (objrefs.length ());

    for (CORBA::ULong i = 0UL; i < objrefs.length (); ++i)
      {
        ::Components::ConnectionDescription *conn = 0;
        ACE_NEW_THROW_EX (conn,
                          ::OBV_Components::ConnectionDescription,
                          CORBA::NO_MEMORY ());
        ::Components::ConnectionDescription_var safe_conn = conn;

        safe_conn->ck (objrefs[i].ck.in ());
        safe_conn->objref (objrefs[i].objref);
        elem->connections ()[i] = safe_conn._retn ();
      }

    descriptions[slot] = safe_elem._retn ();
  }
}
#endif

#endif /* CIAO_CONNECTOR_SERVANT_IMPL_UTILS_T_C */
