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
  int init (int argc, 
            char **argv, 
            CORBA_Environment &env);
  // Initialize the ORB/POA, using the supplied command line
  // arguments.  The <poa_name> is a user-supplied string that is used
  // @@ Sumedh, where is <poa_name> actually used here?  Is this an obsolete comment?
  // to name the POA created.
      
  CORBA::String activate (PortableServer::Servant servant,
                          CORBA_Environment &env);
  // Activate <servant> with the supplied <object_name>.
  // @@ Sumedh, where is <object_name>?  Is this comment obsolete too?
  // @@ Sumedh, can you please explain what is meant by "activate?"  Is
  // @@ this what is meant by the activate() method on the POA?  If so,
  // @@ please make sure you point this out!
  // Users can call this method multiple times to activate multiple
  // objects.
      
  int run (CORBA_Environment &env);
  // Run the ORB event loop.
      
  CORBA::ORB_ptr orb (void);
  // Accessor which returns the ORB pointer.
      
protected:
  CORBA::ORB_ptr orb_;
  // The ORB.
      
  PortableServer::POA_var root_poa_;
  // The root POA.
  // @@ Sumedh, shouldn't the user be able to specify the POA to use
  // @@ when they call the class constructor?!
      
  PortableServer::POAManager_var poa_manager_;
  // The POA manager.
  // @@ Sumedh, shouldn't users be able to specify which POAManager they
  // @@ want to use when they call the class constructor?!
};
  
#endif /* TAO_UTIL_H */
