/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    tao
//
// = FILENAME
//   ORB_Manager.h
//
// = AUTHOR
//    Chris Cleeland <cleeland@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_ORB_MANAGER_H
#define TAO_ORB_MANAGER_H
#include "ace/pre.h"

#include "PortableServer.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/ORB.h"

class TAO_PortableServer_Export TAO_ORB_Manager
{
  // = TITLE
  //     Helper class for simple ORB/POA initialization and
  //     registering servants with the POA.
  //
  // = DESCRIPTION
  //     This class is a TAO extension that makes it easier to write
  //     CORBA applications.  It's just a wrapper and doesn't do
  //     anything special within the ORB itself.
public:
  // = Initialization and termination methods.
  TAO_ORB_Manager (CORBA::ORB_ptr orb = 0,
                   PortableServer::POA_ptr poa = 0,
                   PortableServer::POAManager_ptr poa_manager = 0);
  // Constructor.

  int init (int &argc,
            char *argv[],
            CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ());
  // Initialize the ORB/root POA, using the supplied command line
  // arguments or the default ORB components.  Returns -1 on failure.

  int init (int &argc,
            char *argv[],
            const char *orb_name,
            CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ());
  // Initialize the ORB/root POA, using the supplied command line
  // arguments or the default ORB components.  Returns -1 on failure.

  int init_child_poa (int &argc,
                      char *argv[],
                      const char *poa_name,
                      CORBA_Environment &ACE_TRY_ENV =
                          TAO_default_environment ());
  // Creates a child poa under the root poa with PERSISTENT and
  // USER_ID policies.  Call this if you want a <child_poa> with the
  // above policies, otherwise call init.  Returns -1 on failure.

  int init_child_poa (int &argc,
                      char *argv[],
                      const char *poa_name,
                      const char *orb_name,
                      CORBA_Environment &ACE_TRY_ENV =
                          TAO_default_environment ());
  // Creates a child poa under the root poa with PERSISTENT and
  // USER_ID policies.  Call this if you want a <child_poa> with the
  // above policies, otherwise call init.  Returns -1 on failure.

  int fini (CORBA_Environment &ACE_TRY_ENV);
  // Shut down the <ORB_Manager>.

  ~TAO_ORB_Manager (void);
  // Destructor.

  // = Accessor methods.

  int activate_poa_manager (CORBA_Environment &ACE_TRY_ENV =
                                TAO_default_environment ());
  //  Put POA manager into the <Active> state, so that incoming corba
  //  requests are processed.  This method is useful for clients,
  //  which are not going to enter "orb->run" loop, yet may want to
  //  service incoming requests while waiting for a result of CORBA
  //  call on a server.  Returns -1 on failure.

  char *activate (PortableServer::Servant servant,
                  CORBA_Environment &ACE_TRY_ENV =
                      TAO_default_environment ());
  // Activate <servant>, using the POA <activate_object> call.  Users
  // can call this method multiple times to activate multiple objects.
  // Returns 0 on failure.

  void deactivate (const char *id,
                   CORBA_Environment &ACE_TRY_ENV =
                       TAO_default_environment ());
  // Deactivate object in RootPOA.

  char *activate_under_child_poa (const char *object_name,
                                  PortableServer::Servant servant,
                                  CORBA_Environment &ACE_TRY_ENV =
                                     TAO_default_environment ());
  // Precondition: init_child_poa has been called.  Activate <servant>
  // using the POA <activate_object_with_id> created from the string
  // <object_name>. Users should call this to activate objects under
  // the child_poa.

  void deactivate_under_child_poa (const char *id,
                                   CORBA_Environment &ACE_TRY_ENV =
                                       TAO_default_environment ());
  // Deactivate object in child POA.

  int run (CORBA_Environment &ACE_TRY_ENV,
           ACE_Time_Value *tv);
  int run (ACE_Time_Value &tv,
           CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ());
  int run (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ());
  // Run the ORB event loop with the specified <tv> time value Returns
  // -1 on failure.

  CORBA::ORB_ptr orb (void);
  // Accessor which returns the ORB pointer.  Following the normal
  // CORBA memory management rules of return values from functions,
  // this function duplicates the orb return value before returning
  // it.

  PortableServer::POA_ptr root_poa (void);
  // Accessor which returns the root poa. Following the normal CORBA
  // memory management rules of return values from functions, this
  // function duplicates the poa return value before returning it.

  PortableServer::POA_ptr child_poa (void);
  // Accessor which returns the child poa. Following the normal CORBA
  // memory management rules of return values from functions, this
  // function duplicates the poa return value before returning it.

  PortableServer::POAManager_ptr poa_manager (void);
  // Accessor which returns the poa manager. Following the normal
  // CORBA memory management rules of return values from functions,
  // this function duplicates the poa manager return value before
  // returning it.

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


#include "ace/post.h"
#endif /* TAO_ORB_MANAGER_H */
