/* -*- C++ -*- */

// $Id$

// ============================================================================
//
// = LIBRARY
//    tao
// 
// = FILENAME
//   tao_util.h
//
// = AUTHOR
//    Sumedh Mungee <sumedh@cs.wustl.edu>
//    
// 
// ============================================================================

#if !defined (TAO_UTIL_H)
#define	TAO_UTIL_H

#include "tao/corba.h"

class TAO_ORB_Manager
{
  // = TITLE
  //     Helper class for simple ORB initialization and 
  //     registering objects
public:
  int init (int argc, 
            char **argv, 
            char *poa_name,
            CORBA_Environment &env);
  // Initialize the ORB, using the supplied command line arguments.
  // the poa_name is a user-supplied string that is used to name the
  // POA created.
      
  CORBA::String_var activate (const char *object_name,
                              PortableServer::Servant servant,
                              CORBA_Environment &env);
  // Activate the servant with the supplied object_name
  // Call multiple times to activate multiple objects.
      
  int run (CORBA_Environment &env);
  // Run the ORB event loop
      
  CORBA::ORB_var orb (void);
  // Accessor which returns the ORB pointer
      
protected:
  CORBA::ORB_var orb_;
  // The ORB
      
  PortableServer::POA_var root_poa_;
  // The root POA
      
  PortableServer::POA_var my_poa_;
  // We create our own POA
  
  PortableServer::POAManager_var poa_manager_;
  // The POA manager
      
};
  
#endif /* TAO_UTIL_H */
