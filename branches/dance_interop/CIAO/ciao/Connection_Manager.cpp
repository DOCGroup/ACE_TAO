// $Id$

#include "ciao/Connection_Manager.h"

namespace CIAO
{
  Connection_Manager::Connection_Manager (const Deployment::PlanConnectionDescriptions &pcds)
    : conns_ (pcds.length ()),
      pcds_ (pcds)
  {
    for (CORBA::ULong i = 0; i < pcds.length (); ++i)
      {
        conns_.bind (ACE_CString (pcds[i].name),
                     Conn_Info (i));
                     
      }
  }
  
  void
  Connection_Manager::add_connection (const Deployment::Connection &conn)
  {
    // We could hide logic here to substitute event service connections here.
    Conn_Info cinfo (0);
    
    conns_.find (ACE_CString (conn.name),
                 cinfo);
    
    CORBA::ULong pos = cinfo.conn_.endpoint.length ();
    
    // Extend by the number of endpoints contained in conn...
    cinfo.conn_.endpoint.length (pos + conn.endpoint.length ());
    
    // Copy endpoints....
    for (CORBA::ULong i = 0; i < conn.endpoint.length (); ++i, ++pos)
      {
        cinfo.conn_.endpoint[pos] = conn.endpoint[i];
      }
  }
  
  Deployment::Connections
  Connection_Manager::fetch_connections (void)
  {
    Deployment::Connections retval (this->conns_.current_size ());
    CORBA::ULong pos (0);
    
    for (Connections_Table::iterator i = this->conns_.begin ();
         i != this->conns_.end ();
         ++i)
      {
        retval[pos] = i->int_id_.conn_;
      }
    
    return retval;
  }
}

