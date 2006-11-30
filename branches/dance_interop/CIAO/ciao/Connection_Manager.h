/**
 * @file Connection_Manager.h
 * @author William Otte <wotte@dre.vanderbilt.edu>
 *
 * $Id$
 *
 * Class that aids in managing connection data structures.
 */

#ifndef CONNECTION_MANAGER_H
#define CONNECTION_MANAGER_H

#include /**/ "ace/pre.h"

#include "ace/Hash_Map_Manager.h"
#include "ciao/DeploymentC.h"
#include "ciao/CIAO_Utils_Export.h"

namespace CIAO
{
  /**
   * @class Connection_Manager
   * @brief Manages connection data structures for deployments.
   */
  class CIAO_Utils_Export Connection_Manager
  {
  public:
    /// Constructor
    Connection_Manager (const Deployment::PlanConnectionDescriptions &pcds);
    
    void add_connections (const Deployment::Connections &connection);
    
    /// Add a new connection to the structure.  This will 
    /// currenty handle connection aggregation.
    void add_connection (const Deployment::Connection &connection);
    
    Deployment::Connections fetch_connections (void);
    
  private:
    struct Conn_Info
    {
      Conn_Info (CORBA::ULong pos = 0) : pos_ (pos)
      {
      }
      
      /// Index of relevant connection in PCD structure
      CORBA::ULong pos_;
      /// The Connection structure
      Deployment::Connection conn_;
    };
    
    typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                    Conn_Info,
                                    ACE_Hash<ACE_CString>,
                                    ACE_Equal_To<ACE_CString>,
                                    ACE_Null_Mutex> Connections_Table;

    Connections_Table conns_;
    
    Deployment::PlanConnectionDescriptions pcds_;
  };
}

#include /**/ "ace/post.h"

#endif /* CONNECTION_MANAGER_H */
