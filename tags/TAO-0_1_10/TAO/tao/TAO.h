/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    tao
// 
// = FILENAME
//   tao.h
//
// = DESCRIPTION
//   Externally visible TAO extensions.   
//
// = AUTHOR
//    Chris Cleeland
// 
// ============================================================================

#if !defined (TAO_H)
#define	TAO_H

#include "tao/corba.h"

class TAO_Export TAO_ORB_Manager
{
  // = TITLE
  //     Helper class for simple ORB/POA initialization and 
  //     registering servants with the POA.
public:
  // = Initialization and termination methods.
  TAO_ORB_Manager (CORBA::ORB_ptr orb = 0,
                   PortableServer::POA_ptr poa = 0,
                   PortableServer::POAManager_ptr poa_manager = 0); 
  // Constructor.

  ~TAO_ORB_Manager (void);
  // Destructor.
      
  int init (int argc, 
            char *argv[],
            CORBA_Environment &env);
  // Initialize the ORB/root POA, using the supplied command line
  // arguments or the default ORB components. 
  // Returns -1 on failure
      
  int init_child_poa (int argc,
		      char *argv[],
		      char *poa_name,
		      CORBA_Environment &env);
  // Creates a child poa under the root poa with PERSISTENT and
  // USER_ID policies.  Call this if you want a <child_poa> with the
  // above policies, otherwise call init.
		      
  CORBA::String activate (PortableServer::Servant servant,
                          CORBA_Environment &env);
  // Activate <servant>, using the POA <activate_object> call.  Users
  // can call this method multiple times to activate multiple objects.
  // Returns 0 on failure.
      
  CORBA::String activate_under_child_poa (const char *servant_name,
					  PortableServer::Servant servant,
					  CORBA_Environment& env);
  // Precondition: init_child_poa has been called.  Activate <servant>
  // using the POA <activate_object_with_id> created from the string
  // servant_name. Users should call this to activate objects under
  // the child_poa.

  int run (CORBA_Environment &env,
           ACE_Time_Value *tv = 0);
  // Run the ORB event loop with the specified <tv> time value Returns
  // -1 on failure.
      
  CORBA::ORB_ptr orb (void);
  // Accessor which returns the ORB pointer.
  
protected:
  CORBA::ORB_var orb_;
  // The ORB.
      
  PortableServer::POA_var poa_;
  // The POA for this ORB.

  PortableServer::POA_var child_poa_;
  // Child poa under the root POA.

  PortableServer::POAManager_var poa_manager_;
  // The POA manager of poa_.
};
  
class TAO
{
  // = TITLE
  //   Defines a namespace for externally visible TAO extensions.
public:
  // From Object_KeyC.h
  typedef TAO_ObjectKey ObjectKey;
  typedef TAO_ObjectKey_var ObjectKey_var;
  typedef TAO_ObjectKey_out ObjectKey_out;

  typedef TAO_ORB_Manager ORB_Manager;

  ACE_CLASS_IS_NAMESPACE (TAO);
};

#endif /* TAO_H */
