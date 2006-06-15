// $Id$

#ifndef CIAO_SERVANT_IMPL_UTILS_T_C
#define CIAO_SERVANT_IMPL_UTILS_T_C

#include "Servant_Impl_Base.h"
#include "ciao/Cookies.h"

namespace CIAO
{
  template<typename T_var>
  void
  Servant_Impl_Base::describe_simplex_receptacle (
      const char *port_name,
      const char *port_type_repo_id,
      T_var &connection,
      ::Components::ReceptacleDescriptions_var &descriptions,
      CORBA::ULong slot
    )
  {
    ::Components::ReceptacleDescription *elem = 0;
    ACE_NEW (elem,
             ::OBV_Components::ReceptacleDescription);
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
  Servant_Impl_Base::describe_multiplex_receptacle (
      const char *port_name,
      const char *port_type_repo_id,
      ACE_Active_Map_Manager<T_var> &objrefs,
      ::Components::ReceptacleDescriptions_var &descriptions,
      CORBA::ULong slot
    )
  {
    ::Components::ReceptacleDescription *elem = 0;
    ACE_NEW (elem,
             ::OBV_Components::ReceptacleDescription);
    ::Components::ReceptacleDescription_var safe_elem = elem;

    elem->name (port_name);
    elem->type_id (port_type_repo_id);
    elem->is_multiple (true);
    elem->connections ().length (objrefs.current_size ());

    CORBA::ULong map_slot = 0UL;
    ::Components::ConnectionDescription *conn = 0;

    for (typename ACE_Active_Map_Manager<T_var>::CONST_ITERATOR iter (
             objrefs
           );
         0 == iter.done ();
         iter.advance (), ++map_slot)
      {
        ACE_NEW (conn,
                 ::OBV_Components::ConnectionDescription);
        ::Components::ConnectionDescription_var safe_conn = conn;

        ::Components::Cookie *cookie_from_key = 0;
        ACE_NEW (cookie_from_key,
                 ::CIAO::Map_Key_Cookie ((*iter).ext_id_));

        // Valuetype member set operation calls add_ref.
        conn->ck (cookie_from_key);
        CORBA::remove_ref (cookie_from_key);

        conn->objref ((*iter).int_id_.in ());

        elem->connections ()[map_slot] = safe_conn._retn ();
      }

    descriptions[slot] = safe_elem._retn ();
  }

  template<typename T_var>
  void
  Servant_Impl_Base::describe_pub_event_source (
      const char *port_name,
      const char *port_type_repo_id,
      ACE_Active_Map_Manager<T_var> &consumers,
      ::Components::PublisherDescriptions_var &descriptions,
      CORBA::ULong slot
    )
  {
    ::Components::PublisherDescription *elem = 0;
    ACE_NEW (elem,
             ::OBV_Components::PublisherDescription);
    ::Components::PublisherDescription_var safe_elem = elem;

    elem->name (port_name);
    elem->type_id (port_type_repo_id);
    elem->consumer ().length (consumers.current_size ());

    CORBA::ULong map_slot = 0UL;
    ::Components::SubscriberDescription *sub = 0;

    for (typename ACE_Active_Map_Manager<T_var>::CONST_ITERATOR iter (
             consumers
           );
         0 == iter.done ();
         iter.advance (), ++map_slot)
      {
        ACE_NEW (sub,
                 ::OBV_Components::SubscriberDescription);
        ::Components::SubscriberDescription_var safe_sub = sub;

        ::Components::Cookie *cookie_from_key = 0;
        ACE_NEW (cookie_from_key,
                 ::CIAO::Map_Key_Cookie ((*iter).ext_id_));

        // Valuetype member set operation calls add_ref.
        sub->ck (cookie_from_key);
        CORBA::remove_ref (cookie_from_key);

        sub->consumer ((*iter).int_id_.in ());

        elem->consumer ()[map_slot] = safe_sub._retn ();
      }

    descriptions[slot] = safe_elem._retn ();
  }

  template<typename T_var>
  void
  Servant_Impl_Base::describe_emit_event_source (
      const char *port_name,
      const char *port_type_repo_id,
      T_var &consumer_ref,
      ::Components::EmitterDescriptions_var &descriptions,
      CORBA::ULong slot
    )
  {
    ::Components::EmitterDescription *elem = 0;
    ACE_NEW (elem,
             ::OBV_Components::EmitterDescription);
    ::Components::EmitterDescription_var safe_elem = elem;

    elem->name (port_name);
    elem->type_id (port_type_repo_id);

    // Valuetype 'set' call increments the reference count.
    elem->consumer (consumer_ref.in ());

    descriptions[slot] = safe_elem._retn ();
  }
}

#endif /* CIAO_SERVANT_IMPL_T_C */
