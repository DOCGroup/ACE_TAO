/* -*- C++ -*- */

// $Id$

// ============================================================================
//
// = LIBRARY
//    cos
// 
// = FILENAME
//   util.h
//
// = AUTHOR
//    Sumedh Mungee <sumedh@cs.wustl.edu>
//    
// 
// ============================================================================

#if !defined (UTIL_I_H)
#define	UTIL_I_H

#include "tao/corba.h"

class TAO_ORB_Manager
{
public:
  int init (int argc, 
            char **argv, 
            CORBA_Environment &env);
  CORBA::String_var activate (const char *object_name,
                              PortableServer::Servant servant,
                              CORBA_Environment &env);
  int run (CORBA_Environment &env);
  CORBA::ORB_var orb (void);
  
 protected:
  CORBA::ORB_var orb_;
  PortableServer::POA_var root_poa_;
  PortableServer::POA_var my_poa_;
  PortableServer::POAManager_var poa_manager_;
};


#endif /* UTIL_I_H */
