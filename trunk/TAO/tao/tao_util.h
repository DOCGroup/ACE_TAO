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
// ============================================================================

#if !defined (TAO_UTIL_H)
#define	TAO_UTIL_H

#include "tao/corba.h"

class TAO_ORB_Manager
{
  // = TITLE
  //     Helper class for simple ORB/POA initialization and 
  //     registering servants with the POA.
public:
  // Constructor
  TAO_ORB_Manager (CORBA::ORB_ptr orb = 0,
                   PortableServer::POA_ptr root_poa = 0,
                   PortableServer::POAManager_ptr poa_manager = 0); 

  int init (int argc, 
            char **argv, 
            CORBA_Environment &env);
  // Initialize the ORB/POA, using the supplied command line
  // arguments or the default ORB components.
      
  CORBA::String activate (PortableServer::Servant servant,
                          CORBA_Environment &env);
  // Activate <servant>, using the POA <activate_object> call.  Users
  // can call this method multiple times to activate multiple objects.
      
  int run (CORBA_Environment &env,
           ACE_Time_Value *tv = 0);
  // Run the ORB event loop.
      
  CORBA::ORB_ptr orb (void);
  // Accessor which returns the ORB pointer.
  
  ~TAO_ORB_Manager (void);
  // Destructor
      
protected:
  CORBA::ORB_var orb_;
  // The ORB.
      
  PortableServer::POA_var root_poa_;
  // The root POA.
      
  PortableServer::POAManager_var poa_manager_;
  // The POA manager.
};
  
#endif /* TAO_UTIL_H */
