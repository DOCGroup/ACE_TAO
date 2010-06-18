// $Id$

#ifndef CIAO_CONNECTOR_SERVANT_IMPL_UTILS_T_C
#define CIAO_CONNECTOR_SERVANT_IMPL_UTILS_T_C

#include "ciao/Servants/Connector_Servant_Impl_Base.h"
#include "ciao/Valuetype_Factories/Cookies.h"
#include "ciao/Logger/Log_Macros.h"

#if !defined (CCM_LW)

namespace CIAO
{
  template<typename T_var>
  void
  Servant::describe_simplex_receptacle (
      const char *port_name,
      const char *port_type_repo_id,
      T_var &connection,
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
    conn->objref (connection.in ());

    elem->connections ()[0UL] = safe_conn._retn ();
    descriptions[slot] = safe_elem._retn ();
  }

  template<typename T_var>
  void
  Servant::describe_multiplex_receptacle (
    const char *port_name,
    const char *port_type_repo_id,
    ACE_Array_Map<ptrdiff_t, T_var> &objrefs,
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
    elem->connections ().length (objrefs.size ());

    CORBA::ULong seq_slot = 0UL;
    ::Components::ConnectionDescription *conn = 0;

    typedef typename ACE_Array_Map<ptrdiff_t, T_var>::const_iterator
      CONST_ITERATOR;

    for (CONST_ITERATOR iter = objrefs.begin ();
         iter != objrefs.end ();
         ++iter, ++seq_slot)
      {
        ACE_NEW_THROW_EX (conn,
                          ::OBV_Components::ConnectionDescription,
                          CORBA::NO_MEMORY ());
        ::Components::ConnectionDescription_var safe_conn = conn;

        ::Components::Cookie *key_cookie = 0;

        ACE_NEW_THROW_EX (key_cookie,
                          CIAO::Cookie_Impl (iter->first),
                          CORBA::NO_MEMORY ());

        // Valuetype member set operation calls add_ref.
        conn->ck (key_cookie);
        CORBA::remove_ref (key_cookie);

        conn->objref (iter->second.in ());

        elem->connections ()[seq_slot] = safe_conn._retn ();
      }

    descriptions[slot] = safe_elem._retn ();
  }
}
#endif

#endif /* CIAO_CONNECTOR_SERVANT_IMPL_UTILS_T_C */
